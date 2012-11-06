#include <locale.h>
#include <libintl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ibus.h>
#include "engine.h"
#include "ibus_common.h"
#include "sunpinyin_config.h"
#include "debug.h"

#define N_(String) (String)
#define _(String)  gettext(String)

static ibus::Factory factory;

// options
static gboolean by_ibus = FALSE;
static gboolean verbose = FALSE;

static const GOptionEntry entries[] = 
{
    { "ibus",    'i', 0, G_OPTION_ARG_NONE, &by_ibus, "component is executed by ibus", NULL },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "verbose", NULL },
    { NULL },
};


static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
    ibus_quit ();
}

IBusBus *bus = NULL;
ibus::Component component;

static void
init ()
{
    ibus_init ();
    bus = ibus_bus_new ();
    g_object_ref_sink(bus);
    
    if (!ibus_bus_is_connected (bus)) {
        g_warning("Can not connect to ibus");
        exit (0);
    }
    
    g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb), NULL);
    
    IBusConfig* config = ibus_bus_get_config(bus);
    g_object_ref_sink(config);
    
    SunPinyinConfig::set_config(config);

    
    component = ibus_component_new ("org.freedesktop.IBus.SunPinyin",
                                    "SunPinyin2",
                                    "0.1.0",
                                    "LGPL/CDDL",
                                    "Kov Chai <tchaikov@gmail.com>",
                                    "http://code.google.com/p/sunpinyin/",
                                    "",
                                    "ibus-sunpinyin");
    ibus_component_add_engine (component,
                               ibus_engine_desc_new ("sunpinyin",
                                                     "SunPinyin",
                                                     _("Simplified Chinese Input Method developed by SUN"),
                                                     "zh_CN",
                                                     "LGPL/CDDL",
                                                     "Kov Chai <tchaikov@gmail.com>",
                                                     IBUS_SUNPINYIN_ICON_DIR"/sunpinyin-logo.png",
                                                     "en"));
    
    factory = ibus_factory_new (ibus_bus_get_connection (bus));
    ibus_factory_add_engine (factory, "sunpinyin", IBUS_TYPE_SUNPINYIN_ENGINE);

    if (by_ibus) {
        ibus_bus_request_name (bus, "org.freedesktop.IBus.SunPinyin", 0);
    } else {
        ibus_bus_register_component (bus, component);
    }
    ibus_main ();
}

int main(int argc, char *argv[])
{

    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, IBUS_SUNPINYIN_LOCALEDIR);
    textdomain (GETTEXT_PACKAGE);

    //printf("sunpinyin daemon: main\n");
    ibus::log << "sunpinyin daemon: main\n";
    ibus::log.flush();
    
    GOptionContext* context;
    context = g_option_context_new ("- ibus sunpinyin engine component");
    g_option_context_add_main_entries (context, entries, "ibus-sunpinyin");
    GError *error = NULL;
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("Option parsing failed: %s\n", error->message);
        return -1;
    }
    // mask SIGTERM so that the destroy() method has the chance to be called
    // in case user quits X session.
    sighold(SIGTERM);
    init ();
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
