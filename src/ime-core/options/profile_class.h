#ifndef SUNPY_CSUNPINYIN_PROFILE_H
#define SUNPY_CSUNPINYIN_PROFILE_H

template <class LanguagePolicy, class PinyinSchemePolicy,
          class InputStylePolicy>
class CSunpinyinProfile : public ISunpinyinProfile
{
public:
    CSunpinyinProfile () : m_langPolicy(LanguagePolicy::instance()),
                           m_pySchemePolicy(PinyinSchemePolicy::instance()),
                           m_inputStylePolicy(InputStylePolicy::instance())
    {}

    /* profile by itself is a profile, so we are creating a session here? */
    virtual CIMIView* createProfile(){
        if (!m_langPolicy.loadResources())
            return NULL;

        IPySegmentor* pseg = m_pySchemePolicy.createPySegmentor();
        if (pseg == NULL)
            return NULL;

        CIMIContext *pic = m_langPolicy.createContext();
        CIMIView* pview = m_inputStylePolicy.createView();
        pview->attachIC(pic);
        pview->setPySegmentor(pseg);

        return pview;
    }

    virtual void destroyProfile(CIMIView* pview){
        if (pview) {
            LanguagePolicy::instance().destroyContext(pview->getIC());
            delete pview->getPySegmentor();
            delete pview;
        }
    }
private:
    typename LanguagePolicy::Type & m_langPolicy;
    typename PinyinSchemePolicy::Type & m_pySchemePolicy;
    typename InputStylePolicy::Type & m_inputStylePolicy;
};

#endif
