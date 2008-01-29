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
do_Cairo_select_font_face (Value cv, Value fv, Value sv, Value wv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    char		*family = StrzPart (fv, "invalid family");
    cairo_font_slant_t	slant = EnumIntPart (sv, "invalid slant");
    cairo_font_weight_t	weight = EnumIntPart (wv, "invalid weight");

    if (!aborting)
	cairo_select_font_face (c5c->cr, family, slant, weight);
    return Void;
}

typedef struct {
    DataType		*data;
    Value		name;
    cairo_font_face_t	*font_face;
    double		size;
} cairo_5c_font_t;

static void
cairo_5c_font_mark (void *v)
{
    cairo_5c_font_t *c5f = v;
    
    MemReference (c5f->name);
}

static int
cairo_5c_font_free (void *v)
{
    cairo_5c_font_t *c5f = v;

    if (c5f->font_face) {
	cairo_font_face_destroy (c5f->font_face);
	c5f->font_face = NULL;
    }
    return 1;
}

static DataType Cairo5cFontType = {
    cairo_5c_font_mark,
    cairo_5c_font_free,
    "Cairo5cFont"
};

#define CAIRO_5C_FONT_CACHE	31
#define CAIRO_5C_FONT_REHASH	29

typedef struct {
    DataType	    *data;
    cairo_5c_font_t *cache[CAIRO_5C_FONT_CACHE];
} cairo_5c_font_cache_t;

static void
cairo_5c_font_cache_mark (void *v)
{
    cairo_5c_font_cache_t   *c5fc = v;
    int			    i;

    for (i = 0; i < CAIRO_5C_FONT_CACHE; i++)
	MemReference (c5fc->cache[i]);
}

static DataType Cairo5cFontCacheType = {
    cairo_5c_font_cache_mark,
    NULL,
    "Cairo5cFontCache"
};

static cairo_5c_font_t *
cairo_5c_font_create (Value name)
{
    ENTER ();
    char		*string_name = StrzPart (name, "invalid name");
    FcPattern		*pat, *match;
    cairo_font_face_t	*font_face;
    double		size = 0;
    FcResult		result;
    cairo_5c_font_t	*c5f;

    if (aborting)
	RETURN (NULL);
    
    pat = FcNameParse ((FcChar8 *) string_name);

    FcConfigSubstitute (0, pat, FcMatchPattern);
    FcDefaultSubstitute (pat);

    match = FcFontMatch (0, pat, &result);
    FcPatternDestroy (pat);
    if (!match)
    {
	RaiseStandardException (exception_open_error,
				"can't open font",
				2, FileGetError (ENOENT), name);
	RETURN (NULL);
    }

    FcPatternGetDouble (match, FC_SIZE, 0, &size);
    font_face = cairo_ft_font_face_create_for_pattern (match);
    
    FcPatternDestroy (match);
    if (!font_face)
    {
	RaiseStandardException (exception_open_error,
				"can't open font",
				2, FileGetError (ENOENT), name);
	RETURN (NULL);
    }
    c5f = ALLOCATE (&Cairo5cFontType, sizeof (cairo_5c_font_t));
    c5f->name = name;
    c5f->font_face = font_face;
    c5f->size = size;
    RETURN (c5f);
}

static cairo_5c_font_cache_t	*font_cache;

static int font_matches;
static int font_misses;

static cairo_5c_font_t *
cairo_5c_font_find (Value name)
{
    ENTER ();
    int		    hash = ValueInt (ValueHash (name));
    cairo_5c_font_t *c5f;
    int		    i;
    int		    count;

    if (!font_cache)
    {
	font_cache = ALLOCATE (&Cairo5cFontCacheType, sizeof (cairo_5c_font_cache_t));
	for (i = 0; i < CAIRO_5C_FONT_CACHE; i++)
	    font_cache->cache[i] = NULL;
	MemAddRoot (font_cache);
    }
    for (i = hash % CAIRO_5C_FONT_CACHE, count = 0;
	 (c5f = font_cache->cache[i]) && count < CAIRO_5C_FONT_CACHE;
	 i = (i + (hash % CAIRO_5C_FONT_REHASH + 1)) % CAIRO_5C_FONT_CACHE,
	 count++)
    {
	if (Equal (c5f->name, name) == TrueVal) {
	    ++font_matches;
	    RETURN(c5f);
	}
    }
    ++font_misses;
    c5f = cairo_5c_font_create (name);
    if (c5f)
	font_cache->cache[hash % CAIRO_5C_FONT_CACHE] = c5f;
    RETURN (c5f);
}

Value
do_Cairo_set_font (Value cv, Value fv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_5c_font_t	*c5f = cairo_5c_font_find (fv);

    if (!c5f)
	RETURN (Void);

    cairo_set_font_face (c5c->cr, c5f->font_face);
    cairo_set_font_size (c5c->cr, c5f->size);
    RETURN(Void);
}

Value
do_Cairo_set_font_size (Value cv, Value sv)
{
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    double		size = DoublePart (sv, "invalid size");
    
    if (!aborting)
	cairo_set_font_size (c5c->cr, size);
    return Void;
}

Value
do_Cairo_set_font_matrix (Value cv, Value mv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_matrix_t	matrix;
    
    if (aborting)
	RETURN(Void);
    cairo_matrix_part (mv, &matrix, "invalid matrix");
    if (aborting)
	RETURN(Void);
    cairo_set_font_matrix (c5c->cr, &matrix);
    RETURN(Void);
}

Value
do_Cairo_get_font_matrix (Value cv)
{
    ENTER ();
    cairo_5c_t		*c5c = cairo_5c_get (cv);
    cairo_matrix_t	matrix;
    Value		ret;
    
    if (aborting)
	RETURN(Void);
    cairo_get_font_matrix (c5c->cr, &matrix);
    ret = new_cairo_matrix (&matrix);
    RETURN(ret);
}

Value
do_Cairo_font_extents (Value cv)
{
    ENTER ();
    cairo_5c_t		    *c5c = cairo_5c_get (cv);
    cairo_font_extents_t    extents;
    Value		    ret;
    BoxPtr		    box;

    if (aborting)
	return Void;
    memset (&extents, '\0', sizeof (extents));
    cairo_font_extents (c5c->cr, &extents);
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
    memset (&extents, '\0', sizeof (extents));
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
