#ifndef SUNPINYIN_ENGINE_H
#define SUNPINYIN_ENGINE_H

#include <portability.h>
#include <ime-core/imi_winHandler.h>
#include <ime-core/view/imi_view.h>
#include <ime-core/imi_uiobjects.h>
#include <ime-core/options/imi_option_event.h>
#include <ime-core/options/imi_option_keys.h>
#include <ime-core/options/session_factory.h>
#include <xh/input/segment/TXhSyllableSegment.h>
#include "imi_wdx_win.h"
#include "Config/sunpinyin_config.h"

#include "Globals.h"
#include "LangBar/LangBarMenu.h"
#include "LangBar/LangBarButton.h"


class CTextService;

class SunPinyinEngine : public IConfigurable
{
public:
	enum {
		LANGBAR_ITEM_ID_CONFIG_SETUP = 1000,
	};

    SunPinyinEngine(CTextService *pTextService);
    virtual ~SunPinyinEngine();

	void enable ();

	// called by TextServiceFramework
	bool is_mode_switch_key (CKeyEvent &event);
	bool is_chinese_mode ();
	bool process_key_event (TfEditCookie ec, ITfContext *pContext, CKeyEvent &event);

	// CALLBACKS of CWinHandler
	// update candidates
	void update_candidates(const ICandidateList& cl);
	// update preedit area
	void update_preedit_string(const IPreeditString& preedit);
	// commit selected word
	void commit_string (const WCHAR *wstr, int length);

	// if it's needed to update the candidate window
	void updateNeeded(bool needed) { m_bUpdateNeeded = needed; }
	bool isUpdateNeeded() { return m_bUpdateNeeded; }

	// callback function of CLangBarItem
	bool update_prop_status(unsigned id, bool status);
	// update status of CLangBarItem
	void update_status_property(bool);
    void update_punct_property(bool);
    void update_letter_property(bool);

	// handle language bar items
	bool init_language_bar(ITfLangBarItemMgr *pLangBarItemMgr);
	bool uninit_language_bar(ITfLangBarItemMgr *pLangBarItemMgr);
	void setup_langbar_items();

	virtual bool onConfigChanged(const COptionEvent& event);

private:
    void update_config();
    void update_history_power();
    void update_cand_window_size();
    void update_mode_key();
    void update_punct_key();
    void update_page_key_minus();
    void update_page_key_comma();
    void update_page_key_bracket();
    void update_page_key(const char* conf_key, bool default_val,
                         unsigned page_up, unsigned page_down);
    void update_candi_delete_key();
    void update_cancel_with_backspace();
    void update_smart_punc();
    void update_max_best();
    void update_max_tail_candidate();

    void update_charset_level();
    void update_user_data_dir();
    void update_punct_mappings();
    void update_fuzzy_pinyins();
    void update_correction_pinyins();
    void update_fuzzy_segs();
    void update_shuangpin_type();

private:
	CWinHandler		*m_wh;
    CIMIView        *m_pv;
	CHotkeyProfile  *m_hotkey_profile;
	SunPinyinConfig m_config;

	CTextService	*m_pTextService;
	ITfContext		*m_pContext;
	TfEditCookie	m_oEditCookie;

#ifdef HAVE_ICONV_H
    iconv_t             m_iconv;
#endif

    /** Candidate window */
    char          *m_CandidataArea;

    /** Candidate window */
    char          *m_PreeditArea;

private:
    char m_buf[512];
	// there may be three situations:
	// a) there's no candidate for the input string;
	// b) it's committed;
	// c) preedit chars are all deleted;
	// it's needn't to update candidate list window
	// for b & c.
	bool m_bUpdateNeeded;


	CLangBarButton m_lbbStatus;
	CLangBarMenu m_lbmMenu;
};

#endif