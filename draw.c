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
    cairo_5c_t	*c5c = get_cairo_5c (cv);

    if (!aborting)
	cairo_new_path (c5c->cr);
    return Void;
}

Value
do_Cairo_move_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_move_to (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_line_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
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
    cairo_5c_t	*c5c = get_cairo_5c (cv);
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
	      Value x1v, Value y1v,
	      Value x2v, Value y2v,
	      Value rv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x1 = DoublePart (x1v, "invalid X1 value");
    double	y1 = DoublePart (y1v, "invalid Y1 value");
    double	x2 = DoublePart (x2v, "invalid X2 value");
    double	y2 = DoublePart (y2v, "invalid Y2 value");
    double	r = DoublePart (rv, "invalid radius");

    if (!aborting)
	cairo_arc (c5c->cr, x1, y1, x2, y2, r);
    return Void;
}

Value
do_Cairo_arc_negative (Value cv, 
		       Value x1v, Value y1v,
		       Value x2v, Value y2v,
		       Value rv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x1 = DoublePart (x1v, "invalid X1 value");
    double	y1 = DoublePart (y1v, "invalid Y1 value");
    double	x2 = DoublePart (x2v, "invalid X2 value");
    double	y2 = DoublePart (y2v, "invalid Y2 value");
    double	r = DoublePart (rv, "invalid radius");

    if (!aborting)
	cairo_arc_negative (c5c->cr, x1, y1, x2, y2, r);
    return Void;
}

Value
do_Cairo_rel_move_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (!aborting)
	cairo_rel_move_to (c5c->cr, x, y);
    return Void;
}

Value
do_Cairo_rel_line_to (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
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
    cairo_5c_t	*c5c = get_cairo_5c (cv);
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
    cairo_5c_t	*c5c = get_cairo_5c (cv);
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
    cairo_5c_t	*c5c = get_cairo_5c (cv);

    if (!aborting)
	cairo_close_path (c5c->cr);
    return Void;
}

Value
do_Cairo_fill (Value cv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);

    if (!aborting)
    {
	cairo_fill (c5c->cr);
	dirty_cairo_5c (c5c);
    }
    return Void;
}

Value
do_Cairo_stroke (Value cv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);

    if (!aborting) {
	cairo_stroke (c5c->cr);
	dirty_cairo_5c (c5c);
    }
    return Void;
}

Value
do_Cairo_in_stroke (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (aborting)
	return Void;
    
    return cairo_in_stroke (c5c->cr, x, y) ? TrueVal : FalseVal;
}

Value
do_Cairo_in_fill (Value cv, Value xv, Value yv)
{
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x = DoublePart (xv, "invalid X value");
    double	y = DoublePart (yv, "invalid Y value");

    if (aborting)
	return Void;
    
    return cairo_in_fill (c5c->cr, x, y) ? TrueVal : FalseVal;
}

Value
do_Cairo_stroke_extents (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x, y, w, h;
    Value	ret;
    static int	dims[2] = { 2, 2 };
    
    if (aborting)
	RETURN(Void);
    cairo_stroke_extents (c5c->cr, &x, &y, &w, &h);
    ret = NewArray (False, False, typePrim[rep_float], 2, dims);
    ArrayValueSet(&ret->array, 0, NewDoubleFloat (x));
    ArrayValueSet(&ret->array, 1, NewDoubleFloat (y));
    ArrayValueSet(&ret->array, 2, NewDoubleFloat (w));
    ArrayValueSet(&ret->array, 3, NewDoubleFloat (h));
    RETURN (ret);
}

Value
do_Cairo_fill_extents (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    double	x, y, w, h;
    Value	ret;
    static int	dims[2] = { 2, 2 };
    
    if (aborting)
	RETURN(Void);
    cairo_fill_extents (c5c->cr, &x, &y, &w, &h);
    ret = NewArray (False, False, typePrim[rep_float], 2, dims);
    ArrayValueSet(&ret->array, 0, NewDoubleFloat (x));
    ArrayValueSet(&ret->array, 1, NewDoubleFloat (y));
    ArrayValueSet(&ret->array, 2, NewDoubleFloat (w));
    ArrayValueSet(&ret->array, 3, NewDoubleFloat (h));
    RETURN (ret);
}

typedef struct _path_closure {
    Value   head;
    Value   *tail;
} path_closure_t;

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
path_box (void *closure, Type *type, char *tag)
{
    ENTER ();
    path_closure_t  *pc = closure;
    Value	    elt = NewStruct (TypeCanon (type)->structs.structs, False);
    Value	    elt_ref = NewRef (NewBox (False, False, 1, type), 0);
    Value	    path = path_new (tag, elt_ref);
    BoxPtr	    box = elt->structs.values;

    RefValueSet (elt_ref, elt);
    /* move End element after this element */
    BoxValueSet (box, 0, *pc->tail);
    /* append element to list */
    *pc->tail = path;
    pc->tail = &BoxElements(box)[0];
    RETURN (elt);
}

static void
cairo_5c_move_to (void *closure, double x, double y)
{
    ENTER ();
    Value   elt = path_box (closure, typeCairoMoveTo, "move_to");
    BoxPtr  box = elt->structs.values;
    BoxValueSet (box, 1, NewDoubleFloat (x));
    BoxValueSet (box, 2, NewDoubleFloat (y));
    EXIT ();
}

static void
cairo_5c_line_to (void *closure, double x, double y)
{
    ENTER ();
    Value   elt = path_box (closure, typeCairoLineTo, "line_to");
    BoxPtr  box = elt->structs.values;
    BoxValueSet (box, 1, NewDoubleFloat (x));
    BoxValueSet (box, 2, NewDoubleFloat (y));
    EXIT ();
}

static void
cairo_5c_curve_to (void *closure,
		   double x1, double y1,
		   double x2, double y2,
		   double x3, double y3)
{
    ENTER ();
    Value   elt = path_box (closure, typeCairoCurveTo, "curve_to");
    BoxPtr  box = elt->structs.values;
    BoxValueSet (box, 1, NewDoubleFloat (x1));
    BoxValueSet (box, 2, NewDoubleFloat (y1));
    BoxValueSet (box, 3, NewDoubleFloat (x2));
    BoxValueSet (box, 4, NewDoubleFloat (y2));
    BoxValueSet (box, 5, NewDoubleFloat (x3));
    BoxValueSet (box, 6, NewDoubleFloat (y3));
    EXIT ();
}

static void
cairo_5c_close_path (void *closure)
{
    ENTER ();
    (void) path_box (closure, typeCairoClosePath, "close_path");
    EXIT ();
}

Value
do_Cairo_current_path_list (Value cv)
{
    ENTER ();
    path_closure_t  close;
    cairo_5c_t	    *c5c = get_cairo_5c (cv);
    
    if (aborting)
	RETURN (Void);
    close.head = path_new ("end", Void);
    close.tail = &close.head;
    cairo_current_path (c5c->cr,
			cairo_5c_move_to,
			cairo_5c_line_to,
			cairo_5c_curve_to,
			cairo_5c_close_path,
			&close);
    RETURN(close.head);
}

Value
do_Cairo_current_path_flat_list (Value cv)
{
    ENTER ();
    path_closure_t  close;
    cairo_5c_t	    *c5c = get_cairo_5c (cv);
    
    if (aborting)
	RETURN (Void);
    close.head = path_new ("end", Void);
    close.tail = &close.head;
    cairo_current_path_flat (c5c->cr,
			     cairo_5c_move_to,
			     cairo_5c_line_to,
			     cairo_5c_close_path,
			     &close);
    RETURN(close.head);
}
