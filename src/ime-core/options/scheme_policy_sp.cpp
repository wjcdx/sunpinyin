#include "scheme_policy_sp.h"
#include "imi_option_keys.h"

CShuangpinSchemePolicy::CShuangpinSchemePolicy()
    : m_shuangpinType(MS2003)
{
}

bool
CShuangpinSchemePolicy::onConfigChanged(const COptionEvent& event)
{
    if (event.name == SHUANGPIN_TYPE) {
        setShuangpinType((EShuangpinType)event.get_int());
        return true;
    } else if (event.name == QUANPIN_FUZZY_ENABLED) {
        setFuzzyForwarding(event.get_bool());
        return true;
    } else if (event.name == QUANPIN_FUZZY_PINYINS) {
        setFuzzyPinyinPairs(event.get_string_pair_list());
        return true;
    }

    return false;
}

