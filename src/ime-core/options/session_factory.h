#ifndef SUNPY_SESSION_FACTORY_H
#define SUNPY_SESSION_FACTORY_H

#include <stdlib.h>
#include "ime-core/utils.h"
#include "profile_class.h"
#include "lang_policy_cn.h"
#include "lang_policy_cn_xh.h"
#include "scheme_policy_qp.h"
#include "scheme_policy_sp.h"
#include "scheme_policy_xh.h"
#include "style_policy_classic.h"
#include "style_policy_xh.h"

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
#if 1
        unsigned key = _policiesToKey(m_lang, m_Scheme, m_inputStyle);
        
		std::map <CIMIView *, unsigned>::iterator vit = m_views.begin();
        std::map <CIMIView *, unsigned>::iterator vite = m_views.end();
		for (; vit != vite; vit++) {
			if (vit->second == key) {
				return vit->first;
			}
		}

		CIMIView *pview = createView();
        return pview;
#else
		CIMIView *pv = nextView(NULL);
		return nextView(pv);
#endif
    }

	CIMIView *createXinghuaView() {
		m_Scheme = XINGHUA;
		m_inputStyle = XH_STYLE;
		m_lang = SIMPLIFIED_CHINESE_XH;
		m_candiWindowSize = 10;
		return createView();
	}

	CIMIView *createQuanpinView() {
		m_Scheme = QUANPIN;
		m_inputStyle = CLASSIC_STYLE;
		m_lang = SIMPLIFIED_CHINESE_PY;
		m_candiWindowSize = 10;
		return createView();
	}

	CIMIView *createView() {
trace();
        unsigned key = _policiesToKey(m_lang, m_Scheme, m_inputStyle);
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return NULL;

        CIMIView *pview = profile->createView();
        if (!pview)
            return NULL;

        pview->setHotkeyProfile(&m_hotkeyProfile);
        pview->setCandiWindowSize(m_candiWindowSize);
		m_views[pview] = key;
        return pview;
	}

	CIMIView *nextView(CIMIView *now) {
trace();
        std::map <CIMIView *, unsigned>::iterator vitb = m_views.begin();
        std::map <CIMIView *, unsigned>::iterator vite = m_views.end();
        std::map <CIMIView *, unsigned>::iterator vit = m_views.find(now);

		if (now == NULL) {
			if (vitb != vite)
				return vitb->first;
			else
				return NULL;
		}

		if (vit != vite) {
			if (++vit != vite) {
				return vit->first;
			}
		}
		return vitb->first;
	}

    void destroySession(CIMIView* pview){
		unsigned key = 0;
        std::map <CIMIView *, unsigned>::iterator vit = m_views.find(pview);
		if (vit == m_views.end()) {
			return;
		}
		key = vit->second;
        ISunpinyinProfile *profile = _getProfile(key);
        if (!profile)
            return;
        profile->destroyProfile(pview);
		m_views.erase(vit);
    }

	void initSession(CHotkeyProfile *prof)
	{
        std::map <CIMIView *, unsigned>::iterator vit = m_views.begin();
        std::map <CIMIView *, unsigned>::iterator vite = m_views.end();

		for (; vit != vite; vit++) {
			CIMIView *pv = vit->first;
			pv->setHotkeyProfile(prof);
		}
	}

	void initSession(CIMIWinHandler *wh)
	{
        std::map <CIMIView *, unsigned>::iterator vit = m_views.begin();
        std::map <CIMIView *, unsigned>::iterator vite = m_views.end();

		for (; vit != vite; vit++) {
			CIMIView *pv = vit->first;
			pv->attachWinHandler(wh);
		}
	}

private:
    CSunpinyinSessionFactory ()
        : m_Scheme(QUANPIN), m_inputStyle(CLASSIC_STYLE),
          m_lang(SIMPLIFIED_CHINESE_PY),
          m_candiWindowSize(10) {
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
                                   AXhSchemePolicy, AClassicStylePolicy> ();
    }

    ~CSunpinyinSessionFactory (){
        std::map <CIMIView *, unsigned>::iterator vit;
		for (vit = m_views.begin(); vit != m_views.end(); vit = m_views.begin()) {
			destroySession(vit->first);
		}

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

	std::map<CIMIView *, unsigned> m_views;
};

#endif
