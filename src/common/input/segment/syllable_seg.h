#ifndef SUNPY_SYLLABLE_SEGMENT_H
#define SUNPY_SYLLABLE_SEGMENT_H

#include "segment.h"

struct TSyllableSegment : TSegment {
	TSyllableSegment (unsigned syllable,
			  unsigned start,
			  unsigned length) : TSegment(syllable, start, length)
	{
		m_type = TSegment::SYLLABLE;
	}

	virtual void forward(unsigned i, unsigned j) {};
};

#endif
