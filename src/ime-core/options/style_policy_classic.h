#ifndef SUNPY_STYLE_POLICY_CLASSIC_H
#define SUNPY_STYLE_POLICY_CLASSIC_H

#include "imi_view.h"
#include "imi_view_classic.h"
#include "imi_option_event.h"

struct CClassicStylePolicy : public IConfigurable {
    CIMIView* createView() { return new CIMIClassicView(); }

    template<class> friend class SingletonHolder;
protected: ~CClassicStylePolicy () {}
};

typedef SingletonHolder<CClassicStylePolicy> AClassicStylePolicy;

#endif
