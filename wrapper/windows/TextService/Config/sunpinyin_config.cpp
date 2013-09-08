/*
 * Copyright (c) 2009 Kov Chai <tchaikov@gmail.com>
 *
 * The contents of this file are subject to the terms of either the GNU Lesser
 * General Public License Version 2.1 only ("LGPL") or the Common Development and
 * Distribution License ("CDDL")(collectively, the "License"). You may not use this
 * file except in compliance with the License. You can obtain a copy of the CDDL at
 * http://www.opensource.org/licenses/cddl1.php and a copy of the LGPLv2.1 at
 * http://www.opensource.org/licenses/lgpl-license.php. See the License for the 
 * specific language governing permissions and limitations under the License. When
 * distributing the software, include this License Header Notice in each file and
 * include the full text of the License in the License file as well as the
 * following notice:
 * 
 * NOTICE PURSUANT TO SECTION 9 OF THE COMMON DEVELOPMENT AND DISTRIBUTION LICENSE
 * (CDDL)
 * For Covered Software in this distribution, this License shall be governed by the
 * laws of the State of California (excluding conflict-of-law provisions).
 * Any litigation relating to this License shall be subject to the jurisdiction of
 * the Federal Courts of the Northern District of California and the state courts
 * of the State of California, with venue lying in Santa Clara County, California.
 * 
 * Contributor(s):
 * 
 * If you wish your version of this file to be governed by only the CDDL or only
 * the LGPL Version 2.1, indicate your decision by adding "[Contributor]" elects to
 * include this software in this distribution under the [CDDL or LGPL Version 2.1]
 * license." If you don't indicate a single choice of license, a recipient has the
 * option to distribute your version of this file under either the CDDL or the LGPL
 * Version 2.1, or to extend the choice of license to its licensees as provided
 * above. However, if you add LGPL Version 2.1 code and therefore, elected the LGPL
 * Version 2 license, then the option applies only if the new code is made subject
 * to such option by the copyright holder. 
 */

#include <cassert>
#include <vector>
#include <sunpinyin.h>
#include "sunpinyin_config_keys.h"
#include "sunpinyin_config.h"
#include "ConfigWindow.h"

using namespace std;


SunPinyinConfig::SunPinyinConfig()
	: m_config_file(std::string("D:/cygwin/home/wjcdx/git/spy/wrapper/windows/TextService/Config/configs.dat"))
{
    m_scheme_names["QuanPin"]    = CSunpinyinSessionFactory::QUANPIN;
    m_scheme_names["ShuangPin"]  = CSunpinyinSessionFactory::SHUANGPIN;
    m_scheme_names["XingHua"]  = CSunpinyinSessionFactory::XINGHUA;

	init_configs(m_config_pairs);

	m_pConfigWindow = new CConfigWindow(this);
	if (!m_pConfigWindow->_Create())
		return;
	m_pConfigWindow->_InitConfigItems();
	m_pConfigWindow->_Show();
}

SunPinyinConfig::~SunPinyinConfig()
{
	save_configs();
}

CSunpinyinSessionFactory::EScheme
SunPinyinConfig::get_py_scheme(CSunpinyinSessionFactory::EScheme scheme)
{
    string default_name =
        get_scheme_name(scheme);
    string name = get(PINYIN_SCHEME, default_name);
    return get_scheme(name);
}

bool
SunPinyinConfig::is_initial_mode_cn()
{
    string init_mode("Chinese");
    init_mode = get(CONFIG_GENERAL_INITIAL_MODE, init_mode);
    return (init_mode == "Chinese");
}

bool
SunPinyinConfig::is_initial_punct_full()
{
    string init_punct("Full");
    init_punct = get(CONFIG_GENERAL_INITIAL_PUNCT, init_punct);
    return (init_punct == "Full");
}

bool
SunPinyinConfig::is_initial_letter_full()
{
    string init_letter("Half");
    init_letter = get(CONFIG_GENERAL_INITIAL_LETTER, init_letter);
    return (init_letter == "Full");
}

std::string
SunPinyinConfig::get_scheme_name(CSunpinyinSessionFactory::EScheme scheme)
{
    string val = "ShuangPin";
    for (SchemeNames::iterator it = m_scheme_names.begin();
         it != m_scheme_names.end(); ++it) {
        if (it->second == scheme)
            val = it->first;
    }
    return val;
}

CSunpinyinSessionFactory::EScheme
SunPinyinConfig::get_scheme(const std::string& name)
{
    CSunpinyinSessionFactory::EScheme val = CSunpinyinSessionFactory::SHUANGPIN;
    SchemeNames::iterator it = m_scheme_names.find(name);
    if (it != m_scheme_names.end()) {
        val = it->second;
    }
    return val;
}

bool
SunPinyinConfig::get(const char* key, bool val)
{
    assert(m_config != NULL);
    
    //ConfigItem item(key);
    GVariant value = config_get_value(key);
    bool result = val;
	if (value.TypeEquals(GVariant::G_VARIANT_CLASS_BOOLEAN)) {
		result = (value.GetBool() == TRUE);
    }
    return result;
}

void
SunPinyinConfig::set(const char* key, bool val)
{
    assert(m_config != NULL);
    
    GVariant value(val);
    config_set_value(key, value);
}

std::string
SunPinyinConfig::get(const char* key, const std::string& val)
{
    assert(m_config != NULL);
    
    GVariant value = config_get_value(key);
    string result = val;
    if (value.TypeEquals(GVariant::G_VARIANT_CLASS_STRING)) {
		result = value.GetString();
    }
    return result;
}

void
SunPinyinConfig::set(const char* key, const std::string& val)
{
    assert(m_config != NULL);
    
    GVariant value(val);
    config_set_value(key, value);
}

int
SunPinyinConfig::get(const char* key, int val)
{
    assert(m_config != NULL);
    
    GVariant value = config_get_value(key);
    int result = val;
    if (value.TypeEquals(GVariant::G_VARIANT_CLASS_INT32)) {
		result =  value.GetInt();
    }
    return result;
}

void 
SunPinyinConfig::set(const char* key, int val)
{
    assert(m_config != NULL);
    
    GVariant value(val);
    config_set_value(key, value);
}

std::vector<std::string>
SunPinyinConfig::get(const char* key, const std::vector<std::string>& val)
{
    assert(m_config != NULL);
    
    GVariant value = config_get_value(key);
    vector<string> result(val);
    if (value.TypeEquals(GVariant::G_VARIANT_CLASS_ARRAY)) {
		result =  value.GetArray();
    }
    return result;
}

static COptionEvent
g_value_to_event(const char *key, GVariant &value)
{
    string event_name = key;
    
    switch (value.GetType()) {
    case GVariant::G_VARIANT_CLASS_BOOLEAN:
        return COptionEvent(event_name, value.GetBool());
    case GVariant::G_VARIANT_CLASS_INT32:
		return COptionEvent(event_name, value.GetInt());
    case GVariant::G_VARIANT_CLASS_STRING:
		return COptionEvent(event_name, value.GetString());
	case GVariant::G_VARIANT_CLASS_ARRAY:
		return COptionEvent(event_name, value.GetArray());
    default:
        assert(false && "unknown gvalue");
        return COptionEvent(event_name, 0);
    }   
}

void
SunPinyinConfig::on_config_value_changed(const char *key,
                                         GVariant &value)
{
	config_set_value(key, value);
    COptionEvent event = g_value_to_event(key, value);
    AOptionEventBus::instance().publishEvent(event);
}

GVariant
SunPinyinConfig::config_get_value(const char *key)
{
	assert(key != NULL);
	string name(key);
	NvPairs::iterator it = m_config_pairs.find(key);
	if (it != m_config_pairs.end())
	{
		return it->second;
	}
	return GVariant();
}

void
SunPinyinConfig::config_set_value(const char *key, GVariant &value)
{
	assert(key != NULL);
	string name(key);
	NvPairs::iterator it = m_config_pairs.find(key);
	if (it != m_config_pairs.end())
	{
		it->second = value;
	}
	return;
}

bool SunPinyinConfig::init_configs(NvPairs &nvs)
{
	
	if (m_config_file.length() == 0)
		return false;
	
	ifstream icfg(m_config_file, std::ifstream::in);
	if (!icfg) return false;
	while (!icfg.eof())
	{
		char line[512] = { 0 };
		char key[128] = { 0 };
		char value[128] = { 0 };
		int type = 0;
		icfg.getline(line, 512);

		if (line[0] == '\0' || line[0] == '#')
			continue;

		int ret = sscanf(line, "%s %d %s", key, &type, value);
		if (ret < 3)
			continue;

		GVariant gvar = read_config_value((GVariant::EType)type, value);
		m_config_pairs[std::string(key)] = gvar;
	}
	icfg.close();
	return true;
}

bool SunPinyinConfig::save_configs()
{
	if (m_config_file.length() == 0)
		return false;
	
	ofstream ocfg(m_config_file);
	if (!ocfg) return false;

	NvPairs::iterator it = m_config_pairs.begin();
	NvPairs::iterator ite = m_config_pairs.end();

	for (; it != ite; it++)
	{
		ocfg << it->first.c_str() << " ";
		ocfg << it->second.GetType() << " ";
		ocfg << serialize_config_value(it->second).c_str() << endl;
	}
	return true;
}

GVariant
SunPinyinConfig::read_config_value(GVariant::EType type, const char *value)
{
	int ival = 0;
	switch (type) {
	case GVariant::G_VARIANT_CLASS_BOOLEAN:
		if (strcmp(value, "yes") == 0) {
			return GVariant(true);
		} else {
			return GVariant(false);
		}
    case GVariant::G_VARIANT_CLASS_INT32:
		sscanf(value, "%d", &ival);
		return GVariant(ival);
    case GVariant::G_VARIANT_CLASS_STRING:
		return GVariant(std::string(value));
	case GVariant::G_VARIANT_CLASS_ARRAY:
		return GVariant();
    default:
        assert(false && "unknown gvalue");
		return GVariant();
    }
	return GVariant();
}

std::string
SunPinyinConfig::serialize_config_value(GVariant &gvar)
{
	char value[128] = { 0 };
	switch (gvar.GetType()) {
	case GVariant::G_VARIANT_CLASS_BOOLEAN:
		if (gvar.GetBool()) {
			sprintf(value, "%s", "yes");
		} else {
			sprintf(value, "%s", "no");
		}
		break;
    case GVariant::G_VARIANT_CLASS_INT32:
		sprintf(value, "%d", gvar.GetInt());
		break;
    case GVariant::G_VARIANT_CLASS_STRING:
		sprintf(value, "%s", gvar.GetString());
		break;
	case GVariant::G_VARIANT_CLASS_ARRAY:
		break;
    default:
        assert(false && "unknown gvalue");
		break;
    }
	return std::string(value);
}


// -*- indent-tabs-mode: nil -*- vim:et:ts=4
