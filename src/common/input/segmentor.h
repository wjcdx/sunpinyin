#ifndef SUNPY_SEGMENTOR_H
#define SUNPY_SEGMENTOR_H

#include "portability.h"
#include "pinyin/input/syllable.h"
#include "segment/segment.h"
#include "ime-core/helper/CInputTrieSource.h"
#include <vector>

struct ISegmentor {

    virtual ~ISegmentor () {}
    virtual TSegmentVec& getSegments(bool req_aux_segs = true) = 0;
    virtual const wstring& getInputBuffer() = 0;
    virtual const char* getSylSeps() = 0;

    virtual unsigned push(unsigned ch) = 0;
    virtual unsigned pop() = 0;
    virtual unsigned insertAt(unsigned idx, unsigned ch) = 0;
    virtual unsigned deleteAt(unsigned idx, bool backward = true) = 0;
    virtual unsigned clear(unsigned from = 0) = 0;
    virtual void     notify_best_segpath(std::vector<unsigned>& seg_path) { seg_path; }

    virtual unsigned updatedFrom() = 0;
    virtual void setInputTrieSource(CInputTrieSource* src) = 0;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
