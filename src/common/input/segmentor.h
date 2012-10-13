#ifndef SUNPY_SEGMENTOR_H
#define SUNPY_SEGMENTOR_H

#include "portability.h"
#include "syllable.h"
#include "segment.h"
#include <vector>

struct ISegmentor {

    virtual ~ISegmentor () {}
    virtual TSegmentVec& getSegments(bool req_aux_segs = true);
    virtual const wstring& getInputBuffer();
    virtual const char* getSylSeps();

    virtual unsigned push(unsigned ch);
    virtual unsigned pop();
    virtual unsigned insertAt(unsigned idx, unsigned ch);
    virtual unsigned deleteAt(unsigned idx, bool backward = true);
    virtual unsigned clear(unsigned from );
    virtual void     notify_best_segpath(std::vector<unsigned>& seg_path) {}

    virtual unsigned updatedFrom();
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
