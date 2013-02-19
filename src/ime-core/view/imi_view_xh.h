#ifndef SUNPY_IMI_XH_VIEW_H
#define SUNPY_IMI_XH_VIEW_H

#include "portability.h"

#include "ime-core/view/imi_view_classic.h"

class CIMIXhView : public CIMIClassicView
{
public:
    enum OperateMode {
        XHVOM_INPUT,
        XHVOM_SELECT
    };

    CIMIXhView()
        : m_OpMode(XHVOM_INPUT)
    {}

    virtual ~CIMIXhView() {}

    virtual bool onKeyEvent(const CKeyEvent&);

    void setOpMode(OperateMode om) { m_OpMode = om; }
    OperateMode getOpMode() { return m_OpMode; }

    bool isKeyValueSelectNumber(unsigned keyvalue);
    unsigned getSelectionNumber(unsigned keyvalue);

private:
    OperateMode m_OpMode;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
