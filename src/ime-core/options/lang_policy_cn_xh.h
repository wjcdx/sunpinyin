#ifndef SUNPY_LANG_POLICY_SC_XH_H
#define SUNPY_LANG_POLICY_SC_XH_H

#include "lang_policy_cn.h"
#include "imi_option_event.h"
#include <string>

struct CXhSimplifiedChinesePolicy : public CSimplifiedChinesePolicy {
    bool loadResources();
    CIMIContext* createContext();
};

typedef SingletonHolder<CXhSimplifiedChinesePolicy> AXhSimplifiedChinesePolicy;

#endif

