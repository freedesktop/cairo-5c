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

NamespacePtr	CairoNamespace;
Type		*typeCairo;
Type		*typeCairoStatus;
Type		*typeCairoOperator;
Type		*typeCairoFillRule;
Type		*typeCairoLineCap;
Type		*typeCairoLineJoin;
Type		*typeCairoFontSlant;
Type		*typeCairoFontWeight;
Type		*typeCairoTextExtents;
Type		*typeCairoMatrix;
Type		*typeCairoPoint;
Type		*typeCairoRect;
Type		*typeCairoRgbColor;

#define CAIRO_I		0
#define CAIRO_S		"00"
#define STATUS_I	1
#define STATUS_S	"01"
#define OPERATOR_I	2
#define OPERATOR_S	"02"
#define FILL_RULE_I	3
#define FILL_RULE_S	"03"
#define LINE_CAP_I	4
#define LINE_CAP_S	"04"
#define LINE_JOIN_I	5
#define LINE_JOIN_S	"05"
#define FONT_SLANT_I	6
#define FONT_SLANT_S	"06"
#define FONT_WEIGHT_I	7
#define FONT_WEIGHT_S	"07"
#define	TEXT_EXTENTS_I	8
#define TEXT_EXTENTS_S	"08"
#define MATRIX_I	9
#define MATRIX_S	"09"
#define POINT_I		10
#define POINT_S		"10"
#define RECT_I		11
#define RECT_S		"11"
#define RGB_COLOR_I	12
#define RGB_COLOR_S	"12"

static Type *
make_typedef (char  *name_str,
	      int   usertype_id,
	      Type  *type)
{
    ENTER ();
    Atom    name = AtomId (name_str);
    Symbol  *sym = NewSymbolType (name, type);
    Type    *typed = NewTypeName (NewExprAtom (name, 0, False),
				  sym);
    
    NamespaceAddName (CairoNamespace, sym, publish_public);
    
    BuiltinSetUserdefType (typed, usertype_id);
    MemAddRoot (typed);
    RETURN (typed);
}

static void
init_types (void)
{
    ENTER ();

    CairoNamespace = BuiltinNamespace (/* parent */ 0, "Cairo")->namespace.namespace;

    typeCairo = make_typedef ("cairo_t",
			      CAIRO_I,
			      typePrim[rep_foreign]);

    typeCairoStatus = make_typedef ("status_t",
				    STATUS_I,
				    BuildEnumType (8,
						   "SUCCESS",
						   "NO_MEMORY",
						   "INVALID_RESTORE",
						   "INVALID_POP_GROUP",
						   "NO_CURRENT_POINT",
						   "INVALID_MATRIX",
						   "NO_TARGET_SURFACE",
						   "NULL_POINTER"));
    typeCairoOperator = make_typedef ("operator_t",
				      OPERATOR_I,
				      BuildEnumType (14,
						     "CLEAR",
						     "SRC",
						     "DST",
						     "OVER",
						     "OVER_REVERSE",
						     "IN",
						     "IN_REVERSE",
						     "OUT",
						     "OUT_REVERSE",
						     "ATOP",
						     "ATOP_REVERSE",
						     "XOR",
						     "ADD",
						     "SATURATE"));

    typeCairoFillRule = make_typedef ("fill_rule_t",
				      FILL_RULE_I,
				      BuildEnumType (2,
						     "WINDING",
						     "EVEN_ODD"));

    typeCairoLineCap = make_typedef ("line_cap_t",
				     LINE_CAP_I,
				     BuildEnumType (3,
						    "BUTT",
						    "ROUND",
						    "SQUARE"));

    typeCairoLineJoin = make_typedef ("line_join_t",
				      LINE_JOIN_I,
				      BuildEnumType (3,
						     "MITER",
						     "ROUND",
						     "BEVEL"));

    typeCairoFontSlant = make_typedef ("font_slant_t",
				       FONT_SLANT_I,
				       BuildEnumType (3,
						      "NORMAL",
						      "ITALIC",
						      "OBLIQUE"));
    typeCairoFontWeight = make_typedef ("font_weight_t",
					FONT_WEIGHT_I,
					BuildEnumType (2,
						       "NORMAL",
						       "BOLD"));

    typeCairoTextExtents = make_typedef ("text_extents_t",
					 TEXT_EXTENTS_I,
					 BuildStructType (6, 
							  typePrim[rep_float], "x_bearing",
							  typePrim[rep_float], "y_bearing",
							  typePrim[rep_float], "width",
							  typePrim[rep_float], "height",
							  typePrim[rep_float], "x_advance",
							  typePrim[rep_float], "y_advance"));
    typeCairoMatrix = make_typedef ("matrix_t",
				    MATRIX_I,
				    BuildArrayType (typePrim[rep_float],
						    2, 2, 3));

    typeCairoPoint = make_typedef ("point_t",
				   POINT_I,
				   BuildStructType (2,
						    typePrim[rep_float], "x",
						    typePrim[rep_float], "y"));

    typeCairoRect = make_typedef ("rect_t",
				  RECT_I,
				  BuildStructType (4,
						   typePrim[rep_float], "x",
						   typePrim[rep_float], "y",
						   typePrim[rep_float], "width",
						   typePrim[rep_float], "height"));

    typeCairoRgbColor = make_typedef ("rgb_color_t",
				      RGB_COLOR_I,
				      BuildStructType (3,
						       typePrim[rep_float], "red",
						       typePrim[rep_float], "green",
						       typePrim[rep_float], "blue"));
    
    EXIT();
}

int
EnumIntPart (Value ev, char *err)
{
    Union	*u = &ev->unions;
    StructType	*st = u->type;
    int		i;

    for (i = 0; i < st->nelements; i++)
	if (StructTypeAtoms(st)[i] == u->tag)
	    return i;
    RaiseStandardException (exception_invalid_argument,
			    err,
			    2, ev);
    return -1;
}

Value
IntToEnum (Type *type, int i)
{
    ENTER ();
    Type	*ctype = TypeCanon (type);
    StructType	*st = ctype->structs.structs;
    Value	v = NewUnion (st, False);

    if (i < 0 || st->nelements <= i)
    {
	RaiseStandardException (exception_invalid_argument,
				"invalid enum index",
				2, NewInt (i), v);
	RETURN (Void);
    }
    
    v->unions.tag = StructTypeAtoms(st)[i];
    BoxValueSet (v->unions.value, 0, Void);
    RETURN (v);
}

Value
nickle_init (void)
{
    ENTER ();
    static const struct fbuiltin_v funcs_v[] = {
	{ do_Cairo_new, "new", CAIRO_S, ".i", "\n"
	    " cairo_t new (int...)\n"
	    "\n"
	    " Create a cairo window. Optional arguments are width, height\n" },
	{ 0 }
    };
	
    static const struct fbuiltin_1 funcs_1[] = {
	{ do_Cairo_width, "width", "i", CAIRO_S, "\n"
	    " void width (cairo_t cairo)\n"
	    "\n"
	    " Return the width of a cairo surface\n" },
	{ do_Cairo_height, "height", "i", CAIRO_S, "\n"
	    " void height (cairo_t cairo)\n"
	    "\n"
	    " Return the height of a cairo surface\n" },
	{ do_Cairo_status, "status", STATUS_S, CAIRO_S, "\n"
	    " void status (cairo_t cairo)\n"
	    "\n"
	    " Return the status of a cairo surface\n" },
	{ do_Cairo_status_string, "status_string", "s", CAIRO_S, "\n"
	    " void status_string (cairo_t cairo)\n"
	    "\n"
	    " Return the status string of a cairo surface\n" },
	{ do_Cairo_dispose, "dispose", "v", CAIRO_S, "\n"
	    " void dispose (cairo_t cairo)\n"
	    "\n"
	    " Dispose a cairo surface\n" },
	{ do_Cairo_enable, "enable", "v", CAIRO_S, "\n"
	    " void enable (cairo_t cairo)\n"
	    "\n"
	    " Enable screen updates from a cairo surface\n" },
	{ do_Cairo_disable, "disable", "v", CAIRO_S, "\n"
	    " void disable (cairo_t cairo)\n"
	    "\n"
	    " Disable screen updates from a cairo surface\n" },
	{ do_Cairo_save, "save", "v", CAIRO_S, "\n"
	    " void save (cairo_t cairo)\n"
	    "\n"
	    " Save graphics state\n" },
	{ do_Cairo_restore, "restore", "v", CAIRO_S, "\n"
	    " void restore (cairo_t cairo)\n"
	    "\n"
	    " Restore graphics state\n" },
	{ do_Cairo_identity_matrix, "identity_matrix", "v", CAIRO_S, "\n"
	    " void identity_matrix (cairo_t cairo)\n"
	    "\n"
	    " Set transformation matrix to identity\n" },
	{ do_Cairo_default_matrix, "default_matrix", "v", CAIRO_S, "\n"
	    " void default_matrix (cairo_t cairo)\n"
	    "\n"
	    " Set transformation matrix to default\n" },
	{ do_Cairo_new_path, "new_path", "v", CAIRO_S, "\n"
	    " void new_path (cairo_t cairo)\n"
	    "\n"
	    " Starts a new path\n" },
	{ do_Cairo_close_path, "close_path", "v", CAIRO_S, "\n"
	    " void close_path (cairo_t cairo)\n"
	    "\n"
	    " Closes the current path\n" },
	{ do_Cairo_fill, "fill", "v", CAIRO_S, "\n"
	    " void fill (cairo_t cairo)\n"
	    "\n"
	    " Fill the current path\n" },
	
	{ do_Cairo_stroke, "stroke", "v", CAIRO_S, "\n"
	    " void stroke (cairo_t cairo)\n"
	    "\n"
	    " Stroke the current path\n" },
	
	{ do_Cairo_init_clip, "init_clip", "v", CAIRO_S, "\n"
	    " void init_clip (cairo_t cairo)\n"
	    "\n"
	    " Resets clip list to whole surface\n" },
	
	{ do_Cairo_clip, "clip", "v", CAIRO_S, "\n"
	    " void clip (cairo_t cairo)\n"
	    "\n"
	    " Clip to current path.  Does not empty path.\n" },

	{ do_Cairo_stroke_extents, "stroke_extents", RECT_S, CAIRO_S, "\n"
	    " rect_t stroke_extents (cairo_t cairo)\n"
	    "\n"
	    " Returns bounding box of stroking current path\n" },
	
	{ do_Cairo_fill_extents, "fill_extents", RECT_S, CAIRO_S, "\n"
	    " rect_t fill_extents (cairo_t cairo)\n"
	    "\n"
	    " Returns bounding box of filling current path\n" },
	
	{ do_Cairo_current_operator, "current_operator", OPERATOR_S, CAIRO_S, "\n"
	    " operator_t current_operator (cairo_t cairo)\n"
	    "\n"
	    " Returns the current operator\n" },
	
	{ do_Cairo_current_rgb_color, "current_rgb_color", RGB_COLOR_S, CAIRO_S, "\n"
	    " rgb_color_t current_rgb_color (cairo_t cairo)\n"
	    "\n"
	    " Returns the current rgb color\n" },
	
	{ do_Cairo_current_alpha, "current_alpha", "n", CAIRO_S, "\n"
	    " real current_alpha (cairo_t cairo)\n"
	    "\n"
	    " Returns the current alpha\n" },
	
	{ do_Cairo_current_tolerance, "current_tolerance", "n", CAIRO_S, "\n"
	    " real current_tolerance (cairo_t cairo)\n"
	    "\n"
	    " Returns the current tolerance\n" },
	
	{ do_Cairo_current_point, "current_point", RGB_COLOR_S, CAIRO_S, "\n"
	    " point_t current_point (cairo_t cairo)\n"
	    "\n"
	    " Returns the current point\n" },
	
	{ do_Cairo_current_fill_rule, "current_fill_rule", FILL_RULE_S, CAIRO_S, "\n"
	    " fill_rule_t current_fill_rule (cairo_t cairo)\n"
	    "\n"
	    " Returns the current fill rule\n" },
	
	{ do_Cairo_current_line_width, "current_line_width", "n", CAIRO_S, "\n"
	    " real current_line_width (cairo_t cairo)\n"
	    "\n"
	    " Returns the current line width\n" },
	
	{ do_Cairo_current_line_cap, "current_line_cap", LINE_CAP_S, CAIRO_S, "\n"
	    " line_cap_t current_line_cap (cairo_t cairo)\n"
	    "\n"
	    " Returns the current line_cap\n" },
	
	{ do_Cairo_current_line_join, "current_line_join", LINE_JOIN_S, CAIRO_S, "\n"
	    " line_join_t current_line_join (cairo_t cairo)\n"
	    "\n"
	    " Returns the current line join\n" },
	
	{ do_Cairo_current_miter_limit, "current_miter_limit", "n", CAIRO_S, "\n"
	    " real current_miter_limit (cairo_t cairo)\n"
	    "\n"
	    " Returns the current miter limit\n" },
	
	{ do_Cairo_current_matrix, "current_matrix", MATRIX_S, CAIRO_S, "\n"
	    " matrix_t current_matrix (cairo_t cairo)\n"
	    "\n"
	    " Returns the current transformation matrix\n" },
	
	{ do_Cairo_open_event, "open_event", "f", CAIRO_S, "\n"
	    " file open_event (cairo_t cairo)\n"
	    "\n"
	    " Returns a file which will receive events\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_2 funcs_2[] = {
	{ do_Cairo_set_operator, "set_alpha", "v", CAIRO_S OPERATOR_S, "\n"
	    " void set_operator (cairo_t cr, operator_t operator)\n"
	    "\n"
	    " Set current operator\n" },
	{ do_Cairo_set_alpha, "set_alpha", "v", CAIRO_S "n", "\n"
	    " void set_alpha (cairo_t cr, real alpha)\n"
	    "\n"
	    " Set current alpha\n" },
	{ do_Cairo_set_tolerance, "set_tolerance", "v", CAIRO_S "n", "\n"
	    " void set_tolerance (cairo_t cr, real alpha)\n"
	    "\n"
	    " Set current tolerance\n" },
	{ do_Cairo_set_fill_rule, "set_fill_rule", "v", CAIRO_S FILL_RULE_S, "\n"
	    " void set_fill_rule (cairo_t cr, fill_rule_t rule)\n"
	    "\n"
	    " Set current fill rule\n" },
	{ do_Cairo_set_line_width, "set_line_width", "v", CAIRO_S "n", "\n"
	    " void set_line_width (cairo_t cr, real alpha)\n"
	    "\n"
	    " Set current line width\n" },
	{ do_Cairo_set_line_cap, "set_line_cap", "v", CAIRO_S LINE_CAP_S, "\n"
	    " void set_line_cap (cairo_t cr, line_cap_t cap)\n"
	    "\n"
	    " Set current line cap style\n" },
	{ do_Cairo_set_line_join, "set_line_join", "v", CAIRO_S LINE_JOIN_S, "\n"
	    " void set_line_join (cairo_t cr, line_join_t join)\n"
	    "\n"
	    " Set current line join style\n" },
	{ do_Cairo_set_miter_limit, "set_miter_limit", "v", CAIRO_S "n", "\n"
	    " void set_miter_limit (cairo_t cr, real limit)\n"
	    "\n"
	    " Set current miter join limit\n" },
	{ do_Cairo_rotate, "rotate", "v", CAIRO_S "n", "\n"
	    " void rotate (cairo_t cr, real radians)\n"
	    "\n"
	    " Rotate current transformation matrix by specified amount\n" },
	{ do_Cairo_scale_font, "scale_font", "v", CAIRO_S "n", "\n"
	    " void scale_font (cairo_t cr, real scale)\n"
	    "\n"
	    " Scales current font by specified amount\n" },
	{ do_Cairo_show_text, "show_text", "v", CAIRO_S "s", "\n"
	    " void show_text (cairo_t cr, string text)\n"
	    "\n"
	    " Shows text at current position\n" },
	{ do_Cairo_text_path, "text_path", "v", CAIRO_S "s", "\n"
	    " void text_path (cairo_t cr, string text)\n"
	    "\n"
	    " Appends text to current path\n" },
	{ do_Cairo_text_extents, "text_extents", TEXT_EXTENTS_S, CAIRO_S "s", "\n"
	    " text_extents_t text_extents (cairo_t cr, string text)\n"
	    "\n"
	    " Appends text to current path\n" },
	{ do_Cairo_set_matrix, "set_matrix", "v", CAIRO_S "A**n", "\n"
	    " void set_matrix (cairo_t cr, matrix_t matrix)\n"
	    "\n"
	    " Sets the transformation matrix\n" },
	{ do_Cairo_transform_point, "transform_point", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t transform_point (cairo_t cr, point_t)\n"
	    "\n"
	    " Transform a point from user to device space\n" },
	{ do_Cairo_transform_distance, "transform_distance", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t transform_distance (cairo_t cr, point_t point)\n"
	    "\n"
	    " Transform a distance from user to device space\n" },
	{ do_Cairo_inverse_transform_point, "inverse_transform_point", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t inverse_transform_point (cairo_t cr, point_t point)\n"
	    "\n"
	    " Transform a point from device to user space\n" },
	{ do_Cairo_inverse_transform_distance, "inverse_transform_distance", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t inverse_transform_distance (cairo_t cr, point_t point)\n"
	    "\n"
	    " Transform a distance from device to user space\n" },
	{ 0 }
    };

    static const struct fbuiltin_3 funcs_3[] = {
	{ do_Cairo_new_png, "new_png", CAIRO_S, "sii", "\n"
	    " cairo_t new_png (string filename, int width, int height)\n"
	    "\n"
	    " Create a cairo png file.\n" },
	{ do_Cairo_move_to, "move_to", "v", CAIRO_S "nn", "\n"
	    " void move_to (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Move to specified position\n" },
	{ do_Cairo_line_to, "line_to", "v", CAIRO_S "nn", "\n"
	    " void line_to (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Draw line to specified position\n" },
	{ do_Cairo_rel_move_to, "rel_move_to", "v", CAIRO_S "nn", "\n"
	    " void rel_move_to (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Move to specified offset from the current position\n" },
	{ do_Cairo_rel_line_to, "rel_line_to", "v", CAIRO_S "nn", "\n"
	    " void rel_line_to (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Draw line to specified offset from the current position\n" },
	{ do_Cairo_translate, "translate", "v", CAIRO_S "nn", "\n"
	    " void translate (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Offset current transformation matrix by specified amount\n" },
	{ do_Cairo_scale, "scale", "v", CAIRO_S "nn", "\n"
	    " void scale (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Scale current transformation matrix by specified amount\n" },
	{ do_Cairo_set_dash, "set_dash", "v", CAIRO_S "A*nn", "\n"
	    " void set_dash (cairo_t cr, real[*] dashes, real dash_offset)\n"
	    "\n"
	    " Sets current dash pattern and offset\n" },
	{ do_Cairo_in_fill, "in_fill", "b", CAIRO_S "nn", "\n"
	    " bool in_fill (cairo_t cairo, real x, real y)\n"
	    "\n"
	    " Test whether (x,y) would be covered by filling the current path\n" },
	{ do_Cairo_in_stroke, "in_stroke", "b", CAIRO_S "nn", "\n"
	    " bool in_stroke (cairo_t cairo, real x, real y)\n"
	    "\n"
	    " Test whether (x,y) would be covered by stroking the current path\n" },
	{ 0 }
    };

    static const struct fbuiltin_4 funcs_4[] = {
	{ do_Cairo_set_rgb_color, "set_rgb_color", "v", CAIRO_S "nnn", "\n"
	    " void set_rgb_color (cairo_t cr, real red, real green, real blue)\n"
	    "\n"
	    " Set current color\n" },
	{ do_Cairo_select_font, "select_font", "v", CAIRO_S "s" FONT_SLANT_S FONT_WEIGHT_S, "\n"
	    " void select_font (cairo_t cr, string family, slant_t slant, weight_t weight)\n"
	    "\n"
	    " Set current font\n" },
	{ 0 }
    };

    static const struct fbuiltin_5 funcs_5[] = {
	{ do_Cairo_rectangle, "rectangle", "v", CAIRO_S "nnnn", "\n"
	    " void rectangle (cairo_t cr, real x, real y, real width, real height)\n"
	    "\n"
	    " Adds the specified rectangle to the current path\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_6 funcs_6[] = {
	{ do_Cairo_arc, "arc", "v", CAIRO_S "nnnnn", "\n"
	    " void arc (cairo_t cr, real x1, real y1, real x2, real y2, real radius)\n"
	    "\n"
	    " Draw a clockwise arc from x1,y1 to x2,y2 with the given radius\n" },
	{ do_Cairo_arc_negative, "arc_negative", "v", CAIRO_S "nnnnn", "\n"
	    " void arc (cairo_t cr, real x1, real y1, real x2, real y2, real radius)\n"
	    "\n"
	    " Draw a counter-clockwise arc from x1,y1 to x2,y2 with the given radius\n" },
	{ 0 }
    };

    static const struct fbuiltin_7 funcs_7[] = {
	{ do_Cairo_curve_to, "curve_to", "v", CAIRO_S "nnnnnn", "\n"
	    " void curve_to (cairo_t cr, real x1, real y1, real x2, real y2, real x3, real y3)\n"
	    "\n"
	    " Draw Bézier spline to specified position\n" },
	{ do_Cairo_rel_curve_to, "rel_curve_to", "v", CAIRO_S "nnnnnn", "\n"
	    " void curve_to (cairo_t cr, real x1, real y1, real x2, real y2, real x3, real y3)\n"
	    "\n"
	    " Draw Bézier spline to specified offset from the current position\n" },
	{ 0 }
    };

    init_types ();
    
    BuiltinFuncsV (&CairoNamespace, funcs_v);
    BuiltinFuncs1 (&CairoNamespace, funcs_1);
    BuiltinFuncs2 (&CairoNamespace, funcs_2);
    BuiltinFuncs3 (&CairoNamespace, funcs_3);
    BuiltinFuncs4 (&CairoNamespace, funcs_4);
    BuiltinFuncs5 (&CairoNamespace, funcs_5);
    BuiltinFuncs6 (&CairoNamespace, funcs_6);
    BuiltinFuncs7 (&CairoNamespace, funcs_7);

    RETURN(TrueVal);
}
