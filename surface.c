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

static Bool
create_cairo_window (cairo_5c_surface_t *c5s)
{
    Display	*dpy = cairo_5c_tool_display (c5s);
    
    if (c5s->surface)
	cairo_surface_destroy (c5s->surface);
    if (!c5s->u.window.pixmap)
    {
	RaiseStandardException (exception_invalid_argument,
				"window destroyed",
				2, NewInt (0), NewInt (0));
	return False;
    }
    c5s->surface = cairo_xlib_surface_create (dpy,
					      c5s->u.window.pixmap,
					      DefaultVisual (dpy, DefaultScreen (dpy)),
					      0,
					      DefaultColormap (dpy, DefaultScreen (dpy)));
    c5s->u.window.curpix = c5s->u.window.pixmap;
    return True;
}

cairo_5c_surface_t *
cairo_5c_surface_get (Value av)
{
    cairo_5c_surface_t	*c5s;

    if (av == Void)
    {
	RaiseStandardException (exception_invalid_argument,
				"context not bound to surface",
				2, NewInt(0), av);
    }
    if (av->foreign.id != CairoSurfaceId)
    {
	RaiseStandardException (exception_invalid_argument,
				"not a surface_t",
				2, NewInt(0), av);
	return 0;
    }
    c5s = av->foreign.data;
    if (!c5s)
    {
	RaiseStandardException (exception_invalid_argument,
				"surface destroyed",
				2, NewInt(0), av);
	return 0;
    }
    switch (c5s->kind) {
    case CAIRO_5C_WINDOW:
	if (c5s->u.window.curpix != c5s->u.window.pixmap)
	    if (!create_cairo_window (c5s))
		return 0;
	break;
    case CAIRO_5C_IMAGE:
    case CAIRO_5C_PS:
    case CAIRO_5C_SCRATCH:
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
	cairo_5c_tool_mark (c5s);
	break;
    case CAIRO_5C_IMAGE:
    case CAIRO_5C_PS:
    case CAIRO_5C_SCRATCH:
	break;
    }
}

static void
cairo_5c_surface_destroy (cairo_5c_surface_t *c5s)
{

    if (!c5s->surface)
	return;
    
    switch (c5s->kind) {
    case CAIRO_5C_PS:
	FilePrintf (FileStdout, "Copied %d\n", c5s->copied);
	if (!c5s->copied)
	{
	    cairo_t *cr = cairo_create ();
	    if (cr)
	    {
		cairo_set_target_surface (cr, c5s->surface);
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
	cairo_5c_tool_destroy (c5s);
	break;
    case CAIRO_5C_IMAGE:
	break;
    case CAIRO_5C_PS:
	fclose (c5s->u.ps.file);
	c5s->u.ps.file = NULL;
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

Value
do_Cairo_Surface_create_window (Value namev, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    Value		ret;
    char		*name = StrzPart (namev, "invalid name");
    int			width = IntPart (wv, "invalid width");
    int			height = IntPart (hv, "invalid height");
    
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
    
    if (!cairo_5c_tool_create (c5s, name, width, height))
    {
	RaiseStandardException (exception_open_error,
				"Can't create window",
				0, wv);
	RETURN (Void);
    }
    
    create_cairo_window (c5s);

    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

Value
do_Cairo_Surface_create_image (Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
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
    
    c5s->surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
					       width,
					       height);
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

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
do_Cairo_Surface_create_ps (Value fv, Value wv, Value hv, Value xppiv, Value yppiv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    char		*filename = StrzPart (fv, "invalid filename");
    double		width = DoublePart (wv, "invalid width");
    double    		height = DoublePart (hv, "invalid height");
    double		xppi = DoublePart (xppiv, "invalid x pixels per inch");
    double		yppi = DoublePart (yppiv, "invalid y pixels per inch");
    Value		ret;

    if (aborting)
	RETURN (Void);
    
    c5s = ALLOCATE (&Cairo5cSurfaceType, sizeof (cairo_5c_surface_t));
    c5s->kind = CAIRO_5C_PS;
    c5s->surface = 0;
    c5s->width = width * xppi;
    c5s->height = height * yppi;
    c5s->dirty = False;
    c5s->copied = False;
    c5s->recv_events = Void;
    
    c5s->u.ps.file = fopen (filename, "w");
    
    if (!c5s->u.ps.file)
    {
	RaiseStandardException (exception_open_error,
				"can't open file",
				0, fv);
	RETURN (Void);
    }

    c5s->surface = cairo_ps_surface_create (c5s->u.ps.file, width, height, xppi, yppi);
    
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
}

Value
do_Cairo_Surface_create_similar (Value sv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s;
    cairo_5c_surface_t	*c5os = cairo_5c_surface_get (sv);
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
    
    c5s->surface = cairo_surface_create_similar (c5os->surface,
						 CAIRO_FORMAT_ARGB32,
						 width, height);
						 
    ret = NewForeign (CairoSurfaceId, c5s, 
		      cairo_surface_foreign_mark, cairo_surface_foreign_free);

    RETURN (ret);
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
    RETURN(NewInt (c5s->width));
}

Value
do_Cairo_Surface_height (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN (Void);
    RETURN(NewInt (c5s->height));
}
