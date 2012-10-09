#ifndef SUNPY_LATTICE_MANAGER_H
#define SUNPY_LATTICE_MANAGER_H

#include "segment.h"
#include "userdict.h"
#include "ic_history.h"
#include "imi_funcobjs.h"
#include "lattice.h"

class CLatticeManager {
public:
	virtual void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom) = 0;
	bool buildLatticeStates(int rebuildFrom, unsigned csLevel);
	bool backTracePaths();

	static CLatticeFrame& getLatticeFrame(unsigned i) {
		if (i >= m_lattice.size())
			return m_lattice[m_lattice.size()-1];
		return m_lattice[i];
	}

	unsigned getLatticesCapacity() {
		return m_lattice.capacity();
	}

	CLatticeManager ()
		: m_tailIdx(1), m_historyPower(5)
	{
		m_lattice.resize(MAX_LATTICE_LENGTH);
		m_lattice[0].m_latticeStates.add(TLatticeState(-1.0, 0));
	}

    void printLattice();
	void clear();
	void clearFrom(unsigned idx);

	unsigned getTailIdx() { return m_tailIdx; }
	void setTailIdx(unsigned idx) { m_tailIdx = idx; }

	void setHistoryPower(unsigned power)
    { m_historyPower = power <= 10 ? power : 3; }

    int getHistoryPower()
    { return m_historyPower; }


public:
	static CLattice m_lattice;
    static CTrie* m_pTrie;

protected:
    unsigned m_tailIdx;

    CThreadSlm* m_pModel;
    CUserDict* m_pUserDict;
    CICHistory* m_pHistory;
    unsigned m_historyPower;
};

#endif
