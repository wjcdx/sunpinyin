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

#include "Globals.h"

class CTextService;

class SunPinyinEngine : public IConfigurable
{
public:
    SunPinyinEngine(CTextService *pTextService);
    virtual ~SunPinyinEngine();

	// called by TextServiceFramework
	bool process_key_event (TfEditCookie ec, ITfContext *pContext, CKeyEvent &event);

	// CALLBACKS of CWinHandler
	// update candidates
	void update_candidates(const ICandidateList& cl);
	// update preedit area
	void update_preedit_string(const IPreeditString& preedit);
	// commit selected word
	void commit_string (const WCHAR *wstr, int length);

	void updateNeeded(bool needed) { m_bUpdateNeeded = needed; }
	bool isUpdateNeeded() { return m_bUpdateNeeded; }

private:
	CWinHandler		*m_wh;
    CIMIView        *m_pv;
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
};

#endif