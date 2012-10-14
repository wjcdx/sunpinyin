#ifndef SUNPY_XHLATTICE_MANAGER_H
#define SUNPY_XHLATTICE_MANAGER_H

#include "segment.h"
#include "lattice_manager.h"

class CXhLatticeManager : public CLatticeManager {
public:
	void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom);
};

#endif

