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

cairo_matrix_t *
cairo_matrix_part (Value mv, char *err)
{
    double a = DoublePart (ArrayValue(&mv->array, 0), "invalid matrix");
    double b = DoublePart (ArrayValue(&mv->array, 1), "invalid matrix");
    double c = DoublePart (ArrayValue(&mv->array, 2), "invalid matrix");
    double d = DoublePart (ArrayValue(&mv->array, 3), "invalid matrix");
    double tx = DoublePart (ArrayValue(&mv->array, 4), "invalid matrix");
    double ty = DoublePart (ArrayValue(&mv->array, 5), "invalid matrix");
    cairo_matrix_t  *matrix;

    if (aborting)
	return 0;

    matrix = cairo_matrix_create ();
    if (!matrix)
    {
	RaiseStandardException (exception_invalid_argument,
				err,
				1, mv);
	return 0;
    }
    cairo_matrix_set_affine (matrix, a, b, c, d, tx, ty);
    return matrix;
}

Value
new_cairo_matrix (cairo_matrix_t *matrix)
{
    ENTER ();
    double  a, b, c, d, tx, ty;
    Value   ret;
    static int	dims[2] = { 2, 3 };
    
    cairo_matrix_get_affine (matrix, &a, &b, &c, &d, &tx, &ty);
    ret = NewArray (False, False, typePrim[rep_float], 2, dims);
    ArrayValueSet(&ret->array, 0, NewDoubleFloat (a));
    ArrayValueSet(&ret->array, 1, NewDoubleFloat (b));
    ArrayValueSet(&ret->array, 2, NewDoubleFloat (c));
    ArrayValueSet(&ret->array, 3, NewDoubleFloat (d));
    ArrayValueSet(&ret->array, 4, NewDoubleFloat (tx));
    ArrayValueSet(&ret->array, 5, NewDoubleFloat (ty));
    RETURN (ret);
}
