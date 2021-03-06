#ifndef SUNPY_LEXICON_PATH_H
#define SUNPY_LEXICON_PATH_H

#include "checkpoint.h"

class Path {

public:

	void add(unsigned s) {
		trans.push_back(s);
	}

	void
	buildTrieInfo(CTreeNode *pnode, bool add);
	
	void
	findCheckPoints(TransVecIt it, TransVecIt ite);

	int
	getMostPopularPartialNum();

private:
	TransVec trans;
	CheckPointList cpset;
};

#endif
