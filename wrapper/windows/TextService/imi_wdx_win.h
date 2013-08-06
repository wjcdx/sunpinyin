#ifndef SUNPINYIN_IMI_WINDOWS_WIN_H
#define SUNPINYIN_IMI_WINDOWS_WIN_H

#include "ime-core/imi_winHandler.h"

int
translate_key(int orig);

int
key_press_cb(CKeyEvent *event, CIMIView *pview);

class CWinHandler : public CIMIWinHandler
{
public:
    CWinHandler(char *candidates, char *preedit);

    /* Inherited methods implementation */
    /*@{*/
    virtual ~CWinHandler();

	/** commit a string, normally the converted result */
    virtual void commit(const TWCHAR* wstr);

    /** when a key is not processed */
    //virtual void throwBackKey(unsigned keycode, unsigned keyvalue, unsigned modifier);

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
    char          *m_CandidataArea;

    /** Candidate window */
    char          *m_PreeditArea;

private:
    char m_buf[512];
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
