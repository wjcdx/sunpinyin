#ifndef SUNPY_PYLATTICE_MANAGER_H
#define SUNPY_PYLATTICE_MANAGER_H

#include "segment.h"
#include "lattice_manager.h"

class CPyLatticeManager : CLatticeManager {
public:
	void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom);
};

#endif

