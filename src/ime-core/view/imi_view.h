#ifndef SUNPY_IMI_SESSION_VIEW_H
#define SUNPY_IMI_SESSION_VIEW_H

#include "portability.h"

#include "common/input/segmentor.h"
#include "ime-core/imi_context.h"
#include "ime-core/imi_winHandler.h"
#include "ime-core/imi_uiobjects.h"
#include "ime-core/imi_keys.h"
#include "ime-core/utils.h"
#include <set>

class CHotkeyProfile : private CNonCopyable
{
public:
    CHotkeyProfile();

    void clear(){
        m_pageUpKeys.clear();
        m_pageDownKeys.clear();
        m_modeSwitchKeys.clear();
    }

    void addPageUpKey(const CKeyEvent& key){
        m_pageUpKeys.insert(key);
    }

    void removePageUpKey(const CKeyEvent& key){
        m_pageUpKeys.erase(key);
    }

    bool isPageUpKey(const CKeyEvent& key) const {
        return(m_pageUpKeys.find(key) != m_pageUpKeys.end());
    }

    void addPageDownKey(const CKeyEvent& key){
        m_pageDownKeys.insert(key);
    }

    void removePageDownKey(const CKeyEvent& key){
        m_pageDownKeys.erase(key);
    }

    bool isPageDownKey(const CKeyEvent& key) const {
        return(m_pageDownKeys.find(key) != m_pageDownKeys.end());
    }

    void addModeSwitchKey(const CKeyEvent& key){
        m_modeSwitchKeys.insert(key);
    }

    void removeModeSwitchKey(const CKeyEvent& key){
        m_modeSwitchKeys.erase(key);
    }

    bool isModeSwitchKey(const CKeyEvent& key) const {
        std::set<CKeyEvent>::const_iterator end(m_modeSwitchKeys.end());
        for (std::set<CKeyEvent>::const_iterator it = m_modeSwitchKeys.begin();
             it != end; ++it) {
            if (matches(*it, key))
                return true;
        }
        return false;
    }

    void setPunctSwitchKey(const CKeyEvent& key){
        m_punctSwitchKey = key;
    }

    bool isPunctSwitchKey(const CKeyEvent& key) const {
        return matches(m_punctSwitchKey, key);
    }

    void setSymbolSwitchKey(const CKeyEvent& key){
        m_symbolSwitchKey = key;
    }

    bool isSymbolSwitchKey(const CKeyEvent& key) const {
        return matches(m_symbolSwitchKey, key);
    }

    void rememberLastKey(const CKeyEvent& key){
        m_prevKey = key;
    }

    void setCandiDeleteKey(const CKeyEvent& key){
        m_candiDeleteKey = key;
    }

    bool isCandiDeleteKey(const CKeyEvent& key, unsigned candiWndSize){
        return (key.modifiers == m_candiDeleteKey.modifiers) &&
               (key.value >= '0' && key.value <= '9') &&
               (candiWndSize >= 10 || key.value < ('1' + candiWndSize));
    }

private:
    bool matches(const CKeyEvent& lhs, const CKeyEvent& rhs) const {
        if (lhs == rhs)
            return(!(lhs.modifiers & IM_RELEASE_MASK) ||
                   m_prevKey.code == rhs.code);
        return false;
    }

protected:
    std::set<CKeyEvent> m_pageUpKeys;
    std::set<CKeyEvent> m_pageDownKeys;
    std::set<CKeyEvent> m_modeSwitchKeys;
    CKeyEvent m_punctSwitchKey;
    CKeyEvent m_symbolSwitchKey;
    CKeyEvent m_candiDeleteKey;
    CKeyEvent m_prevKey;
};

class CIMIView {
public:
    enum {
        KEYEVENT_USED  = (1),
        PREEDIT_MASK   = (1 << 2),
        CANDIDATE_MASK = (1 << 3)
    };

public:
    CIMIView();
    virtual ~CIMIView() {}

    void attachIC(CIMIContext* pIC) { m_pIC = pIC; }
    CIMIContext* getIC(void) const { return m_pIC; }

    void setSegmentor(ISegmentor *p) { m_pSegmentor = p; }
    ISegmentor* getSegmentor() const { return m_pSegmentor; }

    void attachWinHandler(CIMIWinHandler* wh) { m_pWinHandler = wh; }
    CIMIWinHandler* getWinHandler(void) const { return m_pWinHandler; }

    void setHotkeyProfile(CHotkeyProfile *prof) { m_pHotkeyProfile = prof; }
    void setCandiWindowSize(unsigned size) {
        m_candiWindowSize = size;
    }

    CHotkeyProfile* getHotkeyProfile() { return m_pHotkeyProfile; }
    unsigned getCandiWindowSize() const { return m_candiWindowSize; }

    void setCancelOnBackspace(bool backspaceCancel)
    { m_backspaceCancel = backspaceCancel; }
    bool getCancelOnBackspace() const { return m_backspaceCancel; }

    void setSmartPunct(bool smart) { m_smartPunct = smart; }
    bool getSmartPunct() const { return m_smartPunct; }

    virtual unsigned clearIC(void) { m_pIC->clear(); return 0; }
    virtual bool isKeyEventHandled(const CKeyEvent&) { return false; }
    virtual bool onKeyEvent(const CKeyEvent&) { return false; }

    virtual void setStatusAttrValue(int key, int value);
    virtual int  getStatusAttrValue(int key);
    virtual void updateWindows(unsigned mask = CANDIDATE_MASK) = 0;

    virtual void getPreeditString(IPreeditString& ps) = 0;
    virtual void getCandidateList(ICandidateList& cl, int start, int size) = 0;

    virtual int  onCandidatePageRequest(int pgno, bool relative) = 0; //pgno == -1, relative == false means last page
    virtual int  onCandidateSelectRequest(int index) = 0;

    virtual void handlerUpdatePreedit(const IPreeditString* ppd);
    virtual void handlerUpdateCandidates(IPreeditString* ppd,
                                         ICandidateList* pcl);
    virtual void handlerCommit(const wstring& wstr);

#ifdef ENABLE_PLUGINS
private:
    void _pluginProvideCandidates(wstring preedit, ICandidateList* pcl);
    void _pluginTranslateCandidate(ICandidateList* pcl);
#endif // ENABLE_PLUGINS

protected:
    CIMIContext        *m_pIC;
    CIMIWinHandler     *m_pWinHandler;
    ISegmentor         *m_pSegmentor;
    CHotkeyProfile     *m_pHotkeyProfile;

    unsigned m_candiWindowSize;

    bool m_bCN;
    bool m_bFullPunct;
    bool m_bFullSymbol;
    bool m_backspaceCancel;
    bool m_smartPunct;
};

#endif

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
