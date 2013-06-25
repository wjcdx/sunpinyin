#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "portability.h"

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#include "datrie.h"

template <typename T, encoder_func_ptr encoder>
bool CDATrie<T, encoder>::load(const char * fname){
    free();

    bool suc = false;
    int fd = open(fname, O_RDONLY);
    if (fd == -1) return false;

    m_memSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

#ifdef HAVE_SYS_MMAN_H
    suc =
        (m_mem =
             (char *) mmap(NULL, m_memSize, PROT_READ, MAP_SHARED, fd,
                           0)) != MAP_FAILED;
#else
    suc = (m_mem = new char [m_memSize]) != NULL;
    suc = suc && (read(fd, m_mem, m_memSize) > 0);
#endif /* HAVE_SYS_MMAN_H */
    close(fd);

    if (!suc)
        return suc;

    m_len = *((unsigned *) m_mem);
    unsigned short elm_size = *((unsigned short *) (m_mem + sizeof(m_len)));
    unsigned short has_value =
        *((unsigned short *) (m_mem + sizeof(m_len) + sizeof(elm_size)));

    if (sizeof(T) != elm_size)
        return false;

    m_base = (T *) (m_mem + sizeof(m_len) + sizeof(elm_size) + sizeof(has_value));
    m_check = m_base + m_len;
    m_value = has_value ? (int *) (m_check + m_len) : NULL;

    return suc;
}

template <typename T, encoder_func_ptr encoder>
void CDATrie<T, encoder>::free(){
    if (m_mem) {
#ifdef HAVE_SYS_MMAN_H
        munmap(m_mem, m_memSize);
#else
        delete [] m_mem;
#endif
        m_mem = NULL;
    }

    m_len = 0;
    m_base = m_check = NULL;
    m_value = NULL;
}

template <typename T, encoder_func_ptr encoder>
unsigned CDATrie<T, encoder>::walk(unsigned s, unsigned ch, int &v){
    unsigned c = encoder(ch);
    unsigned t = abs(m_base[s]) + c;

    if (t < m_len && m_check[t] == (T) s && m_base[t]) {
        if (m_value)
            v = m_value[t];
        else
            v = m_base[t] < 0 ? -1 : 0;

        return t;
    }

    v = 0;
    return 0;
}

template <typename T, encoder_func_ptr encoder>
int CDATrie<T, encoder>::match_longest(const char *str, unsigned &length){
    return match_longest(str, str + strlen(str), length);
}

template <typename T, encoder_func_ptr encoder>
int CDATrie<T, encoder>::match_longest(wstring wstr, unsigned &length){
    return match_longest(wstr.begin(), wstr.end(), length);
}

template <typename T, encoder_func_ptr encoder>
template <typename InputIterator>
int CDATrie<T, encoder>::match_longest(InputIterator first,
                                       InputIterator last,
                                       unsigned &length){
    int l = 0, ret_v = 0, curr_state = 0;
    length = 0;

    for (; first != last; ++first) {
        unsigned ch = *first;
        int val;
        curr_state = walk(curr_state, ch, val);
        if (!curr_state) break;

        l += 1;
        if (val) {
            length = l;
            ret_v = val;
        }
    }

    return ret_v;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
