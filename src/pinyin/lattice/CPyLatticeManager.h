#ifndef SUNPY_PYLATTICE_MANAGER_H
#define SUNPY_PYLATTICE_MANAGER_H

class CPyLatticeManager : CLatticeManager {
public:
	void buildLexiconStates(IPySegmentor::TSegmentVec &segments, unsigned rebuildFrom);
}

#endif

