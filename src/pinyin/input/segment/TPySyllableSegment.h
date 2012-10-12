#ifndef SUNPY_PYSYLLABLE_SEGMENT_H
#define SUNPY_PYSYLLABLE_SEGMENT_H

#include "syllable_seg.h"

struct TPySyllableSegment : TSyllableSegment {

	TPySyllableSegment (unsigned syllable,
			  unsigned start,
			  unsigned length)
		: TSyllableSegment(syllable, start, length), m_inner_fuzzy(false)
	{}
	
	void forward(unsigned i, unsigned j);
	
	void _forwardSingleSyllable(unsigned i,
							unsigned j,
							TSyllable syllable,
							bool fuzzy);

	std::vector<unsigned>           m_fuzzy_syllables;
	bool m_inner_fuzzy;
};

#endif
