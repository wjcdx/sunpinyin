#ifndef SUNPY_IMI_CLASSIC_VIEW_H
#define SUNPY_IMI_CLASSIC_VIEW_H

#include "portability.h"

#include "imi_view.h"

class CIMIClassicView : public CIMIView
{
public:
    CIMIClassicView();
    virtual ~CIMIClassicView();

    virtual void attachIC(CIMIContext* pIC);
    virtual unsigned clearIC(void);

    virtual bool isKeyEventHandled(const CKeyEvent&);
    virtual bool onKeyEvent(const CKeyEvent&);

    virtual void updateWindows(unsigned mask);

    virtual void getPreeditString(IPreeditString& ps);
    virtual void getCandidateList(ICandidateList& cl, int start, int size);

    virtual int  onCandidatePageRequest(int pgno, bool relative);
    virtual int  onCandidateSelectRequest(int index);

    size_t candidateListSize() const
    { return m_candiList.size() + m_sentences.size() + m_tails.size(); }

    void makeSelection(int candiIdx, unsigned& mask);
    void deleteCandidate(int candiIdx, unsigned& mask);

protected:
    static size_t top_candidate_threshold;

    unsigned m_cursorFrIdx;
    unsigned m_candiFrIdx;
    unsigned m_candiPageFirst;

    CCandidateList m_uiCandidateList;
    CPreEditString m_uiPreeditString;

    CCandidates m_candiList;
    std::vector<std::pair<int, wstring> > m_sentences;
    std::vector<std::pair<wstring, CCandidates> > m_tails;

    void _insert(unsigned keyvalue, unsigned& mask);
    void _erase(bool backward, unsigned& mask);

    void _getCandidates();

    void _commitChar(TWCHAR ch);
    void _commitString(const wstring& wstr);
    void _doCommit(bool bConvert = true);

    unsigned _moveLeft(unsigned& mask, bool searchAgain = true);
    unsigned _moveLeftSyllable(unsigned& mask, bool searchAgain = true);
    unsigned _moveHome(unsigned& mask, bool searchAgain = true);

    unsigned _moveRight(unsigned& mask);
    unsigned _moveRightSyllable(unsigned& mask);
    unsigned _moveEnd(unsigned& mask);
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
