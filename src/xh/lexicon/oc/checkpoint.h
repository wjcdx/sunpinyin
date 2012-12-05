#ifndef SUNPY_LEXICON_CHECKPOINT_H
#define SUNPY_LEXICON_CHECKPOINT_H

typedef std::vector<unsigned> TransVec;
typedef TransVec::iterator TransVecIt;

class CheckPoint {
public:
	CheckPoint(TransVecIt &it, TransVecIt &ite) {
		m_Now = m_Start = it;
		m_End = ite;
	}

	bool operator==(CheckPoint &cp) {
		return notEnd() && cp.notEnd()
            && (m_Start == cp.m_Start)
            && (m_Now == cp.m_Now);
	}

	bool notEnd() {
		return (m_Start != m_End && m_Start != m_End);
	}

	TransVecIt m_Now;
	TransVecIt m_Start;
	TransVecIt m_End;
};

typedef std::list<CheckPoint> CheckPointList;

#endif
