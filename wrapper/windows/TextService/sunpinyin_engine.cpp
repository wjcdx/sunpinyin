
#include <portability.h>
#include "sunpinyin_engine.h"
#include "CandidateWindow.h"

SunPinyinEngine::SunPinyinEngine()
{
	CSunpinyinSessionFactory& factory = CSunpinyinSessionFactory::getFactory();

    factory.createXinghuaView();
    factory.createQuanpinView();

	CSunpinyinSessionFactory::EScheme pinyin_scheme = CSunpinyinSessionFactory::XINGHUA;
    //    m_config.get_py_scheme(CSunpinyinSessionFactory::QUANPIN);

    factory.setPinyinScheme(pinyin_scheme);
    if (pinyin_scheme == CSunpinyinSessionFactory::QUANPIN) {
        //update_fuzzy_pinyins();
        //update_correction_pinyins();
        //update_fuzzy_segs();
    } else {
        //update_shuangpin_type();
    }
    //update_user_data_dir();
    //update_punct_mappings();

    //factory.setCandiWindowSize(m_config.get(CONFIG_GENERAL_PAGE_SIZE, 10));
	factory.setCandiWindowSize(10);

    m_pv = factory.createSession();
    if (!m_pv)
        return;

    //m_hotkey_profile = new CHotkeyProfile();
    //factory.initSession(m_hotkey_profile);

	m_wh = new CWinHandler(CCandidateWindow::_rgCandidatesString, CCandidateWindow::_rgPreeditString);
    factory.initSession(m_wh);
}

SunPinyinEngine::~SunPinyinEngine()
{
	if (m_pv) {
        CSunpinyinSessionFactory& factory =
            CSunpinyinSessionFactory::getFactory();
        factory.destroySession(m_pv);
    }

    delete m_wh;
}

bool SunPinyinEngine::process_key_event (CKeyEvent &event)
{
	if (m_pv != NULL) {

		if (event.value < 0x20 && event.value > 0x7E)
            event.value = 0;

        //CKeyEvent key_event (keycode, keyvalue, event->state);
        m_pv->onKeyEvent(event);
    }
    return 1;
}

