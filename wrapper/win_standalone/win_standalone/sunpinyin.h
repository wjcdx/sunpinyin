#ifndef __SUNPINYIN_H__
#define __SUNPINYIN_H__

#include <stdio.h>
#include <locale.h>

#include <ime-core/options/imi_option_event.h>
#include <ime-core/options/imi_option_keys.h>
#include <ime-core/options/session_factory.h>
#include <xh/input/segment/TXhSyllableSegment.h>
#include "imi_gtkwin.h"

CIMIView *init_sunpinyin(CIMIWinHandler *pWinHandler);
void finit_sunpinyin(CIMIView *pv, CIMIWinHandler *pwh);

#endif
