#include "scheme_policy_qp.h"
#include "imi_option_keys.h"

bool
CQuanpinSchemePolicy::onConfigChanged(const COptionEvent& event)
{
    if (event.name == QUANPIN_FUZZY_ENABLED) {
        setFuzzyForwarding(event.get_bool());
        return true;
    } else if (event.name == QUANPIN_FUZZY_PINYINS) {
        setFuzzyPinyinPairs(event.get_string_pair_list());
        return true;
    } else if (event.name == QUANPIN_AUTOCORRECTION_ENABLED) {
        setAutoCorrecting(event.get_bool());
        return true;
    } else if (event.name == QUANPIN_AUTOCORRECTION_PINYINS) {
        setAutoCorrectionPairs(event.get_string_pair_list());
        return true;
    }

    return false;
}

