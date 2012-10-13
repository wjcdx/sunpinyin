#ifndef SUNPY_IMI_FUNCS_H
#define SUNPY_IMI_FUNCS_H

#include "portability.h"
#include "utils.h"
#include <map>

class CGetFullSymbolOp : private CNonCopyable
{
public:
    typedef std::map<unsigned, wstring> CSymbolMap;

    CGetFullSymbolOp () {}
    void initSymbolMap();
    const wstring& operator ()(unsigned ch);

private:
    CSymbolMap m_symbolMap;
};

class CGetFullPunctOp : private CNonCopyable
{
public:
    typedef std::map<unsigned, wstring> CPunctMap;
    typedef std::map<unsigned, bool> CPunctClosingStates;

    CGetFullPunctOp ();

    void initPunctMap(const std::vector<string_pair>& punc_map);
    const wstring& operator ()(unsigned ch);

private:

    CPunctMap m_punctMap;
    CPunctClosingStates m_punctClosingSts;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
