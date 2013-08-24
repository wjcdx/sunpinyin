#include "portability.h"
#include "ime-core/utils.h"
#include "ime-core/helper/CInputTrieSource.h"
#include "pinyin/lattice/CPyLatticeManager.h"
#include "lang_policy_cn.h"
#include "imi_option_keys.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <cassert>
#include <errno.h>

CSimplifiedChinesePolicy::CSimplifiedChinesePolicy()
    : m_bLoaded(false), m_bTried(false), m_csLevel(3),
      m_bEnableFullSymbol(false), m_bEnableFullPunct(true)
{
    m_getFullPunctOp.initPunctMap(getDefaultPunctMapping());
}

bool
CSimplifiedChinesePolicy::loadResources()
{
    if (m_bLoaded || m_bTried)
        return m_bLoaded;

    bool suc = true;
	std::string data_dir = m_data_dir.size() ? m_data_dir : SUNPINYIN_DATA_DIR;
	std::string lm_path = data_dir + "/lm_sc.t3g.py";
    std::string dict_path = data_dir + "/dict3_sc.bin.py";

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
CSimplifiedChinesePolicy::createContext()
{
    CIMIContext* pic = new CIMIContext();
    
	CInputTrieSource* ipt = new CInputTrieSource();
	ipt->setUserDict(&m_userDict);
	pic->setInputTrieSource(ipt);

	pic->setCoreData(&m_coreData);
    pic->setCharsetLevel(m_csLevel);
	pic->setHistoryMemory(&m_historyCache);

	CFullCharManager::setFullSymbolForwarding(m_bEnableFullSymbol);
	CFullCharManager::setGetFullSymbolOp(&m_getFullSymbolOp);

	CFullCharManager::setFullPunctForwarding(m_bEnableFullPunct);
	CFullCharManager::setGetFullPunctOp(&m_getFullPunctOp);
    return pic;
}

void
CSimplifiedChinesePolicy::destroyContext(CIMIContext *context)
{
    assert(context != NULL);
    saveUserHistory();
    delete context;
}

struct u8_code {          
	char uc0, uc1, uc2, uc3, uc4, uc5, uc6, uc7;
};

string_pairs
CSimplifiedChinesePolicy::getDefaultPunctMapping() const
{
    static struct u8_code punc_map[] = {
    	/* " " */  {0x20, 0x00}, {0xe3, 0x80, 0x80, 0x00},
      /* "," */  {0x2c, 0x00}, {0xef, 0xbc, 0x8c, 0x00},
      /* ";" */  {0x3b, 0x00}, {0xef, 0xbc, 0x9b, 0x00},
      /* "!" */  {0x21, 0x00}, {0xef, 0xbc, 0x81, 0x00},
      /* "?" */  {0x3f, 0x00}, {0xef, 0xbc, 0x9f, 0x00},
      /* "." */  {0x2e, 0x00}, {0xe3, 0x80, 0x82, 0x00},
      /* ":" */  {0x3a, 0x00}, {0xef, 0xbc, 0x9a, 0x00},
      /* "^" */  {0x5e, 0x00}, {0xe2, 0x80, 0xa6, 0xe2, 0x80, 0xa6, 0x00},
      /* "\\" */  {0x5c, 0x00}, {0xe3, 0x80, 0x81, 0x00},
      /* "\"" */  {0x22, 0x00}, {0xe2, 0x80, 0x9c, 0x00},
      /* "\"" */  {0x22, 0x00}, {0xe2, 0x80, 0x9d, 0x00},
      /* "'" */  {0x27, 0x00}, {0xe2, 0x80, 0x98, 0x00},
      /* "'" */  {0x27, 0x00}, {0xe2, 0x80, 0x99, 0x00},
      /* "_" */  {0x5f, 0x00}, {0xe2, 0x80, 0x94, 0xe2, 0x80, 0x94, 0x00},
      /* "<" */  {0x3c, 0x00}, {0xe3, 0x80, 0x8a, 0x00},
      /* ">" */  {0x3e, 0x00}, {0xe3, 0x80, 0x8b, 0x00},
      /* "(" */  {0x28, 0x00}, {0xef, 0xbc, 0x88, 0x00},
      /* ")" */  {0x29, 0x00}, {0xef, 0xbc, 0x89, 0x00},
      /* "[" */  {0x5b, 0x00}, {0xe3, 0x80, 0x90, 0x00},
      /* "]" */  {0x5d, 0x00}, {0xe3, 0x80, 0x91, 0x00},
      /* "{" */  {0x7b, 0x00}, {0xe3, 0x80, 0x8e, 0x00},
      /* "}" */  {0x7d, 0x00}, {0xe3, 0x80, 0x8f, 0x00},
      /* "$" */  {0x24, 0x00}, {0xef, 0xbf, 0xa5, 0x00},
      /* end */  {0x00, 0x00},
    };

    string_pairs default_punc_map;

    struct u8_code *p = punc_map;
    while (p->uc0) {
        std::string k = (const char *)p++;
        std::string v = (const char *)p++;
        default_punc_map.push_back(std::make_pair(k, v));
    }
    return default_punc_map;
}

bool
CSimplifiedChinesePolicy::onConfigChanged(const COptionEvent& event)
{
    if (event.name == SYSTEM_DATA_DIR) {
        setDataDir(event.get_string());
    } else if (event.name == USER_DATA_DIR) {
        setUserDataDir(event.get_string());
    } else if (event.name == CONFIG_GENERAL_CHARSET_LEVEL) {
        m_csLevel = event.get_int() & 3;
    }

    return false;
}

bool
CSimplifiedChinesePolicy::saveUserHistory()
{
    return m_historyCache.saveToFile();
}

bool
CSimplifiedChinesePolicy::createDirectory(char *path)
{
    char *p = path;
    while ((p = strchr(p + 1, '/'))) {
        *p = 0;
        if (access(path, F_OK) != 0 && mkdir(path, S_IRWXU) != 0) {
            fprintf(stderr, "mkdir %s: %s\n", path, strerror(errno));
            return false;
        }
        *p = '/';
    }
    return !(access(path, F_OK) != 0 && mkdir(path, S_IRWXU) != 0);
}

