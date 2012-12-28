
#include <stdlib.h>

#include "DictConv.h"

const char*
DictConv::skipSpace(const char* p)
{
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        ++p;
    return p;
}

const char*
DictConv::skipNonSpace(const char* p)
{
    while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
        ++p;
    return p;
}

bool
DictConv::parseLine(char* buf,
          char* word_buf,
          unsigned& id)
{
    /* ignore the empty lines and comment lines */
    if (*buf == '\n' || *buf == '#')
        return false;

    char* p = (char*)skipSpace(buf);
    char* t = (char*)skipNonSpace(p);
    while (p < t) *word_buf++ = *p++;
    *word_buf = 0;

    p = (char*)skipSpace(p);
    t = (char*)skipNonSpace(p);
    if (*t)
        *t++ = 0;
    id = atoi(p);
    return true;
}

bool
DictConv::parseLine(char* buf,
          char* word_buf,
          std::string& syls)
{
    /* ignore the empty lines and comment lines */
    if (*buf == '\n' || *buf == '#')
        return false;

    char* p = (char*)skipSpace(buf);
    char* t = (char*)skipNonSpace(p);
    while (p < t) *word_buf++ = *p++;
    *word_buf = 0;

    p = (char*)skipSpace(p);
    t = (char*)skipNonSpace(p);
    if (*t)
        *t++ = 0;
	p = (char*)skipSpace(t);
    t = (char*)skipNonSpace(p);
    if (*t)
        *t++ = 0;
	syls.assign(p);
    return true;
}

bool
DictConv::constructLexicon(const char *filename)
{
    static char buf[4096];
    static char word_buf[2048];
	static TWCHAR wbuf[1024];
    static std::string syls;

    FILE *fp = fopen(filename, "r");
    if (!fp)
		return false;

    printf("Adding id and corresponding words...\n"); fflush(stdout);
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (parseLine(buf, word_buf, syls)) {
            if (word_buf[0] != L'<' && word_buf[0] != 0) {
				MBSTOWCS(wbuf, word_buf, 1024);
                m_Lexicon[wstring(wbuf)] = syls;
            }
            continue;
        }
	}
    fclose(fp);

    printf("\n    %zd primitive nodes", m_Lexicon.size());  fflush(stdout);
	return true;
}

/*
 * 王见昌 100 wang'jian'chang 
 * ->
 * 王见昌 100 100'101'102
 *
 */

bool
DictConv::convertDictUsingLexicon(const char *ofile, const char *ifile)
{
    static char buf[4096];
    static char word_buf[2048];
	static TWCHAR wbuf[1024];
    unsigned id;

    FILE *ifp = fopen(ifile, "r");
    if (!ifp)
		return false;
    FILE *ofp = fopen(ofile, "w");
    if (!ofp) {
		fclose(ifp);
		return false;
	}

    printf("Iterate the lines...\n"); fflush(stdout);
    while (fgets(buf, sizeof(buf), ifp) != NULL) {
        if (parseLine(buf, word_buf, id)) {
            if (word_buf[0] != L'<' && word_buf[0] != 0) {
				fprintf(ofp, "%s %d ", word_buf, id);
				MBSTOWCS(wbuf, word_buf, 1024);
				int i = 0, sz = WCSLEN(wbuf);
				for (; i < sz; i++) {
					if (i != 0) {
						fprintf(ofp, "'");
					}
					wstring key(&wbuf[i], size_t(1));
					CLexicon::iterator it = m_Lexicon.find(key);
					if (it != m_Lexicon.end()) {
						fprintf(ofp, "%s", m_Lexicon[key].c_str());
					} else {
						fprintf(ofp, "XXX");
					}
				}
				fprintf(ofp, "\n");
            } else {
				fprintf(ofp, "%s\n", buf);
			}
        }
	}
    fclose(ifp);
    fclose(ofp);

	return true;
}

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Usage: %s lexicon-file dict-in dict-out\n", argv[0]);
		return 1;
	}

	const char *lexicon = argv[1];
	const char *idict = argv[2];
	const char *odict = argv[3];

	DictConv dg;
	dg.constructLexicon(lexicon);
	dg.convertDictUsingLexicon(odict, idict);

	return 0;
}

