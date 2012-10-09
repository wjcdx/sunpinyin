#ifndef SUNPY_SEPERATOR_SEGMENT_H
#define SUNPY_SEPERATOR_SEGMENT_H

#include "segment.h"

struct TSeperatorSegment : TSegment {
	TSeperatorSegment (unsigned syllable,
			  unsigned start,
			  unsigned length) : TSegment(syllable, start, length)
	{
		m_type = TSegment::SYLLABLE_SEP;
	}
	void forward(unsigned i, unsigned j);
};

#endif
