
#include <portability.h>
#include "sunpinyin_engine.h"
#include "CandidateWindow.h"
#include "TextService.h"
#include "Globals.h"
#include "Config/sunpinyin_config_keys.h"


SunPinyinEngine::SunPinyinEngine(CTextService *pTextService)
	: m_pTextService(pTextService), m_PreeditArea(CCandidateWindow::_rgPreeditString),
	m_CandidataArea(CCandidateWindow::_rgCandidatesString), m_bUpdateNeeded(true),
	m_lbbStatus(CLangBarButton(pTextService, this, c_guidLangBarItemButton, LANGBAR_ITEM_DESC)),
	m_lbmMenu(CLangBarMenu(pTextService, this, c_guidLangBarItemMenu, LANGBAR_MENU_DESC))
{
	CSunpinyinSessionFactory& factory = CSunpinyinSessionFactory::getFactory();

	//factory.createQuanpinView();
    factory.createXinghuaView();

	CSunpinyinSessionFactory::EScheme pinyin_scheme = //CSunpinyinSessionFactory::XINGHUA;
        m_config.get_py_scheme(CSunpinyinSessionFactory::XINGHUA);

    factory.setPinyinScheme(pinyin_scheme);
    if (pinyin_scheme == CSunpinyinSessionFactory::QUANPIN) {
        update_fuzzy_pinyins();
        update_correction_pinyins();
        update_fuzzy_segs();
	} else if (pinyin_scheme == CSunpinyinSessionFactory::SHUANGPIN) {
        update_shuangpin_type();
    }
    update_user_data_dir();

    m_pv = factory.createSession();
    if (!m_pv)
        return;

    m_hotkey_profile = new CHotkeyProfile();
    factory.initSession(m_hotkey_profile);

	m_wh = new CWinHandler(this);
    factory.initSession(m_wh);

	update_config();

	// init language bare items
	setup_langbar_items();
}

SunPinyinEngine::~SunPinyinEngine()
{
	if (m_pv) {
        CSunpinyinSessionFactory& factory =
            CSunpinyinSessionFactory::getFactory();
        factory.destroySession(m_pv);
    }

    delete m_wh;
	m_wh = NULL;

	// the object will be freed automatically,
	// and it's needn't to care about the ref-count of itself.
	//m_lbbStatus.Release();
}

bool SunPinyinEngine::process_key_event (TfEditCookie ec, ITfContext *pContext, CKeyEvent &event)
{
	m_pContext = pContext;
	m_oEditCookie = ec;

	if (m_pv != NULL) {
        return m_pv->onKeyEvent(event);
    }
    return false;
}

void SunPinyinEngine::commit_string (const WCHAR *wstr, int length)
{
	m_pTextService->_CommitSelectedCandidate(m_oEditCookie, m_pContext, wstr, length);
	updateNeeded(false);
}

void SunPinyinEngine::update_preedit_string(const IPreeditString& preedit)
{
	const TWCHAR *twStr = preedit.string();
	int size = preedit.size();

	if (size <= 0) {
		m_pTextService->_HandleComplete(m_oEditCookie, m_pContext);
		updateNeeded(false);
		return;
	}

	char utf8[1024] = { 0 };
	char ansi[1024] = { 0 };
	

	//ucs-4 => utf8
	size = WCSTOMBS(utf8, twStr, 1024);

	//utf8 => ansi
	size = UTF8toANSI(ansi, utf8);

	if (size > 128) size = 128;
	memset(m_PreeditArea, 0, strlen(m_PreeditArea));
	memcpy(m_PreeditArea, ansi, size);
}


void SunPinyinEngine::update_candidates(const ICandidateList& cl)
{
	wstring cand_str;
	int size = cl.size();

	if (size <= 0) {
		if (!isUpdateNeeded()) {
			return;
		}
		// there's no candidates for the input string
		// TODO: update the candidates window directly.
		memset(m_buf, 0, sizeof(m_buf));
		goto Exit;
	} else {
		updateNeeded(true);
	}

	int i = 0;
    for (i=0; i < size; ++i) {
        const TWCHAR* pcand = cl.candiString(i);
        if (pcand == NULL) break;
        cand_str += (i==9)?'0':TWCHAR('1' + i);
        cand_str += TWCHAR('.');
        cand_str += pcand;
        cand_str += TWCHAR(' ');
    }

#ifdef HAVE_ICONV_H
	
	TIConvSrcPtr src = (TIConvSrcPtr)(cand_str.c_str());
    size_t srclen = (cand_str.size()+1)*sizeof(TWCHAR);
    char * dst = m_buf;
    size_t dstlen = sizeof(m_buf) - 1;
    iconv(m_iconv, &src, &srclen, &dst, &dstlen);

#else

	char utf8[1024] = { 0 };

	//ucs-4 => utf8
	WCSTOMBS(utf8, cand_str.c_str(), 1024);
	
	//utf8 => ansi
	memset(m_buf, 0, sizeof(m_buf));
	UTF8toANSI(m_buf, utf8);

#endif

Exit:
	memset(m_CandidataArea, 0, strlen(m_CandidataArea));
	memcpy(m_CandidataArea, m_buf, strlen(m_buf));
	// SHOW/UPDATE CANDIDATE WINDOW
	m_pTextService->_UpdateCandidateList(m_oEditCookie, m_pContext);
}


bool SunPinyinEngine::update_prop_status(unsigned id, bool status)
{
	if (!m_pv)
		return false;
	m_pv->setStatusAttrValue(id, status);
	return true;
}

void SunPinyinEngine::update_status_property(bool cn)
{
	m_lbbStatus.SetState(cn);
}

void SunPinyinEngine::update_punct_property(bool full)
{
	//m_lbmMenu.SetItemStatus(CIMIWinHandler::STATUS_ID_FULLPUNC, full);
}

void SunPinyinEngine::update_letter_property(bool full)
{
	//m_lbmMenu.SetItemStatus(CIMIWinHandler::STATUS_ID_FULLSYMBOL, full);
}

bool SunPinyinEngine::init_language_bar(ITfLangBarItemMgr *pLangBarItemMgr)
{
	if (pLangBarItemMgr == NULL)
		return false;

	if (pLangBarItemMgr->AddItem(&m_lbbStatus) != S_OK)
        return false;

	if (pLangBarItemMgr->AddItem(&m_lbmMenu) != S_OK)
		return false;

	m_lbbStatus.SetState(true);

	return true;
}

bool SunPinyinEngine::uninit_language_bar(ITfLangBarItemMgr *pLangBarItemMgr)
{
	if (pLangBarItemMgr == NULL)
		return false;
	if (pLangBarItemMgr->RemoveItem(&m_lbbStatus) != S_OK)
        return false;

	if (pLangBarItemMgr->RemoveItem(&m_lbmMenu) != S_OK)
		return false;

	return true;
}

void SunPinyinEngine::setup_langbar_items()
{
	CLangBarItemInfo statusTrueInfo;
	CLangBarItemInfo statusFalseInfo;

	statusTrueInfo.setText(std::string("EN"));
	statusTrueInfo.setIcon(IDI_ENG);
	statusTrueInfo.setTooltip(std::string("Switch to Chinese input mode"));
	statusFalseInfo.setText(std::string("CN"));
	statusFalseInfo.setIcon(IDI_HAN);
	statusFalseInfo.setTooltip(std::string("Switch to English input mode"));

	m_lbbStatus.SetupInfo(statusTrueInfo, statusFalseInfo);
	m_lbbStatus.SetId(CIMIWinHandler::STATUS_ID_CN);

	CLangBarMenuItem lbmItem(this);
	CLangBarMenuItemInfo lbmiiTrueInfo;
	CLangBarMenuItemInfo lbmiiFalseInfo;

	lbmiiTrueInfo.SetText(std::string("FULL"));
	lbmiiTrueInfo.SetBmp(IDB_WIDTHFULL);
	lbmiiFalseInfo.SetText(std::string("HALF"));
	lbmiiFalseInfo.SetBmp(IDB_WIDTHHALF);
	lbmItem.SetInfo(lbmiiTrueInfo, lbmiiFalseInfo);
	lbmItem.SetIndex(0);
	lbmItem.SetId(CIMIWinHandler::STATUS_ID_FULLSYMBOL);
	lbmItem.SetState(true);
	m_lbmMenu.AddItem(lbmItem);

	lbmiiTrueInfo.SetText(std::string("CHN"));
	lbmiiTrueInfo.SetBmp(IDB_PUNCHAN);
	lbmiiFalseInfo.SetText(std::string("ENG"));
	lbmiiFalseInfo.SetBmp(IDB_PUNCENG);
	lbmItem.SetInfo(lbmiiTrueInfo, lbmiiFalseInfo);
	lbmItem.SetIndex(1);
	lbmItem.SetId(CIMIWinHandler::STATUS_ID_FULLPUNC);
	lbmItem.SetState(true);
	m_lbmMenu.AddItem(lbmItem);
	m_lbmMenu.SetText("Menu");
	m_lbmMenu.SetIcon(IDI_MENU);

}

bool
SunPinyinEngine::onConfigChanged(const COptionEvent& event)
{
    if (event.name == CONFIG_GENERAL_MEMORY_POWER) {
        update_history_power();
    } else if (event.name == CONFIG_GENERAL_PAGE_SIZE) {
        update_cand_window_size();
    } else if (event.name == CONFIG_GENERAL_CHARSET_LEVEL) {
        update_charset_level();
    } else if (event.name == CONFIG_GENERAL_MAX_BEST) {
        update_max_best();
    } else if (event.name == CONFIG_GENERAL_MAX_TAIL_CANDIDATE) {
        update_max_tail_candidate();
    } else if (event.name == CONFIG_KEYBOARD_MODE_SWITCH) {
        update_mode_key();
    } else if (event.name == CONFIG_KEYBOARD_PUNCT_SWITCH) {
        update_punct_key();
    } else if (event.name == CONFIG_KEYBOARD_PAGE_COMMA) {
        update_page_key_comma();
    } else if (event.name == CONFIG_KEYBOARD_PAGE_MINUS) {
        update_page_key_minus();
    } else if (event.name == CONFIG_KEYBOARD_PAGE_BRACKET) {
        update_page_key_bracket();
    } else if (event.name == CONFIG_QUANPIN_FUZZYSEGS_ENABLED) {
        update_fuzzy_segs();
    } else if (event.name == CONFIG_KEYBOARD_CANCEL_BACKSPACE) {
        update_cancel_with_backspace();
    } else if (event.name == CONFIG_KEYBOARD_SMARK_PUNCT) {
        update_smart_punc();
    }

    return false;
}

void
SunPinyinEngine::update_config()
{
    update_history_power();
    update_cand_window_size();
    update_max_best();
    update_max_tail_candidate();
    update_charset_level();
    update_page_key_minus();
    update_page_key_comma();
    update_page_key_bracket();
    update_mode_key();
    update_punct_key();
    update_cancel_with_backspace();
    update_smart_punc();
    update_punct_mappings();
    update_candi_delete_key();
    // update_quanpin_config();
    // update_shuangpin_config();
}

void
SunPinyinEngine::update_history_power()
{
    unsigned power = m_config.get(CONFIG_GENERAL_MEMORY_POWER, 3);
    CIMIContext* ic = m_pv->getIC();
    assert(ic);
    ic->setHistoryPower(power);
}

void
SunPinyinEngine::update_charset_level()
{
    std::string charset("GBK");
    charset = m_config.get(CONFIG_GENERAL_CHARSET_LEVEL, charset);
    //printf("charset is %s.\n", charset.c_str());
    CIMIContext* ic = m_pv->getIC();
    assert(ic);
    //if (charset == "GB2312") {
    //    ic->setCharsetLevel(0);
    //} else if (charset == "GBK") {
    //    ic->setCharsetLevel(1);
    //}   
    //else {
    //    ic->setCharsetLevel(2);
    //}

	// TODO: FIX ME!
	// because iconv is not supported for now,
	// the charset level defaults to 3 while
	// lexicon file is parsed. Because the chars
	// may be un-recognized, the default level is
	// not changed.
	ic->setCharsetLevel(3);
}

void
SunPinyinEngine::update_cand_window_size()
{
    unsigned size = m_config.get(CONFIG_GENERAL_PAGE_SIZE, 10);
    m_pv->setCandiWindowSize(size);
}

void
SunPinyinEngine::update_mode_key()
{
    std::string mode_switch("Shift");
    mode_switch = m_config.get(CONFIG_KEYBOARD_MODE_SWITCH, mode_switch);

    CKeyEvent shift_l  (IM_VK_SHIFT_L, 0, IM_SHIFT_MASK|IM_RELEASE_MASK);
    CKeyEvent shift_r  (IM_VK_SHIFT_R, 0, IM_SHIFT_MASK|IM_RELEASE_MASK);
    CKeyEvent control_l(IM_VK_CONTROL_L, 0, IM_CTRL_MASK|IM_RELEASE_MASK);
    CKeyEvent control_r(IM_VK_CONTROL_R, 0, IM_CTRL_MASK|IM_RELEASE_MASK);

    if (mode_switch == "Shift") {
        m_hotkey_profile->removeModeSwitchKey(control_l);
        m_hotkey_profile->removeModeSwitchKey(control_r);
        m_hotkey_profile->addModeSwitchKey(shift_l);
        m_hotkey_profile->addModeSwitchKey(shift_r);
    } else if (mode_switch == "Control") {
        m_hotkey_profile->removeModeSwitchKey(shift_l);
        m_hotkey_profile->removeModeSwitchKey(shift_r);
        m_hotkey_profile->addModeSwitchKey(control_l);
        m_hotkey_profile->addModeSwitchKey(control_r);
    }
}

void
SunPinyinEngine::update_punct_key()
{
    std::string punct_switch("ControlComma");
    punct_switch = m_config.get(CONFIG_KEYBOARD_PUNCT_SWITCH, punct_switch);
    if (punct_switch == "ControlComma") {
        m_hotkey_profile->setPunctSwitchKey(CKeyEvent(IM_VK_COMMA, 0, IM_CTRL_MASK));
    } else if (punct_switch == "ControlPeriod") {
        m_hotkey_profile->setPunctSwitchKey(CKeyEvent(IM_VK_PERIOD, 0, IM_CTRL_MASK));
    }
}

void
SunPinyinEngine::update_page_key_minus()
{
    update_page_key(CONFIG_KEYBOARD_PAGE_MINUS, false,
                    IM_VK_MINUS, IM_VK_EQUALS);
}

void
SunPinyinEngine::update_page_key_comma()
{
    update_page_key(CONFIG_KEYBOARD_PAGE_COMMA, false,
                    IM_VK_COMMA, IM_VK_PERIOD);
}

void
SunPinyinEngine::update_page_key_bracket()
{
    update_page_key(CONFIG_KEYBOARD_PAGE_BRACKET, false,
                    IM_VK_OPEN_BRACKET, IM_VK_CLOSE_BRACKET);
}

void
SunPinyinEngine::update_page_key(const char* conf_key, bool default_val,
                            unsigned page_up, unsigned page_down)
{
    bool enabled = m_config.get(conf_key, default_val);

    if (enabled) {
        m_hotkey_profile->addPageUpKey(CKeyEvent(page_up, 0));
        m_hotkey_profile->addPageDownKey(CKeyEvent(page_down, 0));
    } else {
        m_hotkey_profile->removePageUpKey(CKeyEvent(page_up, 0));
        m_hotkey_profile->removePageDownKey(CKeyEvent(page_down, 0));
    }
}

void
SunPinyinEngine::update_cancel_with_backspace()
{
    bool enabled = m_config.get(CONFIG_KEYBOARD_CANCEL_BACKSPACE, true);
    m_pv->setCancelOnBackspace(enabled);
}

void
SunPinyinEngine::update_smart_punc()
{
    m_pv->setSmartPunct(m_config.get(CONFIG_KEYBOARD_SMARK_PUNCT, true));
}

void
SunPinyinEngine::update_max_best()
{
    if (m_pv->getIC() == NULL) {
        return;
    }
    int oldval = (int) m_pv->getIC()->getMaxBest();
    m_pv->getIC()->setMaxBest(m_config.get(CONFIG_GENERAL_MAX_BEST, oldval));
}

void
SunPinyinEngine::update_max_tail_candidate()
{
    if (m_pv->getIC() == NULL) {
        return;
    }
    int oldval = (int) m_pv->getIC()->getMaxTailCandidateNum();
    m_pv->getIC()->setMaxTailCandidateNum(
        m_config.get(CONFIG_GENERAL_MAX_TAIL_CANDIDATE, oldval));
}

string_pairs parse_pairs(const std::vector<std::string>& strings)
{
    string_pairs pairs;
    for (std::vector<std::string>::const_iterator pair = strings.begin();
         pair != strings.end(); ++pair) {

        std::string::size_type found = pair->find(':');
        if (found == pair->npos || pair->length() < 3)
            continue;
        if (found == 0 && (*pair)[0] == ':')
            found = 1;

        pairs.push_back(make_pair(pair->substr(0, found),
                                  pair->substr(found+1)));
    }
    return pairs;
}

// the mappings in default_pairs will override the ones in user_pairs
string_pairs merge_pairs(const string_pairs& default_pairs,
                         const string_pairs& user_pairs)
{
    typedef std::map<std::string, int> Indexes;
    Indexes indexes;
    int index = 0;
    for (string_pairs::const_iterator it = default_pairs.begin();
         it != default_pairs.end(); ++it, ++index) {
        Indexes::iterator found = indexes.find(it->first);
        if (found == indexes.end()) {
            indexes[it->first] = index;
        } else {
            // it is a paired punct.
            indexes[it->first] = -found->second;
        }
    }
    string_pairs result(default_pairs);
    for (string_pairs::const_iterator it = user_pairs.begin();
         it != user_pairs.end(); ++it) {
        Indexes::iterator found = indexes.find(it->first);
        if (found == indexes.end()) {
            result.push_back(*it);
        } else if (found->second >= 0) {
            result[found->second] = *it;
        } else {
            // it is a paired punct,
            // but we don't support this kind of mapping yet,
            // so quietly ignore it.
        }
    }
    return result;
}

void
SunPinyinEngine::update_punct_mappings()
{
    CSimplifiedChinesePolicy& policy = ASimplifiedChinesePolicy::instance();
    if (m_config.get(PINYIN_PUNCTMAPPING_ENABLED, false)) {
        std::vector<std::string> mappings;
        mappings = m_config.get(PINYIN_PUNCTMAPPING_MAPPINGS, mappings);
        string_pairs pairs(merge_pairs(policy.getDefaultPunctMapping(),
                                       parse_pairs(mappings)));
        policy.setPunctMapping(pairs);
    }
}

void
SunPinyinEngine::update_user_data_dir()
{
    //std::stringstream user_data_dir;
    //user_data_dir << g_get_home_dir()
    //              << G_DIR_SEPARATOR_S << ".sunpinyin";
    //ASimplifiedChinesePolicy::instance().setUserDataDir(user_data_dir.str());
}

void
SunPinyinEngine::update_fuzzy_pinyins()
{
    bool enabled = m_config.get(QUANPIN_FUZZY_ENABLED, false);
    AQuanpinSchemePolicy::instance().setFuzzyForwarding(enabled);
    AShuangpinSchemePolicy::instance().setFuzzyForwarding(enabled);
    if (!enabled)
        return;
    std::vector<std::string> fuzzy_pinyins;
    fuzzy_pinyins = m_config.get(QUANPIN_FUZZY_PINYINS, fuzzy_pinyins);
    AQuanpinSchemePolicy::instance().setFuzzyPinyinPairs(parse_pairs(fuzzy_pinyins));
    AShuangpinSchemePolicy::instance().setFuzzyPinyinPairs(parse_pairs(fuzzy_pinyins));
}

void
SunPinyinEngine::update_correction_pinyins()
{
    bool enabled = m_config.get(QUANPIN_AUTOCORRECTION_ENABLED, false);
    AQuanpinSchemePolicy::instance().setAutoCorrecting(enabled);
    if (!enabled)
        return;
    std::vector<std::string> correction_pinyins;
    correction_pinyins = m_config.get(QUANPIN_AUTOCORRECTION_PINYINS, correction_pinyins);
    AQuanpinSchemePolicy::instance().setAutoCorrectionPairs(parse_pairs(correction_pinyins));
}

void
SunPinyinEngine::update_fuzzy_segs()
{
    bool enable_fuzzy_segs = m_config.get(CONFIG_QUANPIN_FUZZYSEGS_ENABLED, false);
    AQuanpinSchemePolicy::instance().setFuzzySegmentation(enable_fuzzy_segs);
    bool enable_inner_fuzzy = m_config.get(CONFIG_QUANPIN_INNERFUZZY_ENABLED, false);
    AQuanpinSchemePolicy::instance().setInnerFuzzySegmentation(CONFIG_QUANPIN_INNERFUZZY_ENABLED);
}

void
SunPinyinEngine::update_shuangpin_type()
{
    EShuangpinType shuangpin_type = MS2003;
    shuangpin_type = (EShuangpinType) m_config.get(SHUANGPIN_TYPE, (int) shuangpin_type);
    AShuangpinSchemePolicy::instance().setShuangpinType(shuangpin_type);
}

void
SunPinyinEngine::update_candi_delete_key()
{
    /* FIXME: need to get candi_delete_key from user's configuration */
    m_hotkey_profile->setCandiDeleteKey(CKeyEvent(0, 0, IM_ALT_MASK));
}


