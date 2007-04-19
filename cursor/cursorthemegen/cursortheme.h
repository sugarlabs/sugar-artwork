/*
 * Copyright © 2003 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Red Hat not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Red Hat makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL RED HAT
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Owen Taylor, Red Hat, Inc.
 */
#ifndef __CURSORTHEME_H__
#define __CURSORTHEME_H__

#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

typedef struct _Cursor Cursor;
typedef struct _CursorAlias CursorAlias;
typedef struct _CursorFrame CursorFrame;
typedef struct _CursorFrameConfig CursorFrameConfig;
typedef struct _CursorImage CursorImage;
typedef struct _CursorSource CursorSource;
typedef struct _CursorTheme CursorTheme;

struct _Cursor
{
  char *name;
  int row;
  int column;
  GSList *frame_configs;
  GSList *frames;
};

struct _CursorAlias
{
  char *name;
  char *target;
};

struct _CursorFrameConfig
{
  int delay;
};

struct _CursorFrame
{
  int size;
  int hot_x;
  int hot_y;
  int delay;
  GdkPixbuf *image;
};

struct _CursorImage
{
  int use;			/* Frame number or -1 for hotspot */
  GdkPixbuf *image;
};

struct _CursorSource
{
  int size;
  int gridsize;
  GSList *images;
};

struct _CursorTheme
{
  char *name;
  GSList *sources;
  GHashTable *cursors;
  GHashTable *aliases;
};

CursorTheme *cursor_theme_read (const char *filename);

G_END_DECLS

#endif /* __CURSORTHEME_H__ */
