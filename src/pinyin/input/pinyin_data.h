#ifndef _PINYIN_DATA_H_
#define _PINYIN_DATA_H_

#include <string>
#include <map>
#include "syllable.h"

class CPinyinData
{
public:
    static TSyllable encode(const char *);
    static const char *decode(TSyllable,
                                      const char **i = NULL,
                                      const char **f = NULL);

    static const char **getInitials(unsigned &num);
    static const char **getFinals(unsigned &num);
    static const unsigned *getInnerFuzzyFinalMap(unsigned &num);
    static void getFuzzyPreProSyllables(const unsigned **pre_syls,
                                        const unsigned **pro_syls);

    static const TPyTabEntry* getPinyinTable(unsigned &num);

    static const char **getAutoCorrectionPairs(unsigned &num);
    static const char **getFuzzyPairs(unsigned &num);

protected:
    CPinyinData ();
    CPinyinData (const CPinyinData&);
    CPinyinData& operator =(const CPinyinData&);
};

#endif /* _PINYIN_DATA_H_ */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
