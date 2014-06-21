#ifndef SUNPY_SESSION_FACTORY_H
#define SUNPY_SESSION_FACTORY_H

#include <stdlib.h>
#include "ime-core/utils.h"
#include "profile_class.h"
#include "lang_policy_cn.h"
#include "scheme_policy_qp.h"
#include "scheme_policy_sp.h"
#include "scheme_policy_xh.h"
#include "style_policy_classic.h"

//#define trace() printf("%s(%d): %s\n", __FILE__, __LINE__, __func__)
//#define trace() system("echo abc >> /tmp/ibus")
#define trace() 

class CSunpinyinSessionFactory : private CNonCopyable
{
public:
    typedef enum {
        QUANPIN,
        SHUANGPIN,
        YUEPIN,
        ZHUYIN,
        HUNPIN,
        XINGHUA,
    } EScheme;

    typedef enum {
        MSPY_STYLE,
        CLASSIC_STYLE,
    } EInputStyle;

    typedef enum {
        SIMPLIFIED_CHINESE,
        TRADITIONAL_CHINESE,
    } ELanguage;

public:
    static CSunpinyinSessionFactory& getFactory(){
        static CSunpinyinSessionFactory inst;
        return inst;
    }

    void setLanguage(ELanguage lang) { m_lang = lang; }
    void setInputStyle(EInputStyle inputStyle) { m_inputStyle = inputStyle; }
    void setScheme(EScheme scheme) { m_Scheme = scheme; }
    void setCandiWindowSize(unsigned size) { m_candiWindowSize = size; }

    CIMIView* createSession(){
		unsigned key = _policiesToKey(m_lang, m_Scheme, m_inputStyle);
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return NULL;

        m_view = profile->createView();
        if (!m_view)
            return NULL;

        m_view->setHotkeyProfile(&m_hotkeyProfile);
        m_view->setCandiWindowSize(m_candiWindowSize);
        return m_view;
    }
	
    void destroySession(){
		unsigned key = _policiesToKey(m_lang, m_Scheme, m_inputStyle);
        
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return;
        profile->destroyProfile(m_view);
    }

	void initSession(CHotkeyProfile *prof)
	{
		m_view->setHotkeyProfile(prof);
	}

	void initSession(CIMIWinHandler *wh)
	{
		m_view->attachWinHandler(wh);
	}

private:
    CSunpinyinSessionFactory ()
        : m_Scheme(QUANPIN), m_inputStyle(CLASSIC_STYLE),
          m_lang(SIMPLIFIED_CHINESE),
          m_candiWindowSize(10) {
        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE, QUANPIN,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <ASimplifiedChinesePolicy,
                                   AQuanpinSchemePolicy, AClassicStylePolicy> ();

        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE, SHUANGPIN,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <ASimplifiedChinesePolicy,
                                   AShuangpinSchemePolicy, AClassicStylePolicy> ();
        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE, XINGHUA,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <ASimplifiedChinesePolicy,
                                   AXhSchemePolicy, AClassicStylePolicy> ();
    }

    ~CSunpinyinSessionFactory (){
		destroySession();

        std::map <unsigned, ISunpinyinProfile*>::iterator it = m_profiles.begin();
        std::map <unsigned, ISunpinyinProfile*>::iterator ite = m_profiles.end();

        for (; it != ite; ++it)
            delete it->second;
    }

    ISunpinyinProfile* _getProfile(unsigned key){
        std::map <unsigned, ISunpinyinProfile*>::iterator it = m_profiles.find(
            key);
        if (it != m_profiles.end()) {
            return it->second;
        } else {
            return NULL;
        }
    }

    unsigned _policiesToKey(ELanguage lang,
                            EScheme scheme,
                            EInputStyle inputStyle)
    { return (lang << 16) + (scheme << 8) + inputStyle; }

    std::map <unsigned, ISunpinyinProfile*> m_profiles;

    EScheme m_Scheme;
    EInputStyle m_inputStyle;
    ELanguage m_lang;
    unsigned m_candiWindowSize;
    CHotkeyProfile m_hotkeyProfile;
	CIMIView *m_view;
};

#endif
