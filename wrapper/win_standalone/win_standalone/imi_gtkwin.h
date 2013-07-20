#ifndef SUNPINYIN_IMI_WINDOWS_WIN_H
#define SUNPINYIN_IMI_WINDOWS_WIN_H

//#include <iconv.h>

#include "stdafx.h"

#include "ime-core/imi_winHandler.h"

int
translate_key(int orig);

int
key_press_cb(CKeyEvent *event, CIMIView *pview);

class CWinHandler : public CIMIWinHandler
{
public:
    CWinHandler(CString *candidates, CString *preedit);

    bool createWindows(void);

    /* Inherited methods implementation */
    /*@{*/
    virtual ~CWinHandler();

    /** Update window's preedit area using a GTK widget. */
    virtual void updatePreedit(const IPreeditString* ppd);

    /** Update window's candidates area using a GTK widget. */
    virtual void updateCandidates(const ICandidateList* pcl);

    /** Update status of current session using a GTK buttons. */
    virtual void  updateStatus(int key, int value);
    /*@}*/

protected:
#ifdef HAVE_ICONV_H
    iconv_t             m_iconv;
#endif
    /*@{*/
    //CIMIView           *mp_view;

    /** Candidate window */
    CString          *m_CandidataArea;

    /** Candidate window */
    CString          *m_PreeditArea;

private:
    char m_buf[512];
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
