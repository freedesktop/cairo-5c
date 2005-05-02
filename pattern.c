/* $Id$
 *
 * Copyright © 2004 Keith Packard
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is Keith Packard
 *
 * Contributor(s):
 *      Keith Packard <keithp@keithp.com>
 */

#include <png.h>
#include <stdlib.h>
#include "cairo-5c.h"

static char CairoPatternId[] = "CairoPattern";

cairo_pattern_t *
get_cairo_pattern (Value pv)
{
    if (pv->foreign.id != CairoPatternId)
    {
	RaiseStandardException (exception_invalid_argument,
				"not a pattern_t",
				2, NewInt(0), pv);
	return 0;
    }
    return (cairo_pattern_t *) (pv->foreign.data);
}

static void
mark_cairo_pattern (void *v)
{
    ;
}

static void
free_cairo_pattern (void *v)
{
    cairo_pattern_t *pat = v;

    if (pat)
	cairo_pattern_destroy (pat);
}

static Value
make_pattern_value (cairo_pattern_t *pat)
{
    cairo_pattern_reference (pat);
    return NewForeign (CairoPatternId, pat, mark_cairo_pattern, free_cairo_pattern);
}

Value
do_Cairo_set_source (Value cv, Value patv)
{
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    cairo_pattern_t *pat = get_cairo_pattern (patv);

    if (aborting)
	return Void;
    cairo_set_source (c5c->cr, pat);
    return Void;
}

Value
do_Cairo_get_source (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (aborting)
	return Void;
    return make_pattern_value (cairo_get_source (c5c->cr));
}

Value
do_Cairo_Pattern_create_linear (Value x0v, Value y0v, Value x1v, Value y1v)
{
    ENTER ();
    double	    x0 = DoublePart (x0v, "x0 invalid");
    double	    y0 = DoublePart (y0v, "y0 invalid");
    double	    x1 = DoublePart (x1v, "x1 invalid");
    double	    y1 = DoublePart (y1v, "y1 invalid");
    
    if (aborting)
	RETURN(Void);
    RETURN (make_pattern_value (cairo_pattern_create_linear (x0, y0, x1, y1)));
}

Value
do_Cairo_Pattern_create_radial (Value cx0v, Value cy0v, Value radius0v,
				Value cx1v, Value cy1v, Value radius1v)
{
    ENTER ();
    double	    cx0 = DoublePart (cx0v, "cx0 invalid");
    double	    cy0 = DoublePart (cy0v, "cy0 invalid");
    double	    radius0 = DoublePart (radius0v, "radius0 invalid");
    double	    cx1 = DoublePart (cx1v, "cx1 invalid");
    double	    cy1 = DoublePart (cy1v, "cy1 invalid");
    double	    radius1 = DoublePart (radius1v, "radius1 invalid");
    
    if (aborting)
	RETURN(Void);
    RETURN (make_pattern_value (cairo_pattern_create_radial (cx0, cy0, radius0,
							     cx1, cy1, radius1)));
}

Value
do_Cairo_Pattern_create_for_surface (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    cairo_surface_t *surface;

    if (aborting)
	RETURN (Void);
    surface = cairo_current_target_surface (c5c->cr);
    RETURN (make_pattern_value (cairo_pattern_create_for_surface (surface)));
}

static void
premultiply_data (png_structp   png,
                  png_row_infop row_info,
                  png_bytep     data)
{
    int i;

    for (i = 0; i < row_info->rowbytes; i += 4) {
	unsigned char  *base = &data[i];
	unsigned char  blue = base[0];
	unsigned char  green = base[1];
	unsigned char  red = base[2];
	unsigned char  alpha = base[3];
	unsigned long	p;

	red = (unsigned) red * (unsigned) alpha / 255;
	green = (unsigned) green * (unsigned) alpha / 255;
	blue = (unsigned) blue * (unsigned) alpha / 255;
	p = (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
	memcpy (base, &p, sizeof (unsigned long));
    }
}

#if 0
struct _cairo_matrix {
    double m[3][2];
};
#endif

struct _cairo_surface {
    const void *backend;
    unsigned int ref_count;

    cairo_matrix_t matrix;
    cairo_filter_t filter;
    int repeat;
};

struct cairo_image_surface {
    cairo_surface_t base;

    /* libic-specific fields */
    char *data;
    int owns_data;
};

static cairo_surface_t *
create_surface_from_png (const char *filename)
{
    FILE	    *f;
    cairo_surface_t *surface;
    char	    *buffer;
    png_structp	    png;
    png_infop	    info;
    png_bytepp	    rows;
    int		    i;
    png_uint_32	    width, height;
    png_uint_32	    stride;
    int		    depth, color, interlace;
    
    png = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
	return NULL;
    info = png_create_info_struct (png);
    if (info == NULL)
    {
	png_destroy_read_struct (&png, NULL, NULL);
	return NULL;
    }
    if (setjmp (png->jmpbuf))
    {
	png_destroy_read_struct (&png, &info, NULL);
	return NULL;
    }
    f = fopen (filename, "rb");
    if (f == NULL)
    {
	png_destroy_read_struct (&png, &info, NULL);
	return NULL;
    }
    png_init_io (png, f);
    png_read_info (png, info);
    png_get_IHDR (png, info, &width, &height, &depth, &color, &interlace,
		  NULL, NULL);

    if (color == PNG_COLOR_TYPE_PALETTE && depth <= 8)
	png_set_expand (png);

    if (color == PNG_COLOR_TYPE_GRAY && depth < 8)
	png_set_expand (png);

    if (png_get_valid (png, info, PNG_INFO_tRNS))
	png_set_expand (png);

    if (depth == 16)
	png_set_strip_16 (png);

    if (depth < 8)
	png_set_packing (png);

    if (color == PNG_COLOR_TYPE_GRAY || color == PNG_COLOR_TYPE_GRAY_ALPHA)
	png_set_gray_to_rgb (png);

    if (interlace != PNG_INTERLACE_NONE)
	png_set_interlace_handling (png);

    png_set_bgr (png);
    png_set_filler (png, 255, PNG_FILLER_AFTER);

    png_set_read_user_transform_fn (png, premultiply_data);

    png_read_update_info (png, info);

    stride = width * 4;
    buffer = malloc (stride * height);
    
    rows = malloc (sizeof (png_bytep) * height);

    for (i = 0; i < height; i++)
	rows[i] = (png_bytep) (buffer + i * stride);
    
    png_read_image (png, rows);
    png_read_end (png, info);

    free (rows);
    fclose (f);
    png_destroy_read_struct (&png, &info, NULL);

    surface = cairo_image_surface_create_for_data (buffer, CAIRO_FORMAT_ARGB32, 
						   width, height, stride);
    if (!surface)
    {
	free (buffer);
	return NULL;
    }
    
    /*
     * XXX hack internal structure contents to hand buffer to cairo 
     */
    ((struct cairo_image_surface *) surface)->owns_data = 1;
    
    return surface;
}

Value
do_Cairo_Pattern_create_png (Value filenamev)
{
    ENTER ();
    char	    *filename = StrzPart (filenamev, "invalid filename");
    cairo_surface_t *image;
    cairo_pattern_t *pattern;

    if (aborting)
	RETURN(Void);
    image = create_surface_from_png (filename);
    if (!image)
    {
	RaiseStandardException (exception_open_error,
				"cannot read png file",
				1, filenamev);
	RETURN (Void);
    }
    pattern = cairo_pattern_create_for_surface (image);
    cairo_surface_destroy (image);
    RETURN (make_pattern_value (pattern));
}

Value
do_Cairo_Pattern_add_color_stop (Value patv, Value offsetv,
				 Value redv, Value greenv, Value bluev,
				 Value alphav)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    double	    offset = DoublePart (offsetv, "offset invalid");
    double	    red = DoublePart (redv, "red invalid");
    double	    green = DoublePart (greenv, "green invalid");
    double	    blue = DoublePart (bluev, "blue invalid");
    double	    alpha = DoublePart (alphav, "alpha invalid");

    if (aborting)
	RETURN(Void);
    RETURN (IntToEnum (typeCairoStatus,
		       cairo_pattern_add_color_stop (pat, offset, red,
						     green, blue, alpha)));
}

Value
do_Cairo_Pattern_set_matrix (Value patv, Value mv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    cairo_matrix_t  matrix;
    cairo_status_t  status;

    if (aborting)
	RETURN(Void);
    cairo_matrix_part (mv, &matrix, "invalid pattern matrix");
    status = cairo_pattern_set_matrix (pat, &matrix);
    RETURN (IntToEnum (typeCairoStatus, status));
}

Value
do_Cairo_Pattern_get_matrix (Value patv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    cairo_matrix_t  matrix;
    Value	    ret;

    if (aborting)
	RETURN (Void);
    cairo_pattern_get_matrix (pat, &matrix);
    ret = new_cairo_matrix (&matrix);
    RETURN (ret);
}

Value
do_Cairo_Pattern_set_extend (Value patv, Value extendv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    cairo_extend_t  extend = EnumIntPart (extendv, "invalid extend");
    cairo_status_t  status;

    if (aborting)
	RETURN(Void);
    status = cairo_pattern_set_extend (pat, extend);
    RETURN (IntToEnum (typeCairoStatus, status));
}

Value
do_Cairo_Pattern_get_extend (Value patv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
 
    if (aborting)
	RETURN(Void);
    RETURN (IntToEnum (typeCairoPatternExtend,
		       cairo_pattern_get_extend (pat)));
}

Value
do_Cairo_Pattern_set_filter (Value patv, Value filterv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    cairo_filter_t  filter = EnumIntPart (filterv, "invalid filter");
    cairo_status_t  status;

    if (aborting)
	RETURN(Void);
    status = cairo_pattern_set_filter (pat, filter);
    RETURN (IntToEnum (typeCairoStatus, status));
}

Value
do_Cairo_Pattern_get_filter (Value patv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
 
    if (aborting)
	RETURN(Void);
    RETURN (IntToEnum (typeCairoPatternFilter,
		       cairo_pattern_get_filter (pat)));
}
