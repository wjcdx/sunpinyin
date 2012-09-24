#ifndef SUNPY_XH_DATA_H
#define SUNPY_XH_DATA_H

class XhData {
	struct XhKeyMapPair {
	public:
		int getKey() {return key;}
		char *getName() {return name;}
	private:
		int key;
		char *name;
	};

public:
    TUnit encode(const char *name); 
    const char *decode(TUnit, const char **i = NULL,
			const char **f = NULL);

	bool isPattern(unsigned ch);
	bool isStroke(unsigned ch);
private:
	XhKeyMapPair &findPair(const char *name);
	XhKeyMapPair &findPair(int key);
}

#endif
