#ifndef SUNPY_SCHEME_POLICY_XH_H
#define SUNPY_SCHEME_POLICY_XH_H

#include "imi_option_event.h"
#include "common/input/segmentor.h"
#include "xh/input/xh_seg.h"

struct CXhSchemePolicy : IConfigurable {
public:

    ISegmentor* createSegmentor(){
		CXhSegmentor *pseg = new CXhSegmentor();
        return pseg;
    }

    virtual bool onConfigChanged(const COptionEvent& event);

    template<class> friend class SingletonHolder;

protected: ~CXhSchemePolicy () {}
};

typedef SingletonHolder<CXhSchemePolicy> AXhSchemePolicy;


#endif
