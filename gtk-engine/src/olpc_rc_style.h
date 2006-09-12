#include <gtk/gtkrc.h>
#include "ge-support.h"

typedef struct _OlpcRcStyle OlpcRcStyle;
typedef struct _OlpcRcStyleClass OlpcRcStyleClass;

GE_INTERNAL extern GType olpc_type_rc_style;

#define OLPC_TYPE_RC_STYLE              olpc_type_rc_style
#define OLPC_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), OLPC_TYPE_RC_STYLE, OlpcRcStyle))
#define OLPC_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), OLPC_TYPE_RC_STYLE, OlpcRcStyleClass))
#define OLPC_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), OLPC_TYPE_RC_STYLE))
#define OLPC_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), OLPC_TYPE_RC_STYLE))
#define OLPC_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), OLPC_TYPE_RC_STYLE, OlpcRcStyleClass))

struct _OlpcRcStyle
{
  GtkRcStyle parent_instance;

  guint scrollbar_type:1;
  guint scrollbar_marks:1;
  guint scroll_button_marks:1;
  guint handlebox_marks:1;
  guint mark_type1;
  guint mark_type2;
  guint paned_dots;
};

struct _OlpcRcStyleClass
{
  GtkRcStyleClass parent_class;
};

GE_INTERNAL void olpc_rc_style_register_type (GTypeModule *module);

/* Default stuff */
#define DEFAULT_SCROLLSHAPE       SCROLL_SHAPED
#define DEFAULT_SCROLLBARMARKS    MARKS_ON 
#define DEFAULT_SCROLLBUTTONMARKS MARKS_ON
#define DEFAULT_HANDLEBOXMARKS    MARKS_ON
#define DEFAULT_MARKTYPE1         MARKS_SLASH
#define DEFAULT_MARKTYPE2         MARKS_INVSLASH
#define DEFAULT_PANEDDOTS         PANED_DOTSFULL

#define DEFAULT_SCROLLTHUMB_SIZE  12
#define DEFAULT_MIN_SLIDER_SIZE   9
#define SMALLEST_HANDLE           17

enum
  {
    TOKEN_RECTSCROLLBAR = G_TOKEN_LAST + 1,
    TOKEN_SCROLLBARMARKS,
    TOKEN_SCROLLBUTTONMARKS,
    TOKEN_HANDLEBOXMARKS,
    TOKEN_MARKTYPE1,
    TOKEN_MARKTYPE2,
    TOKEN_PANEDDOTS,

    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_NOTHING,
    TOKEN_SLASH,
    TOKEN_INVSLASH,
    TOKEN_DOT,
    TOKEN_INVDOT,
    TOKEN_ARROW,

    TOKEN_FULL,
    TOKEN_SOME,
    TOKEN_NONE
  };
  
enum
  {
    SCROLL_RECT = 0,
    SCROLL_SHAPED = 1,

    MARKS_OFF = 0,
    MARKS_ON = 1,

    MARKS_NOTHING = 0,
    MARKS_SLASH = 1,
    MARKS_INVSLASH = 2,
    MARKS_DOT = 3,
    MARKS_INVDOT = 4,
    MARKS_ARROW = 5,

    PANED_DOTSFULL = 0,
    PANED_DOTSSOME = 1,
    PANED_DOTSNONE = 2
  };

