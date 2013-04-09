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
#include "xh/input/xh_data.h"
#include "path.h"

// build the most popular stroke number in partial 
// from pnode to the most down-level node
// note 1: consider partial boundary as a normal stroke
void
Path::buildTrieInfo(CTreeNode *pnode, bool add)
{
	pnode->m_bOwnWord = !pnode->m_WordIdSet.empty();

    if (pnode->m_Trans.empty()) {

        pnode->m_nMaxStroke = getMostPopularPartialNum();

    } else {
        CTrans::iterator it = pnode->m_Trans.begin();
        CTrans::iterator ite = pnode->m_Trans.end();

        int max = 0;
        for (; it != ite; it++) {

			int m = 0;
			Path p;
			unsigned syl = it->first;
            CTreeNode *sub = it->second;

			if (CXhData::isSplitter(syl)) {

            	p.buildTrieInfo(sub, true);
        		m = getMostPopularPartialNum();

				// most-pop stroke do not across the partial
				// but words do.
				if (add && CXhData::isBoundary(syl)) {
					CTreeWordSet::iterator wit = sub->m_WordIdSet.begin();
					CTreeWordSet::iterator wite = sub->m_WordIdSet.end();
					for (; wit != wite; wit++) {
						TTreeWordId wid = (*wit);
						wid.setForeign();
						pnode->m_WordIdSet.insert(wid);
					}
				}

			} else {

				p = *this;
				p.add(syl);
				p.buildTrieInfo(sub, true);

				if (add) {
					CTreeWordSet::iterator wit = sub->m_WordIdSet.begin();
					CTreeWordSet::iterator wite = sub->m_WordIdSet.end();
					for (; wit != wite; wit++) {
						TTreeWordId wid = (*wit);
						wid.setForeign();
						pnode->m_WordIdSet.insert(wid);
					}
				}

				m = (int)sub->m_nMaxStroke;
			}

			max = m > max ? m : max;
        }
	
        pnode->m_nMaxStroke = max;
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

    TransVecIt it = trans.begin();
    TransVecIt ite = trans.end();
    for (; it != ite; it++) {
        int num = 0;

		// check if the stroke is searched or not
        TransVecIt site = searched.end();
        TransVecIt sit = std::find(searched.begin(), site, *it);
        if (sit != site) {
            continue;
        } else {
            searched.push_back(*it);
        }

        findCheckPoints(it, ite);
		num = cpset.size();

        max = num > max ? num : max;
    }
    return max;
}

