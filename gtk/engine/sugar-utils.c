/*
 * Copyright (C) 2007, Benjamin Berg
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "sugar-utils.h"

GdkPixbuf*
sugar_pixbuf_scale_or_ref (GdkPixbuf *pixbuf,
                           gint       width,
                           gint       height)
{
    gint pixbuf_width = gdk_pixbuf_get_width (pixbuf);
    gint pixbuf_height = gdk_pixbuf_get_height (pixbuf);

    if ((pixbuf_width != width) || (pixbuf_height != height)) {
        return gdk_pixbuf_scale_simple (pixbuf, width, height, GDK_INTERP_BILINEAR);
    } else {
        g_object_ref (pixbuf);
        return pixbuf;
    }
}

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

GdkPixbuf*
sugar_get_insensitive_icon (GdkPixbuf *icon,
                            guint      range,
                            guint      base)
{
    GdkPixbuf *result = gdk_pixbuf_copy (icon);
    guint x, y, rowstride, height, width;
    guint n_channels;
    guint8 *pixel, *pixels;
    guint min_color_value = G_MAXUINT8;
    guint max_color_value = 0;
    guint mult;
    gboolean has_alpha;

    width = gdk_pixbuf_get_width (result);
    height = gdk_pixbuf_get_height (result);
    rowstride = gdk_pixbuf_get_rowstride (result);
    n_channels = gdk_pixbuf_get_n_channels (result);
    pixels = gdk_pixbuf_get_pixels (result);

    for (y = 0; y < height; y++) {
        pixel = pixels + y*rowstride;
        for (x = 0; x < width; x++) {
            if (n_channels == 4 && pixel[ALPHA] == 0) {
                pixel += n_channels;
                continue;
            }

            min_color_value = MIN (min_color_value, pixel[RED]);
            max_color_value = MAX (max_color_value, pixel[RED]);

            min_color_value = MIN (min_color_value, pixel[GREEN]);
            max_color_value = MAX (max_color_value, pixel[GREEN]);

            min_color_value = MIN (min_color_value, pixel[BLUE]);
            max_color_value = MAX (max_color_value, pixel[BLUE]);

            pixel += n_channels;
        }
    }

    /* Shift the base value to the lower side of the range */

    if (max_color_value - min_color_value > 0) {
        base = base - range / 2;
        mult = (range << 8) / (max_color_value - min_color_value);
    } else {
        /* There is only one color, just fill everything with base.
         * Se setting mult to 0 means that the old pixel value will just
         * be discarted. */
        mult = 0;
    }

    for (y = 0; y < height; y++) {
        pixel = pixels + y*rowstride;
        for (x = 0; x < width; x++) {
            pixel[RED] = ((((guint) (pixel[RED] - min_color_value)) * mult) >> 8) + base;
            pixel[GREEN] = ((((guint) (pixel[GREEN] - min_color_value)) * mult) >> 8) + base;
            pixel[BLUE] = ((((guint) (pixel[BLUE] - min_color_value)) * mult) >> 8) + base;

            pixel += n_channels;
        }
    }

    return result;
}

#undef RED
#undef GREEN
#undef BLUE
#undef ALPHA

