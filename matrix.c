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

void
cairo_matrix_part (Value mv, cairo_matrix_t *matrix, char *err)
{
    matrix->xx = DoublePart (StructMemValue (mv, AtomId("xx")), "invalid xx");
    matrix->yx = DoublePart (StructMemValue (mv, AtomId("yx")), "invalid yx");
    matrix->xy = DoublePart (StructMemValue (mv, AtomId("xy")), "invalid xy");
    matrix->yy = DoublePart (StructMemValue (mv, AtomId("yy")), "invalid yy");
    matrix->x0 = DoublePart (StructMemValue (mv, AtomId("x0")), "invalid x0");
    matrix->y0 = DoublePart (StructMemValue (mv, AtomId("y0")), "invalid y0");
}

Value
new_cairo_matrix (cairo_matrix_t *matrix)
{
    ENTER ();
    Value   ret;
    BoxPtr  box;

    ret = NewStruct (TypeCanon (typeCairoMatrix)->structs.structs, False);
    box = ret->structs.values;
    BoxValueSet (box, 0, NewDoubleFloat (matrix->xx));
    BoxValueSet (box, 1, NewDoubleFloat (matrix->yx));
    BoxValueSet (box, 2, NewDoubleFloat (matrix->xy));
    BoxValueSet (box, 3, NewDoubleFloat (matrix->yy));
    BoxValueSet (box, 4, NewDoubleFloat (matrix->x0));
    BoxValueSet (box, 5, NewDoubleFloat (matrix->y0));
    RETURN (ret);
}
