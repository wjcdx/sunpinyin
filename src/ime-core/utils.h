#ifndef SUNPY_UTILS_H
#define SUNPY_UTILS_H

#include <string>
#include <utility>
#include <vector>

typedef std::pair<std::string, std::string> string_pair;
typedef std::vector<string_pair> string_pairs;

class CNonCopyable
{
protected:
    CNonCopyable () {}
    ~CNonCopyable () {}

private:
    CNonCopyable (const CNonCopyable&);
    CNonCopyable& operator =(const CNonCopyable&);
};

template <typename T, bool isArray = false>
class CResourceHandler
{
public:
    CResourceHandler(T* p) : m_p(p) {}
    ~CResourceHandler() { delete m_p; }
protected:
    T *m_p;
};

template <typename T>
class CResourceHandler <T, true>
{
public:
    CResourceHandler(T* p) : m_p(p) {}
    ~CResourceHandler() { delete [] m_p; }
protected:
    T *m_p;
};


template <typename T>
class CSharedPtr
{
private:
    typedef CSharedPtr<T> this_type;

    T*          ptr;
    unsigned*   ref;

public:
    explicit CSharedPtr (T* p = 0) : ptr(p), ref(new unsigned (1)) {}
    CSharedPtr (const this_type& p) : ptr(p.ptr), ref(p.ref) { ++(*ref); }
    ~CSharedPtr () { dispose(); }

    CSharedPtr<T>& operator =(const this_type& p){
        if (this != &p) {
            dispose();
            ptr = p.ptr, ref = p.ref;
            ++(*ref);
        }
        return *this;
    }

    bool equal(const this_type p) const
    { return *ptr == *p.ptr; }

    bool operator ==(const this_type& p) const
    { return ptr == p.ptr && ref == p.ref; }

    T* get()         const { return ptr; }

    operator bool() const { return ptr != 0; }
    operator T&() const { return *ptr; }
    T& operator *() const { return *ptr; }
    T* operator ->() const { return ptr; }

private:
    void dispose(){
        if (--(*ref) == 0) {
            delete ref;
            delete ptr;
        }
    }
};

template <class T>
class SingletonHolder
{
public:
    typedef T Type;
    static T& instance(){
        static T instance_;
        return instance_;
    }
};

#endif /* SUNPY_UTILS_H */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
