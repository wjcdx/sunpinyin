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

#include "common/lexicon/trie_maker.h"
#include "common/lexicon/tree/TrieTreeModel.h"
#include "path.h"

void
Path::buildTrieInfo(CTreeNode *pnode)
{
	pnode->m_bOwnWord = !pnode->m_WordIdSet.empty();

    if (pnode->m_Trans.empty()) {

        pnode->m_nMaxStroke = getMostPopularPartialNum();

    } else {
        CTrans::iterator it = pnode->m_Trans.begin();
        CTrans::iterator ite = pnode->m_Trans.end();

        int max = 0;
        for (; it != ite; it++) {
        	Path p = *this;
            p.add(it->first);

            CTreeNode *sub = it->second;
            p.buildTrieInfo(sub);

            CTreeWordSet::iterator wit = sub->m_WordIdSet.begin();
            CTreeWordSet::iterator wite = sub->m_WordIdSet.end();
            for (; wit != wite; wit++) {
                pnode->m_WordIdSet.insert(*wit);
            }

            if ((int)sub->m_nMaxStroke > max) {
                max = sub->m_nMaxStroke;
            }
        }
        pnode->m_nMaxStroke = max;
    }
}

int
Path::getRepeaterStatus(int count)
{
	int sz = cpset.size();

	if (sz <= count) {
		return -1;
	}

	CheckPointList::iterator it, itn = cpset.begin();
	CheckPointList::iterator ite = cpset.end();

    int max = count = 1;
	for (it = itn, itn++; itn != ite; it = itn, itn++) {
        unsigned nxt = (*(it->m_Now+1));
		if (nxt == *(itn->m_Start)) {
            count++;
		} else {
			count = 1;
		}
        max = count > max ? count : max;
	}
	return max;
}

void
Path::forwardCheckPoint()
{
	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (; it != ite; it++) {
		(*it).m_Now++;
	}
}

int
Path::getSameRepNumber(CheckPoint &cp)
{
	int c = 0;

	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (; it != ite; it++) {
		if (*it == cp) {
			c++;
		}
	}
	return c;
}

void
Path::iterateRepeaters()
{
	if (cpset.size() <= 1)
		return;
	
	forwardCheckPoint();

	CheckPointList::iterator it, itn = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (it = itn, itn++; it != ite; it = itn, itn++) {
		int c = getSameRepNumber(*it);
		if (c <= 1) {
			cpset.erase(it);
		}
	}
}

void
Path::findCheckPoints(TransVecIt it, TransVecIt ite)
{
	cpset.clear();

    unsigned s = *it;
	for (; it != ite; it++) {
		if (*it == s) {
			CheckPoint cp(it, ite);
			cpset.push_back(cp);
		}
	}
}

int
Path::getMostPopularPartialNum()
{
    int max = 0;
    TransVec searched;

    TransVec::iterator it = trans.begin();
    TransVec::iterator ite = trans.end();
    for (; it != ite; it++) {
        int num = 0;
        TransVecIt sit;
        TransVecIt site = searched.end();

        sit = std::find(searched.begin(), site, *it);
        if (sit != site) {
            continue;
        } else {
            searched.push_back(*it);
        }

        findCheckPoints(it, ite);

        while (true) {
            int n = getRepeaterStatus(num);
            if (n < 0)
                break;
            num = n > num ? n : num;
            iterateRepeaters();
        }
        max = num > max ? num : max;
    }
    return max;
}

