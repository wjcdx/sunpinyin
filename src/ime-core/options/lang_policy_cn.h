#ifndef SUNPY_LANG_POLICY_SC_H
#define SUNPY_LANG_POLICY_SC_H

#include "imi_context.h"
#include "imi_option_event.h"
#include <string>

#ifndef SUNPINYIN_USERDATA_DIR_PREFIX
#define SUNPINYIN_USERDATA_DIR_PREFIX ".sunpinyin"
#endif

struct CSimplifiedChinesePolicy : public IConfigurable {
    CSimplifiedChinesePolicy ();

    virtual bool loadResources();

    virtual CIMIContext* createContext();
    void destroyContext(CIMIContext *context);

    void setPunctMapping(string_pairs punc_map)
    { m_getFullPunctOp.initPunctMap(punc_map); }

    string_pairs getDefaultPunctMapping() const;

    void enableFullSymbol(bool v = true) { m_bEnableFullSymbol = v; }
    void enableFullPunct(bool v = true) { m_bEnableFullPunct = v; }

    void setDataDir(const std::string& data_dir)
    { m_data_dir = data_dir; }

    void setUserDataDir(const std::string& user_data_dir)
    { m_user_data_dir = user_data_dir; }

    virtual bool onConfigChanged(const COptionEvent& event);

    template<class> friend class SingletonHolder;

protected: ~CSimplifiedChinesePolicy () {}

    bool createDirectory(char *path);
    bool saveUserHistory();

    CIMIData m_coreData;
    CBigramHistory m_historyCache;
    bool m_bLoaded;
    bool m_bTried;
    unsigned m_csLevel;
    bool m_bEnableFullSymbol;
    CGetFullSymbolOp m_getFullSymbolOp;
    bool m_bEnableFullPunct;
    CGetFullPunctOp m_getFullPunctOp;
    std::string m_data_dir;
    std::string m_user_data_dir;
public:
    CUserDict m_userDict;
};

typedef SingletonHolder<CSimplifiedChinesePolicy> APySimplifiedChinesePolicy;

#endif

