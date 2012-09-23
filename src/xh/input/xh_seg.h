
#ifndef SUNPY_PINYIN_SEG_H
#define SUNPY_PINYIN_SEG_H

#include "portability.h"
#include "segmentor.h"
#include "pinyin_data.h"
#include "datrie.h"
#include "ime-core/utils.h"

#include <algorithm>
#include <climits>

class CXinghuaSegmentor : public ISegmentor
{
public:
    CXinghuaSegmentor ();

    virtual TSegmentVec& getSegments(bool req_aux_segs){
        return m_segs;
    }

    virtual const wstring& getInputBuffer() { return m_inputBuf; }

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

    std::string m_inputstr;
    wstring m_inputBuf;
    TSegmentVec m_segs;

    unsigned m_updatedFrom;
};

#endif /* SUNPY_PINYIN_SEG_H */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
