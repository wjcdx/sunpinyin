#include <portability.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#include "slm.h"

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#elif defined(BEOS_OS)
#include <be/kernel/OS.h>
#endif

bool
CThreadSlm::load(const char* fname, bool MMap)
{
    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "open %s: %s\n", fname, strerror(errno));
        return false;
    }

    m_bufSize = fseek(fp, 0, SEEK_END);
    fseek(fp, 0, SEEK_SET);

    m_bMMap = MMap;
    if (m_bMMap) {
#ifdef HAVE_SYS_MMAN_H
        int fd = fileno(fp);
        void* p = mmap(NULL, m_bufSize, PROT_READ, MAP_SHARED, fd, 0);
        if (p == MAP_FAILED) {
            close(fd);
            return false;
        }
        m_buf = (char*)p;
#elif defined(BEOS_OS)
        char *p = NULL;
        area_id area = create_area("tmp", (void**)&p, B_ANY_ADDRESS,
                                   (m_bufSize +
                                    (B_PAGE_SIZE - 1)) & ~(B_PAGE_SIZE - 1),
                                   B_NO_LOCK, B_READ_AREA | B_WRITE_AREA);
        if (area < 0) {
            fclose(fp);
            return false;
        }
        m_buf = p;

        for (ssize_t len = m_bufSize; len > 0; ) {
            ssize_t n = fread(p, len, 1, fp);
            if (n < 0) break;
            p += n;
            len -= n;
        }
#else // Other OS
        //#error "No implementation for mmap()"
        if ((m_buf = new char[m_bufSize]) == NULL) {
            fclose(fp);
            return false;
        }
        if (fread(m_buf, m_bufSize, 1, fp) != m_bufSize) {
            perror("read lm");
            delete [] m_buf; m_buf = NULL;
            fclose(fp);
            return false;
        }
#endif // HAVE_SYS_MMAN_H
    } else {
        if ((m_buf = new char[m_bufSize]) == NULL) {
            fclose(fp);
            return false;
        }
        if (fread(m_buf, m_bufSize, 1, fp) != m_bufSize) {
            perror("read lm");
            delete [] m_buf; m_buf = NULL;
            fclose(fp);
            return false;
        }
    }
    fclose(fp);

    m_N = *(unsigned*)m_buf;
    m_UseLogPr = *(((unsigned*)m_buf) + 1);
    m_LevelSizes = ((unsigned*)m_buf) + 2;
    m_prTable =
        (float*)(m_buf + 2 * sizeof(unsigned) + (m_N + 1) * sizeof(unsigned));
    m_bowTable = m_prTable + (1 << BITS_PR);

    TNode* pn = (TNode*)(m_bowTable + (1 << BITS_BOW));

    //Solaris CC would cause error in runtime if using some thing like
    //following even using (void**) conversion. So add PtrVoid definition
    //m_Levels = new (void*) [m_N + 1];
    m_Levels = new PtrVoid[m_N + 1];

    for (size_t lvl = 0; lvl <= m_N; ++lvl) {
        m_Levels[lvl] = (void*)pn;
        pn += m_LevelSizes[lvl];
    }
    return true;
}

void
CThreadSlm::free()
{
    if (m_Levels) {
        delete [] m_Levels;
    }
    if (m_buf) {
        if (m_bMMap) {
#ifdef HAVE_SYS_MMAN_H
            munmap(m_buf, m_bufSize);
#elif defined(BEOS_OS)
            delete_area(area_for(m_buf));
#else // Other OS
            //#error "No implementation for munmap()"
            delete [] m_buf;
#endif // HAVE_SYS_MMAN_H
        } else {
            delete [] m_buf;
        }
    }
    m_buf = NULL;
    m_Levels = NULL;
}

template<class NodeT>
unsigned int
find_id(NodeT* base, unsigned int h, unsigned int t, unsigned int id)
{
    unsigned int tail = t;
    while (h < t) {
        int m = h + (t - h) / 2;
        NodeT* pm = base + m;
        unsigned int thisId = pm->wid();
        if (thisId < id)
            h = m + 1;
        else if (thisId > id)
            t = m;
        else
            return m;
    }
    return tail;
}

/**
 * return value as the model suggested. The history state must be historified
 * or the history's level should be 0. when level == 0 but idx != 0, the
 * history is a psuedo unigram state used for this model to combine another
 * bigram cache language model
 */
double
CThreadSlm::rawTransfer(TState history, unsigned int wid, TState& result)
{
    unsigned int lvl = history.getLevel();
    unsigned int pos = history.getIdx();

    double cost = (m_UseLogPr) ? 0.0 : 1.0;

    // NON_Word id must be dealed with special, let it transfer to root
    // without any cost
    if (ID_NOT_WORD == wid) {
        result = 0;
        return cost;
    }

    while (true) {
        //for psuedo cache model unigram state
        TNode* pn = ((TNode*)m_Levels[lvl]) + ((lvl) ? pos : 0);

        unsigned int t = (pn + 1)->ch();

        if (lvl < m_N - 1) {
            TNode* pBase = (TNode*)m_Levels[lvl + 1];
            unsigned int idx = find_id(pBase, pn->ch(), t, wid);
            if (idx != t) {
                result.setIdx(idx);
                result.setLevel(lvl + 1);
                double pr = m_prTable[pBase[idx].pr()];
                return (m_UseLogPr) ? (cost + pr) : (cost * pr);
            }
        } else {
            TLeaf* pBase = (TLeaf*)m_Levels[lvl + 1];
            unsigned int idx = find_id(pBase, pn->ch(), t, wid);
            if (idx != t) {
                result.setIdx(idx);
                result.setLevel(lvl + 1);
                double pr = m_prTable[pBase[idx].pr()];
                return (m_UseLogPr) ? (cost + pr) : (cost * pr);
            }
        }

        if (m_UseLogPr)
            cost += m_bowTable[pn->bow()];
        else
            cost *= m_bowTable[pn->bow()];
        if (lvl == 0)
            break;
        lvl = pn->bol();
        pos = pn->bon();
    }
    result.setLevel(0);
    result.setIdx(0);
    if (m_UseLogPr)
        return cost + m_prTable[((TNode*)m_Levels[0])->pr()];
    else
        return cost * m_prTable[((TNode*)m_Levels[0])->pr()];
}

double
CThreadSlm::transferNegLog(TState history, unsigned int wid, TState& result)
{
    double cost = rawTransfer(history, wid, result);
    if (m_UseLogPr)
        return cost;
    else
        return -log(cost);
}

double
CThreadSlm::transfer(TState history, unsigned int wid, TState& result)
{
    double cost = rawTransfer(history, wid, result);
    if (!m_UseLogPr)
        return cost;
    else
        return exp(-cost);
}

unsigned int
CThreadSlm::lastWordId(TState st)
{
    unsigned int lvl = st.getLevel();
    if (lvl >= m_N) {
        const TLeaf* pn = ((const TLeaf*)m_Levels[m_N]) + st.getIdx();
        return pn->wid();
    } else if (lvl > 0) {
        const TNode *pn = ((const TNode*)m_Levels[st.getLevel()]) + st.getIdx();
        return pn->wid();
    } else {
        unsigned int idx = st.getIdx();
        if (idx == 0) {
            const TNode *pn = ((const TNode*)m_Levels[st.getLevel()]) +
                              st.getIdx();
            return pn->wid();
        }
        return idx; // return the psuedo state word id
    }
}

CThreadSlm::TState
CThreadSlm::history_state_of(TState st)
{
    if (st.getLevel() >= m_N) {
        TLeaf* pl = ((TLeaf*)m_Levels[m_N]) + st.getIdx();
        return TState(pl->bol(), pl->bon());
    } else {
        TNode* pn = ((TNode*)m_Levels[st.getLevel()]) + st.getIdx();
        if (pn->ch() == (pn + 1)->ch())
            return TState(pn->bol(), pn->bon());
        else
            return st;
    }
}

CThreadSlm::TState&
CThreadSlm::historify(TState& st)
{
    if (st.getLevel() >= m_N) {
        TLeaf* pl = ((TLeaf*)m_Levels[m_N]) + st.getIdx();
        st.setLevel(pl->bol());
        st.setIdx(pl->bon());
    } else {
        TNode* pn = ((TNode*)m_Levels[st.getLevel()]) + st.getIdx();
        if (pn->ch() == (pn + 1)->ch()) {
            st.setLevel(pn->bol());
            st.setIdx(pn->bon());
        }
    }
    return st;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
