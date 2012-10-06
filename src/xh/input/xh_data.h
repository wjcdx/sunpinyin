#ifndef SUNPY_XH_DATA_H
#define SUNPY_XH_DATA_H

#include <iostream>
#include "TUnit.h"

using namespace std;
using namespace TrieThreadModel;

struct XhKeyMapPair {
public:
	XhKeyMapPair() : key(0), name(NULL) {}
	XhKeyMapPair(int key, string name): key(key), name(name) {}
	int getKey() {return key;}
	string getName() {return name;}
private:
	int key;
	string name;
};

class CXhData {
public:
    static TUnit encode(const char *name); 
    static const char *decode(TUnit u, const char **i = NULL,
			const char **f = NULL);

	bool isPattern(unsigned ch);
	bool isStroke(unsigned ch);

	static XhKeyMapPair findPair(string name);
	static XhKeyMapPair findPair(int key);
private:
	static XhKeyMapPair xhDataMaps[];
};

#endif
