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
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    if (!aborting)
	cairo_save (c5c->cr);
    return Void;
}

Value
do_Cairo_restore (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    if (!aborting)
	cairo_restore (c5c->cr);
    return Void;
}

Value
do_Cairo_set_operator (Value cv, Value ov)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    int		o = EnumIntPart (ov, "invalid operator");

    if (!aborting)
	cairo_set_operator (c5c->cr, o);
    return Void;
}

Value
do_Cairo_set_source_rgb (Value cv, Value rv, Value gv, Value bv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	r = DoublePart (rv, "invalid red value");
    double	g = DoublePart (gv, "invalid green value");
    double	b = DoublePart (bv, "invalid blue value");
    
    if (!aborting)
	cairo_set_source_rgb (c5c->cr, r, g, b);
    return Void;
}

Value
do_Cairo_set_source_rgba (Value cv, Value rv, Value gv, Value bv, Value av)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	r = DoublePart (rv, "invalid red value");
    double	g = DoublePart (gv, "invalid green value");
    double	b = DoublePart (bv, "invalid blue value");
    double	a = DoublePart (av, "invalid alpha value");
    
    if (!aborting)
	cairo_set_source_rgba (c5c->cr, r, g, b, a);
    return Void;
}

Value
do_Cairo_set_tolerance (Value cv, Value tv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	t = DoublePart (tv, "invalid tolerance");

    if (!aborting)
	cairo_set_tolerance (c5c->cr, t);
    return Void;
}

Value
do_Cairo_set_fill_rule (Value cv, Value fv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    int		f = EnumIntPart (fv, "invalid fill rule");

    if (!aborting)
	cairo_set_fill_rule (c5c->cr, f);
    return Void;
}

Value
do_Cairo_set_line_width (Value cv, Value lv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	l = DoublePart (lv, "invalid line_width");

    if (!aborting)
	cairo_set_line_width (c5c->cr, l);
    return Void;
}

Value
do_Cairo_set_line_cap (Value cv, Value lv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_line_cap_t	l = EnumIntPart (lv, "invalid line_cap");

    if (!aborting)
	cairo_set_line_cap (c5c->cr, l);
    return Void;
}

Value
do_Cairo_set_line_join (Value cv, Value lv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_line_join_t	l = EnumIntPart (lv, "invalid line_join");

    if (!aborting)
	cairo_set_line_join (c5c->cr, l);
    return Void;
}

Value
do_Cairo_set_dash (Value cv, Value dv, Value ov)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    double		o = DoublePart (ov, "invalid dash offset");
    int			ndash = ArrayLimits (&dv->array)[0];
    double		*d = AllocateTemp (ndash * sizeof (double));
    int			i;

    if (aborting)
	RETURN (Void);
    for (i = 0; i < ndash; i++)
    {
	d[i] = DoublePart (ArrayValue(&dv->array, i), "invalid dash length");
	if (aborting)
	    RETURN (Void);
    }
    cairo_set_dash (c5c->cr, d, ndash, o);
    RETURN(Void);
}

Value
do_Cairo_set_miter_limit (Value cv, Value mv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    double		m = DoublePart (mv, "invalid miter limit");
    
    if (!aborting)
	cairo_set_miter_limit (c5c->cr, m);
    return Void;
}

Value
do_Cairo_identity_matrix (Value cv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    if (!aborting)
	cairo_identity_matrix (c5c->cr);
    return Void;
}

Value
do_Cairo_default_matrix (Value cv)
{
    cairo_5c_t	    *c5c = cairo_5c_get (cv);

    if (aborting)
	return Void;
    cairo_default_matrix (c5c->cr);
    return Void;
}
    
Value
do_Cairo_translate (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_translate (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_scale (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_scale (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_rotate (Value cv, Value av)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	a = DoublePart (av, "invalid angle");

    if (!aborting)
	cairo_rotate (c5c->cr, a);
    return Void;
}

Value
do_Cairo_get_matrix (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    cairo_matrix_t  matrix;
    Value	    ret;

    if (aborting)
	RETURN(Void);
    
    cairo_get_matrix (c5c->cr, &matrix);
    ret = new_cairo_matrix (&matrix);
    RETURN (ret);
}

Value
do_Cairo_concat_matrix (Value cv, Value mv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    cairo_matrix_t  matrix;

    if (aborting)
	RETURN(Void);
    cairo_matrix_part (mv, &matrix, "invalid matrix");
    if (aborting)
	RETURN(Void);
    cairo_concat_matrix (c5c->cr, &matrix);
    RETURN (Void);
}

Value
do_Cairo_set_matrix (Value cv, Value mv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    cairo_matrix_t  matrix;

    if (aborting)
	RETURN(Void);
    cairo_matrix_part (mv, &matrix, "invalid matrix");
    if (aborting)
	RETURN(Void);
    cairo_set_matrix (c5c->cr, &matrix);
    RETURN (Void);
}

Value
do_Cairo_transform_point (Value cv, Value pv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    double	    x, y;
    Value	    ret;
    BoxPtr	    box;

    if (aborting)
	RETURN(Void);
    
    x = DoublePart (StructMemValue (pv, AtomId("x")), "invalid x coordinate");
    y = DoublePart (StructMemValue (pv, AtomId("y")), "invalid y coordinate");
    cairo_transform_point (c5c->cr, &x, &y);
    ret = NewStruct (TypeCanon (typeCairoPoint)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (x));
    BoxValueSet (box, 1, NewDoubleFloat (y));
    RETURN (ret);
}

Value
do_Cairo_transform_distance (Value cv, Value pv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    double	    x, y;
    Value	    ret;
    BoxPtr	    box;

    if (aborting)
	RETURN(Void);
    
    x = DoublePart (StructMemValue (pv, AtomId("x")), "invalid x coordinate");
    y = DoublePart (StructMemValue (pv, AtomId("y")), "invalid y coordinate");
    cairo_transform_distance (c5c->cr, &x, &y);
    ret = NewStruct (TypeCanon (typeCairoPoint)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (x));
    BoxValueSet (box, 1, NewDoubleFloat (y));
    RETURN (ret);
}

Value
do_Cairo_inverse_transform_point (Value cv, Value pv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    double	    x, y;
    Value	    ret;
    BoxPtr	    box;

    if (aborting)
	RETURN(Void);
    
    x = DoublePart (StructMemValue (pv, AtomId("x")), "invalid x coordinate");
    y = DoublePart (StructMemValue (pv, AtomId("y")), "invalid y coordinate");
    cairo_inverse_transform_point (c5c->cr, &x, &y);
    ret = NewStruct (TypeCanon (typeCairoPoint)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (x));
    BoxValueSet (box, 1, NewDoubleFloat (y));
    RETURN (ret);
}

Value
do_Cairo_inverse_transform_distance (Value cv, Value pv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    double	    x, y;
    Value	    ret;
    BoxPtr	    box;

    if (aborting)
	RETURN(Void);
    
    x = DoublePart (StructMemValue (pv, AtomId("x")), "invalid x coordinate");
    y = DoublePart (StructMemValue (pv, AtomId("y")), "invalid y coordinate");
    cairo_inverse_transform_distance (c5c->cr, &x, &y);
    ret = NewStruct (TypeCanon (typeCairoPoint)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (x));
    BoxValueSet (box, 1, NewDoubleFloat (y));
    RETURN (ret);
}

Value
do_Cairo_init_clip (Value cv)
{
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    if (!aborting)
	cairo_init_clip (c5c->cr);
    return Void;
}

Value
do_Cairo_clip (Value cv)
{
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    if (!aborting)
	cairo_clip (c5c->cr);
    return Void;
}

Value
do_Cairo_get_operator (Value cv)
{
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	return Void;
    return IntToEnum (typeCairoOperator, cairo_get_operator (c5c->cr));
}

#if 0
Value
do_Cairo_current_rgb_color (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    double	    r, g, b;
    Value	    ret;
    BoxPtr	    box;
    
    if (aborting)
	return Void;
    cairo_current_rgb_color (c5c->cr, &r, &g, &b);
    ret = NewStruct (TypeCanon (typeCairoRgbColor)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (r));
    BoxValueSet (box, 1, NewDoubleFloat (g));
    BoxValueSet (box, 2, NewDoubleFloat (b));
    RETURN (ret);
}

Value
do_Cairo_current_alpha (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN (NewDoubleFloat (cairo_current_alpha (c5c->cr)));
}
#endif

Value
do_Cairo_get_tolerance (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN (NewDoubleFloat (cairo_get_tolerance (c5c->cr)));
}

Value
do_Cairo_get_current_point (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    double	    x, y;
    Value	    ret;
    BoxPtr	    box;
    
    if (aborting)
	return Void;
    cairo_get_current_point (c5c->cr, &x, &y);
    ret = NewStruct (TypeCanon (typeCairoPoint)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (x));
    BoxValueSet (box, 1, NewDoubleFloat (y));
    RETURN (ret);
}

Value
do_Cairo_get_fill_rule (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN(IntToEnum (typeCairoFillRule, cairo_get_fill_rule (c5c->cr)));
}

Value
do_Cairo_get_line_width (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN(NewDoubleFloat (cairo_get_line_width (c5c->cr)));
}

Value
do_Cairo_get_line_cap (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN(IntToEnum (typeCairoLineCap, cairo_get_line_cap (c5c->cr)));
}

Value
do_Cairo_get_line_join (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN(IntToEnum (typeCairoLineJoin, cairo_get_line_join (c5c->cr)));
}

Value
do_Cairo_get_miter_limit (Value cv)
{
    ENTER ();
    cairo_5c_t	    *c5c = cairo_5c_get (cv);
    
    if (aborting)
	RETURN(Void);
    RETURN(NewDoubleFloat (cairo_get_miter_limit (c5c->cr)));
}
