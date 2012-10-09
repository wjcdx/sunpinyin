#ifndef SUNPY_PYLEXICON_STATES_H
#define SUNPY_PYLEXICON_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "lexicon_states.h"

struct TXhLexiconState : TLexiconState {

    const TWordIdInfo *getWords(unsigned &num);
	const double getWeight() {
		return (1);
	}

    void print(std::string prefix) const;
};

#endif

