#include <cassert>
#include <functional>
#include <algorithm>
#include "pinyin_seg.h"
#include "quanpin_trie.h"

CXinghuaSegmentor::CXinghuaSegmentor ()
    : m_pGetFuzzySyllablesOp(NULL),
      m_pGetCorrectionPairOp(NULL),
      m_pGetFuzzySegmentsOp(NULL),
      m_trie(base, check, value, sizeof(base) / sizeof(*base)),
      m_updatedFrom(0)
{
    m_segs.reserve(32);
}

bool
CXinghuaSegmentor::load(const char * trieFileName)
{
    return m_trie.load(trieFileName);
}

#ifdef DEBUG
void
print_inputstr(const std::string inputstr)
{
    for (const char* c = inputstr.c_str();
         c != inputstr.c_str() + inputstr.length();
         ++c) {
        printf("%c", *c & 0x7f);
    }
    printf("<\n");
}
#endif

unsigned
CXinghuaSegmentor::push(unsigned ch)
{
    m_inputBuf.push_back(ch);

    if (m_pGetCorrectionPairOp && m_pGetCorrectionPairOp->isEnabled()) {
        m_inputstr.push_back(ch);
        unsigned l = 0;
        const char * v = (*m_pGetCorrectionPairOp)(m_inputstr, l);

        if (v) {
            unsigned orig_size = m_segs.size();
            _clear(m_inputstr.size() - l);
            m_updatedFrom = _updateWith(v);

            if (m_segs.size() >= orig_size) {
                // does not get better segmentation, revert to original
                _clear(m_inputstr.size() - strlen(v));
                std::string new_pystr;
                std::copy(m_inputBuf.end() - l, m_inputBuf.end(),
                          back_inserter(new_pystr));
                m_updatedFrom = _updateWith(new_pystr);
            } else {
                if (l != strlen(v)) {
                    // e.g. uen -> un
                    m_segs.back().m_len += l - strlen(v);
                    m_inputstr.resize(m_inputBuf.length());
                }
                std::copy(m_inputBuf.end() - l, m_inputBuf.end(),
                          m_inputstr.end() - l);
            }
            return m_updatedFrom;
        }

        m_inputstr.resize(m_inputstr.size() - 1);
    }

    return m_updatedFrom = _push(ch);
}

unsigned
CXinghuaSegmentor::pop()
{
    if (m_inputstr.empty())
        return m_updatedFrom = 0;

    unsigned size = m_inputBuf.size();
    m_inputBuf.resize(size - 1);
    m_inputstr.resize(size - 1);

    unsigned l = m_segs.back().m_len;
    m_segs.pop_back();

    if (l == 1)
        return m_updatedFrom = size - 1;

    std::string new_pystr = m_inputstr.substr(size - l);
    m_inputstr.resize(size - l);

    m_updatedFrom = _updateWith(new_pystr);

    return m_updatedFrom;
}

unsigned
CXinghuaSegmentor::insertAt(unsigned idx, unsigned ch)
{
    unsigned i, j;
    _locateSegment(idx, i, j);

    m_inputBuf.insert(idx, 1, ch);
    m_inputstr.insert(idx, 1, ch);

    std::string new_pystr = m_inputstr.substr(i);
    m_inputstr.resize(i);
    m_segs.erase(m_segs.begin() + j, m_segs.end());

    m_updatedFrom = _updateWith(new_pystr);

    return m_updatedFrom;
}

unsigned
CXinghuaSegmentor::deleteAt(unsigned idx, bool backward)
{
    unsigned i, j;
    if (!backward) idx += 1;
    _locateSegment(idx, i, j);

    m_inputBuf.erase(idx, 1);
    m_inputstr.erase(idx, 1);

    std::string new_pystr = m_inputstr.substr(i);
    m_inputstr.resize(i);
    m_segs.erase(m_segs.begin() + j, m_segs.end());

    m_updatedFrom = _updateWith(new_pystr);

    return m_updatedFrom;
}

unsigned
CXinghuaSegmentor::clear(unsigned from)
{
    m_inputBuf.resize(from);
    return _clear(from);
}

unsigned
CXinghuaSegmentor::_clear(unsigned from)
{
    unsigned i, j;
    _locateSegment(from, i, j);


    std::string new_pystr = m_inputstr.substr(i, from - i);
    m_inputstr.resize(i);
    m_segs.erase(m_segs.begin() + j, m_segs.end());

    m_updatedFrom = _updateWith(new_pystr, from);

    return m_updatedFrom;
}

void
CXinghuaSegmentor::_locateSegment(unsigned idx,
                                  unsigned &strIdx,
                                  unsigned &segIdx)
{
    strIdx = segIdx = 0;

    TSegmentVec::iterator it = m_segs.begin();
    TSegmentVec::iterator ite = m_segs.end();

    for (; it != ite; ++it) {
        if (strIdx + (*it).m_len > idx)
            break;

        strIdx += (*it).m_len;
        segIdx += 1;
    }
}

unsigned
CXinghuaSegmentor::_push(unsigned ch)
{
    unsigned ret;
    m_inputstr.push_back(ch);

    if (m_trie.isStroke(ch)) {
        TSegment &last_seg = m_segs.back();

        last_seg.m_len += 1;
        ret = m_inputstr.size() - l;

    } else if (m_trie.isPattern(ch)) {

        ret = m_inputstr.size() - 1;
        m_segs.push_back(TSyllableSegment(ch, ret, 1));

    } else {

        TSegment &new_seg;
        ret = m_inputstr.size() - 1;
        if (ch == '\'') {
            //seg_type = ISegmentor::SYLLABLE_SEP;
            new_seg = TSeperatorSegment(ch, ret, 1);
        } else {
            //seg_type = ISegmentor::STRING;
            new_seg = TStringSegment(ch, ret, 1);
        }
        m_segs.push_back(new_seg);
    }

    return ret;
}

unsigned
CXinghuaSegmentor::_updateWith(const std::string& new_pystr, unsigned from)
{
    unsigned minUpdatedFrom = from;
    std::string::const_iterator it = new_pystr.begin();
    for (; it != new_pystr.end(); ++it) { 
        unsigned updatedFrom = _push(*it & 0x7f);

        if (updatedFrom < minUpdatedFrom) minUpdatedFrom = updatedFrom;
    }
    return minUpdatedFrom;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4