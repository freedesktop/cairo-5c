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
NamespacePtr	CairoSurfaceNamespace;
NamespacePtr	CairoPatternNamespace;
Type		*typeCairo;
Type		*typeCairoSurface;
Type		*typeCairoStatus;
Type		*typeCairoOperator;
Type		*typeCairoFillRule;
Type		*typeCairoLineCap;
Type		*typeCairoLineJoin;
Type		*typeCairoFontSlant;
Type		*typeCairoFontWeight;
Type		*typeCairoTextExtents;
Type		*typeCairoFontExtents;
Type		*typeCairoMatrix;
Type		*typeCairoPoint;
Type		*typeCairoRect;
Type		*typeCairoRgbColor;
Type		*typeCairoPattern;
Type		*typeCairoPath;
Type		*typeCairoMoveTo;
Type		*typeCairoLineTo;
Type		*typeCairoCurveTo;
Type		*typeCairoClosePath;

Type		*typeCairoPatternExtend;
Type		*typeCairoPatternFilter;

#define CAIRO_I		0
#define CAIRO_S		"00"
#define SURFACE_I	1
#define SURFACE_S	"01"
#define STATUS_I	2
#define STATUS_S	"02"
#define OPERATOR_I	3
#define OPERATOR_S	"03"
#define FILL_RULE_I	4
#define FILL_RULE_S	"04"
#define LINE_CAP_I	5
#define LINE_CAP_S	"05"
#define LINE_JOIN_I	6
#define LINE_JOIN_S	"06"
#define FONT_SLANT_I	7
#define FONT_SLANT_S	"07"
#define FONT_WEIGHT_I	8
#define FONT_WEIGHT_S	"08"
#define	TEXT_EXTENTS_I	9
#define TEXT_EXTENTS_S	"09"
#define	FONT_EXTENTS_I	10
#define FONT_EXTENTS_S	"10"
#define MATRIX_I	11
#define MATRIX_S	"11"
#define POINT_I		12
#define POINT_S		"12"
#define RECT_I		13
#define RECT_S		"13"
#define RGB_COLOR_I	14
#define RGB_COLOR_S	"14"
#define PATTERN_I	15
#define PATTERN_S	"15"
#define PATH_I		16
#define PATH_S		"16"
#define MOVE_TO_I	17
#define MOVE_TO_S	"17"
#define LINE_TO_I	18
#define LINE_TO_S	"18"
#define CURVE_TO_I	19
#define CURVE_TO_S	"19"
#define CLOSE_PATH_I	20
#define CLOSE_PATH_S	"20"

#define EXTEND_I	30
#define EXTEND_S	"30"
#define FILTER_I	31
#define FILTER_S	"31"

static Type *
make_typedef (char	*name_str,
	      Namespace	*namespace,
	      Publish	publish,
	      int	usertype_id,
	      Symbol	**sret,
	      Type	*type)
{
    ENTER ();
    Atom    name = AtomId (name_str);
    Symbol  *sym = NewSymbolType (name, type);
    Type    *typed = NewTypeName (NewExprAtom (name, 0, False),
				  sym);
    
    NamespaceAddName (namespace, sym, publish);
    
    BuiltinSetUserdefType (typed, usertype_id);
    MemAddRoot (typed);
    if (sret)
	*sret = sym;
    RETURN (typed);
}

static void
init_types (void)
{
    Symbol  *mt, *lt, *ct, *cp, *path;
    ENTER ();

    CairoNamespace = BuiltinNamespace (/* parent */ 0, "Cairo")->namespace.namespace;

    typeCairo = make_typedef ("cairo_t",
			      CairoNamespace,
			      publish_public,
			      CAIRO_I,
			      NULL,
			      typePrim[rep_foreign]);

    typeCairoSurface = make_typedef ("surface_t",
				     CairoNamespace,
				     publish_public,
				     SURFACE_I,
				     NULL,
				     typePrim[rep_foreign]);

    typeCairoStatus = make_typedef ("status_t",
				    CairoNamespace,
				    publish_public,
				    STATUS_I,
				    NULL,
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
				      CairoNamespace,
				      publish_public,
				      OPERATOR_I,
				      NULL,
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
				      CairoNamespace,
				      publish_public,
				      FILL_RULE_I,
				      NULL,
				      BuildEnumType (2,
						     "WINDING",
						     "EVEN_ODD"));

    typeCairoLineCap = make_typedef ("line_cap_t",
				     CairoNamespace,
				     publish_public,
				     LINE_CAP_I,
				     NULL,
				     BuildEnumType (3,
						    "BUTT",
						    "ROUND",
						    "SQUARE"));

    typeCairoLineJoin = make_typedef ("line_join_t",
				      CairoNamespace,
				      publish_public,
				      LINE_JOIN_I,
				      NULL,
				      BuildEnumType (3,
						     "MITER",
						     "ROUND",
						     "BEVEL"));

    typeCairoFontSlant = make_typedef ("font_slant_t",
				       CairoNamespace,
				       publish_public,
				       FONT_SLANT_I,
				       NULL,
				       BuildEnumType (3,
						      "NORMAL",
						      "ITALIC",
						      "OBLIQUE"));
    typeCairoFontWeight = make_typedef ("font_weight_t",
					CairoNamespace,
					publish_public,
					FONT_WEIGHT_I,
					NULL,
					BuildEnumType (2,
						       "NORMAL",
						       "BOLD"));

    typeCairoTextExtents = make_typedef ("text_extents_t",
					 CairoNamespace,
					 publish_public,
					 TEXT_EXTENTS_I,
					 NULL,
					 BuildStructType (6, 
							  typePrim[rep_float], "x_bearing",
							  typePrim[rep_float], "y_bearing",
							  typePrim[rep_float], "width",
							  typePrim[rep_float], "height",
							  typePrim[rep_float], "x_advance",
							  typePrim[rep_float], "y_advance"));
    typeCairoFontExtents = make_typedef ("font_extents_t",
					 CairoNamespace,
					 publish_public,
					 FONT_EXTENTS_I,
					 NULL,
					 BuildStructType (5, 
							  typePrim[rep_float], "ascent",
							  typePrim[rep_float], "descent",
							  typePrim[rep_float], "height",
							  typePrim[rep_float], "max_x_advance",
							  typePrim[rep_float], "max_y_advance"));
    typeCairoMatrix = make_typedef ("matrix_t",
				    CairoNamespace,
				    publish_public,
				    MATRIX_I,
				    NULL,
				    BuildArrayType (typePrim[rep_float],
						    2, 2, 3));

    typeCairoPoint = make_typedef ("point_t",
				   CairoNamespace,
				   publish_public,
				   POINT_I,
				   NULL,
				   BuildStructType (2,
						    typePrim[rep_float], "x",
						    typePrim[rep_float], "y"));

    typeCairoRect = make_typedef ("rect_t",
				  CairoNamespace,
				  publish_public,
				  RECT_I,
				  NULL,
				  BuildStructType (4,
						   typePrim[rep_float], "x",
						   typePrim[rep_float], "y",
						   typePrim[rep_float], "width",
						   typePrim[rep_float], "height"));

    typeCairoRgbColor = make_typedef ("rgb_color_t",
				      CairoNamespace,
				      publish_public,
				      RGB_COLOR_I,
				      NULL,
				      BuildStructType (3,
						       typePrim[rep_float], "red",
						       typePrim[rep_float], "green",
						       typePrim[rep_float], "blue"));

    typeCairoPattern = make_typedef ("pattern_t",
				     CairoNamespace,
				     publish_public,
				     PATTERN_I,
				     NULL,
				     typePrim[rep_foreign]);

    /*
     * Recursive datatypes aren't supported by the simple API
     */

    typeCairoMoveTo = make_typedef ("move_to_t",
				    CairoNamespace,
				    publish_public,
				    MOVE_TO_I,
				    &mt,
				    0);

    typeCairoLineTo = make_typedef ("line_to_t",
				    CairoNamespace,
				    publish_public,
				    LINE_TO_I,
				    &lt,
				    0);

    typeCairoCurveTo = make_typedef ("curve_to_t",
				     CairoNamespace,
				     publish_public,
				     CURVE_TO_I,
				     &ct,
				     0);

    typeCairoClosePath = make_typedef ("close_path_t",
				       CairoNamespace,
				       publish_public,
				       CLOSE_PATH_I,
				       &cp,
				       0);

    typeCairoPath = make_typedef ("path_t",
				  CairoNamespace,
				  publish_public,
				  PATH_I,
				  &path,
				  BuildUnionType(5,
						 NewTypeRef (typeCairoMoveTo, True), "move_to",
						 NewTypeRef (typeCairoLineTo, True), "line_to",
						 NewTypeRef (typeCairoCurveTo, True), "curve_to",
						 NewTypeRef (typeCairoClosePath, True), "close_path",
						 typePrim[rep_void], "end"));

    mt->symbol.type = BuildStructType (3,
				       typeCairoPath, "next",
				       typePrim[rep_float], "x",
				       typePrim[rep_float], "y");

    lt->symbol.type = BuildStructType (3,
				       typeCairoPath, "next",
				       typePrim[rep_float], "x",
				       typePrim[rep_float], "y");

    ct->symbol.type = BuildStructType (7,
				       typeCairoPath, "next",
				       typePrim[rep_float], "x1",
				       typePrim[rep_float], "y1",
				       typePrim[rep_float], "x2",
				       typePrim[rep_float], "y2",
				       typePrim[rep_float], "x3",
				       typePrim[rep_float], "y3");

    cp->symbol.type = BuildStructType (1,
				       typeCairoPath, "next");

    

    CairoSurfaceNamespace = BuiltinNamespace (&CairoNamespace, "Surface")->namespace.namespace;


    CairoPatternNamespace = BuiltinNamespace (&CairoNamespace, "Pattern")->namespace.namespace;

    typeCairoPatternExtend = make_typedef ("extend_t",
					   CairoPatternNamespace,
					   publish_public,
					   EXTEND_I,
					   NULL,
					   BuildEnumType (3,
							  "NONE",
							  "REPEAT",
							  "REFLECT"));

    typeCairoPatternFilter = make_typedef ("filter_t",
					   CairoPatternNamespace,
					   publish_public,
					   FILTER_I,
					   NULL,
					   BuildEnumType (6,
							  "FAST",
							  "GOOD",
							  "BEST",
							  "NEAREST",
							  "BILINEAR",
							  "GAUSSIAN"));

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
	
    static const struct fbuiltin_0 funcs_0[] = {
	{ do_Cairo_create, "create", CAIRO_S, "", "\n"
	    " cairo_t create ()\n"
	    "\n"
	    " Create a cairo context.\n"
	},
	{ 0 },
    };
    
    static const struct fbuiltin_1 funcs_1[] = {
	{ do_Cairo_destroy, "destroy", "v", CAIRO_S, "\n"
	    " void destroy (cairo_t cairo)\n"
	    "\n"
	    " destroy a rendering context.\n"},
	{ do_Cairo_current_target_surface, "current_target_surface", SURFACE_S, CAIRO_S, "\n"
	    " surface_t current_target_surface (cairo_t cairo)\n"
	    "\n"
	    " Return current target surface\n" },
	{ do_Cairo_status, "status", STATUS_S, CAIRO_S, "\n"
	    " status_t status (cairo_t cairo)\n"
	    "\n"
	    " Return the status of a cairo surface\n" },
	{ do_Cairo_status_string, "status_string", "s", CAIRO_S, "\n"
	    " string status_string (cairo_t cairo)\n"
	    "\n"
	    " Return the status string of a cairo surface\n" },
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
	
	{ do_Cairo_copy_page, "copy_page", "v", CAIRO_S, "\n"
	    " void copy_page (cairo_t cairo)\n"
	    "\n"
	    " Write out the current page, leaving it intact.\n" },
	
	{ do_Cairo_show_page, "show_page", "v", CAIRO_S, "\n"
	    " void show_page (cairo_t cairo)\n"
	    "\n"
	    " Write out the current page, and erase it.\n" },
	
	{ do_Cairo_current_path_flat_list, "current_path_flat_list", PATH_S, CAIRO_S, "\n"
	    " void current_path_flat (cairo_t cr)\n"
	    "\n"
	    " Returns the current path with curves tesselated to lines\n" },

	{ do_Cairo_current_path_list, "current_path_list", PATH_S, CAIRO_S, "\n"
	    " void current_path (cairo_t cr)\n"
	    "\n"
	    " Returns the current path\n" },
	
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
	
	{ do_Cairo_current_pattern, "current_pattern", PATTERN_S, CAIRO_S, "\n"
	    " pattern_t current_pattern (cairo_t cairo)\n"
	    "\n"
	    " Returns the current pattern\n" },
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
	
	{ do_Cairo_current_font_extents, "current_font_extents", FONT_EXTENTS_S, CAIRO_S, "\n"
	    " font_extents_t current_font_extents (cairo_t cairo)\n"
	    "\n"
	    " Returns metrics for current font\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_2 funcs_2[] = {
	{ do_Cairo_set_target_surface, "set_target_surface", "v", CAIRO_S SURFACE_S, "\n"
	    " void set_target_surface (cairo_t cr, surface_t surface)\n"
	    "\n"
	    " Set target surface for drawing operations\n" },
	{ do_Cairo_copy, "copy", "v", CAIRO_S CAIRO_S, "\n"
	    " void copy (cairo_t dst, cairo_t src)\n"
	    "\n"
	    " Copy state from one rendering context to another\n" },
	{ do_Cairo_set_operator, "set_operator", "v", CAIRO_S OPERATOR_S, "\n"
	    " void set_operator (cairo_t cr, operator_t operator)\n"
	    "\n"
	    " Set current operator\n" },
	{ do_Cairo_set_pattern, "set_pattern", "v", CAIRO_S PATTERN_S, "\n"
	    " void set_pattern (cairo_t cr, pattern_t pattern)\n"
	    "\n"
	    " Set current pattern\n" },
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
	{ do_Cairo_set_font, "set_font", "v", CAIRO_S "s", "\n"
	    " void set_font (cairo_t cr, string name)\n"
	    "\n"
	    " Select and set current font from name.\n" },
	{ do_Cairo_scale_font, "scale_font", "v", CAIRO_S "n", "\n"
	    " void scale_font (cairo_t cr, real scale)\n"
	    "\n"
	    " Scales current font by specified amount\n" },
	{ do_Cairo_transform_font, "transform_font", "v", CAIRO_S MATRIX_S, "\n"
	    " void transform_font (cairo_t cr, matrix_t matrix)\n"
	    "\n"
	    " Transforms current font by specified matrix\n" },
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
	{ do_Cairo_concat_matrix, "concat_matrix", "v", CAIRO_S MATRIX_S, "\n"
	    " void concat_matrix (cairo_t cr, matrix_t matrix)\n"
	    "\n"
	    " Mixes in another matrix to the current transformation\n" },
	{ do_Cairo_set_matrix, "set_matrix", "v", CAIRO_S MATRIX_S, "\n"
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
#if 0
	{ do_Cairo_new_png, "new_png", CAIRO_S, "sii", "\n"
	    " cairo_t new_png (string filename, int width, int height)\n"
	    "\n"
	    " Create a cairo png file.\n" },
	{ do_Cairo_new_scratch, "new_scratch", CAIRO_S, CAIRO_S "ii", "\n"
	    " cairo_t new_scratch (cairo_t cr, int width, int height)\n"
	    "\n"
	    " Create a scratch surface related to the given surface.\n" },
#endif
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
	    " void arc (cairo_t cr, real xc, real yc, real r, real angle1, real angle2)\n"
	    "\n"
	    " Draw a clockwise arc centered at xc,yc from angle1 to angle2 with the given radius\n" },
	{ do_Cairo_arc_negative, "arc_negative", "v", CAIRO_S "nnnnn", "\n"
	    " void arc (cairo_t cr, real xc, real yc, real r, real angle1, real angle2)\n"
	    "\n"
	    " Draw a counter-clockwise arc centered at xc,yc from angle1 to angle2 with the given radius\n" },
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

    static const struct fbuiltin_1 surfuncs_1[] = {
	{ do_Cairo_Surface_destroy, "destroy", "v", SURFACE_S, "\n"
	    " void destroy (surface_t surface)\n"
	    "\n"
	    " Destroy a surface.  Further usage will raise an exception.\n" },
	{ do_Cairo_Surface_width, "width", "i", SURFACE_S, "\n"
	    " int width (surface_t surface)\n"
	    "\n"
	    " Returns the width of the given surface\n" },
	{ do_Cairo_Surface_height, "height", "i", SURFACE_S, "\n"
	    " int height (surface_t surface)\n"
	    "\n"
	    " Returns the height of the given surface\n" },
	{ do_Cairo_Surface_open_event, "open_event", "f", SURFACE_S, "\n"
	    " file open_event (surface_t surface)\n"
	    "\n"
	    " Returns a file which will receive events\n" },
	{ 0 }
    };

    static const struct fbuiltin_3 surfuncs_3[] = {
	{ do_Cairo_Surface_create_window, "create_window", SURFACE_S, "snn", "\n"
	    " surface_t create_window (real width, real height)\n"
	    "\n"
	    " Create a window and return a surface pointer for it\n" },
	{ do_Cairo_Surface_create_png, "create_png", SURFACE_S, "snn", "\n"
	    " surface_t create_png (string filename, real width, real height)\n"
	    "\n"
	    " Create a png file and return a surface pointer for it\n" },
	{ do_Cairo_Surface_create_similar, "create_similar", SURFACE_S, SURFACE_S "nn", "\n"
	    " surface_t create_similar (surface_t related, real width, real height)\n"
	    "\n"
	    " Create a similar surface related to another surface\n" },
	{ 0 }
    };

    static const struct fbuiltin_5 surfuncs_5[] = {
	{ do_Cairo_Surface_create_ps, "create_ps", SURFACE_S, "snnnn", "\n"
	    " surface_t create_ps (string filename, real width_inches, real height_inches, real xppi, real yppi)\n"
	    "\n"
	    " Create a PostScript file and return a surface pointer for it\n" },
	{ 0 }
    };

    static const struct fbuiltin_1 patfuncs_1[] = {
	{ do_Cairo_Pattern_create_png, "create_png", PATTERN_S, "s", "\n"
	    " pattern_t create_png (string filename)\n"
	    "\n"
	    " Returns a pattern containing the specified png image\n" },
	{ do_Cairo_Pattern_create_for_surface, "create_for_surface", PATTERN_S, CAIRO_S, "\n"
	    " pattern_t create_for_surface (cairo_t cr)\n"
	    "\n"
	    " Returns a pattern referencing the specified surface\n" },
	{ do_Cairo_Pattern_get_matrix, "get_matrix", MATRIX_S, PATTERN_S, "\n"
	    " matrix_t get_matrix (pattern_t pattern)\n"
	    "\n"
	    " Returns current pattern transformation matrix\n" },
	{ do_Cairo_Pattern_get_extend, "get_extend", EXTEND_S, PATTERN_S, "\n"
	    " status_t get_extend (pattern_t pattern, extend_t extend)\n"
	    "\n"
	    " Returns current pattern extend method\n" },
	{ do_Cairo_Pattern_get_filter, "get_filter", FILTER_S, PATTERN_S, "\n"
	    " status_t get_filter (pattern_t pattern, filter_t filter)\n"
	    "\n"
	    " Returns current pattern filter method\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_2 patfuncs_2[] = {
	{ do_Cairo_Pattern_set_matrix, "set_matrix", STATUS_S, PATTERN_S MATRIX_S, "\n"
	    " status_t set_matrix (pattern_t pattern, matrix_t matrix)\n"
	    "\n"
	    " Set a transformation matrix for a pattern\n" },
	{ do_Cairo_Pattern_set_extend, "set_extend", STATUS_S, PATTERN_S EXTEND_S, "\n"
	    " status_t set_extend (pattern_t pattern, extend_t extend)\n"
	    "\n"
	    " Set a extend method for a pattern\n" },
	{ do_Cairo_Pattern_set_filter, "set_filter", STATUS_S, PATTERN_S FILTER_S, "\n"
	    " status_t set_filter (pattern_t pattern, filter_t filter)\n"
	    "\n"
	    " Set a filter method for a pattern\n" },
	{ 0 }
    };
	
    static const struct fbuiltin_4 patfuncs_4[] = {
	{ do_Cairo_Pattern_create_linear, "create_linear", PATTERN_S, "nnnn", "\n"
	    " pattern_t create_linear (real x0, real y0, real x1, real y1)\n"
	    "\n"
	    " Create a linear gradient pattern\n" },
	{ 0 }
    };

    static const struct fbuiltin_6 patfuncs_6[] = {
	{ do_Cairo_Pattern_create_radial, "create_radial", PATTERN_S, "nnnnnn", "\n"
	    " pattern_t create_radial (real cx0, real cy0, real radius0, real cx1, real cy1, real radius1)\n"
	    "\n"
	    " Create a radial gradient pattern\n" },
	{ do_Cairo_Pattern_add_color_stop, "add_color_stop", STATUS_S, PATTERN_S "nnnnn", "\n"
	    " status_t add_color_stop (pattern_t cr, real offset, real red, real green, real blue, real alpha)\n"
	    "\n"
	    " Add a color stop in a gradient pattern.\n" },
	{ 0 }
    };

    init_types ();
    
    BuiltinFuncs0 (&CairoNamespace, funcs_0);
    BuiltinFuncs1 (&CairoNamespace, funcs_1);
    BuiltinFuncs2 (&CairoNamespace, funcs_2);
    BuiltinFuncs3 (&CairoNamespace, funcs_3);
    BuiltinFuncs4 (&CairoNamespace, funcs_4);
    BuiltinFuncs5 (&CairoNamespace, funcs_5);
    BuiltinFuncs6 (&CairoNamespace, funcs_6);
    BuiltinFuncs7 (&CairoNamespace, funcs_7);

    BuiltinFuncs1 (&CairoSurfaceNamespace, surfuncs_1);
    BuiltinFuncs3 (&CairoSurfaceNamespace, surfuncs_3);
    BuiltinFuncs5 (&CairoSurfaceNamespace, surfuncs_5);
    
    BuiltinFuncs1 (&CairoPatternNamespace, patfuncs_1);
    BuiltinFuncs2 (&CairoPatternNamespace, patfuncs_2);
    BuiltinFuncs4 (&CairoPatternNamespace, patfuncs_4);
    BuiltinFuncs6 (&CairoPatternNamespace, patfuncs_6);

    RETURN(TrueVal);
}
