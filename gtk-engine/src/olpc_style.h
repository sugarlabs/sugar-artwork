#include <gtk/gtkstyle.h>
#include "ge-support.h"

typedef struct _OlpcStyle OlpcStyle;
typedef struct _OlpcStyleClass OlpcStyleClass;

GE_INTERNAL extern GType olpc_type_style;

#define OLPC_TYPE_STYLE              olpc_type_style
#define OLPC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), OLPC_TYPE_STYLE, OlpcStyle))
#define OLPC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), OLPC_TYPE_STYLE, OlpcStyleClass))
#define OLPC_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), OLPC_TYPE_STYLE))
#define OLPC_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), OLPC_TYPE_STYLE))
#define OLPC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), OLPC_TYPE_STYLE, OlpcStyleClass))

struct _OlpcStyle
{
  GtkStyle parent_instance;
  CairoColorCube color_cube;
};

struct _OlpcStyleClass
{
  GtkStyleClass parent_class;
};

GE_INTERNAL void olpc_style_register_type (GTypeModule *module);


