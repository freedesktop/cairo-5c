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

Value
do_Cairo_save (Value cv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    if (!aborting)
	cairo_save (c5c->cr);
    return Void;
}

Value
do_Cairo_restore (Value cv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    if (!aborting)
	cairo_restore (c5c->cr);
    return Void;
}

Value
do_Cairo_set_rgb_color (Value cv, Value rv, Value gv, Value bv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	r = DoublePart (rv, "invalid red value");
    double	g = DoublePart (gv, "invalid green value");
    double	b = DoublePart (bv, "invalid blue value");
    
    if (!aborting)
	cairo_set_rgb_color (c5c->cr, r, g, b);
    return Void;
}

Value
do_Cairo_set_alpha (Value cv, Value av)
{
   cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	a = DoublePart (av, "invalid alpha value");

    if (!aborting)
	cairo_set_alpha (c5c->cr, a);
    return Void;
}

Value
do_Cairo_set_tolerance (Value cv, Value tv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	t = DoublePart (tv, "invalid tolerance");

    if (!aborting)
	cairo_set_tolerance (c5c->cr, t);
    return Void;
}

Value
do_Cairo_set_line_width (Value cv, Value lv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	l = DoublePart (lv, "invalid line_width");

    if (!aborting)
	cairo_set_line_width (c5c->cr, l);
    return Void;
}

Value
do_Cairo_set_line_cap (Value cv, Value lv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    cairo_line_cap_t	l = IntPart (lv, "invalid line_cap");

    if (!aborting)
	cairo_set_line_cap (c5c->cr, l);
    return Void;
}

Value
do_Cairo_set_line_join (Value cv, Value lv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    cairo_line_join_t	l = IntPart (lv, "invalid line_join");

    if (!aborting)
	cairo_set_line_join (c5c->cr, l);
    return Void;
}

Value
do_Cairo_identity_matrix (Value cv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    if (!aborting)
	cairo_identity_matrix (c5c->cr);
    return Void;
}

Value
do_Cairo_translate (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_translate (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_scale (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_scale (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_rotate (Value cv, Value av)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	a = DoublePart (av, "invalid angle");

    if (!aborting)
	cairo_rotate (c5c->cr, a);
    return Void;
}

Value
do_Cairo_current_matrix (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = get_cairo_5c (cv);
    cairo_matrix_t  *matrix;
    double	    a, b, c, d, tx, ty;
    Value	    ret;
    static int	dims[2] = { 2, 3 };

    if (aborting)
	RETURN(Void);
    
    matrix = cairo_matrix_create ();

    if (!matrix)
    {
	RaiseStandardException (exception_invalid_argument,
				"can't create matrix",
				2, cv, Void);
	RETURN(Void);
    }
    cairo_current_matrix (c5c->cr, matrix);
    cairo_matrix_get_affine (matrix, &a, &b, &c, &d, &tx, &ty);
    cairo_matrix_destroy (matrix);
    ret = NewArray (False, False, typePrim[rep_float], 2, dims);
    ArrayValueSet(&ret->array, 0, NewDoubleFloat (a));
    ArrayValueSet(&ret->array, 1, NewDoubleFloat (b));
    ArrayValueSet(&ret->array, 2, NewDoubleFloat (c));
    ArrayValueSet(&ret->array, 3, NewDoubleFloat (d));
    ArrayValueSet(&ret->array, 4, NewDoubleFloat (tx));
    ArrayValueSet(&ret->array, 5, NewDoubleFloat (ty));
    RETURN (ret);
}

Value
do_Cairo_set_matrix (Value cv, Value mv)
{
    ENTER ();
    cairo_5c_t	    *c5c = get_cairo_5c (cv);
    cairo_matrix_t  *matrix;
    double	    a, b, c, d, tx, ty;

    if (aborting)
	RETURN(Void);
    
    matrix = cairo_matrix_create ();
    if (!matrix)
    {
	RaiseStandardException (exception_invalid_argument,
				"can't create matrix",
				2, cv, Void);
	RETURN(Void);
    }
    a = DoublePart (ArrayValueGet(&mv->array, 0), "invalid matrix");
    b = DoublePart (ArrayValueGet(&mv->array, 1), "invalid matrix");
    c = DoublePart (ArrayValueGet(&mv->array, 2), "invalid matrix");
    d = DoublePart (ArrayValueGet(&mv->array, 3), "invalid matrix");
    tx = DoublePart (ArrayValueGet(&mv->array, 4), "invalid matrix");
    ty = DoublePart (ArrayValueGet(&mv->array, 5), "invalid matrix");
    cairo_matrix_set_affine (matrix, a, b, c, d, tx, ty);
    cairo_set_matrix (c5c->cr, matrix);
    cairo_matrix_destroy (matrix);
    RETURN (Void);
}

