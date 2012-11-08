#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <locale.h>
#include <iconv.h>
#include <unistd.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

#include <ime-core/options/imi_option_event.h>
#include <ime-core/options/imi_option_keys.h>
#include <ime-core/options/session_factory.h>
#include "imi_gtkwin.h"
#include "ime-core/imi_plugin.h"

using namespace std;

int main(int argc, char* argv[])
{
    #ifdef DEBUG
        printf("sizef 'long double' %d, size 'of double' %d, size of 'float' %d \n", sizeof(long double), sizeof(double), sizeof(float));

        double test_array[] = { 3.0, 2.0, 1.5, 1.0, 0.5, 0.25, 0.0, -0.0, -0.25, -0.5, -1.0, -2.0, -3.0 };
        char valbuf[256];

        for (int i=0, sz=sizeof(test_array)/sizeof(double); i < sz; ++i) {
            TLongExpFloat lef(test_array[i]);
            lef.toString(valbuf);
            printf("value %lf ==> %s\n", test_array[i], valbuf);
        }
        fflush(stdout);
    #endif

    setlocale(LC_ALL, "zh_CN.UTF-8");
    int opt;
    char py_scheme = 'q';
    bool do_auto_correction = false;

    string_pairs auto_correction_pairs;
    auto_correction_pairs.push_back(make_pair("ign", "ing"));
    auto_correction_pairs.push_back(make_pair("img", "ing"));
    auto_correction_pairs.push_back(make_pair("uei", "ui"));
    auto_correction_pairs.push_back(make_pair("uen", "un"));
    auto_correction_pairs.push_back(make_pair("iou", "iu"));

    string_pairs fuzzy_pairs;
    fuzzy_pairs.push_back(make_pair("s", "sh"));
    fuzzy_pairs.push_back(make_pair("z", "zh"));
    fuzzy_pairs.push_back(make_pair("c", "ch"));

    while ((opt = getopt(argc, argv, "p:c")) != -1) {
        switch (opt) {
        case 'p':
            py_scheme = *optarg;
            break;
        case 'c':
            do_auto_correction = true;
            break;
        }
    }

    gtk_init(&argc, &argv);
    CSunpinyinSessionFactory& factory = CSunpinyinSessionFactory::getFactory ();
    switch (py_scheme) {
    case 'q':
        factory.setPinyinScheme (CSunpinyinSessionFactory::QUANPIN);
        break;
    case 's':
        factory.setPinyinScheme (CSunpinyinSessionFactory::SHUANGPIN);
        break;
    default:
        factory.setPinyinScheme (CSunpinyinSessionFactory::SHUANGPIN);
        break;
    }
    if (py_scheme == 'q' && do_auto_correction) {
        AQuanpinSchemePolicy::instance().setAutoCorrecting(true);
        AQuanpinSchemePolicy::instance().setAutoCorrectionPairs(auto_correction_pairs);
    }

    AQuanpinSchemePolicy::instance().setFuzzyForwarding(false, true);
    AQuanpinSchemePolicy::instance().setFuzzyPinyinPairs(fuzzy_pairs, false);
    AQuanpinSchemePolicy::instance().setFuzzySegmentation(true);
    AQuanpinSchemePolicy::instance().setInnerFuzzySegmentation(true);

    // test plugin
    // CIMIPluginManager& manager = AIMIPluginManager::instance();
    // manager.loadPlugin("someplugin.py");

    //AShuangpinSchemePolicy::instance().setShuangpinType(ZIGUANG);
    CIMIView *pv = factory.createSession ();

    CGTKWinHandler *pwh = new CGTKWinHandler(pv);
    pv->attachWinHandler(pwh);
    pv->getIC()->setMaxBest(3);

    pwh->createWindows();
    gtk_main();

    factory.destroySession (pv);
    delete pwh;

    return 0;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
