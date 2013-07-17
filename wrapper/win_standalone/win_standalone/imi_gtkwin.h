#ifndef SUNPINYIN_IMI_WINDOWS_WIN_H
#define SUNPINYIN_IMI_WINDOWS_WIN_H

//#include <iconv.h>

#include "stdafx.h"

#include "ime-core/imi_winHandler.h"

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
    //iconv_t             m_iconv;

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
