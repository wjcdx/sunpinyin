#ifndef SUNPY_LATTICE_MANAGER_H
#define SUNPY_LATTICE_MANAGER_H

#include "segment.h"
#include "userdict.h"
#include "ic_history.h"
#include "imi_funcobjs.h"
#include "lattice.h"

typedef std::vector<unsigned> TPath;

class CLatticeManager {
public:
	virtual void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom);
	bool buildLatticeStates(unsigned rebuildFrom, unsigned csLevel);
	bool backTracePaths(const std::vector<TLatticeState>& tail_states,
								 int rank, TPath& path, TPath& segmentPath);

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
	void _clearFrom(unsigned idx);
	void clearFrom(unsigned idx);

	unsigned getTailIdx() { return m_tailIdx; }
	void setTailIdx(unsigned idx) { m_tailIdx = idx; }

	void setHistoryPower(unsigned power)
    { m_historyPower = power <= 10 ? power : 3; }

    int getHistoryPower()
    { return m_historyPower; }

    void setUserDict(CUserDict *pUserDict) { m_pUserDict = pUserDict; }
    CUserDict* getUserDict() { return m_pUserDict; }

    void setHistoryMemory(CICHistory *phm) { m_pHistory = phm; }
    CICHistory * getHistoryMemory() { return m_pHistory; }


public:
	static CLattice m_lattice;
    static CTrie* m_pTrie;
    static CUserDict* m_pUserDict;
    static CThreadSlm* m_pModel;
    static CICHistory* m_pHistory;

protected:
    unsigned m_tailIdx;
    unsigned m_historyPower;
};

#endif
