#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "imi_gtkwin.h"
#include "ime-core/view/imi_view.h"
#include "ime-core/imi_uiobjects.h"


const char *CGTKWinHandler::hanzi_image_file_name = "images/han.png";
const char *CGTKWinHandler::eng_image_file_name = "images/eng.png";
const char *CGTKWinHandler::cnpunc_image_file_name = "images/cnpunc.png";
const char *CGTKWinHandler::enpunc_image_file_name = "images/enpunc.png";
const char *CGTKWinHandler::fullwidth_image_file_name = "images/fullwidth.png";
const char *CGTKWinHandler::halfwidth_image_file_name = "images/halfwidth.png";
const char *CGTKWinHandler::tuichu_image_file_name = "images/tuichu.png";

GtkWidget *CGTKWinHandler::hanzi_image = NULL;
GtkWidget *CGTKWinHandler::eng_image = NULL;
GtkWidget *CGTKWinHandler::cnpunc_image = NULL;
GtkWidget *CGTKWinHandler::enpunc_image = NULL;
GtkWidget *CGTKWinHandler::fullwidth_image = NULL;
GtkWidget *CGTKWinHandler::halfwidth_image = NULL;
GtkWidget *CGTKWinHandler::tuichu_image = NULL;


static gint
key_press_cb(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (data != NULL) {
        unsigned keycode = event->keyval;
        unsigned keyvalue = event->keyval;
        if (keyvalue < 0x20 && keyvalue > 0x7E)
            keyvalue = 0;

        CIMIView* pview = (CIMIView*)(data);
        CKeyEvent key_event (keycode, keyvalue, event->state);
        pview->onKeyEvent(key_event);
    }
    return 1;
}

static void
button_click_cn_en(GtkWidget* button, gpointer data)
{
    /*
    if (data != NULL) {
        CIMIView* pv = (CIMSAView*)data;
        unsigned int mask = 0;
        pv->switchCN(&mask);
        pv->updateWindows(mask);
    }
    */
}

static void
button_click_close(GtkWidget* button, gpointer data)
{
    gtk_main_quit();
}

static void
button_click_fullhalf_punc(GtkWidget* button, gpointer data)
{
    /*
    if (data != NULL)
      ((CIMSAView*)data)->switchFullPunc(NULL);
    */
}

static void
button_click_fullhalf_simbol(GtkWidget* button, gpointer data)
{
    /*
    if (data != NULL)
        ((CIMSAView*)data)->switchFullSimbol(NULL);
    */
}

CGTKWinHandler::CGTKWinHandler(CIMIView* pv)
  : mp_view(pv), m_pWin(NULL), m_CandidataArea(NULL), m_PreeditArea(NULL),
    m_pLangButton(NULL), m_pPuncButton(NULL), m_pSimbButton(NULL)
{
    m_iconv = iconv_open("UTF-8", TWCHAR_ICONV_NAME);
    load_images();
}

CGTKWinHandler::~CGTKWinHandler()
{
    iconv_close(m_iconv);
    g_object_unref(hanzi_image);
    g_object_unref(eng_image);
    g_object_unref(cnpunc_image);
    g_object_unref(enpunc_image);
    g_object_unref(fullwidth_image);
    g_object_unref(halfwidth_image);
    g_object_unref(tuichu_image);
}

void
CGTKWinHandler::load_images()
{
    if (hanzi_image == NULL) {
        hanzi_image = gtk_image_new_from_file(hanzi_image_file_name);
        eng_image = gtk_image_new_from_file(eng_image_file_name);
        cnpunc_image = gtk_image_new_from_file(cnpunc_image_file_name);
        enpunc_image = gtk_image_new_from_file(enpunc_image_file_name);
        fullwidth_image = gtk_image_new_from_file(fullwidth_image_file_name);
        halfwidth_image = gtk_image_new_from_file(halfwidth_image_file_name);
        tuichu_image = gtk_image_new_from_file(tuichu_image_file_name);
    }
    g_object_ref(hanzi_image);
    g_object_ref(eng_image);
    g_object_ref(cnpunc_image);
    g_object_ref(enpunc_image);
    g_object_ref(fullwidth_image);
    g_object_ref(halfwidth_image);
    g_object_ref(tuichu_image);
}


void
CGTKWinHandler::updateStatus(int key, int value)
{
    switch (key) {
    case STATUS_ID_CN:          switchCN( value != 0 ); break;
    case STATUS_ID_FULLPUNC:    switchFullPunc( value != 0 ); break;
    case STATUS_ID_FULLSYMBOL:  switchFullSimbol( value != 0 ); break;
    }
}

void
CGTKWinHandler::switchCN(bool cn)
{
    gtk_container_remove(GTK_CONTAINER(m_pLangButton), (!cn)?hanzi_image:eng_image);
    gtk_container_add(GTK_CONTAINER(m_pLangButton), (cn)?hanzi_image:eng_image);
    gtk_widget_show_all(m_pLangButton);
}

void
CGTKWinHandler::switchFullPunc(bool full)
{
    gtk_container_remove(GTK_CONTAINER(m_pPuncButton), (!full)?cnpunc_image:enpunc_image);
    gtk_container_add(GTK_CONTAINER(m_pPuncButton), (full)?cnpunc_image:enpunc_image);
    gtk_widget_show_all(m_pPuncButton);
}

void
CGTKWinHandler::switchFullSimbol(bool full)
{
    gtk_container_remove(GTK_CONTAINER(m_pSimbButton), (!full)?fullwidth_image:halfwidth_image);
    gtk_container_add(GTK_CONTAINER(m_pSimbButton), (full)?fullwidth_image:halfwidth_image);
    gtk_widget_show_all(m_pSimbButton);
}

bool
CGTKWinHandler::createWindows()
{
    GtkWidget *window;
    GtkWidget *chedit;
    GtkWidget *button;
    GtkWidget *box, *box1;
    GtkWidget *img;
    GtkWidget *frame;

    m_pWin = window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 2);
    box = gtk_vbox_new(FALSE, 1);
    gtk_container_add(GTK_CONTAINER(window), box);

    frame = gtk_frame_new(NULL);
    m_CandidataArea = gtk_label_new("这里显示候选字/词");
    gtk_container_add(GTK_CONTAINER(frame), m_CandidataArea);
    gtk_label_set_justify(GTK_LABEL(m_CandidataArea), GTK_JUSTIFY_LEFT);
    gtk_box_pack_end(GTK_BOX(box), frame, TRUE, TRUE, 1);

    frame = gtk_frame_new(NULL);
    m_PreeditArea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(m_PreeditArea), false);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(m_PreeditArea), true);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (m_PreeditArea));
    gtk_text_buffer_set_text(buffer, "请输入拼音", -1);
    GtkTextIter ti;
    gtk_text_buffer_get_iter_at_offset(buffer, &ti, 0);
    gtk_text_buffer_place_cursor(buffer, &ti);

    gtk_container_add(GTK_CONTAINER(frame), m_PreeditArea);
    gtk_box_pack_end(GTK_BOX(box), frame, TRUE, TRUE, 1);

    box1 = gtk_hbox_new(FALSE, 1);
    gtk_box_pack_start(GTK_BOX(box), box1, TRUE, TRUE, 1);

    frame = gtk_frame_new(NULL);
    chedit = gtk_label_new("最佳整句候选结果");
    gtk_container_add(GTK_CONTAINER(frame), chedit);
    gtk_label_set_justify(GTK_LABEL(chedit), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(box1), frame, TRUE, TRUE, 1);

    m_pSimbButton = button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), halfwidth_image);
    gtk_box_pack_end(GTK_BOX(box1), button, FALSE, FALSE, 2);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_click_fullhalf_simbol), mp_view);

    m_pPuncButton = button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), cnpunc_image);
    gtk_box_pack_end(GTK_BOX(box1), button, FALSE, FALSE, 2);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_click_fullhalf_punc), mp_view);

    m_pLangButton = button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), hanzi_image);
    gtk_box_pack_end(GTK_BOX(box1), button, FALSE, FALSE, 2);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_click_cn_en), mp_view);

    m_pCloseButton = button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), tuichu_image);
    gtk_box_pack_end(GTK_BOX(box1), button, FALSE, FALSE, 2);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_click_close), mp_view);

    gtk_window_set_decorated((GtkWindow*)window, FALSE);
    g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(key_press_cb), mp_view);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    g_signal_connect(G_OBJECT(m_pWin), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(m_pWin);

    return true;
}

void
CGTKWinHandler::updatePreedit(const IPreeditString* ppd)
{
    TIConvSrcPtr src = (TIConvSrcPtr)(ppd->string());
    size_t srclen = (ppd->size()+1)*sizeof(TWCHAR);
    char * dst = m_buf;
    size_t dstlen = sizeof(m_buf)-1;
    iconv(m_iconv, &src, &srclen, &dst, &dstlen);

    //gtk_widget_grab_focus(m_PreeditArea);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (m_PreeditArea));
    gtk_text_buffer_set_text (buffer, m_buf, -1);
    GtkTextIter ti;
    gtk_text_buffer_get_iter_at_offset(buffer, &ti, ppd->caret());
    gtk_text_buffer_place_cursor(buffer, &ti);
}

void
CGTKWinHandler::updateCandidates(const ICandidateList* pcl)
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

    TIConvSrcPtr src = (TIConvSrcPtr)(cand_str.c_str());
    size_t srclen = (cand_str.size()+1)*sizeof(TWCHAR);
    char * dst = m_buf;
    size_t dstlen = sizeof(m_buf) - 1;
    iconv(m_iconv, &src, &srclen, &dst, &dstlen);
    gtk_label_set_text(GTK_LABEL(m_CandidataArea), m_buf);
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
