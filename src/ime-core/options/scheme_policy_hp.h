#ifndef SUNPY_SCHEME_POLICY_HP_H
#define SUNPY_SCHEME_POLICY_HP_H

#include "imi_option_event.h"
#include "segmentor.h"
#include "pinyin_data.h"
#include "shuangpin_data.h"
#include "hunpin_seg.h"

struct  CHunpinSchemePolicy : public IConfigurable {
public:
    CHunpinSchemePolicy();

    ISegmentor* createSegmentor(){
        CHunpinSegmentor *pseg = new CHunpinSegmentor(m_shuangpinType);
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
protected: ~CHunpinSchemePolicy () {}

    EShuangpinType m_shuangpinType;
    CGetFuzzySyllablesOp<CPinyinData>   m_getFuzzySyllablesOp;
};

typedef SingletonHolder<CHunpinSchemePolicy> AHunpinSchemePolicy;

#endif
