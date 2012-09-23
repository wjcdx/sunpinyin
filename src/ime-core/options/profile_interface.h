#ifndef SUNPY_ISUNPINYIN_PROFILE_H
#define SUNPY_ISUNPINYIN_PROFILE_H

struct ISunpinyinProfile {
    virtual CIMIView* createProfile() = 0;
    virtual void destroyProfile(CIMIView *) = 0;
    virtual ~ISunpinyinProfile () {};
};


#endif