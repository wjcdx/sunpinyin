#ifndef SUNPY_LATTICE_MANAGER_H
#define SUNPY_LATTICE_MANAGER_H

#include "common/input/segment/segment.h"
#include "ime-core/ic_history.h"
#include "ime-core/imi_defines.h"
#include "ime-core/imi_funcobjs.h"
#include "lattice.h"

typedef std::vector<unsigned> TPath;
struct GlobalLatticeInfo;

class CLatticeManager {
public:
	virtual void buildLexiconStates(TSegmentVec &segments, unsigned rebuildFrom);
	virtual bool buildLatticeStates(unsigned rebuildFrom, GlobalLatticeInfo &info);
	bool backTracePaths(const std::vector<TLatticeState>& tail_states,
								 int rank, TPath& path, TPath& segmentPath);
	void _transferBetween(unsigned start, unsigned end,
								  TLexiconState* plxst, unsigned wid,
								  double ic = 1.0);

	static CLatticeFrame& getLatticeFrame(unsigned i) {
		if (i >= m_lattice.size())
			return m_lattice[m_lattice.size()-1];
		return m_lattice[i];
	}

	unsigned getLatticesCapacity() {
		return m_lattice.capacity();
	}

	CLatticeManager ()
		: m_pModel(NULL), m_tailIdx(1), m_historyPower(10)
	{
		m_lattice.resize(MAX_LATTICE_LENGTH);
		m_lattice[0].m_latticeStates.add(TLatticeState(-1.0, 0));
	}

	virtual ~CLatticeManager() {}

    void printLattice();
	void clear();
	void clearFrom(unsigned idx);

	unsigned getTailIdx() { return m_tailIdx; }
	void setTailIdx(unsigned idx) { m_tailIdx = idx; }

	void setHistoryPower(unsigned power)
    { m_historyPower = power <= 10 ? power : 3; }

    int getHistoryPower()
    { return m_historyPower; }

	void setLangModel(CThreadSlm *pModel) { m_pModel = pModel; }
	void setInputTrieSource(CInputTrieSource* src) { m_pInputTrieSrc = src; }

private:
	unsigned getClass(unsigned strokes, unsigned cls) {
		unsigned ret = strokes - cls;
		return ret > 31 ? 31 : ret;
	}

public:
	static CLattice m_lattice;
	static double exp2_tbl[32];

protected:
    CThreadSlm* m_pModel;
    CInputTrieSource* m_pInputTrieSrc;
    unsigned m_tailIdx;
    unsigned m_historyPower;
};

#endif
