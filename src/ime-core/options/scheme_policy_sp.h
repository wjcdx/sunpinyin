#ifndef SUNPY_SCHEME_POLICY_SP_H
#define SUNPY_SCHEME_POLICY_SP_H

struct CShuangpinSchemePolicy : public IConfigurable {
public:
    CShuangpinSchemePolicy();

    IPySegmentor* createPySegmentor(){
        CShuangpinSegmentor *pseg = new CShuangpinSegmentor(m_shuangpinType);
        pseg->setGetFuzzySyllablesOp(&m_getFuzzySyllablesOp);
        return pseg;
    }

    void setShuangpinType(EShuangpinType t) { m_shuangpinType = t; }

    void setFuzzyForwarding(bool enable_fuzzies = true,
                            bool enable_simpler_initials = true){
        m_getFuzzySyllablesOp.setEnableFuzzies(enable_fuzzies);
        m_getFuzzySyllablesOp.setEnableSimplerInitials(enable_simpler_initials);
    }

    void clearFuzzyPinyinPairs()
    { m_getFuzzySyllablesOp.clearFuzzyMap(); }

    void setFuzzyPinyinPairs(const string_pairs& pairs, bool duplex = true)
    { m_getFuzzySyllablesOp.initFuzzyMap(pairs, duplex); }

    virtual bool onConfigChanged(const COptionEvent& event);

    template<class> friend class SingletonHolder;
protected: ~CShuangpinSchemePolicy () {}

    EShuangpinType m_shuangpinType;
    CGetFuzzySyllablesOp<CPinyinData>   m_getFuzzySyllablesOp;
};

typedef SingletonHolder<CShuangpinSchemePolicy> AShuangpinSchemePolicy;


#endif
