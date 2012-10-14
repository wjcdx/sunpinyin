#ifndef SUNPY_SCHEME_POLICY_QP_H
#define SUNPY_SCHEME_POLICY_QP_H

#include "imi_option_event.h"
#include "segmentor.h"
#include "pinyin_data.h"
#include "pinyin_seg.h"

struct CQuanpinSchemePolicy : IConfigurable {
public:

    ISegmentor* createSegmentor(){
        CQuanpinSegmentor *pseg = new CQuanpinSegmentor();
        pseg->setGetFuzzySyllablesOp(&m_getFuzzySyllablesOp);
        pseg->setGetCorrectionPairOp(&m_getCorrectionPairOp);
        pseg->setGetFuzzySegmentsOp(&m_getFuzzySegmentsOp);
        return pseg;
    }

    void setFuzzyForwarding(bool enable_fuzzies = true,
                            bool enable_simpler_initials = true){
        m_getFuzzySyllablesOp.setEnableFuzzies(enable_fuzzies);
        m_getFuzzySyllablesOp.setEnableSimplerInitials(enable_simpler_initials);
    }

    void clearFuzzyPinyinPairs()
    { m_getFuzzySyllablesOp.clearFuzzyMap(); }

    void setFuzzyPinyinPairs(const string_pairs& pairs, bool duplex = true)
    { m_getFuzzySyllablesOp.initFuzzyMap(pairs, duplex); }

    void setAutoCorrecting(bool v = true)
    { m_getCorrectionPairOp.setEnable(v); }

    void setAutoCorrectionPairs(const string_pairs& pairs)
    { m_getCorrectionPairOp.setCorrectionPairs(pairs); }

    void setFuzzySegmentation(bool v = true)
    { m_getFuzzySegmentsOp.setEnable(v); }

    void setInnerFuzzySegmentation(bool v = true)
    { m_getFuzzySegmentsOp.setInnerFuzzyEnable(v); }

    virtual bool onConfigChanged(const COptionEvent& event);

    template<class> friend class SingletonHolder;

protected: ~CQuanpinSchemePolicy () {}

    CGetFuzzySyllablesOp<CPinyinData>   m_getFuzzySyllablesOp;
    CGetCorrectionPairOp m_getCorrectionPairOp;
    CGetFuzzySegmentsOp m_getFuzzySegmentsOp;
};

typedef SingletonHolder<CQuanpinSchemePolicy> AQuanpinSchemePolicy;


#endif
