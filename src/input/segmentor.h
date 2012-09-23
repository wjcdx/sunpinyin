#ifndef SUNPY_SEGMENTOR_H
#define SUNPY_SEGMENTOR_H

#include "portability.h"
#include "syllable.h"
#include <vector>

struct ISegmentor {
    enum ESegmentType
    { SYLLABLE, SYLLABLE_SEP, INVALID, STRING };

    // it requires the segments are sorted by its m_start field
    typedef std::vector<TSegment>  TSegmentVec;

    virtual ~ISegmentor () {}
    virtual TSegmentVec& getSegments(bool req_aux_segs = true) = 0;
    virtual const wstring& getInputBuffer() = 0;
    virtual const char* getSylSeps() = 0;

    virtual unsigned push(unsigned ch) = 0;
    virtual unsigned pop() = 0;
    virtual unsigned insertAt(unsigned idx, unsigned ch) = 0;
    virtual unsigned deleteAt(unsigned idx, bool backward = true) = 0;
    virtual unsigned clear(unsigned from = 0) = 0;
    virtual void     notify_best_segpath(std::vector<unsigned>& seg_path) {}

    virtual unsigned updatedFrom() = 0;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4