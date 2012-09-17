#ifndef SUNPY_PYLATTICE_MANAGER_H
#define SUNPY_PYLATTICE_MANAGER_H

class CPYLatticeManager : CLatticeManager {
public:
	void buildLexiconStates(IPySegmentor::TSegmentVec &segments, unsigned rebuildFrom);
	bool buildLatticeStates(int rebuildFrom);
    

private:
    void _forwardSyllables(unsigned i, unsigned j,
                           const IPySegmentor::TSegment& seg);
    void _forwardSingleSyllable(unsigned i, unsigned j, TSyllable syllable,
                                const IPySegmentor::TSegment& seg,
                                bool fuzzy = false);
    void _forwardSyllableSep(unsigned i, unsigned j);
    void _forwardString(unsigned i, unsigned j,
                        const std::vector<unsigned>& strbuf);
    void _forwardPunctChar(unsigned i, unsigned j, unsigned ch);
    void _forwardOrdinaryChar(unsigned i, unsigned j, unsigned ch);
    void _forwardTail(unsigned i, unsigned j);

protected:
}

#endif

