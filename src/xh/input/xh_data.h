#ifndef SUNPY_XH_DATA_H
#define SUNPY_XH_DATA_H

#include "common/lexicon/thread/TUnit.h"

using namespace TrieThreadModel;

struct XhKeyMapPair {
public:
	XhKeyMapPair() : key(0), name() {}
	XhKeyMapPair(int key, std::string name): key(key), name(name) {}
	int getKey() {return key;}
	std::string getName() {return name;}
private:
	int key;
	std::string name;
};

class CXhData {
public:
    static TUnit encode(const char *name); 
    static const char *decode(TUnit u, const char **i = NULL,
			const char **f = NULL);

	bool isPattern(unsigned ch);
	bool isStroke(unsigned ch);

	static XhKeyMapPair &findPair(std::string name);
	static XhKeyMapPair &findPair(int key);
private:
	static XhKeyMapPair xhDataMaps[];
};

#endif
