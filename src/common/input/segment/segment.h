#ifndef SUNPY_SEGMENT_H
#define SUNPY_SEGMENT_H

struct TSegment {
	TSegment (ESegmentType type = SYLLABLE) : m_type(type),
											  m_inner_fuzzy(0) {}
	TSegment (unsigned syllable,
			  unsigned start,
			  unsigned length,
			  ESegmentType type = SYLLABLE)
		: m_start(start), m_len(length), m_type(type), m_inner_fuzzy(0)
	{ m_syllables.push_back(syllable); }

	bool operator <(const TSegment& other) const {
		if (m_start < other.m_start)
			return true;

		if (m_start == other.m_start)
			return m_len < m_len;

		return false;
	}

	// if segment is a STRING type, m_syllables may contain the string buffer without the '\0'
	std::vector<unsigned>           m_syllables;
	std::vector<unsigned>           m_fuzzy_syllables;
	unsigned m_start        : 16;
	unsigned m_len          : 8;
	ESegmentType m_type         : 7;
	bool m_inner_fuzzy  : 1;

public:
	virtual void forward(unsigned i, unsigned j) = 0;
};

#endif

