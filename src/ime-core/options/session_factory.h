#ifndef SUNPY_SESSION_FACTORY_H
#define SUNPY_SESSION_FACTORY_H

#include "utils.h"
#include "profile_class.h"
#include "lang_policy_cn.h"
#include "lang_policy_cn_xh.h"
#include "scheme_policy_qp.h"
#include "scheme_policy_sp.h"
#include "scheme_policy_xh.h"
#include "style_policy_classic.h"
#include "style_policy_xh.h"

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
        XH_STYLE,
    } EInputStyle;

    typedef enum {
        SIMPLIFIED_CHINESE_PY,
        SIMPLIFIED_CHINESE_XH,
        TRADITIONAL_CHINESE,
    } ELanguage;

public:
    static CSunpinyinSessionFactory& getFactory(){
        static CSunpinyinSessionFactory inst;
        return inst;
    }

    void setLanguage(ELanguage lang) { m_lang = lang; }
    void setInputStyle(EInputStyle inputStyle) { m_inputStyle = inputStyle; }
    void setPinyinScheme(EScheme scheme) { m_Scheme = scheme; }
    void setCandiWindowSize(unsigned size) { m_candiWindowSize = size; }

    CIMIView* createSession(){
        unsigned key = _policiesToKey(m_lang, m_Scheme, m_inputStyle);
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return NULL;

        CIMIView *pview = profile->createProfile();
        if (!pview)
            return NULL;

        pview->setHotkeyProfile(&m_hotkeyProfile);
        pview->setCandiWindowSize(m_candiWindowSize);
        return pview;
    }

    void destroySession(CIMIView* pview){
        unsigned key = _policiesToKey(m_lang, m_Scheme, m_inputStyle);
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return;
        profile->destroyProfile(pview);
    }

private:
    CSunpinyinSessionFactory ()
        : m_Scheme(QUANPIN), m_inputStyle(CLASSIC_STYLE),
          m_lang(SIMPLIFIED_CHINESE_PY),
          m_candiWindowSize(10){
        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE_PY, QUANPIN,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <APySimplifiedChinesePolicy,
                                   AQuanpinSchemePolicy, AClassicStylePolicy> ();

        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE_PY, SHUANGPIN,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <APySimplifiedChinesePolicy,
                                   AShuangpinSchemePolicy, AClassicStylePolicy> ();
        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE_XH, XINGHUA,
                                   XH_STYLE)] =
            new CSunpinyinProfile <AXhSimplifiedChinesePolicy,
                                   AXhSchemePolicy, AXhStylePolicy> ();
    }

    ~CSunpinyinSessionFactory (){
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
};

#endif
