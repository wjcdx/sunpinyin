#ifndef SUNPY_XINGHUA_SEG_H
#define SUNPY_XINGHUA_SEG_H

#include "portability.h"
#include "common/input/segmentor.h"
#include "common/input/segment/segment_types.h"
#include "xhoc_data.h"

#include <algorithm>
#include <climits>

class CXhSegmentor : public ISegmentor
{
public:
    CXhSegmentor ();
    ~CXhSegmentor();

    virtual TSegmentVec& getSegments(bool req_aux_segs){
        return m_segs;
    }

    const wstring& getInputBuffer() { return m_inputBuf; }

    virtual const char* getSylSeps() { return "'"; }

    virtual unsigned push(unsigned ch);
    virtual unsigned pop();
    virtual unsigned insertAt(unsigned idx, unsigned ch);
    virtual unsigned deleteAt(unsigned idx, bool backward = true);
    virtual unsigned clear(unsigned from = 0);

    virtual unsigned updatedFrom() { return m_updatedFrom; }

    bool load(const char * trieFileName);

private:
    inline unsigned _push(unsigned ch);
    inline unsigned _clear(unsigned from);
    inline unsigned _updateWith(const std::string& new_str,
                                unsigned from = UINT_MAX);
    inline void _locateSegment(unsigned idx, unsigned &strIdx, unsigned &segIdx);

    CXhocData m_trie;
    std::string m_inputstr;
    wstring m_inputBuf;
    TSegmentVec m_segs;

    unsigned m_updatedFrom;
};

#endif /* SUNPY_PINYIN_SEG_H */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
