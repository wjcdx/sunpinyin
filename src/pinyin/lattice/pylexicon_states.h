#ifndef SUNPY_PYLEXICON_STATES_H
#define SUNPY_PYLEXICON_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "imi_data.h"
#include "pinyin/trie/pinyin_seg.h"

/**
 * This class is used to record lexicon state (pinyin trie nodes)
 * just before a bone. From the bone, it could see when arriving
 * it, how many valid Pinyin Trie Node still could be used to search
 * more words further, and what bone is its starting bone.
 */
struct TPYLexiconState : TLexiconState {
    typedef std::vector<CPinyinTrie::TWordIdInfo> TWordIdInfoVec;

    const CPinyinTrie::TNode   *m_pPYNode;
    TWordIdInfoVec m_words;
    // accumulated syllables, may contain fuzzy syllables
    CSyllables m_syls;
    // accumulated segments,  may contain fuzzy segments
    std::vector<unsigned> m_seg_path;

    unsigned m_start                 : 16;
    unsigned m_num_of_inner_fuzzies  : 14;
    bool m_bFuzzy                : 1;
    bool m_bPinyin               : 1;

    TPYLexiconState (unsigned start,
                   const CPinyinTrie::TNode *pnode,
                   CSyllables& syls,
                   std::vector<unsigned>& seg_path,
                   bool fuzzy = false)
    : m_pPYNode(pnode), m_syls(syls), m_seg_path(seg_path), m_start(start),
        m_num_of_inner_fuzzies(0), m_bFuzzy(fuzzy), m_bPinyin(true) {}

    TPYLexiconState (unsigned start,
                   TWordIdInfoVec &words,
                   CSyllables &syls,
                   std::vector<unsigned>& seg_path,
                   bool fuzzy = false)
    : m_pPYNode(NULL), m_words(words), m_syls(syls), m_seg_path(seg_path),
        m_start(start), m_num_of_inner_fuzzies(0), m_bFuzzy(fuzzy),
        m_bPinyin(true) {}

    TPYLexiconState (unsigned start, unsigned wid)
    : m_pPYNode(NULL), m_start(start), m_bPinyin(false) {
        m_words.push_back(wid);
        m_seg_path.push_back(start);
        m_seg_path.push_back(start + 1);
    }

    const CPinyinTrie::TWordIdInfo *getWords(unsigned &num);

	const double getWeight() {
		return (m_bFuzzy ? 0.5 : 1);
	}

    void print(std::string prefix) const;
};

#endif
