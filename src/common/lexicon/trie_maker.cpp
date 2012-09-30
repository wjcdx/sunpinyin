#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

#include <algorithm>

#include "trie_maker.h"
#include "CUnitData.h"
#include "trie_writer.h"

using namespace TrieTreeModel;
using namespace TrieThreadModel;

static const char*
skipSpace(const char* p)
{
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        ++p;
    return p;
}

static const char*
skipNonSpace(const char* p)
{
    while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
        ++p;
    return p;
}

static void
insertWordId(CTrieMaker::CTreeWordSet& idset, TTreeWordId id)
{
    CTrieMaker::CTreeWordSet::const_iterator it = idset.find(id);
    if (it == idset.end())
        idset.insert(id);
    else {
        const TTreeWordId& a = *it;
        if ((a.anony.m_bHide &&
             !id.anony.m_bHide) ||
            (a.anony.m_bHide == id.anony.m_bHide && a.anony.m_cost >
             id.anony.m_cost)) {
            idset.erase(it);
            idset.insert(id);
        }
    }
}

struct TUnitInfo {
    std::string m_unit;
    int m_cost;

    TUnitInfo(const char* unit = NULL, int cost = 0) : m_unit(unit), m_cost(cost)
    {
    }
    bool
    operator<(const TUnitInfo& b) const
    {
        return m_unit < b.m_unit;
    }
};

#ifdef HAVE_ICONV_H
bool
isCorrectConverted(const char* utf8, iconv_t ic, iconv_t ric)
{
    static char gbstr[256];
    static char utstr[256];

    TIConvSrcPtr src = (TIConvSrcPtr)utf8;
    size_t srclen = strlen((char*)src) + 1;
    char* dst = (char*)gbstr;
    size_t dstlen = 256;
    size_t res = iconv(ic, &src, &srclen, &dst, &dstlen);

    if (res != size_t(-1) && srclen == 0) {
        // do revert convertion and compare them
        src = (TIConvSrcPtr)gbstr;
        srclen = strlen((char*)src) + 1;
        dst = (char*)utstr;
        dstlen = 256;
        res = iconv(ric, &src, &srclen, &dst, &dstlen);
        if (res != size_t(-1) && srclen == 0)
            return(strcmp(utf8, utstr) == 0);
    }
    return false;
}

//return: bit 0x1: contains some gbk out of gb2312, bit 0x2: contains some gb18030 outof gbk
unsigned
getPureGBEncoding(const char* utf8str)
{
    static iconv_t ic_gb = iconv_open("GB2312", "UTF-8");
    static iconv_t ic_gbk = iconv_open("GBK", "UTF-8");
    static iconv_t ric_gb = iconv_open("UTF-8", "GB2312");
    static iconv_t ric_gbk = iconv_open("UTF-8", "GBK");

    unsigned ret = 0;

    if (!isCorrectConverted(utf8str, ic_gb, ric_gb)) {
        ret = 1; // at least it is contains some GBK char
        if (!isCorrectConverted(utf8str, ic_gbk, ric_gbk))
            ret = 3;  //contains some GB18030-only char

        #ifdef DEBUG
        fprintf(stderr, "==> GB category of (%s) is (0x%x)\n ", utf8str, ret);
        fflush(stderr);
        #endif
    }
    return ret;
}
#else // !HAVE_ICONV_H
unsigned
getPureGBEncoding(const char* utf8str)
{
    // FIXME
    return 0x3;
}
#endif // HAVE_ICONV_H

bool
parseLine(char* buf,
          char* word_buf,
          unsigned& id,
          std::set<TUnitInfo>& unitset)
{
    unitset.clear();

    /* ignore the empty lines and comment lines */
    if (*buf == '\n' || *buf == '#')
        return 0;

    char* p = (char*)skipSpace(buf);
    char* t = (char*)skipNonSpace(p);
    while (p < t) *word_buf++ = *p++;
    *word_buf = 0;

    p = (char*)skipSpace(p);
    t = (char*)skipNonSpace(p);
    if (*t)
        *t++ = 0;
    id = atoi(p);
    p = (char*)skipSpace(t);
    while (*p) {
        const char* s = p;
        t = (char*)skipNonSpace(p);
        if (*t)
            *t++ = 0;
        while ((*p >= 'a' && *p <= 'z') || (*p == '\''))
            ++p;
        if ((p > s) && ((*p == 0) || (*p == ':'))) {
            int cost = 0;
            if (*p == ':') {
                *p++ = 0;
                cost = -log2(atof(p)/100);
            }
            unitset.insert(TUnitInfo(s, cost));
        }
        p = (char*)skipSpace(t);
    }
    return unitset.size() > 0;
}


CTrieMaker::CTrieMaker()
{
}
/**********************************************************
    lexicon文件格式：
        以行为单位的文本文件。行中是空格或TAB(1个或多个)分
        隔开的字段。 第一个字段为词，第二个字段是word id。
        后面的字段中，如果一个字段仅仅由小写字母和'构成，
        则认为该字段是该词的一个拼音。行长最大4095;
**********************************************************/

bool
CTrieMaker::constructFromLexicon(const char* fileName)
{
    static char buf[4096];
    static char word_buf[2048];

    unsigned id;
    bool suc = true;
    std::set<TUnitInfo> unitset;
    FILE *fp = fopen(fileName, "r");
    if (!fp) return false;
    printf("Adding pinyin and corresponding words..."); fflush(stdout);
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (!parseLine(buf, word_buf, id, unitset)) {
            if (word_buf[0] != L'<' && word_buf[0] != 0) {
                if (m_Lexicon.size() < id + 1) m_Lexicon.resize(id + 1);
                m_Lexicon[id] = std::string(word_buf);
            }
            continue;
        }
        unsigned gbcategory = getPureGBEncoding(word_buf);

        std::set<TUnitInfo>::const_iterator its = unitset.begin();
        std::set<TUnitInfo>::const_iterator ite = unitset.end();
        for (; its != ite; ++its) {
            const char *ustr = its->m_py.c_str();
            if (m_Lexicon.size() < id + 1) m_Lexicon.resize(id + 1);
            m_Lexicon[id] = std::string(word_buf);

            TTreeWordId wid(id, its->m_cost, false, gbcategory);
            suc = insertPair(ustr, wid) && suc;
        }
    }
    fclose(fp);

    printf("\n    %zd primitive nodes", CTreeNode::m_AllNodes.size());  fflush(stdout);
    return suc;
}

static void
parseUnit(const char *unit, std::vector<TUnit> &ret)
{
    char *buf = strdup(pinyin);
    char *p = buf, *q = buf;
    ret.clear();

    while (*p) {
        if (*p == '\'') {
            *p = '\0';
            unsigned s = m_UnitData::encode(q);
            if (s)
                ret.push_back(TUnit(s));
            else
                printf("\nWarning! unrecognized syllable %s", q);
            q = p + 1;
        }
        p++;
    }

    if (*q) {
        unsigned s = m_UnitData::encode(q);
        if (s)
            ret.push_back(TUnit(s));
        else
            printf("\nWarning! unrecognized unit %s", q);
    }

    free(buf);
}

CTreeNode*
CTrieMaker::insertTransfer(CTreeNode* pnode, unsigned s)
{
    CTrans::const_iterator itt = pnode->m_Trans.find(s);
    CTrans::const_iterator ite = pnode->m_Trans.end();
    if (itt == ite) {
        CTreeNode *p = new CTreeNode();
        pnode->m_Trans[s] = p;
        return p;
    }
    return itt->second;
}

bool
CTrieMaker::insertPair(const char* unit, TTreeWordId wid)
{
    CTreeNode *pnode = &m_RootNode;
    std::vector<TUnit> units;
    parseUnit(unit, units);

    if (units.empty())
        return true;

    std::vector<TUnit>::const_iterator it = units.begin();
    std::vector<TUnit>::const_iterator ite = units.end();

    for (; it != ite; ++it)
        pnode = insertTransfer(pnode, *it);

    insertWordId(pnode->m_WordIdSet, wid);
    return true;
}

bool
CTrieMaker::write(const char* fileName, CWordEvaluator* psrt,
                        bool revert_endian)
{
    bool suc = false;
    FILE* fp = fopen(fileName, "wb");
    if (fp != NULL) {
        suc = write(fp, psrt, revert_endian);
        fclose(fp);
    }
    return suc;
}

bool
CTrieMaker::write(FILE *fp, CWordEvaluator* psrt, bool revert_endian)
{
    bool suc = true;
    static TWCHAR wbuf[1024];

    std::map<CTreeNode*, unsigned int> nodeOffsetMap;

    unsigned int nWord = m_Lexicon.size();
    unsigned int nNode = CTreeNode::m_AllNodes.size();
    unsigned int lexiconOffset;
    unsigned int offset = sizeof(unsigned int) * 3;

    CNodeList::const_iterator itNode = CTreeNode::m_AllNodes.begin();
    CNodeList::const_iterator itNodeLast = CTreeNode::m_AllNodes.end();
    for (; itNode != itNodeLast; ++itNode) {
        nodeOffsetMap[*itNode] = offset;
        offset += CTreeNode::size_for((*itNode)->m_Trans.size(),
                                               (*itNode)->m_WordIdSet.size());
    }
    lexiconOffset = offset;
    CLexicon::const_iterator itWordStr = m_Lexicon.begin();
    CLexicon::const_iterator itWordStrLast = m_Lexicon.end();
    for (; itWordStr != itWordStrLast; ++itWordStr) {
        MBSTOWCS(wbuf, itWordStr->c_str(), 1024);
        int sz = WCSLEN(wbuf);
        offset += (sz + 1) * sizeof(TWCHAR);
    }

    Writer f(fp, revert_endian);

    suc = f.write(nWord);
    suc = f.write(nNode);
    suc = f.write(lexiconOffset);

    itNode = CTreeNode::m_AllNodes.begin();
    itNodeLast = CTreeNode::m_AllNodes.end();

    for (; itNode != itNodeLast && suc; ++itNode) {
        CTreeNode outNode;
        CTreeNode *pnode = *itNode;

        outNode.m_nTransfer = pnode->m_Trans.size();
        outNode.m_nWordId = pnode->m_WordIdSet.size();
        outNode.m_csLevel = 0;

        CTreeWordSet::const_iterator itId = pnode->m_WordIdSet.begin();
        CTreeWordSet::const_iterator itIdLast = pnode->m_WordIdSet.end();
        for (; itId != itIdLast && outNode.m_csLevel < 3; ++itId) {
            if (outNode.m_csLevel < itId->anony.m_csLevel)
                outNode.m_csLevel = itId->anony.m_csLevel;
        }

        suc = f.write(outNode);

        CTrans::const_iterator itTrans = pnode->m_Trans.begin();
        CTrans::const_iterator itTransLast = pnode->m_Trans.end();
        for (; itTrans != itTransLast && suc; ++itTrans) {
            TTransUnit tru;
            tru.m_Unit = itTrans->first;
            tru.m_Offset = nodeOffsetMap[itTrans->second];
            assert(tru.m_Offset != 0 && tru.m_Offset < lexiconOffset);
            suc = f.write(tru);
        }

        CTreeWordVec vec;
        itId = pnode->m_WordIdSet.begin();
        itIdLast = pnode->m_WordIdSet.end();
        for (; itId != itIdLast; ++itId)
            vec.push_back(TWordInfo(*itId, psrt->getCost(*itId) + itId->anony.m_cost,
                                    psrt->isSeen(*itId)));
        std::make_heap(vec.begin(), vec.end());
        std::sort_heap(vec.begin(), vec.end());

        CTreeWordVec::const_iterator itv = vec.begin();
        CTreeWordVec::const_iterator itve = vec.end();
        for (; itv != itve && suc; ++itv) {
            TWordIdInfo wi;
            wi.m_id = itv->m_id.anony.m_id;
            assert(wi.m_id < nWord);
            wi.m_csLevel = itv->m_id.anony.m_csLevel;
            wi.m_bSeen = ((itv->m_bSeen) ? (1) : (0));
            wi.m_cost = itv->m_id.anony.m_cost;
            suc = f.write(wi);
        }
    }

    itWordStr = m_Lexicon.begin();
    itWordStrLast = m_Lexicon.end();
    for (; itWordStr != itWordStrLast && suc; ++itWordStr) {
        MBSTOWCS(wbuf, itWordStr->c_str(), 1024);
        int sz = WCSLEN(wbuf);
        suc = f.write(wbuf, (sz + 1));
    }
    return suc;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
