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
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    char		*family = StrzPart (fv, "invalid family");
    cairo_font_slant_t	slant = EnumIntPart (sv, "invalid slant");
    cairo_font_weight_t	weight = EnumIntPart (wv, "invalid weight");

    if (!aborting)
	cairo_select_font (c5c->cr, family, slant, weight);
    return Void;
}

Value
do_Cairo_set_font (Value cv, Value fv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    char		*name = StrzPart (fv, "invalid name");
    FcPattern		*pat;
    static FT_Library	ft_library;
    cairo_font_t	*font;
    double		scale = 0;

    if (aborting)
	RETURN (Void);
    if (!ft_library)
	if (FT_Init_FreeType (&ft_library))
	{
	    RaiseStandardException (exception_open_error,
				    "can't open FreeType",
				    1, fv);
	    RETURN (Void);
	}
    pat = FcNameParse (name);
    FcPatternGetDouble (pat, FC_SIZE, 0, &scale);
    font = cairo_ft_font_create (ft_library, pat);
    FcPatternDestroy (pat);
    if (!font)
    {
	RaiseStandardException (exception_open_error,
				"can't open font",
				1, fv);
	RETURN (Void);
    }
    cairo_set_font (c5c->cr, font);
    if (scale != 0)
	cairo_scale_font(c5c->cr, scale);
    cairo_font_destroy (font);
    RETURN(Void);
}

Value
do_Cairo_scale_font (Value cv, Value sv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    double		scale = DoublePart (sv, "invalid scale");
    
    if (!aborting)
	cairo_scale_font (c5c->cr, scale);
    return Void;
}

Value
do_Cairo_transform_font (Value cv, Value mv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_matrix_t	*matrix;
    
    if (aborting)
	RETURN(Void);
    matrix = cairo_matrix_part (mv, "invalid matrix");
    if (aborting)
	RETURN(Void);
    cairo_transform_font (c5c->cr, matrix);
    cairo_matrix_destroy (matrix);
    RETURN(Void);
}

Value
do_Cairo_current_font_extents (Value cv)
{
    ENTER ();
    cairo_5c_t		    *c5c = cairo_5c_get (cv);
    cairo_font_extents_t    extents;
    Value		    ret;
    BoxPtr		    box;

    if (aborting)
	return Void;
    cairo_current_font_extents (c5c->cr, &extents);
    ret = NewStruct (TypeCanon (typeCairoFontExtents)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (extents.ascent));
    BoxValueSet (box, 1, NewDoubleFloat (extents.descent));
    BoxValueSet (box, 2, NewDoubleFloat (extents.height));
    BoxValueSet (box, 3, NewDoubleFloat (extents.max_x_advance));
    BoxValueSet (box, 4, NewDoubleFloat (extents.max_y_advance));
    RETURN (ret);
}

Value
do_Cairo_show_text (Value cv, Value uv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    char		*utf8 = StrzPart (uv, "invalid text");

    if (!aborting)
    {
	cairo_show_text (c5c->cr, utf8);
	cairo_5c_dirty (c5c);
    }
    return Void;
}

Value
do_Cairo_text_path (Value cv, Value uv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    char		*utf8 = StrzPart (uv, "invalid text");

    if (!aborting)
	cairo_text_path (c5c->cr, utf8);
    return Void;
}

Value
do_Cairo_text_extents (Value cv, Value uv)
{
    ENTER ();
    cairo_5c_t		    *c5c = cairo_5c_get (cv);
    char		    *utf8 = StrzPart (uv, "invalid text");
    cairo_text_extents_t    extents;
    Value		    ret;
    BoxPtr		    box;

    if (aborting)
	return Void;
    cairo_text_extents (c5c->cr, utf8, &extents);
    ret = NewStruct (TypeCanon (typeCairoTextExtents)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (extents.x_bearing));
    BoxValueSet (box, 1, NewDoubleFloat (extents.y_bearing));
    BoxValueSet (box, 2, NewDoubleFloat (extents.width));
    BoxValueSet (box, 3, NewDoubleFloat (extents.height));
    BoxValueSet (box, 4, NewDoubleFloat (extents.x_advance));
    BoxValueSet (box, 5, NewDoubleFloat (extents.y_advance));
    RETURN (ret);
}
