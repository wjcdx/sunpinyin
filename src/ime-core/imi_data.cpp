#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_ICONV_H
#include <iconv.h>
#else
#include "portability.h"
#endif // HAVE_ICONV_H

#include "imi_data.h"

void
print_wide(const TWCHAR* wstr)
{
    char buf[512];

#ifdef HAVE_ICONV_H
    iconv_t icv = iconv_open("UTF-8", TWCHAR_ICONV_NAME);
    TIConvSrcPtr src = (TIConvSrcPtr)wstr;
    size_t srclen = (WCSLEN(wstr) + 1) * sizeof(TWCHAR);
    char *dst = buf;
    size_t dstlen = 1024;
    iconv(icv, &src, &srclen, &dst, &dstlen);
    iconv_close(icv);
#else // !HAVE_ICONV_H
    memset(&buf[0], 0, sizeof(buf));
    WCSTOMBS(&buf[0], wstr, sizeof(buf) - 1);
#endif // HAVE_ICONV_H

    printf("%s", buf);
}

bool
CIMIData::loadResource(const char* lm_file_path, const char* trie_file_path, const char *trie_oc)
{
    clear();

    #ifdef DEBUG
    printf("\n    openning lm file %s, trie file %s, trie file for oc %s...",
           lm_file_path,
           trie_file_path, (trie_oc == NULL ? "" : trie_oc));
    #endif

    if ((m_pLM = new CThreadSlm()) != NULL &&
        m_pLM->load(lm_file_path, true) &&
        (m_pTrie = new CTrie()) != NULL &&
        m_pTrie->load(trie_file_path)) {
        
        bool ok = true;
        if (trie_oc != NULL) {
            m_pTrieOc = new CTrie();
            if (m_pTrieOc != NULL) {
                ok = m_pTrieOc->load(trie_oc);
            } else {
                ok = false;
            }
        }

        if (ok) {
            #ifdef DEBUG
            printf("done! LM @%p, Trie @%p\n", m_pLM, m_pTrie);
            fflush(stdout);
            #endif

            return true;
        }
    }

    clear();

    #ifdef DEBUG
    printf("fail!\n");
    fflush(stdout);
    #endif

    return false;
}

void
CIMIData::clear()
{
    if (m_pLM) {
       delete m_pLM;
        m_pLM = NULL;
    }

    if (m_pTrie) {
        delete m_pTrie;
        m_pTrie = NULL;
    }

    if (m_pTrieOc) {
        delete m_pTrieOc;
        m_pTrieOc = NULL;
    }

}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
