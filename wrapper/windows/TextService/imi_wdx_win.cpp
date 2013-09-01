
#include "portability.h"
#include "ime-core/view/imi_view.h"
#include "ime-core/imi_uiobjects.h"

#include "imi_wdx_win.h"
#include "sunpinyin_engine.h"


CWinHandler::CWinHandler(SunPinyinEngine *pEngine)
	: m_engine(pEngine)
{
#ifdef HAVE_ICONV_H
    m_iconv = iconv_open("UTF-8", TWCHAR_ICONV_NAME);
#endif
}

CWinHandler::~CWinHandler()
{
#ifdef HAVE_ICONV_H
    iconv_close(m_iconv);
#endif
}

void
CWinHandler::commit(const TWCHAR* wstr)
{
	if (wstr) {
		std::wstring str;
        copy(wstr, wstr+WCSLEN(wstr)+1, back_inserter(str));
		m_engine->commit_string((const WCHAR *)str.c_str(), str.length()-1);
	}
}

void
CWinHandler::updateStatus(int key, int value)
{
    switch (key) {
    case STATUS_ID_CN:
		m_engine->update_status_property(value);
		break;
    case STATUS_ID_FULLPUNC:
		m_engine->update_punct_property(value);
		break;
    case STATUS_ID_FULLSYMBOL:
		m_engine->update_letter_property(value);
		break;
    }
}

void
CWinHandler::updatePreedit(const IPreeditString* ppd)
{
	if (ppd) {
		m_engine->update_preedit_string(*ppd);
	}
}

void
CWinHandler::updateCandidates(const ICandidateList* pcl)
{
	if (pcl) {
		m_engine->update_candidates(*pcl);
	}
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4