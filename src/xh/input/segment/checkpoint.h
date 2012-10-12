#ifndef SUNPY_CHECK_POINT_H
#define SUNPY_CHECK_POINT_H

#include "pathnode.h"

struct CheckPoint {
	PathNode *m_Start;
	PathNode *m_PNode;
	CheckPoint(PathNode *node) {
		m_Start = m_PNode = node;
	}

	bool operator==(CheckPoint &cp) {
		return (m_Start == cp.m_Start)
			&& (m_PNode == cp.m_PNode);
	}
};

typedef std::list<CheckPoint> CheckPointList;

#endif
