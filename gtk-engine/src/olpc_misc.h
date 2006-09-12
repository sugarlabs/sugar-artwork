GtkShadowType
GE_INTERNAL olpc_shadow_type (GtkStyle *style, const char *detail, GtkShadowType requested);

GE_INTERNAL void
olpc_slash_two(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height);

GE_INTERNAL void
olpc_slash_one(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height);

GE_INTERNAL void
olpc_dot(cairo_t *cr,
            CairoColor *color1,
            CairoColor *color2,
            gint x,
            gint y);

GE_INTERNAL void olpc_arrow (cairo_t *canvas,
			CairoColor *color,
			GtkArrowType arrow_type, 
                        gboolean fill,
			gint x, 
			gint y, 
			gint width, 
			gint height);

GE_INTERNAL void olpc_draw_separator(cairo_t *cr, gboolean horizontal,
                           int x, int y, int width, int height);
