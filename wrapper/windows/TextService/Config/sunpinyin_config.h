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

#ifndef SUNPINYIN_CONFIG_H
#define SUNPINYIN_CONFIG_H

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sunpinyin.h>
#include "GVariant.h"

class ConfigWindow;

enum CharsetLevel
{
    GB2312,
    GBK,
    GB18030
};

class SunPinyinConfig
{
public:
    typedef std::map<std::string,
        CSunpinyinSessionFactory::EScheme> SchemeNames;
    typedef std::map<std::string,
        EShuangpinType> TypeNames;
	typedef std::map<std::string,
		GVariant> NvPairs;

private:

    SchemeNames        m_scheme_names;
    TypeNames          m_type_names;
	NvPairs			   m_config_pairs;

public:
    SunPinyinConfig();
    virtual ~SunPinyinConfig();

	bool init_configs(NvPairs &nvs);
	bool save_configs();
	GVariant read_config_value(GVariant::EType type, const char *value);
	std::string serialize_config_value(GVariant &gvar);


	// called by SunPinyinEngine to get configs
    bool get(const char* key, bool val);
    void set(const char* key, bool val);

    int get(const char* key, int val);
    void set(const char* key, int val);

    std::string get(const char *key, const std::string& default_val);
    void set(const char* key, const std::string& val);

    std::vector<std::string> get(const char *key, const std::vector<std::string>& val);

    CSunpinyinSessionFactory::EScheme get_py_scheme(CSunpinyinSessionFactory::EScheme);

    bool is_initial_mode_cn();
    bool is_initial_punct_full();
    bool is_initial_letter_full();

	/**
	 * show & check changes from ConfigWindow;
	 * if anything changed, config.on_config_value_changed
	 * will be called.
	 */
	bool show_config_window(SunPinyinConfig *config);



private:

	GVariant config_get_value(const char *key);
	void config_set_value(const char *key, GVariant &value);

    /**
     * called by ibus when a value changed in config
     */
    void on_config_value_changed(const char *key,
                                       GVariant &value);

    std::string get_scheme_name(CSunpinyinSessionFactory::EScheme scheme);
    CSunpinyinSessionFactory::EScheme get_scheme(const std::string& name);

	std::string m_config_file;
};

#endif // SUNPINYIN_CONFIG_H

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
