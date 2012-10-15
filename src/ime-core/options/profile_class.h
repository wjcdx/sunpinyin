#ifndef SUNPY_CSUNPINYIN_PROFILE_H
#define SUNPY_CSUNPINYIN_PROFILE_H

#include "profile_interface.h"
#include "common/input/segmentor.h"

template <class LanguagePolicy, class SchemePolicy,
          class InputStylePolicy>
class CSunpinyinProfile : public ISunpinyinProfile
{
public:
    CSunpinyinProfile () : m_langPolicy(LanguagePolicy::instance()),
                           m_SchemePolicy(SchemePolicy::instance()),
                           m_inputStylePolicy(InputStylePolicy::instance())
    {}

    /* profile by itself is a profile, so we are creating a session here? */
    virtual CIMIView* createProfile(){
        if (!m_langPolicy.loadResources())
            return NULL;

        ISegmentor* pseg = m_SchemePolicy.createSegmentor();
        if (pseg == NULL)
            return NULL;

        CIMIContext *pic = m_langPolicy.createContext();
        CIMIView* pview = m_inputStylePolicy.createView();
        pview->attachIC(pic);
        pview->setSegmentor(pseg);

        return pview;
    }

    virtual void destroyProfile(CIMIView* pview){
        if (pview) {
            LanguagePolicy::instance().destroyContext(pview->getIC());
            delete pview->getSegmentor();
            delete pview;
        }
    }
private:
    typename LanguagePolicy::Type & m_langPolicy;
    typename SchemePolicy::Type & m_SchemePolicy;
    typename InputStylePolicy::Type & m_inputStylePolicy;
};

#endif
