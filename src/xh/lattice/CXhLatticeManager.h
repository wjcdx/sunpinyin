#ifndef SUNPY_XHLATTICE_MANAGER_H
#define SUNPY_XHLATTICE_MANAGER_H

class CXhLatticeManager : CLatticeManager {
public:
	void buildLexiconStates(IPySegmentor::TSegmentVec &segments, unsigned rebuildFrom);

private:

protected:
}

#endif

