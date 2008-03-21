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
do_Cairo_new_path (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting)
	cairo_new_path (c5c->cr);
    return Void;
}

Value
do_Cairo_move_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_move_to (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_line_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_line_to (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_curve_to (Value cv, 
		   Value x1v, Value y1v,
		   Value x2v, Value y2v,
		   Value x3v, Value y3v)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x1 = DoublePart (x1v, "invalid X1 value");
    double	y1 = DoublePart (y1v, "invalid Y1 value");
    double	x2 = DoublePart (x2v, "invalid X2 value");
    double	y2 = DoublePart (y2v, "invalid Y2 value");
    double	x3 = DoublePart (x3v, "invalid X3 value");
    double	y3 = DoublePart (y3v, "invalid Y3 value");

    if (!aborting)
	cairo_curve_to (c5c->cr, x1, y1, x2, y2, x3, y3);
    return Void;
}

Value
do_Cairo_arc (Value cv, 
	      Value xcv, Value ycv, Value rv,
	      Value a1v, Value a2v)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	xc = DoublePart (xcv, "invalid xc value");
    double	yc = DoublePart (ycv, "invalid yc value");
    double	r = DoublePart (rv, "invalid radius");
    double	a1 = DoublePart (a1v, "invalid angle1 value");
    double	a2 = DoublePart (a2v, "invalid angle2 value");

    if (!aborting)
	cairo_arc (c5c->cr, xc, yc, r, a1, a2);
    return Void;
}

Value
do_Cairo_arc_negative (Value cv, 
		       Value xcv, Value ycv, Value rv,
		       Value a1v, Value a2v)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	xc = DoublePart (xcv, "invalid xc value");
    double	yc = DoublePart (ycv, "invalid yc value");
    double	r = DoublePart (rv, "invalid radius");
    double	a1 = DoublePart (a1v, "invalid angle1 value");
    double	a2 = DoublePart (a2v, "invalid angle2 value");

    if (!aborting)
	cairo_arc_negative (c5c->cr, xc, yc, r, a1, a2);
    return Void;
}

Value
do_Cairo_rel_move_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_rel_move_to (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_rel_line_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_rel_line_to (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_rel_curve_to (Value cv, 
		   Value x1v, Value y1v,
		   Value x2v, Value y2v,
		   Value x3v, Value y3v)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x1 = DoublePart (x1v, "invalid X1 value");
    double	y1 = DoublePart (y1v, "invalid Y1 value");
    double	x2 = DoublePart (x2v, "invalid X2 value");
    double	y2 = DoublePart (y2v, "invalid Y2 value");
    double	x3 = DoublePart (x3v, "invalid X3 value");
    double	y3 = DoublePart (y3v, "invalid Y3 value");

    if (!aborting)
	cairo_rel_curve_to (c5c->cr, x1, y1, x2, y2, x3, y3);
    return Void;
}

Value
do_Cairo_rectangle (Value cv, Value xv, Value yv, Value wv, Value hv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid x value");
    double	y = DoublePart (yv, "invalid y value");
    double	w = DoublePart (wv, "invalid width value");
    double	h = DoublePart (hv, "invalid height value");
    
    if (!aborting)
	cairo_rectangle (c5c->cr, x, y, w, h);
    return Void;
}

Value
do_Cairo_close_path (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting)
	cairo_close_path (c5c->cr);
    return Void;
}

Value
do_Cairo_paint (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting)
    {
	cairo_paint (c5c->cr);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_paint_with_alpha (Value cv, Value av)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	alpha = DoublePart (av, "invalid alpha value");

    if (!aborting)
    {
	cairo_paint_with_alpha (c5c->cr, alpha);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_mask (Value cv, Value pv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_pattern_t	*pat = get_cairo_pattern (pv);

    if (!aborting)
    {
	cairo_mask (c5c->cr, pat);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_mask_surface (Value cv, Value sv, Value xv, Value yv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_5c_surface_t	*c5s = cairo_5c_surface_get (sv);
    double		x = DoublePart (xv, "invalid X value");
    double		y = DoublePart (yv, "invalid Y value");

    if (!aborting)
    {
	cairo_mask_surface (c5c->cr, c5s->surface, x, y);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_fill (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting)
    {
	cairo_fill (c5c->cr);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_fill_preserve (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting)
    {
	cairo_fill_preserve (c5c->cr);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_stroke (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting) {
	cairo_stroke (c5c->cr);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_stroke_preserve (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (!aborting) {
	cairo_stroke_preserve (c5c->cr);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_copy_page (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (aborting)
	return Void;
    if (c5c->surface != Void)
    {
	cairo_5c_surface_t *c5s = cairo_5c_surface_get (c5c->surface);
	if (!c5s)
	    return 0;
	c5s->copied = True;
	cairo_copy_page (c5c->cr);
    }
    return Void;
}

Value
do_Cairo_show_page (Value cv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);

    if (aborting)
	return Void;
    if (c5c->surface != Void)
    {
	cairo_5c_surface_t *c5s = cairo_5c_surface_get (c5c->surface);
	if (!c5s)
	    return 0;
	c5s->copied = True;
	cairo_show_page (c5c->cr);
    }
    return Void;
}

Value
do_Cairo_in_stroke (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (aborting)
	return Void;
    
    return cairo_in_stroke (c5c->cr, x, y) ? TrueVal : FalseVal;
}

Value
do_Cairo_in_fill (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (aborting)
	return Void;
    
    return cairo_in_fill (c5c->cr, x, y) ? TrueVal : FalseVal;
}

static Value
do_extents (Value cv, void (*extents) (cairo_t *,
				       double *, double *,
				       double *, double *))
{
    ENTER ();
    cairo_5c_t	*c5c = cairo_5c_get (cv);
    double	x1, y1, x2, y2;
    Value	ret;
    BoxPtr	box;
    
    if (aborting)
	RETURN(Void);
    /* XXX cairo doesn't initialize these on error */
    x1 = y1 = x2 = y2 = 0;
    (*extents) (c5c->cr, &x1, &y1, &x2, &y2);
    ret = NewStruct (TypeCanon (typeCairoRect)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (x1));
    BoxValueSet (box, 1, NewDoubleFloat (y1));
    BoxValueSet (box, 2, NewDoubleFloat (x2 - x1));
    BoxValueSet (box, 3, NewDoubleFloat (y2 - y1));
    RETURN (ret);
}

Value
do_Cairo_stroke_extents (Value cv)
{
    return do_extents (cv, cairo_stroke_extents);
}

Value
do_Cairo_fill_extents (Value cv)
{
    return do_extents (cv, cairo_fill_extents);
}

static Value
path_new (char *tag, Value value)
{
    ENTER ();
    Value   pt = NewUnion (TypeCanon (typeCairoPath)->structs.structs, False);
    BoxPtr  box = pt->unions.value;
    pt->unions.tag = AtomId (tag);
    BoxValueSet (box, 0, value);
    RETURN (pt);
}

static Value
path_elt_new (Type *type)
{
    return NewStruct (TypeCanon (type)->structs.structs, False);
}

static Value
path_array (Value cv, cairo_path_t* (*copy_path) (cairo_t *cr))
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_path_t	*path;
    Value		pv;
    Value		pev;
    Value		e;
    BoxPtr		box;
    int			zero = 0;
    int			n = 0;
    int			i, j;
    cairo_path_data_t	*data;
    char		*tag;
    
    if (aborting)
	RETURN (Void);
    
    path = (*copy_path) (c5c->cr);
    if (!path)
    {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString("Can't copy path"),
				NewInt (0), cv);
	RETURN (Void);
    }
    pv = NewArray (False, True, typeCairoPath, 1, &zero);
    for (i=0; i < path->num_data; i += path->data[i].header.length) 
    {
	data = &path->data[i];
	switch (data->header.type) {
	case CAIRO_PATH_MOVE_TO:
	case CAIRO_PATH_LINE_TO:
	    tag = data->header.type == CAIRO_PATH_MOVE_TO ? "move_to" : "line_to";
	    e = path_elt_new (typeCairoPoint);
	    box = e->structs.values;
	    BoxValueSet (box, 0, NewDoubleFloat (data[1].point.x));
	    BoxValueSet (box, 1, NewDoubleFloat (data[1].point.y));
	    break;
	case CAIRO_PATH_CURVE_TO:
	    tag = "curve_to";
	    e = path_elt_new (typeCairoCurveTo);
	    box = e->structs.values;
	    for (j = 0; j < 3; j++)
	    {
		BoxValueSet (box, j*2+0, NewDoubleFloat (data[j+1].point.x));
		BoxValueSet (box, j*2+1, NewDoubleFloat (data[j+1].point.y));
	    }
	    break;
	case CAIRO_PATH_CLOSE_PATH:
	    tag = "close_path";
	    e = Void;
	    break;
	default:
	    tag = "unknown";
	    e = Void;
	    break;
	}
	pev = path_new (tag, e);
	ArrayResize (pv, 0, n+1);
	ArrayValueSet (&pv->array, n, pev);
	n++;
    }
    
    cairo_path_destroy (path);
    RETURN(pv);
}

Value
do_Cairo_copy_path (Value cv)
{
    ENTER ();
    RETURN (path_array (cv, cairo_copy_path));
}

Value
do_Cairo_copy_path_flat (Value cv)
{
    ENTER ();
    RETURN (path_array (cv, cairo_copy_path_flat));
}

Value
do_Cairo_append_path (Value cv, Value pv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    Array		*a = &pv->array;
    int			nelement = ArrayLimits(a)[0];
    int			element;
    Atom		move_to_atom = AtomId("move_to");
    Atom		line_to_atom = AtomId("line_to");
    Atom		curve_to_atom = AtomId("curve_to");
    Atom		close_path_atom = AtomId("close_path");
    cairo_path_data_t	*pd;
    cairo_path_t	*path;

    if (aborting)
	RETURN (Void);

    path = malloc(sizeof(*path));
    if (!path)
	panic("Out of memory");
    
    /*
     * Make sure there's plenty of space
     */
    pd = malloc(4 * nelement * sizeof(*pd));
    if (!pd)
	panic("Out of memory");

    path->status = 0;
    path->data = pd;
    for (element = 0; element < nelement; element++) 
    {
	Value	pev = ArrayValueGet(a, element);
	Union	*u = &pev->unions;
	Value	v = BoxValue(u->value, 0);
	BoxPtr	w = v->structs.values;
	int	ncoord = 1, coord;

	if (u->tag == move_to_atom)
	    pd->header.type = CAIRO_PATH_MOVE_TO;
	else if (u->tag == line_to_atom)
	    pd->header.type = CAIRO_PATH_LINE_TO;
	else if (u->tag == curve_to_atom) 
	{
	    pd->header.type = CAIRO_PATH_CURVE_TO;
	    ncoord = 3;
	}
	else if (u->tag == close_path_atom)
	{
	    pd->header.type = CAIRO_PATH_CLOSE_PATH;
	    ncoord = 0;
	}
	else
	{
	    RaiseStandardException (exception_invalid_argument, 3,
				    NewStrString ("Bad path component"),
				    NewInt (1), pv);
	    break;
	}
	pd->header.length = ncoord + 1;
	pd++;
	for (coord = 0; coord < ncoord; coord++)
	{
	    pd->point.x = DoublePart (BoxValueGet (w, coord * 2 + 0),
				      "Bogus X coordinate in path");
	    pd->point.y = DoublePart (BoxValueGet (w, coord * 2 + 1),
				      "Bogus Y coordinate in path");
	    if (aborting)
		break;
	    pd++;
	}
	if (aborting)
	    break;
    }
    if (!aborting)
    {
	path->num_data = pd - path->data;
	cairo_append_path(c5c->cr, path);
    }
    free(path->data);
    free(path);
    RETURN (Void);
}
