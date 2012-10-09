#ifndef SUNPY_INVALID_SEGMENT_H
#define SUNPY_INVALID_SEGMENT_H

#include "string_seg.h"

struct TInvalidSegment : TStringSegment {
	TInvalidSegment (unsigned syllable,
			  unsigned start,
			  unsigned length) : TStringSegment(syllable, start, length)
	{
		m_type = TSegment::INVALID;
	}
};

#endif
