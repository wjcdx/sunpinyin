
#ifndef SUNPY_LEXICON_STATES_H
#define SUNPY_LEXICON_STATES_H

#include <vector>
#include <map>
#include "portability.h"
#include "common/lexicon/trie.h"
#include "common/lexicon/thread/TrieThreadModel.h"
#include "pinyin/input/syllable.h"

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
	virtual const double getWeight() {
		return (m_bFuzzy ? 0.5 : 1);
	}
    virtual void print(std::string prefix) const;
    virtual int getLength() { return m_syls.size(); }

	TLexiconState (unsigned start,
                   const TThreadNode *pnode,
                   CSyllables& syls,
                   std::vector<unsigned>& seg_path)
     : m_start(start), m_pNode(pnode), m_syls(syls), m_seg_path(seg_path)
    {}   

	TLexiconState (unsigned start,
				   TWordIdInfoVec &words,
                   CSyllables& syls,
                   std::vector<unsigned>& seg_path)
		: m_start(start), m_pNode(NULL), m_syls(syls), m_words(words),
		m_seg_path(seg_path) {}

    TLexiconState (unsigned start, unsigned wid)
    : m_start(start), m_pNode(NULL) {
        m_words.push_back(wid);
        m_seg_path.push_back(start);
        m_seg_path.push_back(start + 1); 
    }
	
public:
	//unsigned m_start;
	unsigned m_start                 : 16;
    unsigned m_num_of_inner_fuzzies  : 14; 
    bool m_bFuzzy                : 1;
    bool m_bPinyin               : 1;

	const TThreadNode *m_pNode;

	CSyllables m_syls;

	TWordIdInfoVec m_words;
	// accumulated segments,  may contain fuzzy segments
	std::vector<unsigned> m_seg_path;
};

/**
 * A list of Lexicon State. Every state may from different
 * starting position. Later, when Fuzzy PinYin are added,
 * more than one state may comes from one starting bone.
 */
typedef std::vector<TLexiconState>    CLexiconStates;

#endif

