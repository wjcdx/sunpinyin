
#include "stdafx.h"

#include "portability.h"
#include "ime-core/view/imi_view.h"
#include "ime-core/imi_uiobjects.h"

#include "imi_gtkwin.h"

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

CWinHandler::CWinHandler(CString *candidates, CString *preedit)
	: m_CandidataArea(candidates), m_PreeditArea(preedit)
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
    TIConvSrcPtr src = (TIConvSrcPtr)(ppd->string());
	
	CString tmp(src);
	*m_PreeditArea = tmp;
}

void
CWinHandler::updateCandidates(const ICandidateList* pcl)
{
    wstring cand_str;
    for (int i=0, sz=pcl->size(); i < sz; ++i) {
        const TWCHAR* pcand = pcl->candiString(i);
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

	CString tmp(m_buf);
    *m_CandidataArea = tmp;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4