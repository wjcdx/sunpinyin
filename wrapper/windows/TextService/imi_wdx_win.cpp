
#include "portability.h"
#include "ime-core/view/imi_view.h"
#include "ime-core/imi_uiobjects.h"

#include "imi_wdx_win.h"
#include "sunpinyin_engine.h"

int
translate_key(int orig)
{
	int chr = orig;
	if (orig == VK_BACK || orig == VK_DELETE) {
		chr = IM_VK_BACK_SPACE;
	} else if (orig == VK_PRIOR) {
		chr = IM_VK_PAGE_UP;
	} else if (orig == VK_NEXT) {
		chr = IM_VK_PAGE_DOWN;
	} else if (orig < 0x20 && orig > 0x7E) {
		chr = 0;
	}
	return chr;
}

int
key_press_cb(CKeyEvent *event, CIMIView *pview)
{
    if (pview != NULL) {

		if (event->value < 0x20 && event->value > 0x7E)
            event->value = 0;

        //CKeyEvent key_event (keycode, keyvalue, event->state);
        pview->onKeyEvent(*event);
    }
    return 1;
}

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
    /*switch (key) {
    case STATUS_ID_CN:          switchCN( value != 0 ); break;
    case STATUS_ID_FULLPUNC:    switchFullPunc( value != 0 ); break;
    case STATUS_ID_FULLSYMBOL:  switchFullSimbol( value != 0 ); break;
    }*/
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