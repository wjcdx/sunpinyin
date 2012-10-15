#ifndef SUNPY_PYLEXICON_STATES_H
#define SUNPY_PYLEXICON_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "ime-core/lattice/lexicon_states.h"

/**
 * This class is used to record lexicon state (pinyin trie nodes)
 * just before a bone. From the bone, it could see when arriving
 * it, how many valid Pinyin Trie Node still could be used to search
 * more words further, and what bone is its starting bone.
 */
struct TPyLexiconState : TLexiconState {
    //unsigned m_start                 : 16;
    unsigned m_num_of_inner_fuzzies  : 14;
    bool m_bFuzzy                : 1;
    bool m_bPinyin               : 1;

    TPyLexiconState (unsigned start,
                   const TThreadNode *pnode,
                   CSyllables& syls,
                   std::vector<unsigned>& seg_path,
                   bool fuzzy = false)
		: TLexiconState(start, pnode, syls, seg_path),
			m_num_of_inner_fuzzies(0), m_bFuzzy(fuzzy), m_bPinyin(true) 
	{
	}

    TPyLexiconState (unsigned start,
                   TWordIdInfoVec &words,
                   CSyllables &syls,
                   std::vector<unsigned>& seg_path,
                   bool fuzzy = false)
		: TLexiconState(start, NULL, syls, seg_path),
			m_num_of_inner_fuzzies(0),
			m_bFuzzy(fuzzy), m_bPinyin(true)
	{
		m_words = words;
	}

    TPyLexiconState (unsigned start, unsigned wid)
    	: TLexiconState(start, wid), m_bPinyin(false)
	{}

    const TWordIdInfo *getWords(unsigned &num);

	const double getWeight() {
		return (m_bFuzzy ? 0.5 : 1);
	}

    void print(std::string prefix) const;
    int getLength() { return m_syls.size() - m_num_of_inner_fuzzies; }
};

#endif

