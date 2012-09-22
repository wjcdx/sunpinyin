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

#include "pytrie_gen.h"
#include "pinyin_data.h"
#include "trie_writer.h"

CPinyinTrieMaker::CPinyinTrieMaker()
{
    this.m_UnitData = CPinyinData();
    m_RootNode.m_bExpanded = true;
}
/**********************************************************
    lexicon文件格式：
        以行为单位的文本文件。行中是空格或TAB(1个或多个)分
        隔开的字段。 第一个字段为词，第二个字段是word id。
        后面的字段中，如果一个字段仅仅由小写字母和'构成，
        则认为该字段是该词的一个拼音。行长最大4095;
**********************************************************/

bool
CPinyinTrieMaker::constructFromLexicon(const char* fileName)
{
    static char buf[4096];
    static char word_buf[2048];

    unsigned id;
    bool suc = true;
    std::set<TSyllableInfo> pyset;
    FILE *fp = fopen(fileName, "r");
    if (!fp) return false;
    printf("Adding pinyin and corresponding words..."); fflush(stdout);
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (!parseLine(buf, word_buf, id, pyset)) {
            if (word_buf[0] != L'<' && word_buf[0] != 0) {
                if (m_Lexicon.size() < id + 1) m_Lexicon.resize(id + 1);
                m_Lexicon[id] = std::string(word_buf);
            }
            continue;
        }
        unsigned gbcategory = getPureGBEncoding(word_buf);

        std::set<TSyllableInfo>::const_iterator its = pyset.begin();
        std::set<TSyllableInfo>::const_iterator ite = pyset.end();
        for (; its != ite; ++its) {
            const char *pystr = its->m_py.c_str();
            if (m_Lexicon.size() < id + 1) m_Lexicon.resize(id + 1);
            m_Lexicon[id] = std::string(word_buf);

            CPinyinTrieMaker::TWordId wid(id, its->m_cost, false, gbcategory);
            suc = insertPair(pystr, wid) && suc;
        }
    }
    fclose(fp);

    printf("\n    %zd primitive nodes", TNode::m_AllNodes.size());  fflush(stdout);

    threadNonCompletePinyin();
    printf("\n    %zd total nodes", TNode::m_AllNodes.size());  fflush(stdout);

    std::string pyPrefix = "";
    printf("\n");  fflush(stdout);

    return suc;
}

CPinyinTrieMaker::TNode*
CPinyinTrieMaker::insertTransfer(TNode* pnode, unsigned s)
{
    CTrans::const_iterator itt = pnode->m_Trans.find(s);
    CTrans::const_iterator ite = pnode->m_Trans.end();
    if (itt == ite) {
        TNode *p = new TNode();
        p->m_bFullSyllableTransfer = true;
        p->m_bExpanded = true;
        pnode->m_Trans[s] = p;
        return p;
    }
    return itt->second;
}

CPinyinTrieMaker::TNode*
CPinyinTrieMaker::addCombinedTransfers(TNode *pnode,
                                       unsigned s,
                                       const CNodeSet& nodes)
{
    assert(!nodes.empty());

    TNode *p = NULL;
    if (nodes.size() == 1) {
        p = *(nodes.begin());
    } else {
        p = new TNode();
        p->m_cmbNodes = nodes;
        m_StateMap[&p->m_cmbNodes] = p;

        CNodeSet::const_iterator it = nodes.begin();
        CNodeSet::const_iterator ite = nodes.end();
        for (; it != ite; ++it) {
            CWordSet::const_iterator wit  = (*it)->m_WordIdSet.begin();
            CWordSet::const_iterator wite = (*it)->m_WordIdSet.end();

            for (; wit != wite; ++wit) {
                CWordSet::iterator tmp = p->m_WordIdSet.find (*wit);

                if (tmp == p->m_WordIdSet.end()) {
                    p->m_WordIdSet.insert (*wit);
                } else if (tmp->anony.m_cost > wit->anony.m_cost) {
                    p->m_WordIdSet.erase (tmp);
                    p->m_WordIdSet.insert (*wit);
                }
            }
        }
    }

    pnode->m_Trans[s] = p;
    return p;
}

void
CPinyinTrieMaker::combineInitialTrans(TNode *pnode)
{
    std::map<unsigned, CNodeSet> combTrans;

    CTrans::const_iterator itTrans = pnode->m_Trans.begin();
    CTrans::const_iterator itTransLast = pnode->m_Trans.end();
    for (; itTrans != itTransLast; ++itTrans) {
        TSyllable s = (TSyllable)itTrans->first;
        if (s.initial) {
            s.final = s.tone = 0;
            combTrans[s].insert(itTrans->second);
        }
    }

    std::map<unsigned, CNodeSet>::const_iterator itCombTrans = combTrans.begin();
    for (; itCombTrans != combTrans.end(); ++itCombTrans)
        addCombinedTransfers(pnode, itCombTrans->first, itCombTrans->second);
}

void
CPinyinTrieMaker::expandCombinedNode(TNode *pnode)
{
    assert(pnode->m_cmbNodes.size() >= 1);

    std::map<unsigned, CNodeSet> combTrans;
    CNodeSet::const_iterator itNode = pnode->m_cmbNodes.begin();
    CNodeSet::const_iterator itNodeLast = pnode->m_cmbNodes.end();
    for (; itNode != itNodeLast; ++itNode) {
        CTrans::const_iterator itTrans = (*itNode)->m_Trans.begin();
        CTrans::const_iterator itTransLast = (*itNode)->m_Trans.end();
        for (; itTrans != itTransLast; ++itTrans)
            combTrans[itTrans->first].insert(itTrans->second);
    }

    std::map<unsigned, CNodeSet>::const_iterator itCombTrans = combTrans.begin();
    for (; itCombTrans != combTrans.end(); ++itCombTrans) {
        TNode* p = NULL;
        unsigned s = itCombTrans->first;
        CNodeSet nodes = itCombTrans->second;

        CStateMap::const_iterator itStateMap = m_StateMap.find(&nodes);
        if (itStateMap != m_StateMap.end())
            p = itStateMap->second;
        else
            p = addCombinedTransfers(pnode, s, nodes);

        pnode->m_Trans[s] = p;
    }

    pnode->m_bExpanded = true;
}

bool
CPinyinTrieMaker::threadNonCompletePinyin(void)
{
    CNodeList::const_iterator itNode = TNode::m_AllNodes.begin();
    for (; itNode != TNode::m_AllNodes.end(); ++itNode) {
        TNode* pnode = *itNode;
        if (pnode->m_bExpanded)
            combineInitialTrans(pnode);
        else
            expandCombinedNode(pnode);
    }
    return true;
}

bool
CPinyinTrieMaker::write(FILE *fp, CWordEvaluator* psrt, bool revert_endian)
{
    bool suc = true;
    static TWCHAR wbuf[1024];

    std::map<TNode*, unsigned int> nodeOffsetMap;

    unsigned int nWord = m_Lexicon.size();
    unsigned int nNode = TNode::m_AllNodes.size();
    unsigned int lexiconOffset;
    unsigned int offset = sizeof(unsigned int) * 3;

    CNodeList::const_iterator itNode = TNode::m_AllNodes.begin();
    CNodeList::const_iterator itNodeLast = TNode::m_AllNodes.end();
    for (; itNode != itNodeLast; ++itNode) {
        nodeOffsetMap[*itNode] = offset;
        offset += CPinyinTrie::TNode::size_for((*itNode)->m_Trans.size(),
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

    itNode = TNode::m_AllNodes.begin();
    itNodeLast = TNode::m_AllNodes.end();

    for (; itNode != itNodeLast && suc; ++itNode) {
        CPinyinTrie::TNode outNode;
        TNode *pnode = *itNode;

        outNode.m_nTransfer = pnode->m_Trans.size();
        outNode.m_nWordId = pnode->m_WordIdSet.size();
        outNode.m_bFullSyllableTransfer = pnode->m_bFullSyllableTransfer;
        outNode.m_csLevel = 0;

        CWordSet::const_iterator itId = pnode->m_WordIdSet.begin();
        CWordSet::const_iterator itIdLast = pnode->m_WordIdSet.end();
        for (; itId != itIdLast && outNode.m_csLevel < 3; ++itId) {
            if (outNode.m_csLevel < itId->anony.m_csLevel)
                outNode.m_csLevel = itId->anony.m_csLevel;
        }

        suc = f.write(outNode);

        CTrans::const_iterator itTrans = pnode->m_Trans.begin();
        CTrans::const_iterator itTransLast = pnode->m_Trans.end();
        for (; itTrans != itTransLast && suc; ++itTrans) {
            CPinyinTrie::TTransUnit tru;
            tru.m_Syllable = itTrans->first;
            tru.m_Offset = nodeOffsetMap[itTrans->second];
            assert(tru.m_Offset != 0 && tru.m_Offset < lexiconOffset);
            suc = f.write(tru);
        }

        CWordVec vec;
        itId = pnode->m_WordIdSet.begin();
        itIdLast = pnode->m_WordIdSet.end();
        for (; itId != itIdLast; ++itId)
            vec.push_back(TWordInfo(*itId, psrt->getCost(*itId) + itId->anony.m_cost,
                                    psrt->isSeen(*itId)));
        std::make_heap(vec.begin(), vec.end());
        std::sort_heap(vec.begin(), vec.end());

        CWordVec::const_iterator itv = vec.begin();
        CWordVec::const_iterator itve = vec.end();
        for (; itv != itve && suc; ++itv) {
            CPinyinTrie::TWordIdInfo wi;
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
