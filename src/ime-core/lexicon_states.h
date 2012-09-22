
#ifndef SUNPY_LEXICON_STATES_H
#define SUNPY_LEXICON_STATES_H

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
struct TLexiconState {
    virtual const CPinyinTrie::TWordIdInfo *getWords(unsigned &num) = 0;
	virtual const double getWeight() = 0;
    void print(std::string prefix) const;
};

/**
 * A list of Lexicon State. Every state may from different
 * starting position. Later, when Fuzzy PinYin are added,
 * more than one state may comes from one starting bone.
 */
typedef std::vector<TLexiconState>    CLexiconStates;

#endif

