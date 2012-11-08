#ifndef SUNPINYIN_IMI_GTK_WIN_H
#define SUNPINYIN_IMI_GTK_WIN_H

#include "portability.h"

#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <iconv.h>

#include "ime-core/imi_glibHandler.h"

class CGTKWinHandler : public CIMIGlibHandler
{
public:
    CGTKWinHandler(CIMIView* pv);

    bool createWindows(void);

    /* Inherited methods implementation */
    /*@{*/
    virtual ~CGTKWinHandler();

    /** Update window's preedit area using a GTK widget. */
    virtual void updatePreedit(const IPreeditString* ppd);

    /** Update window's candidates area using a GTK widget. */
    virtual void updateCandidates(const ICandidateList* pcl);

    /** Update status of current session using a GTK buttons. */
    virtual void  updateStatus(int key, int value);
    /*@}*/

protected:
    void switchCN(bool cn);
    void switchFullPunc(bool full);
    void switchFullSimbol(bool full);

protected:
    /*@{*/
    static const char *hanzi_image_file_name;
    static const char *eng_image_file_name;
    static const char *cnpunc_image_file_name;
    static const char *enpunc_image_file_name;
    static const char *fullwidth_image_file_name;
    static const char *halfwidth_image_file_name;
    /*@}*/

    /*@{*/
    static GtkWidget *hanzi_image;
    static GtkWidget *eng_image;
    static GtkWidget *cnpunc_image;
    static GtkWidget *enpunc_image;
    static GtkWidget *fullwidth_image;
    static GtkWidget *halfwidth_image;
    /*@}*/

protected:
    iconv_t             m_iconv;

    /*@{*/
    CIMIView           *mp_view;

    /** Main window */
    GtkWidget          *m_pWin;

    /** Candidate window */
    GtkWidget          *m_CandidataArea;

    /** Candidate window */
    GtkWidget          *m_PreeditArea;

    /** Button for CN/EN state*/
    GtkWidget          *m_pLangButton;

    /** Button for FULL/HALF Punc state */
    GtkWidget          *m_pPuncButton;

    /** Button for FULL/HALF Simbol state */
    GtkWidget          *m_pSimbButton;
    /*@}*/

private:
    /** load the images representing CN/EN, FULL/HALF PUNC|Simbol Simbol STATE*/
    void
    load_images();

private:
    char m_buf[512];
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
