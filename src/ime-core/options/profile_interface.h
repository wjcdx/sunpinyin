#ifndef SUNPY_ISUNPINYIN_PROFILE_H
#define SUNPY_ISUNPINYIN_PROFILE_H

#include "ime-core/view/imi_view.h"

struct ISunpinyinProfile {
    virtual CIMIView* createView() = 0;
    virtual void destroyProfile(CIMIView *) = 0;
    virtual ~ISunpinyinProfile () {};
};


#endif
