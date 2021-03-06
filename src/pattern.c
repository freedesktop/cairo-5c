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
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("not a pattern_t"),
				NewInt(0), pv);
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
do_Cairo_Pattern_create_rgb (Value rv, Value gv, Value bv)
{
    ENTER ();
    double	    r = DoublePart (rv, "red invalid");
    double	    g = DoublePart (gv, "green invalid");
    double	    b = DoublePart (bv, "blue invalid");
    
    if (aborting)
	RETURN(Void);
    RETURN (make_pattern_value (cairo_pattern_create_rgb (r, g, b)));
}

Value
do_Cairo_Pattern_create_rgba (Value rv, Value gv, Value bv, Value av)
{
    ENTER ();
    double	    r = DoublePart (rv, "red invalid");
    double	    g = DoublePart (gv, "green invalid");
    double	    b = DoublePart (bv, "blue invalid");
    double	    a = DoublePart (av, "alpha invalid");
    
    if (aborting)
	RETURN(Void);
    RETURN (make_pattern_value (cairo_pattern_create_rgba (r, g, b, a)));
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
do_Cairo_Pattern_create_for_surface (Value sv)
{
    ENTER ();
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);

    if (aborting)
	RETURN (Void);
    RETURN (make_pattern_value (cairo_pattern_create_for_surface (c5s->surface)));
}

Value
do_Cairo_Pattern_add_color_stop_rgba (Value patv, Value offsetv,
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
    cairo_pattern_add_color_stop_rgba (pat, offset, red, green, blue, alpha);
    RETURN (Void);
}

Value
do_Cairo_Pattern_add_color_stop_rgb (Value patv, Value offsetv,
				     Value redv, Value greenv, Value bluev)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    double	    offset = DoublePart (offsetv, "offset invalid");
    double	    red = DoublePart (redv, "red invalid");
    double	    green = DoublePart (greenv, "green invalid");
    double	    blue = DoublePart (bluev, "blue invalid");

    if (aborting)
	RETURN(Void);
    cairo_pattern_add_color_stop_rgb (pat, offset, red, green, blue);
    RETURN (Void);
}

Value
do_Cairo_Pattern_set_matrix (Value patv, Value mv)
{
    ENTER ();
    cairo_pattern_t *pat = get_cairo_pattern (patv);
    cairo_matrix_t  matrix;

    if (aborting)
	RETURN(Void);
    cairo_matrix_part (mv, &matrix, "invalid pattern matrix");
    cairo_pattern_set_matrix (pat, &matrix);
    RETURN (Void);
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

    if (aborting)
	RETURN(Void);
    cairo_pattern_set_extend (pat, extend);
    RETURN (Void);
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

    if (aborting)
	RETURN(Void);
    cairo_pattern_set_filter (pat, filter);
    RETURN (Void);
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
