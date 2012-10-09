
#ifndef SUNPY_LEXICON_STATES_H
#define SUNPY_LEXICON_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "trie.h"
#include "TrieThreadModel.h"

using namespace TrieThreadModel;

/**
 * This class is used to record lexicon state (pinyin trie nodes)
 * just before a bone. From the bone, it could see when arriving
 * it, how many valid Pinyin Trie Node still could be used to search
 * more words further, and what bone is its starting bone.
 */
struct TLexiconState {
	typedef std::vector<TWordIdInfo> TWordIdInfoVec;
public:
    virtual const TWordIdInfo *getWords(unsigned &num);
	virtual const double getWeight();
    virtual void print(std::string prefix) const;

    TLexiconState (unsigned start, unsigned wid)
    : m_start(start), m_pNode(NULL) {
        m_words.push_back(wid);
        m_seg_path.push_back(start);
        m_seg_path.push_back(start + 1); 
    }
	
public:
	TThreadNode *m_pNode;
	TWordIdInfoVec m_words;
	// accumulated segments,  may contain fuzzy segments
	std::vector<unsigned> m_seg_path;
	unsigned m_start;
};

/**
 * A list of Lexicon State. Every state may from different
 * starting position. Later, when Fuzzy PinYin are added,
 * more than one state may comes from one starting bone.
 */
typedef std::vector<TLexiconState>    CLexiconStates;

#endif

