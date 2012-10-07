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
#include "CXhTrieMaker.h"
#include "TTreeWordId.h"
#include "trie_writer.h"

CXhTrieMaker::CXhTrieMaker()
{
    m_pRootNode = new CTreeNode();
}

CXhTrieMaker::~CXhTrieMaker()
{
    delete m_pRootNode;
}
/**********************************************************
    lexicon文件格式：
        以行为单位的文本文件。行中是空格或TAB(1个或多个)分
        隔开的字段。 第一个字段为词，第二个字段是word id。
        后面的字段中，如果一个字段仅仅由小写字母和'构成，
        则认为该字段是该词的一个拼音。行长最大4095;
**********************************************************/

bool
CXhTrieMaker::constructFromLexicon(const char* fileName)
{
    static char buf[4096];
    static char word_buf[2048];

    unsigned id;
    bool suc = true;
    std::set<TUnitInfo> unitset;
    FILE *fp = fopen(fileName, "r");
    if (!fp) return false;
    printf("CXhTrieMaker..."); fflush(stdout);
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
            const char *ustr = its->m_ustr.c_str();
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

CTreeNode*
CXhTrieMaker::insertTransfer(CTreeNode* pnode, unsigned s)
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
CXhTrieMaker::write(FILE *fp, CWordEvaluator* psrt, bool revert_endian)
{
    bool suc = true;
    static TWCHAR wbuf[1024];

    std::map<CTreeNode*, unsigned int> nodeOffsetMap;

    unsigned int nWord = m_Lexicon.size();
    unsigned int nNode = CTreeNode::m_AllNodes.size();
    unsigned int lexiconOffset;
    unsigned int offset = sizeof(unsigned int) * 3;

    CTreeNodeList::const_iterator itNode = CTreeNode::m_AllNodes.begin();
    CTreeNodeList::const_iterator itNodeLast = CTreeNode::m_AllNodes.end();
    for (; itNode != itNodeLast; ++itNode) {
        nodeOffsetMap[*itNode] = offset;
        offset += TThreadNode::size_for((*itNode)->m_Trans.size(),
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
        TThreadNode outNode;
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
            vec.push_back(TTreeWordInfo(*itId, psrt->getCost(*itId) + itId->anony.m_cost,
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
