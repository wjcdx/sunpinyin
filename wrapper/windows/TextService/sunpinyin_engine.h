#ifndef SUNPINYIN_ENGINE_H
#define SUNPINYIN_ENGINE_H

#include <portability.h>
#include <ime-core/imi_winHandler.h>
#include <ime-core/view/imi_view.h>
#include <ime-core/imi_uiobjects.h>
#include <ime-core/options/imi_option_event.h>
#include <ime-core/options/imi_option_keys.h>
#include <ime-core/options/session_factory.h>
#include <xh/input/segment/TXhSyllableSegment.h>
#include "imi_wdx_win.h"

class SunPinyinEngine : public IConfigurable
{
public:
    SunPinyinEngine();
    virtual ~SunPinyinEngine();

	bool process_key_event (CKeyEvent &event);

private:
	CWinHandler		*m_wh;
    CIMIView        *m_pv;
};

#endif