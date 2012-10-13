#ifndef SUNPY_IMI_XH_VIEW_H
#define SUNPY_IMI_XH_VIEW_H

#include "portability.h"

#include "imi_view_classic.h"

class CIMIXhView : public CIMIClassicView
{
public:
    CIMIXhView();
    virtual ~CIMIXhView();

    virtual bool onKeyEvent(const CKeyEvent&);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
