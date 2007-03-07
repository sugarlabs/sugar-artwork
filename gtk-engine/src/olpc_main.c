#include <gmodule.h>
#include "olpc_rc_style.h"
#include "olpc_style.h"

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  olpc_rc_style_register_type (module);
  olpc_style_register_type (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  void *ptr;
  ptr = GTK_RC_STYLE (g_object_new (OLPC_TYPE_RC_STYLE, NULL));  
  return (GtkRcStyle *)ptr;
}
