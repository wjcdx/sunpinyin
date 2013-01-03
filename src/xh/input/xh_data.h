#ifndef SUNPY_XHOC_DATA_H
#define SUNPY_XHOC_DATA_H

#include "common/lexicon/thread/TUnit.h"

using namespace TrieThreadModel;

struct XhKeyMapPair {
public:
	XhKeyMapPair() : key(0) {}
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

	static bool isPattern(unsigned ch);
	static bool isStroke(unsigned ch);

	static XhKeyMapPair &findPair(std::string name);
	static XhKeyMapPair &findPair(int key);
private:
	static XhKeyMapPair xhDataMaps[];
};

#endif
