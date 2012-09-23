#ifndef SUNPY_SEGMENTOR_H
#define SUNPY_SEGMENTOR_H

#include "portability.h"
#include "syllable.h"
#include <vector>

struct IPySegmentor : ISegmentor {
    enum ESegmentType
    { SYLLABLE, SYLLABLE_SEP, INVALID, STRING };

    struct TSegment : ISegmentor::TSegment {
        TSegment (ESegmentType type = SYLLABLE) : m_type(type),
                                                  m_inner_fuzzy(0) {}
        TSegment (unsigned syllable,
                  unsigned start,
                  unsigned length,
                  ESegmentType type = SYLLABLE)
            : m_start(start), m_len(length), m_type(type), m_inner_fuzzy(0)
        { m_syllables.push_back(syllable); }

        bool operator <(const TSegment& other) const {
            if (m_start < other.m_start)
                return true;

            if (m_start == other.m_start)
                return m_len < m_len;

            return false;
        }

        // if segment is a STRING type, m_syllables may contain the string buffer without the '\0'
        std::vector<unsigned>           m_syllables;
        std::vector<unsigned>           m_fuzzy_syllables;
        unsigned m_start        : 16;
        unsigned m_len          : 8;
        ESegmentType m_type         : 7;
        bool m_inner_fuzzy  : 1;
    };

    // it requires the segments are sorted by its m_start field
    typedef std::vector<TSegment>  TSegmentVec;

    virtual ~IPySegmentor () {}
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
