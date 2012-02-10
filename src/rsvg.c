/*
 * Copyright © 2012 Keith Packard
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

static char	RsvgId[] = "Rsvg";

typedef struct _rsvg_5c_t {
    DataType	*data;
    RsvgHandle	*rsvg;
} rsvg_5c_t;

static rsvg_5c_t *
rsvg_5c_get (Value av)
{
    rsvg_5c_t	*r5c;

    if (av->foreign.id != RsvgId) {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("not a rsvg_t"),
				NewInt(0), av);
	return 0;
    }

    r5c = av->foreign.data;
    if (!r5c) {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString ("rsvg destroyed"),
				NewInt(0), av);
	return 0;
    }
    return r5c;
}

static int
rsvg_5c_free(void *v)
{
    rsvg_5c_t	*r5c = v;

    if (r5c->rsvg) {
	rsvg_handle_free(r5c->rsvg);
	r5c->rsvg = NULL;
    }
    return 1;
}

static DataType Rsvg5cType = {
    NULL,
    rsvg_5c_free,
    "Rsvg5c"
};

static void
rsvg_foreign_mark(void *object)
{
    MemReference(object);
}

static void
rsvg_foreign_free(void *object)
{
    /* let nickle finalizer deal with this */
    ;
}

Value
do_Rsvg_new_from_string(Value sv)
{
    return Void;
}

Value
do_Rsvg_new_from_file(Value fv)
{
    ENTER();
    rsvg_5c_t	*r5c;
    RsvgHandle	*rsvg;
    char	*name = StrzPart(fv, "invalid filename");
    GError	*error;
    Value	ret;

    if (!name)
	RETURN(Void);
    
    error = NULL;
    rsvg = rsvg_handle_new_from_file(name, &error);
    if (!rsvg) {
	RaiseStandardException (exception_invalid_argument, 3,
				NewStrString (error->message),
				NewInt(0), fv);
	RETURN (Void);
    }
    r5c = ALLOCATE(&Rsvg5cType, sizeof (rsvg_5c_t));
    r5c->rsvg = rsvg;
    ret = NewForeign (RsvgId, r5c,
		      rsvg_foreign_mark, rsvg_foreign_free);
    RETURN(ret);
}

Value
do_Rsvg_render(Value rv, Value cv)
{
    rsvg_5c_t	*r5c = rsvg_5c_get(rv);
    if (!r5c)
	return Void;

    cairo_5c_t	*c5c = cairo_5c_get(cv);

    if (!c5c)
	return Void;

    rsvg_handle_render_cairo(r5c->rsvg, c5c->cr);
    return Void;
}

Value
do_Rsvg_render_sub(Value cv, Value rv, Value sv)
{
    rsvg_5c_t	*r5c = rsvg_5c_get(cv);
    if (!r5c)
	return Void;

    cairo_5c_t	*c5c = cairo_5c_get(cv);

    if (!c5c)
	return Void;

    char *id = StrzPart(sv, "invalid SVG id");
    if (!id)
	return Void;

    rsvg_handle_render_cairo_sub(r5c->rsvg, c5c->cr, id);
    return Void;
}

Value
do_Rsvg_set_dpi(Value rv, Value dv)
{
    rsvg_5c_t	*r5c = rsvg_5c_get(rv);
    double	dpi = DoublePart(dv, "invalid DPI value");

    if (!aborting)
	rsvg_handle_set_dpi(r5c->rsvg, dpi);
    return Void;
}

Value
do_Rsvg_set_dpi_x_y(Value rv, Value dx, Value dy)
{
    rsvg_5c_t	*r5c = rsvg_5c_get(rv);
    double	dpi_x = DoublePart(dx, "invalid X DPI value");
    double	dpi_y = DoublePart(dy, "invalid Y DPI value");

    if (!aborting)
	rsvg_handle_set_dpi_x_y(r5c->rsvg, dpi_x, dpi_y);
    return Void;
}

Value
do_Rsvg_get_dimensions(Value rv)
{
    ENTER();
    rsvg_5c_t	*r5c = rsvg_5c_get(rv);
    RsvgDimensionData 	dimension_data;
    Value	ret;
    BoxPtr	box;

    if (aborting)
	RETURN(Void);
    rsvg_handle_get_dimensions(r5c->rsvg, &dimension_data);
    ret = NewStruct (TypeCanon (typeRsvgDimensions)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet(box, 0, NewInt(dimension_data.width));
    BoxValueSet(box, 1, NewInt(dimension_data.height));
    BoxValueSet(box, 2, NewDoubleFloat(dimension_data.ex));
    BoxValueSet(box, 3, NewDoubleFloat(dimension_data.em));
    RETURN(ret);
}

Value
do_Rsvg_get_dimensions_sub(Value rv, Value sv)
{
    ENTER();
    rsvg_5c_t	*r5c = rsvg_5c_get(rv);
    char	*id = StrzPart(sv, "Invalid SVG ID");
    RsvgDimensionData 	dimension_data;
    Value	ret;
    BoxPtr	box;

    if (aborting)
	RETURN(Void);
    rsvg_handle_get_dimensions_sub(r5c->rsvg, &dimension_data, id);
    ret = NewStruct (TypeCanon (typeRsvgDimensions)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet(box, 0, NewInt(dimension_data.width));
    BoxValueSet(box, 1, NewInt(dimension_data.height));
    BoxValueSet(box, 2, NewDoubleFloat(dimension_data.ex));
    BoxValueSet(box, 3, NewDoubleFloat(dimension_data.em));
    RETURN(ret);
}

Value
do_Rsvg_get_position_sub(Value rv, Value sv)
{
    ENTER();
    rsvg_5c_t	*r5c = rsvg_5c_get(rv);
    char	*id = StrzPart(sv, "Invalid SVG ID");
    RsvgPositionData 	position_data;
    Value	ret;
    BoxPtr	box;

    if (aborting)
	RETURN(Void);
    rsvg_handle_get_position_sub(r5c->rsvg, &position_data, id);
    ret = NewStruct (TypeCanon (typeRsvgPosition)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet(box, 0, NewInt(position_data.x));
    BoxValueSet(box, 1, NewInt(position_data.y));
    RETURN(ret);
}
