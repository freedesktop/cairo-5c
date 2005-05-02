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
cairo_5c_get (Value av)
{
    cairo_5c_t		*c5c;
    cairo_5c_surface_t	*c5s;
    
    if (av->foreign.id != CairoId)
    {
	RaiseStandardException (exception_invalid_argument,
				"not a cairo_t",
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
    if (c5c->surface != Void)
    {
	c5s = cairo_5c_surface_get (c5c->surface);
	if (!c5s)
	    return 0;
	if (c5s->surface != cairo_current_target_surface (c5c->cr))
	    cairo_set_target_surface (c5c->cr, c5s->surface);
    }
    return c5c;
}

/*
 * nickle memory manager functions for the CairoType datatype
 */

static void
cairo_5c_mark (void *v)
{
    cairo_5c_t	*c5c = v;

    MemReference (c5c->surface);
}

static int
cairo_5c_free (void *v)
{
    cairo_5c_t	*c5c = v;

    if (c5c->cr)
    {
	cairo_destroy (c5c->cr);
	c5c->cr = NULL;
    }
    c5c->surface = Void;
    return 1;
}

static DataType Cairo5cType = {
    cairo_5c_mark,
    cairo_5c_free,
    "Cairo5c"
};

/*
 * Foreign datatype functions for the "Cairo" foreign datatype
 */

static void
cairo_foreign_mark (void *object)
{
    MemReference (object);
}

static void
cairo_foreign_free (void *object)
{
    /* let nickle finalizer deal with this */
    ;
}

Value
do_Cairo_create (void)
{
    ENTER ();
    Value	ret;
    cairo_5c_t	*c5c;
    cairo_t	*cr;

    cr = cairo_create ();
    if (!cr)
    {
	RaiseStandardException (exception_invalid_argument,
				"can't create cairo object",
				1, Void);
	RETURN (Void);
    }
    c5c = ALLOCATE (&Cairo5cType, sizeof (cairo_5c_t));
    c5c->cr = cr;
    c5c->surface = Void;
    ret = NewForeign (CairoId, c5c, 
		      cairo_foreign_mark, cairo_foreign_free);
    RETURN (ret);
}

void
cairo_5c_dirty (cairo_5c_t *c5c)
{
    if (c5c->surface != Void)
    {
	cairo_5c_surface_t  *c5s = cairo_5c_surface_get (c5c->surface);
	
	if (c5s)
	{
	    switch (c5s->kind) {
	    case CAIRO_5C_WINDOW:
		cairo_5c_tool_dirty (c5s);
		break;
	    case CAIRO_5C_IMAGE:
	    case CAIRO_5C_PS:
	    case CAIRO_5C_SCRATCH:
		break;
	    }
	}
    }
}

static Bool
cairo_5c_enable (cairo_5c_t *c5c)
{
    if (c5c->surface != Void)
    {
	cairo_5c_surface_t  *c5s = cairo_5c_surface_get (c5c->surface);

	if (c5s)
	{
	    switch (c5s->kind) {
	    case CAIRO_5C_WINDOW:
		cairo_5c_tool_enable (c5s);
		break;
	    case CAIRO_5C_IMAGE:
	    case CAIRO_5C_PS:
	    case CAIRO_5C_SCRATCH:
		break;
	    }
	}
    }
    return True;
}

static Bool
cairo_5c_disable (cairo_5c_t *c5c)
{
    if (c5c->surface != Void)
    {
	cairo_5c_surface_t  *c5s = cairo_5c_surface_get (c5c->surface);

	if (c5s)
	{
	    switch (c5s->kind) {
	    case CAIRO_5C_WINDOW:
		cairo_5c_tool_disable (c5s);
		break;
	    case CAIRO_5C_IMAGE:
	    case CAIRO_5C_PS:
	    case CAIRO_5C_SCRATCH:
		break;
	    }
	}
    }
    return True;
}

Value
do_Cairo_set_target_surface (Value cv, Value sv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN (Void);
    cairo_set_target_surface (c5c->cr, c5s->surface);
    c5c->surface = sv;
    RETURN (Void);
}

Value
do_Cairo_get_target_surface (Value cv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);

    if (aborting)
	RETURN (Void);
    if (c5c->surface == Void)
    {
	RaiseStandardException (exception_invalid_argument,
				"No current surface",
				2, NewInt (0), cv);
	RETURN (Void);
    }
    RETURN (c5c->surface);
}

Value
do_Cairo_destroy (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (aborting)
	RETURN (Void);
    
    cv->foreign.data = 0;
    cairo_5c_free (c5c);
    
    RETURN (Void);
}

Value
do_Cairo_copy (Value dstv, Value srcv)
{
    ENTER ();
    cairo_5c_t	*dst = cairo_5c_get (dstv);
    cairo_5c_t	*src = cairo_5c_get (srcv);
    
    if (aborting)
	RETURN (Void);
    
    cairo_copy (dst->cr, src->cr);

    RETURN (Void);
}

Value
do_Cairo_status (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (aborting)
	return Void;
    RETURN(IntToEnum (typeCairoStatus, cairo_status (c5c->cr)));
}

Value
do_Cairo_status_string (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (aborting)
	return Void;
    RETURN(NewStrString (cairo_status_string (c5c->cr)));
}

Value
do_Cairo_enable (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    
    if (aborting)
	return Void;
    if (!cairo_5c_enable (c5c))
    {
	RaiseStandardException (exception_invalid_argument,
				"already enabled",
				2, NewInt(0), cv);
    }
    return Void;
}

Value
do_Cairo_disable (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    
    if (aborting)
	return Void;
    if (!cairo_5c_disable (c5c))
    {
	RaiseStandardException (exception_invalid_argument,
				"can't disable",
				2, NewInt(0), cv);
    }
    return Void;
}
