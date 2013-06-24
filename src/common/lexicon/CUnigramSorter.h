#ifndef SUNPY_CUNIGRAMSORTER_H
#define SUNPY_CUNIGRAMSORTER_H

#include "CWordEvaluator.h"
#include "slm/slm.h"

class CUnigramSorter : public CWordEvaluator {
public:
	virtual ~CUnigramSorter() {}
    virtual double
    getCost(unsigned int wid);

    virtual bool
    isSeen(unsigned int wid);

    bool
    open(const char* lm_file)
    {
        return m_Model.load(lm_file);
    }

    void
    close()
    {
        m_Model.free();
    }

protected:
    CThreadSlm m_Model;
};

#endif
