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
do_Cairo_select_font (Value cv, Value fv, Value sv, Value wv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    char		*family = StrzPart (fv, "invalid family");
    cairo_font_slant_t	slant = IntPart (sv, "invalid slant");
    cairo_font_weight_t	weight = IntPart (wv, "invalid weight");

    if (!aborting)
	cairo_select_font (c5c->cr, family, slant, weight);
    return Void;
}

Value
do_Cairo_scale_font (Value cv, Value sv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    double		scale = DoublePart (sv, "invalid scale");
    
    if (!aborting)
	cairo_scale_font (c5c->cr, scale);
    return Void;
}

Value
do_Cairo_show_text (Value cv, Value uv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    char		*utf8 = StrzPart (uv, "invalid text");

    if (!aborting)
    {
	cairo_show_text (c5c->cr, utf8);
	dirty_cairo_5c (c5c);
    }
    return Void;
}

Value
do_Cairo_text_path (Value cv, Value uv)
{
    cairo_5c_t		*c5c = get_cairo_5c (cv);
    char		*utf8 = StrzPart (uv, "invalid text");

    if (!aborting)
	cairo_text_path (c5c->cr, utf8);
    return Void;
}

Value
do_Cairo_text_extents (Value cv, Value uv)
{
    ENTER ();
    cairo_5c_t		    *c5c = get_cairo_5c (cv);
    char		    *utf8 = StrzPart (uv, "invalid text");
    cairo_text_extents_t    extents;
    Value		    ret;
    BoxPtr		    box;

    if (aborting)
	return Void;
    cairo_text_extents (c5c->cr, utf8, &extents);
    ret = NewStruct (TypeCanon (typeTextExtents)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (extents.x_bearing));
    BoxValueSet (box, 1, NewDoubleFloat (extents.y_bearing));
    BoxValueSet (box, 2, NewDoubleFloat (extents.width));
    BoxValueSet (box, 3, NewDoubleFloat (extents.height));
    BoxValueSet (box, 4, NewDoubleFloat (extents.x_advance));
    BoxValueSet (box, 5, NewDoubleFloat (extents.y_advance));
    RETURN (ret);
}
