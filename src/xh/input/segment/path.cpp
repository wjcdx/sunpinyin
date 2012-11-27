#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "pathnode.h"
#include "path.h"
#include "checkpoint.h"

Path&
Path::operator=(const Path &rhs) {
	if (this == &rhs)
		return *this;

	PathNodeList::const_iterator it = rhs.m_Nodes.begin();
	PathNodeList::const_iterator ite = rhs.m_Nodes.end();
	for (; it != ite; it++) {
		add(const_cast<PathNode &>(*it));
	}
	m_bFullForwarded = rhs.m_bFullForwarded;
	return *this;
}

void
Path::add(PathNode &node) {
	if (m_Nodes.empty()) {
		add_first_node(node);
		return;
	}

	PathNode *priv = &m_Nodes.back();
	m_Nodes.push_back(node);

	PathNode *n = &m_Nodes.back();
	m_NextMap[priv] = n;

	if (n->isNow()) {
		m_Now->setFlag(PathNode::HISTORY);
		m_Now = n;
	}
}

void
Path::push_front(PathNode &node)
{
	if (m_Nodes.empty()) {
		add_first_node(node);
		return;
	}
	PathNode *next = &m_Nodes.front();
	m_Nodes.push_front(node);

	PathNode *n = &m_Nodes.front();
	m_NextMap[n] = next;

	if (n->isNow()) {
		m_Now->setFlag(PathNode::FUTURE);
		m_Now = n;
	}
}

void
Path::addPseudoHead()
{
	if (m_Nodes.empty())
		return;
	static PathNode node;
	PathNode *head = &m_Nodes.front();
	m_Now = &node;
	m_NextMap[m_Now] = head;
}

PathNode*
Path::next(PathNode *node)
{
	if (m_NextMap.find(node) != m_NextMap.end())
		return m_NextMap[node];
	return NULL;
}

PathNode*
Path::next(TSyllable s)
{
	PathNode *nxt = next(m_Now);
	for (; nxt; nxt = next(nxt)) {
		if (nxt->transFrom(s)) {
			return nxt;
		} else {
			forward();
		}
	}
	return NULL;
}

void
Path::forward()
{
	PathNode *nxt = next(m_Now);
	if (!nxt)
		return;
	m_Now->setFlag(PathNode::HISTORY);
	nxt->setFlag(PathNode::JUSTNOW);
	m_Now = nxt;
}

int
Path::getTransNum(TSyllable s)
{
	int c = 0;
	PathNodeList::iterator it = m_Nodes.begin();
	PathNodeList::iterator ite = m_Nodes.end();
	for (; it != ite; it++)
		if ((*it).transFrom(s))
			c++;
	return c;
}

void
Path::printNodes()
{
	PathNodeList::iterator nit = m_Nodes.begin();
	PathNodeList::iterator nite = m_Nodes.end();
	std::cout << "\tNodes:\t";
	fflush(stdout);
	for (; nit != nite; nit++) {
		if ((*nit).m_Trans) {
			std::cout << (char)(*nit).m_Trans->m_Unit << " ";
			fflush(stdout);
	    }
	}
	std::cout << std::endl;
	fflush(stdout);
}

void
Path::printNextMap()
{
	std::cout << "\tNextMap:\t";
	fflush(stdout);
	PathNode *nxt = m_Now;
	while (nxt) {
		if (nxt && nxt->m_Trans) {
			std::cout << (char)nxt->m_Trans->m_Unit << " ";
			fflush(stdout);
			nxt = next(nxt);
		} else {
			break;
		}
	}
	std::cout << std::endl;
	std::cout << std::endl;
	fflush(stdout);
}

bool
Path::forward(TSyllable syllable, int num, PathList &paths)
{
	if (m_bFullForwarded) {
		if (num > 1) {
			return checkNumInPath(syllable, num);
		}

		PathNode *node = next(syllable);
		if (!node) {
			return false;
		}
		setNow(node);
		return true;
	}

	bool forwarded = false;
	if (num > 1) {
		Path path;
		forwarded = m_Now->findAllSubNode(syllable, num, paths, path);
	} else {
		forwarded = m_Now->findNextSubNode(syllable, paths);
	}

	return forwarded;
}

/**
 * return value:
 * 0: go on iterate;
 * 1: iterate end succeeded;
 * -1: iterate end failed;
 */
int
Path::getRepeaterStatus(int count, CheckPointList &cphooks)
{
	int len = cpset.size();

	if (len < count) {
		return -1;
	}

	CheckPointList::iterator it, itn = cpset.begin();
	CheckPointList::iterator ite = cpset.end();

	if (len == 1 && count == 1) {
		cphooks.push_back(*it);
		return 1;
	}

	for (it = itn, itn++; itn != ite; it = itn, itn++) {
		if (next(it->m_PNode) == itn->m_Start) {
			//label the most forwarded stroke
			cphooks.push_back(*itn);
		} else {
			cphooks.clear();
		}

		/*
		 * a_a_a
		 * size: num of 'a'
		 * count: number of '_';
		 */
		if ((int)cphooks.size() + 1 >= count)
			return 1;
	}
	return 0;
}

void
Path::forwardCheckPoint()
{
	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (; it != ite; it++) {
		(*it).m_PNode = next((*it).m_PNode);
	}
}

int
Path::getSameRepNumber(CheckPoint &cp)
{
	int c = 0;
	PathNode *n1 = cp.m_PNode;

	CheckPointList::iterator it = cpset.begin();
	CheckPointList::iterator ite = cpset.end();
	for (; it != ite; it++) {
		if (*it == cp)
			continue;
		if (n1 != NULL && n1->isTransSameAs((*it).m_PNode)) {
			c++;
		}
	}
	return c;
}

void
Path::iterateRepeaters(int count)
{
	if (cpset.size() <= 1)
		return;
	
	forwardCheckPoint();

	CheckPointList::iterator it = cpset.begin();
	for (; it != cpset.end(); ) {
		int c = getSameRepNumber(*it);
		if (c+1 < count) {
			it = cpset.erase(it);
		} else {
			it++;
		}
	}
}

void
Path::findCheckPoints(TSyllable syllable)
{
	cpset.clear();

	PathNode *n = next(syllable);
	for (; n; n = next(n)) {
		if (n->transFrom(syllable)) {
			CheckPoint cp(n);
			cpset.push_back(cp);
		}
	}
}

void
Path::labelPath(CheckPointList &cphooks)
{
	assert(!cphooks.empty());
	/*
	if (cphooks.empty())
		return;
	*/

	CheckPoint &cp = cphooks.back();
	cp.m_PNode->setFlag(PathNode::JUSTNOW);
}

bool
Path::checkNumInPath(TSyllable syllable, int num)
{
	int stat = 0;

	findCheckPoints(syllable);
	if ((int)cpset.size() < num)
		return false;

	while (true) {
		CheckPointList cphooks;
		stat = getRepeaterStatus(num, cphooks);
		if (stat == 1) {
			labelPath(cphooks);
			return true;
		} else if (stat == -1) {
			return false;
		}
		iterateRepeaters(num);
	}
	return false;
}

bool
Path::checkNumInPaths(TSyllable syllable, int num, PathList &paths)
{
	PathList::iterator it = paths.begin();
	for (; it != paths.end();) {
		(*it).addPseudoHead();
		if (!(*it).checkNumInPath(syllable, num)) {
			it = paths.erase(it);
		} else {
			it++;
		}
	}
	return !paths.empty();
}

