#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pinyin_data.h"
#include "pinyin_info.h"

static int
pytab_entry_compare(const char *s, TPyTabEntry *v)
{
    return strcmp(s, v->pystr);
}

TSyllable
CPinyinData::encode(const char *pinyin)
{
    typedef int (*bsearch_compare)(const void*, const void*);
    TPyTabEntry *e = (TPyTabEntry*)bsearch(pinyin, pinyin_table,
                                           sizeof(pinyin_table) /
                                           sizeof(pinyin_table[0]),
                                           sizeof(pinyin_table[0]),
                                           (bsearch_compare)pytab_entry_compare);
    if (e)
        return e->id;

    return 0;
}

const char *
CPinyinData::decode(TSyllable s, const char **i, const char **f)
{
    if (i) *i = initials[s.initial];
    if (f) *f = finals[s.final];

    static char buf[128];
    snprintf(buf, sizeof(buf), "%s%s", initials[s.initial], finals[s.final]);

    typedef int (*bsearch_compare)(const void*, const void*);
    TPyTabEntry *e = (TPyTabEntry*)bsearch(buf, pinyin_table,
                                           sizeof(pinyin_table) /
                                           sizeof(pinyin_table[0]),
                                           sizeof(pinyin_table[0]),
                                           (bsearch_compare)pytab_entry_compare);

    if (e)
        return e->pystr;

    return NULL;
}

const char **
CPinyinData::getAutoCorrectionPairs(unsigned &num)
{
    num = num_auto_correction_pairs;
    return auto_correction_pairs;
}

const char **
CPinyinData::getFuzzyPairs(unsigned &num)
{
    num = num_fuzzy_pairs;
    return fuzzy_pairs;
}

const char **
CPinyinData::getInitials(unsigned &num)
{
    num = num_initials;
    return initials;
}

const char **
CPinyinData::getFinals(unsigned &num)
{
    num = num_finals;
    return finals;
}

const TPyTabEntry *
CPinyinData::getPinyinTable(unsigned &num)
{
    num = sizeof(pinyin_table) / sizeof(TPyTabEntry);
    return pinyin_table;
}

const unsigned *
CPinyinData::getInnerFuzzyFinalMap(unsigned &num)
{
    num = num_fuzzy_finals;
    return fuzzy_finals_map;
}

void
CPinyinData::getFuzzyPreProSyllables(const unsigned **pre_syls,
                                     const unsigned **pro_syls)
{
    *pre_syls = fuzzy_pre_syllables;
    *pro_syls = fuzzy_pro_syllables;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
