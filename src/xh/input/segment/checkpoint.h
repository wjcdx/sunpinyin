#ifndef SUNPY_CHECK_POINT_H
#define SUNPY_CHECK_POINT_H

#include "pathnode.h"

class CheckPoint {
public:
	PathNode *m_Start;
	PathNode *m_PNode;

	CheckPoint(PathNode *node) {
		m_Start = m_PNode = node;
	}

	bool operator==(CheckPoint &cp) {
		return notNull() && cp.notNull()
			&& (m_Start == cp.m_Start)
			&& (m_PNode == cp.m_PNode);
	}

	bool notNull() {
		return (m_Start != NULL && m_PNode != NULL);
	}
};

typedef std::list<CheckPoint> CheckPointList;

#endif
