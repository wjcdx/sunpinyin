#ifndef SUNPY_SEGMENT_H
#define SUNPY_SEGMENT_H

#include <vector>

struct TSegment {
	enum ESegmentType
	{ SYLLABLE, SYLLABLE_SEP, INVALID, STRING };

	TSegment () {}
	TSegment (unsigned syllable,
			  unsigned start,
			  unsigned length)
		: m_start(start), m_len(length), m_type(SYLLABLE)
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
	unsigned m_start        : 16;
	unsigned m_len          : 8;
	unsigned m_type         : 7;

public:
	virtual void forward(unsigned i, unsigned j);
};

// it requires the segments are sorted by its m_start field
typedef std::vector<TSegment>  TSegmentVec;

#endif

