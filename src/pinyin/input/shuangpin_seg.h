#ifndef SUNPY_SHUANGPIN_SEG_H
#define SUNPY_SHUANGPIN_SEG_H

#include "portability.h"
#include "shuangpin_data.h"
#include "common/input/segmentor.h"
#include "pinyin_data.h"
#include <vector>

class CShuangpinSegmentor : public ISegmentor
{
public:
    CShuangpinSegmentor (EShuangpinType shpType);

    virtual TSegmentVec& getSegments(bool) { return m_segs; }
    virtual const wstring& getInputBuffer() { return m_inputBuf; }
    virtual const char* getSylSeps() { return "'"; }

    virtual unsigned push(unsigned ch);
    virtual unsigned pop();
    virtual unsigned insertAt(unsigned idx, unsigned ch);
    virtual unsigned deleteAt(unsigned idx, bool backward = true);
    virtual unsigned clear(unsigned from = 0);

    virtual unsigned updatedFrom() { return m_updatedFrom; }

    void setGetFuzzySyllablesOp(CGetFuzzySyllablesOp<CPinyinData> *op) {
        m_pGetFuzzySyllablesOp = op; }

private:
    unsigned _push(unsigned ch);
    unsigned _clear(unsigned from);
    int _getNumberOfNonAlpha() const;
    int _encode(const char* buf, char ch, bool isComplete);
    void _locateSegment(unsigned idx, unsigned &strIdx, unsigned &segIdx);

    inline void _addFuzzySyllables(TSegment &seg);

    static CShuangpinData s_shpData;
    std::string m_pystr;
    wstring m_inputBuf;
    TSegmentVec m_segs;
    unsigned m_updatedFrom;
    int m_nAlpha;                                     /* number of non-py chars in m_pystr*/
    bool m_hasInvalid;                                /* true if there is invalid py in m_pystr */
    unsigned m_nLastValidPos;

    CGetFuzzySyllablesOp<CPinyinData>  *m_pGetFuzzySyllablesOp;
};

#endif /* SUNPY_SHUANGPIN_SEG_H */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
