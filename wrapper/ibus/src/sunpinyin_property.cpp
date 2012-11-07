#include <libintl.h>
#include <ibus.h>
#include "ibus_portable.h"
#include "sunpinyin_property.h"

#define N_(String) (String)
#define _(String)  gettext(String)

static const char *PROP_STATUS = "status";
static const char *PROP_LETTER = "full_letter";
static const char *PROP_PUNCT  = "full_punct";

PropertyInfo::PropertyInfo()
    : label(NULL), tooltip(NULL)
{}

PropertyInfo::~PropertyInfo()
{}


SunPinyinProperty 
SunPinyinProperty::create_status_prop(ibus::Engine engine, bool state)
{
    SunPinyinProperty prop(engine, PROP_STATUS);
    prop.m_info[0].label = ibus_text_new_from_static_string("EN");
    prop.m_info[0].icon  = IBUS_SUNPINYIN_ICON_DIR"/eng.svg";
    prop.m_info[0].tooltip = ibus_text_new_from_static_string(_("Switch to Chinese input mode"));
    prop.m_info[1].label = ibus_text_new_from_static_string("CN");
    prop.m_info[1].icon  = IBUS_SUNPINYIN_ICON_DIR"/han.svg";
    prop.m_info[1].tooltip = ibus_text_new_from_static_string(_("Switch to English input mode"));
    prop.init(state);
    
    return prop;
}

SunPinyinProperty
SunPinyinProperty::create_letter_prop(ibus::Engine engine, bool state)
{
    SunPinyinProperty prop(engine, PROP_LETTER);
    prop.m_info[0].label = ibus_text_new_from_static_string("Aa");
    prop.m_info[0].icon  = IBUS_SUNPINYIN_ICON_DIR"/halfwidth.svg";
    prop.m_info[0].tooltip = ibus_text_new_from_static_string(_("Switch to full-width letter input mode"));
    prop.m_info[1].label = ibus_text_new_from_static_string("Ａａ");
    prop.m_info[1].icon  = IBUS_SUNPINYIN_ICON_DIR"/fullwidth.svg";
    prop.m_info[1].tooltip = ibus_text_new_from_static_string(_("Switch to half-width letter input mode"));
    prop.init(state);
    return prop;
}

SunPinyinProperty 
SunPinyinProperty::create_punct_prop(ibus::Engine engine, bool state)
{
    SunPinyinProperty prop(engine, PROP_PUNCT);
    prop.m_info[0].label = ibus_text_new_from_static_string(",.");
    prop.m_info[0].icon  = IBUS_SUNPINYIN_ICON_DIR"/enpunc.svg";
    prop.m_info[0].tooltip = ibus_text_new_from_static_string(_("Switch to Chinese punctuation"));
    prop.m_info[1].label = ibus_text_new_from_static_string("，。");
    prop.m_info[1].icon  = IBUS_SUNPINYIN_ICON_DIR"/cnpunc.svg";
    prop.m_info[1].tooltip = ibus_text_new_from_static_string(_("Switch to English punctuation"));
    prop.init(state);
    return prop;

}

SunPinyinProperty::SunPinyinProperty(ibus::Engine engine, const std::string& name)
    : ibus::Property(
        ibus_property_new(name.c_str(),
                          PROP_TYPE_NORMAL,
                          NULL, /* label */ NULL, /* icon */
                          NULL, /* tooltip */ TRUE, /* sensitive */
                          TRUE, /* visible */ PROP_STATE_UNCHECKED, /* state */
                          NULL)),
      m_engine(engine),
      m_name(name),
      m_state(false)
{}

SunPinyinProperty::~SunPinyinProperty()
{
}

bool
SunPinyinProperty::toggle(const std::string& name)
{
    if (name == m_name) {
        // called by ibus, simple toggle current state
        update(!m_state);
        return true;
    }
    return false;
}

void
SunPinyinProperty::update(bool state)
{
    if (state == m_state)
        return;
    init(state);
    ibus_engine_update_property(m_engine, *this);
}

void
SunPinyinProperty::init(bool state)
{
    m_state = state;
    int which = m_state ? 1 : 0;
    PropertyInfo& info = m_info[which];
    ibus_property_set_label(*this, info.label);
    ibus_property_set_icon(*this, info.icon.c_str());
    ibus_property_set_tooltip(*this, info.tooltip);
    ibus_property_set_visible(*this, TRUE);
    ibus_property_set_state(*this, state ? PROP_STATE_CHECKED : PROP_STATE_UNCHECKED);
}

bool
SunPinyinProperty::state() const
{
    return m_state;
}

SetupLauncher::SetupLauncher()
    : ibus::Property(
        ibus_property_new("setup",
                          PROP_TYPE_NORMAL,
                          NULL, /* label */ NULL, /* icon */
                          NULL, /* tooltip */ TRUE, /* sensitive */
                          TRUE, /* visible */ PROP_STATE_UNCHECKED, /* state */
                          NULL)),
      m_name("setup")
{
    m_info.label   = ibus_text_new_from_static_string(_("Preference"));
    m_info.tooltip = ibus_text_new_from_static_string(_("Preference"));
    m_info.icon    = IBUS_SUNPINYIN_ICON_DIR"/setup.svg";
    init();
}

void
SetupLauncher::launch(const std::string& name)
{
    if (m_name != name) return;
    
    GError *error = NULL;
    gchar *argv[2] = { NULL, };
    gchar *path;
    const char* libexecdir;
    
    libexecdir = g_getenv("LIBEXECDIR");
    if (libexecdir == NULL)
        libexecdir = LIBEXECDIR;
    
    path = g_build_filename(libexecdir, "ibus-setup-sunpinyin", NULL);
    argv[0] = path;
    argv[1] = NULL;
    gboolean success;
    success = g_spawn_async (NULL, argv, NULL,
                             G_SPAWN_SEARCH_PATH,
                             NULL, NULL, NULL, &error);
    if (!success) {
        g_message("Unabled to launch \"%s\"", path);
    }
    g_free(path);
}

void
SetupLauncher::init()
{
    ibus_property_set_label (*this, m_info.label);
    ibus_property_set_icon (*this, m_info.icon.c_str());
    ibus_property_set_tooltip (*this, m_info.tooltip);
    ibus_property_set_visible (*this, TRUE);
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
