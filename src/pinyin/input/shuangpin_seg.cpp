#include <cassert>
#include <climits>
#include "shuangpin_seg.h"
#include "common/input/segment/segment_types.h"

CShuangpinData CShuangpinSegmentor::s_shpData;

extern void
erase_segs(TSegmentVec& segs,
    TSegmentVec::iterator itb,
    TSegmentVec::iterator ite);

extern void
erase_seg(TSegmentVec& segs, TSegmentVec::iterator it);

extern void
erase_segs(TSegmentVec& segs);

CShuangpinSegmentor::CShuangpinSegmentor (EShuangpinType shpType)
    : m_updatedFrom(0), m_nAlpha(0), m_hasInvalid(false), m_nLastValidPos(0)
{
    m_segs.reserve(32);
    m_pystr.reserve(32);
    s_shpData.setShuangpinType(shpType);
}

CShuangpinSegmentor::~CShuangpinSegmentor ()
{
    erase_segs(m_segs);
}

unsigned
CShuangpinSegmentor::push(unsigned ch)
{
    m_inputBuf.push_back(ch);

    return m_updatedFrom = _push(ch);
}

unsigned
CShuangpinSegmentor::pop()
{
    if (m_pystr.empty())
        return m_updatedFrom = 0;

    unsigned size = m_inputBuf.size();

    EShuangpinType shpType = s_shpData.getShuangpinType();
    bool isInputPy = (islower(m_pystr[size - 1]) ||
                      (m_pystr[size - 1] == ';' &&
                       (shpType == MS2003 || shpType == ZIGUANG)));
    if (!isInputPy) {
        m_nAlpha -= 1;
    }

    m_inputBuf.resize(size - 1);
    m_pystr.resize(size - 1);

    unsigned l = m_segs.back()->m_len;
    //m_segs.pop_back();
    erase_seg(m_segs, m_segs.end()-1);

    if (size == 1 || m_segs.back()->m_type != TSegment::INVALID) {
        m_hasInvalid = false;
    }
    if (l == 1)
        return m_updatedFrom = size - 1;

    std::string new_pystr = m_pystr.substr(size - l);
    m_pystr.resize(size - l);

    m_updatedFrom = UINT_MAX;
    std::string::const_iterator it = new_pystr.begin();
    for (; it != new_pystr.end(); ++it) {
        unsigned tmp = _push((*it) & 0x7f);
        if (tmp < m_updatedFrom) m_updatedFrom = tmp;
    }

    return m_updatedFrom;
}

unsigned
CShuangpinSegmentor::insertAt(unsigned idx, unsigned ch)
{
    unsigned pyIdx, segIdx;
    _locateSegment(idx, pyIdx, segIdx);

    m_inputBuf.insert(idx, 1, ch);
    m_pystr.insert(idx, 1, ch);

    std::string new_pystr = m_pystr.substr(pyIdx);
    m_pystr.resize(pyIdx);
    erase_segs(m_segs, m_segs.begin() + segIdx, m_segs.end());

    if (m_nLastValidPos == idx) {
        m_hasInvalid = false;
    } else if (m_nLastValidPos + 1 == idx) {
        m_hasInvalid = false;
        int nSize = m_pystr.size();
        if (islower(m_pystr[nSize - 1])) {
            m_nLastValidPos = idx - 1;
            new_pystr.insert((size_t)0, 1, m_pystr[nSize - 1]);
            m_pystr.erase(nSize - 1, 1);
            erase_seg(m_segs, m_segs.begin() + segIdx - 1);
        }
    } else if (m_nLastValidPos + 1 > idx) {
        m_hasInvalid = false;
        m_nLastValidPos = idx;
    }
    m_nAlpha = _getNumberOfNonAlpha();

    m_updatedFrom = UINT_MAX;
    std::string::const_iterator it = new_pystr.begin();
    for (; it != new_pystr.end(); ++it) {
        unsigned tmp = _push((*it) & 0x7f);
        if (tmp < m_updatedFrom) m_updatedFrom = tmp;
    }

    return m_updatedFrom;
}

unsigned
CShuangpinSegmentor::deleteAt(unsigned idx, bool backward)
{
    unsigned pyIdx, segIdx;
    if (!backward) idx += 1;
    _locateSegment(idx, pyIdx, segIdx);

    m_inputBuf.erase(idx, 1);
    m_pystr.erase(idx, 1);

    std::string new_pystr = m_pystr.substr(pyIdx);
    m_pystr.resize(pyIdx);
    
    TSegmentVec tmp_segs(m_segs.begin() + segIdx + 1, m_segs.end());
    //just erase, the pointer will be deleted later
    erase_seg(m_segs, m_segs.begin() + segIdx);
    m_segs.erase(m_segs.begin() + segIdx, m_segs.end());

    if (m_nLastValidPos + 1 < idx) {
        //del invalid ch, and do not effect current status.
        m_pystr.insert(idx, new_pystr);
        m_segs.insert(m_segs.end(), tmp_segs.begin(), tmp_segs.end());
        return m_inputBuf.size() - 1;
    } else {
        m_hasInvalid = false;
        m_nAlpha = _getNumberOfNonAlpha();
        erase_segs(tmp_segs);
    }

    m_updatedFrom = UINT_MAX;
    std::string::const_iterator it = new_pystr.begin();
    for (; it != new_pystr.end(); ++it) {
        unsigned tmp = _push((*it) & 0x7f);
        if (tmp < m_updatedFrom) m_updatedFrom = tmp;
    }

    return m_updatedFrom;
}

unsigned
CShuangpinSegmentor::clear(unsigned from)
{
    m_inputBuf.resize(from);
    return _clear(from);
}

unsigned
CShuangpinSegmentor::_clear(unsigned from)
{
    unsigned i, j;
    _locateSegment(from, i, j);

    std::string new_pystr = m_pystr.substr(i, from - i);
    m_pystr.resize(i);
    m_nAlpha = _getNumberOfNonAlpha();

    erase_segs(m_segs, m_segs.begin() + j, m_segs.end());

    if (m_nLastValidPos + 1 >= from) {
        m_hasInvalid = false;
    }

    m_updatedFrom = from;

    for (std::string::const_iterator it = new_pystr.begin();
         it != new_pystr.end(); ++it) {
        unsigned tmp = _push((*it) & 0x7f);
        if (tmp < m_updatedFrom) m_updatedFrom = tmp;
    }

    return m_updatedFrom;
}

int
CShuangpinSegmentor::_getNumberOfNonAlpha() const
{
    int nNonAlpha = 0;
    for (const char* c = m_pystr.c_str(); *c != 0; ++c) {
        if (!islower(*c))
            ++nNonAlpha;
    }
    return nNonAlpha;
}

void
CShuangpinSegmentor::_locateSegment(unsigned idx,
                                    unsigned &strIdx,
                                    unsigned &segIdx)
{
    strIdx = segIdx = 0;

    TSegmentVec::const_iterator it = m_segs.begin();
    TSegmentVec::const_iterator ite = m_segs.end();

    for (; it != ite; ++it) {
        if (strIdx + (*it)->m_len > idx)
            break;

        strIdx += (*it)->m_len;
        segIdx += 1;
    }
}

int
CShuangpinSegmentor::_encode(const char* buf, char ch, bool isComplete)
{
    CMappedYin syls;
    syls.reserve(8);
    s_shpData.getMapString(buf, syls);
    if (syls.empty())
        return -1;

    const int len = m_pystr.size();
    CMappedYin::const_iterator iter = syls.begin();
    CMappedYin::const_iterator iter_end = syls.end();

    if (isComplete) {
        TSegment *s = m_segs.back();
        s->m_len = 2;
        s->m_start = len - s->m_len;
        s->m_syllables.clear();
        s->m_type = TSegment::SYLLABLE;
        for (; iter != iter_end; iter++) {
            s->m_syllables.push_back(s_shpData.encode(iter->c_str()));
        }
        m_nLastValidPos += 1;
        return s->m_start;
    } else {
        TSegment s;
        s.m_len = 1;
        s.m_start = len - s.m_len;
        m_nLastValidPos += 1;

        for (; iter != iter_end; ++iter) {
            TSyllable syl = s_shpData.encode(iter->c_str());
            if ((int)syl != 0) {
                TSegment* new_seg = new TSyllableSegment(syl, s.m_start, 1);
                new_seg->setInputTrieSource(m_pInputTrieSrc);
                m_segs.push_back(new_seg);
            } else {
                TSegment* new_seg = new TStringSegment(ch, s.m_start, 1);
                new_seg->setInputTrieSource(m_pInputTrieSrc);
                m_segs.push_back(new_seg);
            }
        }
        return s.m_start;
    }
}

unsigned
CShuangpinSegmentor::_push(unsigned ch)
{
    int startFrom = 0;
    bool isInputPy;
    EShuangpinType shpType;

    m_pystr.push_back(ch);
    const int len = m_pystr.size();
    if (m_hasInvalid) {
        startFrom = len - 1;

        TSegment* new_seg = new TInvalidSegment(ch, startFrom, 1);
        new_seg->setInputTrieSource(m_pInputTrieSrc);
        m_segs.push_back(new_seg);

        goto RETURN;
    }

    shpType = s_shpData.getShuangpinType();
    isInputPy = (islower(ch) ||
                 (ch == ';' && (shpType == MS2003 || shpType == ZIGUANG)));

    if (!isInputPy) {
        startFrom = len - 1;

        if (ch == '\'' && m_inputBuf.size() > 1) {
            TSegment* new_seg = new TSeperatorSegment(ch, startFrom, 1);
            new_seg->setInputTrieSource(m_pInputTrieSrc);
            m_segs.push_back(new_seg);
        } else {
            TSegment* new_seg = new TStringSegment(ch, startFrom, 1);
            new_seg->setInputTrieSource(m_pInputTrieSrc);
            m_segs.push_back(new_seg);
        }
        
        m_nAlpha += 1;
        m_nLastValidPos += 1;
    } else {
        bool bCompleted = !((len - m_nAlpha) % 2) && isInputPy;
        char buf[4];
        if (bCompleted) {
            sprintf(buf, "%c%c", m_pystr[len - 2], ch);
        } else {
            sprintf(buf, "%c", ch);
        }
        startFrom = _encode(buf, ch, bCompleted);
        if (startFrom < 0) {
            m_hasInvalid = true;
            startFrom = m_pystr.size() - 1;
            
            TSegment* new_seg = new TInvalidSegment(ch, startFrom, 1);
            new_seg->setInputTrieSource(m_pInputTrieSrc);
            m_segs.push_back(new_seg);
        }
    }

RETURN:;

    if (m_pGetFuzzySyllablesOp && m_pGetFuzzySyllablesOp->isEnabled())
        if (m_segs.back()->m_type == TSegment::SYLLABLE)
            _addFuzzySyllables(m_segs.back());

    return startFrom;
}

void
CShuangpinSegmentor::_addFuzzySyllables(TSegment *seg)
{
    assert(seg->m_type == TSegment::SYLLABLE);

    TPySyllableSegment *pyseg = (TPySyllableSegment *)(seg);

    pyseg->m_fuzzy_syllables.clear();

    std::vector<unsigned>::iterator it = pyseg->m_syllables.begin();
    std::vector<unsigned>::iterator ite = pyseg->m_syllables.end();
    for (; it != ite; ++it) {
        CSyllables fuzzy_set = (*m_pGetFuzzySyllablesOp)(*it);

        CSyllables::const_iterator _it = fuzzy_set.begin();
        CSyllables::const_iterator _ite = fuzzy_set.end();
        for (; _it != _ite; ++_it)
            pyseg->m_fuzzy_syllables.push_back(*_it);
    }
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
