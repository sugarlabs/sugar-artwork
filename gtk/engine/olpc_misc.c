#include <math.h>
#include <string.h>
#include <gtk/gtknotebook.h>

#include "olpc_style.h"
#include "olpc_rc_style.h"
#include "olpc_misc.h"

GtkShadowType
olpc_shadow_type (GtkStyle *style, const char *detail, GtkShadowType requested)
{
    GtkShadowType retval = GTK_SHADOW_NONE;

    if (requested != GTK_SHADOW_NONE) {
        retval = GTK_SHADOW_ETCHED_IN;
    }

    if (CHECK_DETAIL (detail, "dockitem") || CHECK_DETAIL (detail, "handlebox_bin") || CHECK_DETAIL (detail, "spinbutton_up") || CHECK_DETAIL (detail, "spinbutton_down")) {
        retval = GTK_SHADOW_NONE;
    } else if (CHECK_DETAIL (detail, "button") || CHECK_DETAIL (detail, "togglebutton") || CHECK_DETAIL (detail, "notebook") || CHECK_DETAIL (detail, "optionmenu")) {
        retval = requested;
    } else if (CHECK_DETAIL (detail, "menu")) {
        retval = GTK_SHADOW_ETCHED_IN;
    }

    return retval;
}

/***********************************************
 * olpc_arrow-
 *  
 *    borrowed from redmond till shareable
 ***********************************************/
void olpc_arrow (cairo_t *canvas,
               CairoColor * color,
               GtkArrowType arrow_type,
               gboolean fill,
               gint x, 
               gint y, 
               gint width, 
               gint height)
{
	gint aw = width, ah = height;
 	GdkPoint points[3];

	switch (arrow_type)
	{
		case GTK_ARROW_UP:
		case GTK_ARROW_DOWN:
		{
			gdouble tmp=((aw+1)/2) - ((height%2)?1:0);
       
			if (tmp > ah) 
			{
				aw = 2*ah - 1 - ((height%2)?1:0);
				ah = (aw+1)/2;
			} 
			else 
			{
				ah = (gint) tmp;
				aw = 2*ah - 1;
			}  
 
			if ((aw < 5) || (ah < 3)) 
			{
				aw = 5;
				ah = 3;
			}
 
			x += (width - aw) / 2 ;
			y += (height - ah) / 2;
			width = aw;
			height = ah;
 		
			width += width % 2 - 1;

			points[0].x = x;
			points[1].x = x + width - 1;
			points[2].x = x + ((height - 1) - (height - (1 + width / 2)));

			points[0].y = points[1].y = y;
			points[2].y = y + height - 1;

			if (arrow_type == GTK_ARROW_UP)
			{
				gint flip = points[1].y;

				points[0].y = points[1].y = points[2].y;
				points[2].y = flip;
			}
		}
		break;

		case GTK_ARROW_LEFT:
		case GTK_ARROW_RIGHT:
		{
			gdouble tmp=((ah+1)/2) - ((width%2)?1:0);
 
			if (tmp > aw) 
			{
				ah = 2*aw - 1 - ((width%2)?1:0);
				aw = (ah+1)/2;
			} 
			else 
			{
				aw = (gint) tmp;
				ah = 2*aw - 1;
			}  
 
			if ((ah < 5) || (aw < 3)) 
			{
				ah = 5;
				aw = 3;
			}
 
			x += (width - aw) / 2 ;
			y += (height - ah) / 2;
			width = aw;
			height = ah;
 
			height += height % 2 - 1;

			points[0].y = y;
			points[1].y = y + height - 1;
			points[2].y = y + ((width - 1) - (width - (1 + height / 2)));

			points[0].x = points[1].x = x;
			points[2].x = x + width - 1;

			if (arrow_type == GTK_ARROW_LEFT)
			{
				gint flip = points[0].x;

				points[0].x = points[1].x = points[2].x;
				points[2].x = flip;
			}
		}
		break;

		default:
		{
			return;
		}
	}

	cairo_save(canvas);

	ge_cairo_set_color(canvas, color);	
	cairo_set_line_width (canvas, 0.5);

	cairo_move_to(canvas, points[0].x + 0.5, points[0].y + 0.5);
	cairo_line_to(canvas, points[1].x + 0.5, points[1].y + 0.5);
	cairo_line_to(canvas, points[2].x + 0.5, points[2].y + 0.5);
	cairo_line_to(canvas, points[0].x + 0.5, points[0].y + 0.5);

	if (fill)
	{
		cairo_stroke_preserve(canvas);

		cairo_fill(canvas);
	}
	else
	{
		cairo_stroke(canvas);
	}

	cairo_restore(canvas);
}

void
olpc_slash_two(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height)
{
  gdouble centerx, centery, thick;
  gint ax1=0,ax2=0,ay1=0,ay2=0;

  centerx = width/2 + x + 0.5;
  centery = height/2 + y + 0.5;
  if (width > height)
    {
      ax1 = -2; ax2 = 1;
    }
  else
    {
      ay1 = -2; ay2 = 1;
    }

  thick = ((width < height?width-1:height-1) >> 1) - 1.5;

	cairo_set_line_width (cr, 1);

	ge_cairo_set_color(cr, color2);	
  	cairo_move_to(cr, centerx - thick + ax1, centery + thick + ay1);
	cairo_line_to(cr, centerx + thick + ax1, centery - thick + ay1);
	cairo_stroke(cr);

	ge_cairo_set_color(cr, color1);	
	cairo_move_to(cr, centerx - thick + ax1 + ax2, centery + thick + ay1 + ay2);
	cairo_line_to(cr, centerx + thick + ax1 + ax2, centery - thick + ay1 + ay2);
	cairo_stroke(cr);

  if (width > height)
    {
      ax1 = 2; /* ax2 = 1; */
    }
  else
    {
      ay1 = 2; /* ay2 = 1; */
    }

	cairo_set_line_width (cr, 1);

	ge_cairo_set_color(cr, color2);	
  	cairo_move_to(cr, centerx - thick + ax1, centery + thick + ay1);
	cairo_line_to(cr, centerx + thick + ax1, centery - thick + ay1);
	cairo_stroke(cr);

	ge_cairo_set_color(cr, color1);	
	cairo_move_to(cr, centerx - thick + ax1 + ax2, centery + thick + ay1 + ay2);
	cairo_line_to(cr, centerx + thick + ax1 + ax2, centery - thick + ay1 + ay2);
	cairo_stroke(cr);
}


void
olpc_slash_one(cairo_t *cr,
                  CairoColor *color1,
                  CairoColor *color2,
                  gint x,
                  gint y,
                  gint width,
                  gint height)
{
  gint centerx, centery, thick;

  centerx = width/2 + x + 0.5;
  centery = height/2 + y + 0.5;

  thick = ((width < height?width:height) >> 1) - 1.5;

  cairo_set_line_width (cr, 1);

  ge_cairo_set_color(cr, color2);	
  cairo_move_to(cr, centerx - thick, centery + thick);
  cairo_line_to(cr, centerx + thick, centery - thick);
  cairo_stroke(cr);

  ge_cairo_set_color(cr, color2);	
  cairo_move_to(cr, centerx - thick, centery + thick - 1);
  cairo_line_to(cr, centerx + thick - 1, centery - thick);
  cairo_stroke(cr);
}

void
olpc_dot(cairo_t *cr,
            CairoColor *color1,
            CairoColor *color2,
            gint x,
            gint y)
{
	cairo_save(cr);

	cairo_set_line_width (cr, 0.5);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	ge_cairo_set_color(cr, color2);	

	cairo_rectangle (cr, x - 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x - 1, y - 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y - 1, 0.5, 0.5);

	cairo_stroke(cr);
	
	ge_cairo_set_color(cr, color1);	

	cairo_rectangle (cr, x + 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x + 1, y + 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y + 1, 0.5, 0.5);

	cairo_stroke(cr);

	cairo_restore(cr);
}


void
olpc_draw_separator(cairo_t *cr, gboolean horizontal,
                           int x, int y, int width, int height)
{
	if (horizontal)
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x, y+0.5);
		
		cairo_move_to         (cr, 0.0,     0.0);
		cairo_line_to         (cr, width+1, 0.0);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2);
		cairo_stroke          (cr);
		
		cairo_move_to         (cr, 0.0,   1.0);
		cairo_line_to         (cr, width, 1.0);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8);
		cairo_stroke          (cr);				
	}
	else
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x+0.5, y);
		
		cairo_move_to         (cr, 0.0, 0.0);
		cairo_line_to         (cr, 0.0, height);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2);
		cairo_stroke          (cr);
		
		cairo_move_to         (cr, 1.0, 0.0);
		cairo_line_to         (cr, 1.0, height);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8);
		cairo_stroke          (cr);		
	}
}
