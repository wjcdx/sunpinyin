
#include "stdafx.h"

#include "sunpinyin.h"

using namespace std;

CIMIView* init_sunpinyin(CIMIWinHandler *pWinHandler)
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

	py_scheme = 'x';
    do_auto_correction = true;

    CSunpinyinSessionFactory& factory = CSunpinyinSessionFactory::getFactory ();
    switch (py_scheme) {
    case 'q':
        factory.setPinyinScheme (CSunpinyinSessionFactory::QUANPIN);
        break;
    case 's':
        factory.setPinyinScheme (CSunpinyinSessionFactory::SHUANGPIN);
        break;
    case 'x':
        factory.setPinyinScheme (CSunpinyinSessionFactory::XINGHUA);
        break;
    default:
        factory.setPinyinScheme (CSunpinyinSessionFactory::SHUANGPIN);
        return NULL;
    }
    
    if (py_scheme == 'q' && do_auto_correction) {
        AQuanpinSchemePolicy::instance().setAutoCorrecting(true);
        AQuanpinSchemePolicy::instance().setAutoCorrectionPairs(auto_correction_pairs);
    }

    AQuanpinSchemePolicy::instance().setFuzzyForwarding(false, true);
    AQuanpinSchemePolicy::instance().setFuzzyPinyinPairs(fuzzy_pairs, false);
    AQuanpinSchemePolicy::instance().setFuzzySegmentation(true);
    AQuanpinSchemePolicy::instance().setInnerFuzzySegmentation(true);

    factory.setInputStyle(CSunpinyinSessionFactory::XH_STYLE);
    factory.setLanguage(CSunpinyinSessionFactory::SIMPLIFIED_CHINESE_XH);
    factory.setCandiWindowSize(5);

    // test plugin
    // CIMIPluginManager& manager = AIMIPluginManager::instance();
    // manager.loadPlugin("someplugin.py");

    //AShuangpinSchemePolicy::instance().setShuangpinType(ZIGUANG);
    CIMIView *pv = factory.createSession ();
    pv->attachWinHandler(pWinHandler);
    pv->getIC()->setMaxBest(10);

    return pv;
}

void finit_sunpinyin(CIMIView *pv, CIMIWinHandler *pwh)
{
	CSunpinyinSessionFactory& factory = CSunpinyinSessionFactory::getFactory ();
	factory.destroySession (pv);
	delete pwh;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
