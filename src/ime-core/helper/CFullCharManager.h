#ifndef SUNPY_FULL_CHAR_MANAGER_H
#define SUNPY_FULL_CHAR_MANAGER_H

class CFullCharManager {
public:
	static void setFullSymbolForwarding(bool value = true) {
        m_bFullSymbolForwarding = value;
    }
    static bool getFullSymbolForwarding() { return m_bFullSymbolForwarding; }
    static void setGetFullSymbolOp(CGetFullSymbolOp *op) { m_pGetFullSymbolOp = op; }
    static CGetFullSymbolOp& fullSymbolOp() { return *m_pGetFullSymbolOp; }

    static void setFullPunctForwarding(bool value = true) {
        m_bFullPunctForwarding = value;
    }
    static bool getFullPunctForwarding() { return m_bFullPunctForwarding; }
    static void setFullPunctOp(CGetFullPunctOp *op) { m_pGetFullPunctOp = op; }
    static CGetFullPunctOp& fullPuncOp() { return *m_pGetFullPunctOp; }
    static void omitNextPunct() { m_bOmitPunct = true; }

public:
	static bool m_bOmitPunct;

	static bool m_bFullSymbolForwarding;
	static CGetFullSymbolOp  *m_pGetFullSymbolOp;

	static bool m_bFullPunctForwarding;
	static CGetFullPunctOp *m_pGetFullPunctOp;
};

#endif
