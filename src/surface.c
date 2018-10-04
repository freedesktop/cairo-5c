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

#include "cairo-5c.h"

static char	CairoSurfaceId[] = "CairoSurface";

cairo_5c_surface_t *
cairo_5c_surface_get (Value av)
{
    cairo_5c_surface_t	*c5s;

    if (av == Void)
    {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("context not bound to surface"),
				NewInt(0), av);
	return 0;
    }
    if (av->foreign.id != CairoSurfaceId)
    {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("not a surface_t"),
				NewInt(0), av);
	return 0;
    }
    c5s = av->foreign.data;
    if (!c5s)
    {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("surface destroyed"),
				NewInt(0), av);
	return 0;
    }
    switch (c5s->kind) {
    case CAIRO_5C_WINDOW:
#if HAVE_CAIRO_XLIB_H
	cairo_5c_gui_check_size (c5s);
#endif
	break;
    case CAIRO_5C_IMAGE:
    case CAIRO_5C_SCRATCH:
    case CAIRO_5C_PDF:
    case CAIRO_5C_SVG:
    case CAIRO_5C_PS:
	break;
    }
    return c5s;
}

/*
 * These are the functions for the nickle memory allocator interface
 * for the CairoSurfaceType datatype
 */

static void
cairo_5c_surface_mark (void *object)
{
    cairo_5c_surface_t	*c5s = object;

    MemReference (c5s->recv_events);
    switch (c5s->kind) {
    case CAIRO_5C_WINDOW:
	break;
    case CAIRO_5C_IMAGE:
    case CAIRO_5C_SCRATCH:
	break;
    case CAIRO_5C_PDF:
	MemReference (c5s->u.pdf.file);
	break;
    case CAIRO_5C_SVG:
	MemReference (c5s->u.svg.file);
	break;
    case CAIRO_5C_PS:
	MemReference (c5s->u.ps.file);
	break;
    }
}

static void
cairo_5c_surface_destroy (cairo_5c_surface_t *c5s)
{

    if (!c5s->surface)
	return;
    
    switch (c5s->kind) {
    case CAIRO_5C_PDF:
    case CAIRO_5C_PS:
	if (!c5s->copied)
	{
	    cairo_t *cr = cairo_create (c5s->surface);
	    if (cr)
	    {
		cairo_copy_page (cr);
		cairo_destroy (cr);
	    }
	}
	break;
    default:
	break;
    }

    cairo_surface_destroy (c5s->surface);
    c5s->surface = 0;
    
    switch (c5s->kind) {
    case CAIRO_5C_WINDOW:
	cairo_5c_gui_destroy (c5s);
	break;
    case CAIRO_5C_IMAGE:
	break;
    case CAIRO_5C_PDF:
	c5s->u.pdf.file = Void;
	break;
    case CAIRO_5C_SVG:
	c5s->u.svg.file = Void;
	break;
    case CAIRO_5C_PS:
	c5s->u.ps.file = Void;
	break;
    case CAIRO_5C_SCRATCH:
	break;
    }
}

static int
cairo_5c_surface_free (void *object)
{
    cairo_5c_surface_t	*c5s = object;

    cairo_5c_surface_destroy (c5s);
    if (c5s->kind == CAIRO_5C_WINDOW)
	cairo_5c_gui_free (c5s);
    return 1;
}

static DataType Cairo5cSurfaceType = { 
    cairo_5c_surface_mark,
    cairo_5c_surface_free,
    "Cairo5cSurface"
};
    
/*
 * These are the functions for the nickle foreign function interface
 * for the "CairoSurface" foreign datatype
 */

static void
cairo_surface_foreign_mark (void *object)
{
    MemReference (object);
}

static void
cairo_surface_foreign_free (void *object)
{
    /* let nickle finalizer deal with this */
    ;
}

#if HAVE_CAIRO_XLIB_H
static Value
create_window (Value namev, Value wv, Value hv, Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    Value		ret;
    char		*name = StrzPart (namev, "invalid name");
    int			width = IntPart (wv, "invalid width");
    int			height = IntPart (hv, "invalid height");
    Bool		shown = sv == TrueVal;
    
    if (aborting )
	RETURN (Void);

    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_WINDOW;
    c5s->surface = 0;
    c5s->width = width;
    c5s->height = height;
    c5s->dirty = False;
    c5s->copied = False;
    c5s->recv_events = Void;
    c5s->u.window.gui = NULL;
    c5s->dpi = 72.0;
    
    if (!cairo_5c_gui_create (c5s, name, width, height, shown))
    {
	int err = errno;
	RaiseStandardException (exception_open_error, 3,
				NewStrString ("Can't create window"),
				FileGetError (err), namev);
	RETURN (Void);
    }
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

Value
do_Cairo_Surface_create_window (Value namev, Value wv, Value hv)
{
    return create_window (namev, wv, hv, TrueVal);
}

Value
do_Cairo_Surface_create_window_hidden (Value namev, Value wv, Value hv)
{
    return create_window (namev, wv, hv, FalseVal);
}

Value
do_Cairo_Surface_set_window_shown (Value gv, Value sv)
{
    ENTER();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get(gv);
    Bool		shown = sv == TrueVal;

    if (aborting)
	RETURN(Void);

    cairo_5c_gui_set_shown (c5s, shown);
    RETURN(Void);
}

Value
do_Cairo_Surface_resize_window (Value sv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get(sv);
    int			width = IntPart (wv, "invalid width");
    int			height = IntPart (hv, "invalid height");
    
    if (aborting )
	RETURN (Void);

    if (c5s->kind != CAIRO_5C_WINDOW) {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("Not a window"),
				NewInt(0), sv);
	RETURN (Void);
	    
    }
    
    cairo_5c_gui_resize (c5s, width, height);

    RETURN (Void);
}
#endif

Value
do_Cairo_Surface_write_to_png (Value sv, Value fv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);
    char		*filename = StrzPart (fv, "invalid filename");
    cairo_status_t	status;		
    
    if (aborting)
	RETURN (Void);
    status = cairo_surface_write_to_png (c5s->surface, filename);
    RETURN (IntToEnum (typeCairoStatus, status));
}

Value
do_Cairo_Surface_write_to_png_file (Value sv, Value fv)
{
    ENTER ();
    RETURN (Void);
}

Value
do_Cairo_Surface_set_device_offset (Value sv, Value xv, Value yv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");
    if (!aborting)
	cairo_surface_set_device_offset (c5s->surface, x, y);
    RETURN (Void);
}

static cairo_content_t cairo_contents[] = {
    CAIRO_CONTENT_COLOR, CAIRO_CONTENT_ALPHA, CAIRO_CONTENT_COLOR_ALPHA
};

Value
do_Cairo_Surface_create_similar (Value sv, Value cv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    cairo_5c_surface_t	*c5os = cairo_5c_surface_get (sv);
    cairo_content_t	content = cairo_contents[EnumIntPart (cv, "invalid content_t")];
    int			width = IntPart (wv, "invalid width");
    int			height = IntPart (hv, "invalid height");
    Value		ret;

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_SCRATCH;
    c5s->surface = 0;
    c5s->width = width;
    c5s->height = height;
    c5s->dirty = False;
    c5s->copied = False;
    c5s->recv_events = Void;
    c5s->dpi = 72.0;
    
    c5s->surface = cairo_surface_create_similar (c5os->surface,
						 content,
						 width, height);
						 
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

Value
do_Cairo_Surface_finish (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (!aborting)
	cairo_surface_finish (c5s->surface);
    RETURN (Void);
}

Value
do_Cairo_Surface_status (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN(Void);
    RETURN(IntToEnum (typeCairoStatus, cairo_surface_status (c5s->surface)));
}

Value
do_Cairo_Surface_destroy (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN (Void);
    cairo_5c_surface_destroy (c5s);
    RETURN(Void);
}

Value
do_Cairo_Surface_width (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN (Void);
    RETURN(Reduce (NewDoubleFloat (c5s->width)));
}

Value
do_Cairo_Surface_height (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN (Void);
    RETURN(Reduce (NewDoubleFloat (c5s->height)));
}


Value
do_Cairo_Image_surface_create (Value fv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    cairo_format_t	format = EnumIntPart (fv, "invalid format_t");
    int			width = IntPart (wv, "invalid width");
    int			height = IntPart (hv, "invalid height");
    Value		ret;

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_IMAGE;
    c5s->surface = 0;
    c5s->width = width;
    c5s->height = height;
    c5s->dirty = False;
    c5s->recv_events = Void;
    c5s->copied = False;
    c5s->dpi = 72.0;
    
    c5s->surface = cairo_image_surface_create (format,
					       width,
					       height);
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

Value
do_Cairo_Image_surface_create_from_png (Value filenamev)
{
    ENTER ();
    char		*filename = StrzPart (filenamev, "invalid filename");
    cairo_surface_t	*image;
    cairo_5c_surface_t	*c5s;
    Value		ret;

    if (aborting)
	RETURN(Void);
    image = cairo_image_surface_create_from_png (filename);
    if (cairo_surface_status (image) != CAIRO_STATUS_SUCCESS)
    {
	int err = errno;
	
	if (image)
	    cairo_surface_destroy (image);
	RaiseStandardException (exception_open_error, 3,
				FileGetErrorMessage (err),
				FileGetError (err), filenamev);
	RETURN (Void);
    }

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_IMAGE;
    c5s->surface = image;
    c5s->width = cairo_image_surface_get_width (image);
    c5s->height = cairo_image_surface_get_height (image);
    c5s->dirty = False;
    c5s->recv_events = Void;
    c5s->copied = False;
    c5s->dpi = 72.0;

    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

Value
do_Cairo_Image_surface_create_from_png_file (Value filev)
{
    ENTER ();
    /* XXX */
    RETURN (Void);
}

Value
do_Cairo_Image_get_pixel (Value sv, Value xv, Value yv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);
    int			x = IntPart (xv, "invalid X");
    int			y = IntPart (yv, "invalid Y");
    int			width;
    int			height;
    unsigned char    	*data;
    uint32_t		pixel;

    if (aborting)
	RETURN (Void);
    if (c5s->kind != CAIRO_5C_IMAGE) {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("not an image surface_t"),
				NewInt(0), sv);
	RETURN(Void);
    }
    width = cairo_image_surface_get_width (c5s->surface);
    height = cairo_image_surface_get_height (c5s->surface);
    if (x < 0 || width <= x)
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("x out of range"),
				NewInt(1), xv);
    if (y < 0 || height <= y)
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("y out of range"),
				NewInt(2), yv);

    if (aborting)
	RETURN (Void);

    data = (cairo_image_surface_get_data (c5s->surface) + 
	    y * cairo_image_surface_get_stride (c5s->surface));
    
    switch (cairo_image_surface_get_format (c5s->surface)) {
    case CAIRO_FORMAT_ARGB32:
    case CAIRO_FORMAT_RGB24:
	pixel = ((uint32_t *) data)[x];
	break;
    case CAIRO_FORMAT_A8:
	pixel = data[x];
	break;
    default:
	pixel = 0;  /* XXX handle FORMAT_A1 */
	break;
    }
    RETURN (Reduce (NewInteger (Positive, NewDoubleDigitNatural (pixel))));
}

Value
do_Cairo_Image_put_pixel (Value sv, Value xv, Value yv, Value pv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);
    int			x = IntPart (xv, "invalid X");
    int			y = IntPart (yv, "invalid Y");
    int			width;
    int			height;
    unsigned char    	*data;
    uint32_t		pixel, max;
    Natural		*n;

    if (aborting)
	RETURN (Void);
    if (c5s->kind != CAIRO_5C_IMAGE)
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("not an image surface_t"),
				NewInt(0), sv);
    if (aborting)
	RETURN (Void);
    width = cairo_image_surface_get_width (c5s->surface);
    height = cairo_image_surface_get_height (c5s->surface);
    if (x < 0 || width <= x)
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("x out of range"),
				NewInt(1), xv);
    if (y < 0 || height <= y)
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("y out of range"),
				NewInt(2), yv);

    switch (cairo_image_surface_get_format (c5s->surface)) {
    case CAIRO_FORMAT_ARGB32:
	max = 0xffffffff;
	break;
    case CAIRO_FORMAT_RGB24:
	max = 0xffffff;
	break;
    case CAIRO_FORMAT_A8:
	max = 0xff;
	break;
    default:
	max = 0;    /* XXX other formats */
	break;
    }

    if (Negativep (pv) || 
	TrueVal == Greater (pv, NewInteger (Positive,
					    NewDoubleDigitNatural (max))))
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("pixel out of range"),
				NewInt(3), pv);
    if (aborting)
	RETURN (Void);

    switch (ValueTag (pv)) {
    case rep_int:
	pixel = ValueInt (pv);
	break;
    case rep_integer:
	n = IntegerMag(pv);
	pixel = 0;
	if (n->length > 0)
	    pixel = NaturalDigits(n)[0];
	break;
    default:
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("Invalid pixel"),
				NewInt(3), pv);
	pixel = 0;
	break;
    }
    if (aborting)
	RETURN (Void);

    data = (cairo_image_surface_get_data (c5s->surface) + 
	    y * cairo_image_surface_get_stride (c5s->surface));
    
    switch (cairo_image_surface_get_format (c5s->surface)) {
    case CAIRO_FORMAT_ARGB32:
    case CAIRO_FORMAT_RGB24:
	((uint32_t *) data)[x] = pixel;
	break;
    case CAIRO_FORMAT_A8:
	data[x] = pixel;
	break;
    default:
	break;
    }

    RETURN (Void);
}

#if HAVE_CAIRO_PDF_H
Value
do_Cairo_Pdf_surface_create (Value fnv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    char		*filename = StrzPart (fnv, "invalid filename");
    double		width = DoublePart (wv, "invalid width_in_points");
    double    		height = DoublePart (hv, "invalid height_in_points");
    Value		ret;

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_PDF;
    c5s->surface = 0;
    c5s->width = width;
    c5s->height = height;
    c5s->dirty = False;
    c5s->copied = False;
    c5s->recv_events = Void;
    c5s->dpi = 72.0;
    
    c5s->u.pdf.file = Void;
    
    c5s->surface = cairo_pdf_surface_create (filename, width, height);
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}
#endif

#if HAVE_CAIRO_SVG_H
Value
do_Cairo_Svg_surface_create (Value fnv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    char		*filename = StrzPart (fnv, "invalid filename");
    double		width = DoublePart (wv, "invalid width_in_points");
    double    		height = DoublePart (hv, "invalid height_in_points");
    Value		ret;

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_SVG;
    c5s->surface = 0;
    c5s->width = width;
    c5s->height = height;
    c5s->dirty = False;
    c5s->copied = False;
    c5s->recv_events = Void;
    c5s->dpi = 72.0;
    
    c5s->u.svg.file = Void;
    
    c5s->surface = cairo_svg_surface_create (filename, width, height);
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}
#endif

#if HAVE_CAIRO_PS_H
Value
do_Cairo_Ps_surface_create (Value fnv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    char		*filename = StrzPart (fnv, "invalid filename");
    double		width = DoublePart (wv, "invalid width_in_points");
    double    		height = DoublePart (hv, "invalid height_in_points");
    Value		ret;

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_PS;
    c5s->surface = 0;
    c5s->width = width;
    c5s->height = height;
    c5s->dirty = False;
    c5s->copied = False;
    c5s->recv_events = Void;
    c5s->dpi = 72.0;

    c5s->u.ps.file = Void;
    
    c5s->surface = cairo_ps_surface_create (filename, width, height);
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}
#endif
