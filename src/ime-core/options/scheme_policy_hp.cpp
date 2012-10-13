#include "scheme_policy_hp.h"
#include "imi_option_keys.h"

bool
CHunpinSchemePolicy::onConfigChanged(const COptionEvent& event)
{
    if (event.name == SHUANGPIN_TYPE) {
        setShuangpinType((EShuangpinType)event.get_int());
        //刷新segmentor狀態
        COptionEventBus& event_bus = AOptionEventBus::instance();
        event_bus.publishEvent(COptionEvent(PINYIN_SCHEME, -1));        //第二個參數沒有用
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


CHunpinSchemePolicy::CHunpinSchemePolicy()
    : m_shuangpinType(MS2003)
{
}

