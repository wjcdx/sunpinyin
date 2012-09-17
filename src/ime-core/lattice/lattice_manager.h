#ifndef SUNPY_LATTICE_MANAGER_H
#define SUNPY_LATTICE_MANAGER_H


class CLatticeManager {
public:
	virtual void buildLexiconStates(IPySegmentor::TSegmentVec &segments, unsigned rebuildFrom);
	bool buildLatticeStates(int rebuildFrom, unsigned csLevel);
	bool backTracePaths();

	CLattices& getLatticeFrame(int i) {
		if (i >= m_lattices.size())
			return NULL;
		return m_lattices[i];
	}

	unsigned getLatticesCapacity() {
		return m_lattices.capacity();
	}

	LatticeManager ()
		: m_tailIdx(1), m_historyPower(5)
	{
		m_lattice.resize(MAX_LATTICE_LENGTH);
		m_lattice[0].m_latticeStates.add(TLatticeState(-1.0, 0));
	}

    void printLattice();
	void clear();
	void clearFrom(unsigned idx);

	unsigned getTailIdx() { return m_tailIdx };
	void setTailIdx(unsigned idx) { m_tailIdx = idx };

	void setOmitPunct(bool o) { m_bOmitPunct = o; }
    
	void setHistoryPower(unsigned power)
    { m_historyPower = power <= 10 ? power : 3; }

    int getHistoryPower()
    { return m_historyPower; }

	void setFullSymbolForwarding(bool value = true) {
        m_bFullSymbolForwarding = value;
    }
    bool getFullSymbolForwarding() { return m_bFullSymbolForwarding; }
    void setGetFullSymbolOp(CGetFullSymbolOp *op) { m_pGetFullSymbolOp = op; }
    CGetFullSymbolOp& fullSymbolOp() const { return *m_pGetFullSymbolOp; }

    void setFullPunctForwarding(bool value = true) {
        m_bFullPunctForwarding = value;
    }
    bool getFullPunctForwarding() { return m_bFullPunctForwarding; }
    void setGetFullPunctOp(CGetFullPunctOp *op) { m_pGetFullPunctOp = op; }
    CGetFullPunctOp& fullPuncOp() const { return *m_pGetFullPunctOp; }
protected:
	CLattices m_lattices;
    unsigned m_tailIdx;
    bool m_bOmitPunct;

	bool m_bFullSymbolForwarding;
    CGetFullSymbolOp  *m_pGetFullSymbolOp;
    
	bool m_bFullPunctForwarding;
    CGetFullPunctOp *m_pGetFullPunctOp;

    CThreadSlm* m_pModel;
    CPinyinTrie* m_pPinyinTrie;
    CUserDict* m_pUserDict;
    CICHistory* m_pHistory;
    unsigned m_historyPower;
}

#endif
