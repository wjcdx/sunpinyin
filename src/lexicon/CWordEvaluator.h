#ifndef SUNPY_CWORD_EVALUATOR_H
#define SUNPY_CWORD_EVALUATOR_H

class CWordEvaluator {
public:
    virtual double
    getCost(unsigned int wid) = 0;

    virtual bool
    isSeen(unsigned int wid) = 0;
};

#endif

