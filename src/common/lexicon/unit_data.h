#ifndef SUNPY_XH_DATA_H
#define SUNPY_XH_DATA_H

class CUnitData {

    TUnit encode(const char *); 
    const char *decode(TUnit, const char **i = NULL,
			const char **f = NULL);	

}

#endif
