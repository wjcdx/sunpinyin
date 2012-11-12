#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "imi_view_xh.h"
#include "ime-core/imi_uiobjects.h"
#include "ime-core/imi_keys.h"
#include "ime-core/helper/CFullCharManager.h"

bool
CIMIXhView::onKeyEvent(const CKeyEvent& key)
{
    unsigned changeMasks = 0;

    unsigned keycode = key.code;
    unsigned keyvalue = key.value;
    unsigned modifiers = key.modifiers;

#ifdef DEBUG
    printf("Xh View got a key (0x%x-0x%x-0x%x)...",
           keycode, keyvalue, modifiers);
    if (((modifiers & IM_CTRL_MASK) != 0) &&
        (keyvalue == 'P' || keyvalue == 'p'))
        m_pIC->printLattice();
#endif

    if (m_pHotkeyProfile && m_pHotkeyProfile->isModeSwitchKey(key)) {
        setStatusAttrValue(CIMIWinHandler::STATUS_ID_CN, (!m_bCN) ? 1 : 0);
        if (!m_pIC->isEmpty()) {
            changeMasks |= CANDIDATE_MASK | PREEDIT_MASK;
            clearIC();
        }
    } else if (m_pHotkeyProfile && m_pHotkeyProfile->isPunctSwitchKey(key)) {
        // On CTRL+. switch Full/Half punc
        changeMasks |= KEYEVENT_USED;
        setStatusAttrValue(CIMIWinHandler::STATUS_ID_FULLPUNC,
                           (!m_bFullPunct) ? 1 : 0);
    } else if (m_pHotkeyProfile && m_pHotkeyProfile->isSymbolSwitchKey(key)) {
        // On SHIFT+SPACE switch Full/Half symbol
        changeMasks |= KEYEVENT_USED;
        setStatusAttrValue(CIMIWinHandler::STATUS_ID_FULLSYMBOL,
                           (!m_bFullSymbol) ? 1 : 0);
    } else if (modifiers == IM_CTRL_MASK && keycode == IM_VK_LEFT) {
        // move left
        if (!m_pIC->isEmpty()) {
            changeMasks |= KEYEVENT_USED;
            _moveLeft(changeMasks);
        }
    } else if (modifiers == IM_CTRL_MASK && keycode == IM_VK_RIGHT) {
        // move right
        if (!m_pIC->isEmpty()) {
            changeMasks |= KEYEVENT_USED;
            _moveRight(changeMasks);
        }
    } else if (((modifiers == 0 && keycode == IM_VK_PAGE_UP)
                || (m_pHotkeyProfile && m_pHotkeyProfile->isPageUpKey(key)))
               && !m_pIC->isEmpty()) {
        changeMasks |= KEYEVENT_USED;
        if (m_candiPageFirst > 0) {
            m_candiPageFirst -= m_candiWindowSize;
            if (m_candiPageFirst < 0) m_candiPageFirst = 0;
            changeMasks |= CANDIDATE_MASK;
        }
    } else if (((modifiers == 0 && keycode == IM_VK_PAGE_DOWN)
                || (m_pHotkeyProfile && m_pHotkeyProfile->isPageDownKey(key)))
               && !m_pIC->isEmpty()) {
        changeMasks |= KEYEVENT_USED;
        if (m_candiPageFirst + m_candiWindowSize < candidateListSize()) {
            m_candiPageFirst += m_candiWindowSize;
            changeMasks |= CANDIDATE_MASK;
        }
    } else if (m_pHotkeyProfile
              && m_pHotkeyProfile->isCandiDeleteKey(key, m_candiWindowSize)
              && !m_pIC->isEmpty()) {
        changeMasks |= KEYEVENT_USED;
        unsigned sel = (keyvalue == '0' ? 9 : keyvalue - '1');
        deleteCandidate(sel, changeMasks);
        goto PROCESSED;
    } else if ((modifiers &
                (IM_CTRL_MASK | IM_ALT_MASK | IM_SUPER_MASK |
                 IM_RELEASE_MASK)) == 0) {
        if (((keyvalue >= '6' && keyvalue <= '9') || keyvalue == '0')
            && (m_candiWindowSize >= 5
                || keyvalue < ('6' + m_candiWindowSize))) {
            // try to make selection
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                unsigned sel = (keyvalue == '0' ? 4 : keyvalue - '6');
                makeSelection(sel, changeMasks);
            } else if (m_smartPunct) {
                m_pIC->omitNextPunct();
            }
            goto PROCESSED;
        }

        if (keyvalue >= '1' && keyvalue <= '5') {
            changeMasks |= KEYEVENT_USED;
            _insert(keyvalue, changeMasks);
        } else if (keyvalue > 0x60 && keyvalue < 0x7b) {
            /* islower(keyvalue) */
            changeMasks |= KEYEVENT_USED;
            _insert(keyvalue, changeMasks);
        } else if (keyvalue > 0x20 && keyvalue < 0x7f) {
            /* isprint(keyvalue) && !isspace(keyvalue) */
            changeMasks |= KEYEVENT_USED;
            if (m_pIC->isEmpty()) {
                _insert(keyvalue, changeMasks);
                _doCommit();
                clearIC();
            } else {
                _insert(keyvalue, changeMasks);
            }
        } else if (keycode == IM_VK_BACK_SPACE || keycode == IM_VK_DELETE) {
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                _erase(keycode == IM_VK_BACK_SPACE, changeMasks);
            }
        } else if (keycode == IM_VK_SPACE) {
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                makeSelection(0, changeMasks);
            } else {
                wstring wstr = (CFullCharManager::fullPuncOp())(keyvalue);
                if (wstr.size()) {
                    _commitString(wstr);
                    changeMasks |= KEYEVENT_USED;
                }
            }
        } else if (keycode == IM_VK_ENTER) {
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED | CANDIDATE_MASK | PREEDIT_MASK;
                _doCommit(false);
                clearIC();
            }
        } else if (keycode == IM_VK_ESCAPE) {
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED | CANDIDATE_MASK | PREEDIT_MASK;
                clearIC();
            }
        } else if (keycode == IM_VK_LEFT) { // move left syllable
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                _moveLeftSyllable(changeMasks);
            }
        } else if (keycode == IM_VK_RIGHT) { // move right syllable
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                _moveRightSyllable(changeMasks);
            }
        } else if (keycode == IM_VK_HOME) { // move home
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                _moveHome(changeMasks);
            }
        } else if (keycode == IM_VK_END) { // move end
            if (!m_pIC->isEmpty()) {
                changeMasks |= KEYEVENT_USED;
                _moveEnd(changeMasks);
            }
        }
    } else {
        goto RETURN;
    }

PROCESSED:;
    m_pHotkeyProfile->rememberLastKey(key);

RETURN:;

#ifdef DEBUG
    printf("   |-->(Mask=0x%x)\n", changeMasks);
#endif

    updateWindows(changeMasks);
    return changeMasks & KEYEVENT_USED;
}
