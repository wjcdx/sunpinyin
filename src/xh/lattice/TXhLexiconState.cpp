#include "trie.h"
#include "TXhLexiconState.h"
#include <algorithm>

const TWordIdInfo*
TXhLexiconState::getWords(unsigned &num)
{
    num = 0;

    if (!m_words.empty()) {
        num = m_words.size();
        return &m_words[0];
    }

    if (m_pNode) {
        num = m_pNode->m_nWordId;
        return m_pNode->getWordIdPtr();
    }

    return NULL;
}

void
TXhLexiconState::print(std::string prefix) const
{
    printf("%s", prefix.c_str());
    printf("from frame[%d] ", m_start);
/*
    if (m_bPinyin) {
        printf("%sdict ", m_pNode ? "sys" : "usr");
        if (!m_syls.empty()) {
            printf("pinyin: ");
            CSyllables::const_iterator it = m_syls.begin();
            for (; it != m_syls.end(); ++it)
                printf("%x:%x:%x ", it->initial, it->final, it->tone);
        }

        printf("seg_ranges: (");
        for (std::vector<unsigned>::const_iterator it = m_seg_path.begin();
             it != m_seg_path.end();
             ++it)
            printf("%d ", *it);
        printf(")");
    } else
	*/
	{
        printf("word id ");
        printf("%d", m_words.front().m_id);
    }

    printf("\n");
}

