#ifndef SUNPY_STRING_SEGMENT_H
#define SUNPY_STRING_SEGMENT_H

#include "segment.h"

struct TStringSegment : TSegment {
	TStringSegment (unsigned syllable,
			  unsigned start,
			  unsigned length) : TSegment(syllable, start, length)
	{
		m_type = TSegment::STRING;
	}

public:
	void forward(unsigned i, unsigned j);
};

#endif
