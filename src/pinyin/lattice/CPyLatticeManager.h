#ifndef SUNPY_PYLATTICE_MANAGER_H
#define SUNPY_PYLATTICE_MANAGER_H

#include "common/input/segment/segment.h"
#include "ime-core/lattice/lattice_manager.h"

class CPyLatticeManager : public CLatticeManager {
public:
	void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom);
};

#endif

