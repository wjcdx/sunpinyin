#ifndef SUNPY_STYLE_POLICY_XH_H
#define SUNPY_STYLE_POLICY_XH_H

#include "ime-core/view/imi_view_xh.h"
#include "imi_option_event.h"

struct CXhStylePolicy : public IConfigurable {
    CIMIView* createView() { return new CIMIXhView(); }

    template<class> friend class SingletonHolder;
protected: ~CXhStylePolicy () {}
};

typedef SingletonHolder<CXhStylePolicy> AXhStylePolicy;

#endif
