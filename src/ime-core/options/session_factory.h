#ifndef SUNPY_SESSION_FACTORY_H
#define SUNPY_SESSION_FACTORY_H

class CSunpinyinSessionFactory : private CNonCopyable
{
public:
    typedef enum {
        QUANPIN,
        SHUANGPIN,
        YUEPIN,
        ZHUYIN,
        HUNPIN,
    } EPyScheme;

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
    void setPinyinScheme(EPyScheme pyScheme) { m_pyScheme = pyScheme; }
    void setCandiWindowSize(unsigned size) { m_candiWindowSize = size; }

    CIMIView* createSession(){
        unsigned key = _policiesToKey(m_lang, m_pyScheme, m_inputStyle);
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
        unsigned key = _policiesToKey(m_lang, m_pyScheme, m_inputStyle);
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return;
        profile->destroyProfile(pview);
    }

private:
    CSunpinyinSessionFactory ()
        : m_pyScheme(QUANPIN), m_inputStyle(CLASSIC_STYLE),
          m_lang(SIMPLIFIED_CHINESE),
          m_candiWindowSize(10){
        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE, QUANPIN,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <ASimplifiedChinesePolicy,
                                   AQuanpinSchemePolicy, AClassicStylePolicy> ();

        m_profiles [_policiesToKey(SIMPLIFIED_CHINESE, SHUANGPIN,
                                   CLASSIC_STYLE)] =
            new CSunpinyinProfile <ASimplifiedChinesePolicy,
                                   AShuangpinSchemePolicy, AClassicStylePolicy> ();
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
                            EPyScheme pyScheme,
                            EInputStyle inputStyle)
    { return (lang << 16) + (pyScheme << 8) + inputStyle; }

    std::map <unsigned, ISunpinyinProfile*> m_profiles;

    EPyScheme m_pyScheme;
    EInputStyle m_inputStyle;
    ELanguage m_lang;
    unsigned m_candiWindowSize;
    CHotkeyProfile m_hotkeyProfile;
};

#endif
