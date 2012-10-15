#ifndef SUNPY_XHLATTICE_MANAGER_H
#define SUNPY_XHLATTICE_MANAGER_H

#include "common/input/segment/segment.h"
#include "ime-core/lattice/lattice_manager.h"

class CXhLatticeManager : public CLatticeManager {
public:
	void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom);
};

#endif

