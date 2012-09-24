#include "string_seg.h"

void
TStringSegment::forward(unsigned i, unsigned j) 
{
	strbuf = this.m_syllables;
    if (strbuf.size() == 1) {
        unsigned ch = strbuf[0];       
        if (ispunct(ch)) {
            _forwardPunctChar(i, j, ch);   
        } else {
            _forwardOrdinaryChar(i, j, ch);
        }
    } else{
        CLatticeFrame &fr = this.getLatticeFrame(j); 
        fr.m_wstr.assign(strbuf.begin(), strbuf.end());
        fr.m_lexiconStates.push_back(TLexiconState(i, 0));
    }
}

static void
_forwardPunctChar(unsigned i, unsigned j, unsigned ch)
{
    CLatticeFrame &fr = this.getLatticeFrame(j);

    wstring wstr;
    unsigned wid = 0;

    if (m_pGetFullPunctOp) {
        if (m_bFullPunctForwarding && !m_bOmitPunct) {
            wstr = (*m_pGetFullPunctOp)(ch);
            wid = m_pPinyinTrie->getSymbolId(wstr);
        }
    }

    fr.m_type = CLatticeFrame::PUNC;

    if (!wstr.empty())
        fr.m_wstr = wstr;
    else
        fr.m_wstr.push_back(ch);

    fr.m_lexiconStates.push_back(TLexiconState(i, wid));
}

static void
_forwardOrdinaryChar(unsigned i, unsigned j, unsigned ch)
{
    CLatticeFrame &fr = this.getLatticeFrame(j);

    wstring wstr;
    unsigned wid = 0;

    if (m_pGetFullSymbolOp) {
        wstr = (*m_pGetFullSymbolOp)(ch);
        wid = m_pPinyinTrie->getSymbolId(wstr);

        if (!m_bFullSymbolForwarding)
            wstr.clear();
    }

    fr.m_type = wid ? CLatticeFrame::SYMBOL : CLatticeFrame::ASCII;

    if (!wstr.empty())
        fr.m_wstr = wstr;
    else
        fr.m_wstr.push_back(ch);

    fr.m_lexiconStates.push_back(TLexiconState(i, wid));
}

