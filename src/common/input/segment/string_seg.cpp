#include "string_seg.h"
#include "ime-core/lattice/lattice_manager.h"
#include "ime-core/helper/CFullCharManager.h"
#include "ime-core/helper/CInputTrieSource.h"
#include "ime-core/lattice/lexicon_states.h"

static void
_forwardPunctChar(unsigned i, unsigned j, unsigned ch)
{
    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);

    wstring wstr;
    unsigned wid = 0;

    if (CFullCharManager::m_pGetFullPunctOp) {
        if (CFullCharManager::m_bFullPunctForwarding && !CFullCharManager::m_bOmitPunct) {
            wstr = (*CFullCharManager::m_pGetFullPunctOp)(ch);
            wid = CInputTrieSource::m_pTrie->getSymbolId(wstr);
        }
    }

    fr.m_type = CLatticeFrame::PUNC;

    if (!wstr.empty())
        fr.m_wstr = wstr;
    else
        fr.m_wstr.push_back(ch);

    fr.m_lexiconStates.push_back(new TLexiconState(i, wid));
}

static void
_forwardOrdinaryChar(unsigned i, unsigned j, unsigned ch)
{
    CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);

    wstring wstr;
    unsigned wid = 0;

    if (CFullCharManager::m_pGetFullSymbolOp) {
        wstr = (*CFullCharManager::m_pGetFullSymbolOp)(ch);
        wid = CInputTrieSource::m_pTrie->getSymbolId(wstr);

        if (!CFullCharManager::m_bFullSymbolForwarding)
            wstr.clear();
    }

    fr.m_type = wid ? CLatticeFrame::SYMBOL : CLatticeFrame::ASCII;

    if (!wstr.empty())
        fr.m_wstr = wstr;
    else
        fr.m_wstr.push_back(ch);

    fr.m_lexiconStates.push_back(new TLexiconState(i, wid));
}

void
TStringSegment::forward(unsigned i, unsigned j) 
{
	const std::vector<unsigned>& strbuf = this->m_syllables;
    if (strbuf.size() == 1) {
        unsigned ch = strbuf[0];       
        if (ispunct(ch)) {
            _forwardPunctChar(i, j, ch);   
        } else {
            _forwardOrdinaryChar(i, j, ch);
        }
    } else {
        CLatticeFrame &fr = CLatticeManager::getLatticeFrame(j);
		fr.m_type = CLatticeFrame::SYMBOL;
        fr.m_wstr.assign(strbuf.begin(), strbuf.end());
        fr.m_lexiconStates.push_back(new TLexiconState(i, 0));
    }
}

