#ifndef __WIN_DEFS_H__
#define __WIN_DEFS_H__

#define NOMINMAX

#include "warnings.h"
#include <windows.h>

#define snprintf _snprintf

#include <io.h>

#define F_OK    0
#define R_OK    4
#define W_OK    2
#define X_OK    1

#define access  _access


#include <direct.h>
#define mkdir(x, y) _mkdir(x)

void UTF8toANSI(char *ansi, char *utf8);

#endif

