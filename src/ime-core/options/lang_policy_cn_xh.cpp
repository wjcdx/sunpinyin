#include "portability.h"
#include "ime-core/utils.h"
#include "ime-core/helper/CInputTrieSource.h"
#include "xh/lattice/CXhLatticeManager.h"
#include "lang_policy_cn_xh.h"
#include "imi_option_keys.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <errno.h>

bool
CXhSimplifiedChinesePolicy::loadResources()
{
    if (m_bLoaded || m_bTried)
        return m_bLoaded;

    bool suc = true;
    std::string data_dir = m_data_dir.size() ? m_data_dir : SUNPINYIN_DATA_DIR;
    std::string lm_path = data_dir + "/lm_sc.t3g.xh";
    std::string dict_path = data_dir + "/dict3_sc.bin.xh";

    suc &= m_coreData.loadResource(lm_path.c_str(), dict_path.c_str());

    if (!m_user_data_dir.size()) {
        char path[256];
        const char *home = getenv("HOME");
        snprintf(path,
                 sizeof(path),
                 "%s/%s",
                 home,
                 SUNPINYIN_USERDATA_DIR_PREFIX);
        m_user_data_dir = path;
    }

    char * tmp = strdup(m_user_data_dir.c_str());
    createDirectory(tmp);
    free(tmp);

    std::string history_path = m_user_data_dir + "/history";
    suc &= m_historyCache.loadFromFile(history_path.c_str());

    std::string user_dict_path = m_user_data_dir + "/userdict";
    suc &= m_userDict.load(user_dict_path.c_str());

    m_bTried = true;
    return m_bLoaded = suc;
}

CIMIContext *
CXhSimplifiedChinesePolicy::createContext()
{
    CIMIContext* pic = new CIMIContext();
    
	pic->setCoreData(&m_coreData);
    pic->setCharsetLevel(m_csLevel);
	pic->setHistoryMemory(&m_historyCache);
	
	CInputTrieSource::setUserDict(&m_userDict);

	CFullCharManager::setFullSymbolForwarding(m_bEnableFullSymbol);
	CFullCharManager::setGetFullSymbolOp(&m_getFullSymbolOp);

	CFullCharManager::setFullPunctForwarding(m_bEnableFullPunct);
	CFullCharManager::setGetFullPunctOp(&m_getFullPunctOp);
    return pic;
}

