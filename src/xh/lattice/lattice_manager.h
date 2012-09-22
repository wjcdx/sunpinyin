#ifndef SUNPY_PYLATTICE_MANAGER_H
#define SUNPY_PYLATTICE_MANAGER_H

class CXhLatticeManager : CLatticeManager {
public:
	void buildLexiconStates(IPySegmentor::TSegmentVec &segments, unsigned rebuildFrom);

private:

protected:
}

#endif

