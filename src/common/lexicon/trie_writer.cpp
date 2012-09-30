#include "trie_writer.h"

template <>
bool revert_write<TTransUnit
                  > (const TTransUnit& t, FILE *fp)
{
    return revert_write(t.m_Unit, fp) && revert_write(t.m_Offset, fp);
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
