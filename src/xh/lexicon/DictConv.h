#ifndef SUNPY_DICTGEN_H
#define SUNPY_DICTGEN_H

#include "portability.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

class DictConv {
public:
	typedef std::map<wstring, std::string>        CLexicon;

public:
	DictConv() {}
	bool constructLexicon(const char *filename);

	bool convertDictUsingLexicon(const char *ofile, const char *ifile);

private:
	const char* skipSpace(const char* p);
	const char* skipNonSpace(const char* p);
	bool parseLine(char* buf, char* word_buf, unsigned& id);
	bool parseLine(char* buf, char* word_buf, std::string& syls);

protected:
	CLexicon m_Lexicon;
};


#endif

