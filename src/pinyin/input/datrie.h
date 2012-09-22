#ifndef SUNPY_DATRIE_H
#define SUNPY_DATRIE_H

#include "portability.h"

template <unsigned lower, unsigned upper>
unsigned character_based_encoder(unsigned ch){
    int ret = ch - lower + 1;
    if (ret <= 0) ret = upper + 1;
    return ret;
}

typedef unsigned (*encoder_func_ptr)(unsigned ch);
template <typename T, encoder_func_ptr encoder =
              character_based_encoder<'a', 'z'> >
class CDATrie
{
private:
    typedef CDATrie<T> this_type;

public:
    CDATrie () : m_mem(0), m_len(0), m_base(0), m_check(0), m_value(0) {};
    CDATrie (T* base, T* check, int* value, unsigned len)
        : m_mem(0), m_len(len), m_base(base), m_check(check), m_value(value) {};

    ~CDATrie () { free(); }

    bool load(const char* fname);
    void free();

    int match_longest(const char * str, unsigned &length);
    int match_longest(wstring wstr, unsigned &length);
    template <typename InputIterator>
    int match_longest(InputIterator first, InputIterator last, unsigned &length);

protected:
    unsigned walk(unsigned s, unsigned ch, int &v);

    char     * m_mem;
    unsigned m_memSize;

    unsigned m_len;
    T        * m_base;
    T        * m_check;
    int      * m_value;
};

#include "datrie_impl.h"

#endif /* SUNPY_DATRIE_H */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
