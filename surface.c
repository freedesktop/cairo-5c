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

static char	CairoId[] = "Cairo";

cairo_5c_t *
get_cairo_5c (Value av)
{
    cairo_5c_t	*c5c;
    
    if (av->foreign.id != CairoId)
    {
	RaiseStandardException (exception_invalid_argument,
				"not a cairo",
				2, NewInt(0), av);
	return 0;
    }
    c5c = av->foreign.data;
    if (!c5c)
    {
	RaiseStandardException (exception_invalid_argument,
				"cairo destroyed",
				2, NewInt(0), av);
	return 0;
    }
    return c5c;
}

static void
Cairo_block_handler (void *closure)
{
    cairo_5c_t	*c5c = closure;

    if (c5c->dirty)
    {
	XFlush (c5c->u.window.dpy);
	c5c->dirty = False;
    }
}

void
free_cairo_5c (void *v)
{
    cairo_5c_t	*c5c = v;

    if (c5c)
    {
	cairo_destroy (c5c->cr);
	switch (c5c->kind) {
	case CAIRO_5C_WINDOW:
	    XCloseDisplay (c5c->u.window.dpy);
	    ThreadsUnregisterBlockHandler (Cairo_block_handler, c5c);
	    break;
	case CAIRO_5C_PNG:
	    fflush (c5c->u.png.file);
	    break;
	}
        free (c5c);
    }
}

Value
do_Cairo_new (int n, Value *v)
{
    ENTER ();
    cairo_5c_t	*c5c;
    int		screen;
    Value	ret;
    Display	*dpy;
    
    c5c = malloc (sizeof (cairo_5c_t));
    if (!c5c)
	RETURN (Void);

    c5c->kind = CAIRO_5C_WINDOW;

    dpy = XOpenDisplay (0);
    c5c->u.window.dpy = dpy;
    if (!dpy)
    {
	RaiseStandardException (exception_open_error,
				"can't open X display",
				0, Void);
	free (c5c);
	RETURN (Void);
    }
    screen = DefaultScreen(dpy);

    if (n > 0)
	c5c->width = IntPart (v[0], "invalid width");
    else
	c5c->width = DisplayWidth (dpy, screen) / 3;
    if (n > 1)
	c5c->height = IntPart (v[1], "invalid height");
    else
	c5c->height = DisplayWidth (dpy, screen) / 3;
    if (aborting)
    {
	XCloseDisplay (dpy);
	free (c5c);
	RETURN (Void);
    }
    
    c5c->u.window.w = XCreateSimpleWindow (dpy, RootWindow (dpy, screen),
					   0, 0, c5c->width, c5c->height, 0,
					   BlackPixel (dpy, screen),
					   WhitePixel (dpy, screen));
    c5c->cr = cairo_create ();
    cairo_set_target_drawable (c5c->cr, dpy, c5c->u.window.w);
    
    cairo_save (c5c->cr); {
	cairo_identity_matrix (c5c->cr);
	cairo_set_rgb_color (c5c->cr, 1, 1, 1);
	cairo_rectangle (c5c->cr, 0, 0, c5c->width, c5c->height);
	cairo_fill (c5c->cr);
    } cairo_restore (c5c->cr);
    
    cairo_set_rgb_color (c5c->cr, 0, 0, 0);

    XMapWindow (dpy, c5c->u.window.w);
    XFlush (dpy);

    ret = NewForeign (CairoId, c5c, free_cairo_5c);

    ThreadsRegisterBlockHandler (Cairo_block_handler, c5c);

    RETURN (ret);
}

Value
do_Cairo_new_png (Value fv, Value wv, Value hv)
{
    ENTER ();
    cairo_5c_t	*c5c;
    char	*filename = StrzPart (fv, "invalid filename");
    int		width = IntPart (wv, "invalid width");
    int		height = IntPart (hv, "invalid height");
    Value	ret;

    if (aborting)
	RETURN (Void);
    
    c5c = malloc (sizeof (cairo_5c_t));
    if (!c5c)
	RETURN (Void);

    c5c->kind = CAIRO_5C_PNG;

    c5c->u.png.file = fopen (filename, "w");
    if (!c5c->u.png.file)
    {
	RaiseStandardException (exception_open_error,
				"can't open file",
				0, fv);
	free (c5c);
	RETURN (Void);
    }
    c5c->width = width;
    c5c->height = height;
    c5c->cr = cairo_create ();
    cairo_set_target_png (c5c->cr, c5c->u.png.file,
			  CAIRO_FORMAT_ARGB32, c5c->width,
			  c5c->height);
    cairo_set_rgb_color (c5c->cr, 0, 0, 0);
    cairo_set_alpha (c5c->cr, 0);
    cairo_rectangle (c5c->cr, 0, 0, c5c->width, c5c->height);
    cairo_fill (c5c->cr);

    ret = NewForeign (CairoId, c5c, free_cairo_5c);

    RETURN (ret);
}

Value
do_Cairo_width (Value av)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (av);
    if (aborting)
	return Void;
    RETURN(NewInt (c5c->width));
}

Value
do_Cairo_height (Value av)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (av);

    if (aborting)
	return Void;
    RETURN(NewInt (c5c->height));
}

Value
do_Cairo_status (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (cv);

    if (aborting)
	return Void;
    RETURN(NewInt (cairo_status (c5c->cr)));
}

Value
do_Cairo_status_string (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (cv);

    if (aborting)
	return Void;
    RETURN(NewStrString (cairo_status_string (c5c->cr)));
}

Value
do_Cairo_dispose (Value av)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (av);

    if (c5c)
    {
	free_cairo_5c (c5c);
	av->foreign.data = 0;
    }
    RETURN (Void);
}
