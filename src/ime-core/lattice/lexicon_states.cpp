#include "lexicon_states.h"
#include "common/lexicon/thread/TrieThreadModel.h"

const TrieThreadModel::TWordIdInfo*
TLexiconState::getWords(unsigned &num)
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
TLexiconState::print(std::string prefix) const
{
    printf("%s", prefix.c_str());
    printf("from frame[%d] ", m_start);

	printf("word id ");
	printf("%d", m_words.front().m_id);

    printf("\n");
}

