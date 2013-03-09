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
	bool forwarded = false;
	if (num > 1) {
		Path path;
		forwarded = m_Now->findAllSubNode(syllable, num, paths, path);
	} else {
		forwarded = m_Now->findNextSubNode(syllable, paths);
	}

	return forwarded;
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

	CheckPoint &cp = cphooks.back();
	cp.m_PNode->setFlag(PathNode::JUSTNOW);
	setNow(cp.m_PNode);
}

bool
Path::checkNumInPath(TSyllable syllable, int num)
{
	findCheckPoints(syllable);
	if ((int)cpset.size() < num)
		return false;

	labelPath(cpset);
	return true;
}

