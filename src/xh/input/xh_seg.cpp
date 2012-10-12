#include <cassert>
#include <functional>
#include <algorithm>
#include "xh_seg.h"

CXhSegmentor::CXhSegmentor ()
    : m_updatedFrom(0)
{
    m_segs.reserve(32);
}

bool
CXhSegmentor::load(const char * trieFileName)
{
    //return m_trie.load(trieFileName);
    return true;
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
CXhSegmentor::push(unsigned ch)
{
    m_inputBuf.push_back(ch);
    return m_updatedFrom = _push(ch);
}

unsigned
CXhSegmentor::pop()
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
CXhSegmentor::insertAt(unsigned idx, unsigned ch)
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
CXhSegmentor::deleteAt(unsigned idx, bool backward)
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
CXhSegmentor::clear(unsigned from)
{
    m_inputBuf.resize(from);
    return _clear(from);
}

unsigned
CXhSegmentor::_clear(unsigned from)
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
CXhSegmentor::_locateSegment(unsigned idx,
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
CXhSegmentor::_push(unsigned ch)
{
    unsigned ret;
    m_inputstr.push_back(ch);

    if (m_trie.isStroke(ch)) {
        TSegment &last_seg = m_segs.back();

        last_seg.m_len += 1;
        last_seg.m_syllables.push_back(ch);

        ret = m_inputstr.size() - 1;

    } else if (m_trie.isPattern(ch)) {

        ret = m_inputstr.size() - 1;
        m_segs.push_back(TXhSyllableSegment(ch, ret, 1));

    } else {

        TSegment *new_seg;
        ret = m_inputstr.size() - 1;
        if (ch == '\'') {
            new_seg = new TSeperatorSegment(ch, ret, 1);
        } else {
            new_seg = new TStringSegment(ch, ret, 1);
        }
        m_segs.push_back(*new_seg);
        delete new_seg;
    }

    return ret;
}

unsigned
CXhSegmentor::_updateWith(const std::string& new_pystr, unsigned from)
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
