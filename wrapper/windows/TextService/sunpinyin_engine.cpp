
#include <portability.h>
#include "sunpinyin_engine.h"
#include "CandidateWindow.h"
#include "TextService.h"


SunPinyinEngine::SunPinyinEngine(CTextService *pTextService)
	: m_pTextService(pTextService), m_PreeditArea(CCandidateWindow::_rgPreeditString),
	m_CandidataArea(CCandidateWindow::_rgCandidatesString), m_bUpdateNeeded(true)
{
	CSunpinyinSessionFactory& factory = CSunpinyinSessionFactory::getFactory();

	//factory.createQuanpinView();
    factory.createXinghuaView();

#if 0
	CSunpinyinSessionFactory::EScheme pinyin_scheme = CSunpinyinSessionFactory::XINGHUA;
        m_config.get_py_scheme(CSunpinyinSessionFactory::QUANPIN);

    factory.setPinyinScheme(pinyin_scheme);
    if (pinyin_scheme == CSunpinyinSessionFactory::QUANPIN) {
        update_fuzzy_pinyins();
        update_correction_pinyins();
        update_fuzzy_segs();
    } else {
        update_shuangpin_type();
    }
    update_user_data_dir();
    update_punct_mappings();

    factory.setCandiWindowSize(m_config.get(CONFIG_GENERAL_PAGE_SIZE, 10));
#endif
	factory.setCandiWindowSize(10);

    m_pv = factory.createSession();
    if (!m_pv)
        return;

	m_pv->setStatusAttrValue(CIMIWinHandler::STATUS_ID_FULLSYMBOL, true);
	m_pv->setStatusAttrValue(CIMIWinHandler::STATUS_ID_FULLPUNC, true);

    //m_hotkey_profile = new CHotkeyProfile();
    //factory.initSession(m_hotkey_profile);

	m_wh = new CWinHandler(this);
    factory.initSession(m_wh);
}

SunPinyinEngine::~SunPinyinEngine()
{
	if (m_pv) {
        CSunpinyinSessionFactory& factory =
            CSunpinyinSessionFactory::getFactory();
        factory.destroySession(m_pv);
    }

    delete m_wh;
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
