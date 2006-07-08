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
NamespacePtr	CairoImageNamespace;
NamespacePtr	CairoPdfNamespace;
NamespacePtr	CairoPsNamespace;
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
Type		*typeCairoRgbaColor;
Type		*typeCairoPattern;
Type		*typeCairoPath;
Type		*typeCairoCurveTo;
Type		*typeCairoScaledFont;
Type		*typeCairoFontFace;

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
#define RGBA_COLOR_I	15
#define RGBA_COLOR_S	"15"
#define PATTERN_I	16
#define PATTERN_S	"16"
#define PATH_I		17
#define PATH_S		"17"
#define CURVE_TO_I	20
#define CURVE_TO_S	"20"
#define SCALED_FONT_I	22
#define SCALED_FONT_S	"22"
#define FONT_FACE_I	23
#define FONT_FACE_S	"23"

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
    Symbol  *ct, *path;
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
				    BuildEnumType (14,
						   "SUCCESS",
						   "NO_MEMORY",
						   "INVALID_RESTORE",
						   "INVALID_POP_GROUP",
						   "NO_CURRENT_POINT",
						   "INVALID_MATRIX",
						   "NO_TARGET_SURFACE",
						   "NULL_POINTER",
						   "INVALID_STRING",
						   "INVALID_PATH_DATA",
						   "READ_ERROR",
						   "WRITE_ERROR",
						   "SURFACE_FINISHED",
						   "SURFACE_TYPE_MISMATCH"));
    typeCairoOperator = make_typedef ("operator_t",
				      CairoNamespace,
				      publish_public,
				      OPERATOR_I,
				      NULL,
				      BuildEnumType (14,
						     "CLEAR",
						     
						     "SOURCE",
						     "OVER",
						     "IN",
						     "OUT",
						     "ATOP",
						     
						     "DEST",
						     "DEST_OVER",
						     "DEST_IN",
						     "DEST_OUT",
						     "DEST_ATOP",
						     
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
				    BuildStructType (6,
						     typePrim[rep_float], "xx",
						     typePrim[rep_float], "yx",
						     typePrim[rep_float], "xy",
						     typePrim[rep_float], "yy",
						     typePrim[rep_float], "x0",
						     typePrim[rep_float], "y0"));

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

    typeCairoRgbaColor = make_typedef ("rgba_color_t",
				       CairoNamespace,
				       publish_public,
				       RGBA_COLOR_I,
				       NULL,
				       BuildStructType (4,
							typePrim[rep_float], "red",
							typePrim[rep_float], "green",
							typePrim[rep_float], "blue",
							typePrim[rep_float], "alpha"));

    typeCairoPattern = make_typedef ("pattern_t",
				     CairoNamespace,
				     publish_public,
				     PATTERN_I,
				     NULL,
				     typePrim[rep_foreign]);

    /*
     * Path data structures
     */
    
    typeCairoCurveTo = make_typedef ("curve_to_t",
				     CairoNamespace,
				     publish_public,
				     CURVE_TO_I,
				     &ct,
				     0);

    ct->symbol.type = BuildStructType (6,
				       typePrim[rep_float], "x1",
				       typePrim[rep_float], "y1",
				       typePrim[rep_float], "x2",
				       typePrim[rep_float], "y2",
				       typePrim[rep_float], "x3",
				       typePrim[rep_float], "y3");

    typeCairoPath = make_typedef ("path_t",
				  CairoNamespace,
				  publish_public,
				  PATH_I,
				  &path,
				  BuildUnionType(4,
						 NewTypeRef (typeCairoPoint, True), "move_to",
						 NewTypeRef (typeCairoPoint, True), "line_to",
						 NewTypeRef (typeCairoCurveTo, True), "curve_to",
						 typePrim[rep_void], "close_path"));


    CairoSurfaceNamespace = BuiltinNamespace (&CairoNamespace, "Surface")->namespace.namespace;

    CairoImageNamespace = BuiltinNamespace (&CairoNamespace, "Image")->namespace.namespace;
    
    CairoPdfNamespace = BuiltinNamespace (&CairoNamespace, "Pdf")->namespace.namespace;
    CairoPsNamespace = BuiltinNamespace (&CairoNamespace, "Ps")->namespace.namespace;

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

    typeCairoScaledFont = make_typedef ("scaled_font_t",
					CairoNamespace,
					publish_public,
					SCALED_FONT_I,
					NULL,
					typePrim[rep_foreign]);
    typeCairoFontFace = make_typedef ("font_face_t",
				      CairoNamespace,
				      publish_public,
				      FONT_FACE_I,
				      NULL,
				      typePrim[rep_foreign]);

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
	
    static const struct fbuiltin_1 funcs_1[] = {
	{ do_Cairo_create, "create", CAIRO_S, SURFACE_S, "\n"
	    " cairo_t create (surface_t sv)\n"
	    "\n"
	    " Create a cairo context for the specified surface.\n"
	},
	{ do_Cairo_destroy, "destroy", "v", CAIRO_S, "\n"
	    " void destroy (cairo_t cr)\n"
	    "\n"
	    " destroy a rendering context.\n"},
	{ do_Cairo_get_target, "get_target", SURFACE_S, CAIRO_S, "\n"
	    " surface_t get_target (cairo_t cr)\n"
	    "\n"
	    " Return current target surface\n" },
	{ do_Cairo_status, "status", STATUS_S, CAIRO_S, "\n"
	    " status_t status (cairo_t cr)\n"
	    "\n"
	    " Return the status of a cairo surface\n" },
	{ do_Cairo_status_to_string, "status_string", "s", STATUS_S, "\n"
	    " string status_to_string (status_t status)\n"
	    "\n"
	    " Return a string describing the status\n" },
	{ do_Cairo_enable, "enable", "v", CAIRO_S, "\n"
	    " void enable (cairo_t cr)\n"
	    "\n"
	    " Enable screen updates from a cairo surface\n" },
	{ do_Cairo_disable, "disable", "v", CAIRO_S, "\n"
	    " void disable (cairo_t cr)\n"
	    "\n"
	    " Disable screen updates from a cairo surface\n" },
	{ do_Cairo_save, "save", "v", CAIRO_S, "\n"
	    " void save (cairo_t cr)\n"
	    "\n"
	    " Save graphics state\n" },
	{ do_Cairo_restore, "restore", "v", CAIRO_S, "\n"
	    " void restore (cairo_t cr)\n"
	    "\n"
	    " Restore graphics state\n" },
	{ do_Cairo_identity_matrix, "identity_matrix", "v", CAIRO_S, "\n"
	    " void identity_matrix (cairo_t cr)\n"
	    "\n"
	    " Set transformation matrix to identity\n" },
	{ do_Cairo_new_path, "new_path", "v", CAIRO_S, "\n"
	    " void new_path (cairo_t cr)\n"
	    "\n"
	    " Starts a new path\n" },
	{ do_Cairo_close_path, "close_path", "v", CAIRO_S, "\n"
	    " void close_path (cairo_t cr)\n"
	    "\n"
	    " Closes the current path\n" },
	{ do_Cairo_fill, "fill", "v", CAIRO_S, "\n"
	    " void fill (cairo_t cr)\n"
	    "\n"
	    " Fill the current path\n" },
	
	{ do_Cairo_fill_preserve, "fill_preserve", "v", CAIRO_S, "\n"
	    " void fill_preserve (cairo_t cr)\n"
	    "\n"
	    " Fill the current path without making an implicit call to new_path\n" },
	
	{ do_Cairo_stroke, "stroke", "v", CAIRO_S, "\n"
	    " void stroke (cairo_t cr)\n"
	    "\n"
	    " Stroke the current path\n" },
	
	{ do_Cairo_stroke_preserve, "stroke_preserve", "v", CAIRO_S, "\n"
	    " void stroke_preserve (cairo_t cr)\n"
	    "\n"
	    " Stroke the current path without making an implicit call to new_path\n" },
	
	{ do_Cairo_copy_page, "copy_page", "v", CAIRO_S, "\n"
	    " void copy_page (cairo_t cr)\n"
	    "\n"
	    " Write out the current page, leaving it intact.\n" },
	
	{ do_Cairo_show_page, "show_page", "v", CAIRO_S, "\n"
	    " void show_page (cairo_t cr)\n"
	    "\n"
	    " Write out the current page, and erase it.\n" },
	
	{ do_Cairo_copy_path, "copy_path", PATH_S, CAIRO_S, "\n"
	    " void copy_path (cairo_t cr)\n"
	    "\n"
	    " Returns the current path\n" },

	{ do_Cairo_copy_path_flat, "copy_path_flat", PATH_S, CAIRO_S, "\n"
	    " void copy_path (cairo_t cr)\n"
	    "\n"
	    " Returns the current path with curves tesselated to lines\n" },
	
	{ do_Cairo_reset_clip, "reset_clip", "v", CAIRO_S, "\n"
	    " void reset_clip (cairo_t cr)\n"
	    "\n"
	    " Reset clip to the entire surface.\n" },

	{ do_Cairo_clip, "clip", "v", CAIRO_S, "\n"
	    " void clip (cairo_t cr)\n"
	    "\n"
	    " Clip to current path. Consumes path.\n" },

	{ do_Cairo_clip_preserve, "clip_preserve", "v", CAIRO_S, "\n"
	    " void clip_preserve (cairo_t cr)\n"
	    "\n"
	    " Clip to current path without consuming path.\n" },

	{ do_Cairo_stroke_extents, "stroke_extents", RECT_S, CAIRO_S, "\n"
	    " rect_t stroke_extents (cairo_t cr)\n"
	    "\n"
	    " Returns bounding box of stroking current path\n" },
	
	{ do_Cairo_fill_extents, "fill_extents", RECT_S, CAIRO_S, "\n"
	    " rect_t fill_extents (cairo_t cr)\n"
	    "\n"
	    " Returns bounding box of filling current path\n" },
	
	{ do_Cairo_get_operator, "get_operator", OPERATOR_S, CAIRO_S, "\n"
	    " operator_t get_operator (cairo_t cr)\n"
	    "\n"
	    " Returns the current operator\n" },
	
	{ do_Cairo_get_source, "get_source", PATTERN_S, CAIRO_S, "\n"
	    " pattern_t get_source (cairo_t cr)\n"
	    "\n"
	    " Returns the current source pattern\n" },
	
	{ do_Cairo_get_tolerance, "get_tolerance", "n", CAIRO_S, "\n"
	    " real get_tolerance (cairo_t cr)\n"
	    "\n"
	    " Returns the current tolerance\n" },
	
	{ do_Cairo_get_current_point, "get_current_point", POINT_S, CAIRO_S, "\n"
	    " point_t current_point (cairo_t cr)\n"
	    "\n"
	    " Returns the current point\n" },
	
	{ do_Cairo_get_fill_rule, "get_fill_rule", FILL_RULE_S, CAIRO_S, "\n"
	    " fill_rule_t get_fill_rule (cairo_t cr)\n"
	    "\n"
	    " Returns the current fill rule\n" },
	
	{ do_Cairo_get_line_width, "get_line_width", "n", CAIRO_S, "\n"
	    " real get_line_width (cairo_t cr)\n"
	    "\n"
	    " Returns the current line width\n" },
	{ do_Cairo_get_line_cap, "get_line_cap", LINE_CAP_S, CAIRO_S, "\n"
	    " line_cap_t get_line_cap (cairo_t cr)\n"
	    "\n"
	    " Returns the current line_cap\n" },
	{ do_Cairo_get_line_join, "get_line_join", LINE_JOIN_S, CAIRO_S, "\n"
	    " line_join_t get_line_join (cairo_t cr)\n"
	    "\n"
	    " Returns the current line join\n" },
	{ do_Cairo_get_miter_limit, "get_miter_limit", "n", CAIRO_S, "\n"
	    " real get_miter_limit (cairo_t cr)\n"
	    "\n"
	    " Returns the current miter limit\n" },
	{ do_Cairo_get_matrix, "get_matrix", MATRIX_S, CAIRO_S, "\n"
	    " matrix_t get_matrix (cairo_t cr)\n"
	    "\n"
	    " Returns the current transformation matrix\n" },
	{ do_Cairo_paint, "paint", "v", CAIRO_S, "\n"
	    " void paint (cairo_t cr)\n"
	    "\n"
	    " Fill cr with the pattern\n" },
	{ do_Cairo_get_font_matrix, "get_font_matrix", MATRIX_S, CAIRO_S, "\n"
	    " matrix_t get_font_matrix (cairo_t cr)\n"
	    "\n"
	    " Returns the current font matrix\n" },
	{ do_Cairo_font_extents, "font_extents", FONT_EXTENTS_S, CAIRO_S, "\n"
	    " font_extents_t font_extents (cairo_t cr)\n"
	    "\n"
	    " Returns metrics for current font\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_2 funcs_2[] = {
	{ do_Cairo_set_operator, "set_operator", "v", CAIRO_S OPERATOR_S, "\n"
	    " void set_operator (cairo_t cr, operator_t operator)\n"
	    "\n"
	    " Set current operator\n" },
	{ do_Cairo_set_source, "set_source", "v", CAIRO_S PATTERN_S, "\n"
	    " void set_source (cairo_t cr, pattern_t pattern)\n"
	    "\n"
	    " Set current source pattern\n" },
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
	{ do_Cairo_paint_with_alpha, "paint_with_alpha", "v", CAIRO_S "n", "\n"
	    " void paint_with_alpha (cairo_t cr, real alpha)\n"
	    "\n"
	    " Fill cr with the source with the constant mask alpha\n" },
	{ do_Cairo_mask, "mask", "v", CAIRO_S PATTERN_S, "\n"
	    " void mask (cairo_t cr, pattern_t pattern)\n"
	    "\n"
	    " Fill cr with the source with the specified mask\n" },
	{ do_Cairo_append_path, "append_path", "v", CAIRO_S PATH_S, "\n"
	    " void append_path (cairo_t cr, path_t path)\n"
	    "\n"
	    " Append path to cr\n" },
	{ do_Cairo_set_font, "set_font", "v", CAIRO_S "s", "\n"
	    " void set_font (cairo_t cr, string fontname)\n"
	    "\n"
	    " Sets the current font using fontname\n" },
	{ do_Cairo_set_font_size, "set_font_size", "v", CAIRO_S "n", "\n"
	    " void set_font_size (cairo_t cr, real size)\n"
	    "\n"
	    " Sets font size\n" },
	{ do_Cairo_set_font_matrix, "set_font_matrix", "v", CAIRO_S MATRIX_S, "\n"
	    " void set_font_matrix (cairo_t cr, matrix_t matrix)\n"
	    "\n"
	    " Sets current font matrix\n" },
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
	{ do_Cairo_transform, "transform", "v", CAIRO_S MATRIX_S, "\n"
	    " void transform (cairo_t cr, matrix_t matrix)\n"
	    "\n"
	    " Mixes in another matrix to the current transformation\n" },
	{ do_Cairo_set_matrix, "set_matrix", "v", CAIRO_S MATRIX_S, "\n"
	    " void set_matrix (cairo_t cr, matrix_t matrix)\n"
	    "\n"
	    " Sets the transformation matrix\n" },
	{ do_Cairo_user_to_device, "user_to_device", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t user_to_device (cairo_t cr, point_t)\n"
	    "\n"
	    " Transform a point from user to device space\n" },
	{ do_Cairo_user_to_device_distance, "user_to_device_distance", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t user_to_device_distance (cairo_t cr, point_t point)\n"
	    "\n"
	    " Transform a distance from user to device space\n" },
	{ do_Cairo_device_to_user, "device_to_user", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t device_to_user (cairo_t cr, point_t point)\n"
	    "\n"
	    " Transform a point from device to user space\n" },
	{ do_Cairo_device_to_user_distance, "device_to_user_distance", POINT_S, CAIRO_S POINT_S, "\n"
	    " point_t device_to_user_distance (cairo_t cr, point_t point)\n"
	    "\n"
	    " Transform a distance from device to user space\n" },
	{ 0 }
    };

    static const struct fbuiltin_3 funcs_3[] = {
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
	    " bool in_fill (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Test whether (x,y) would be covered by filling the current path\n" },
	{ do_Cairo_in_stroke, "in_stroke", "b", CAIRO_S "nn", "\n"
	    " bool in_stroke (cairo_t cr, real x, real y)\n"
	    "\n"
	    " Test whether (x,y) would be covered by stroking the current path\n" },
	{ 0 }
    };

    static const struct fbuiltin_4 funcs_4[] = {
	{ do_Cairo_set_source_rgb, "set_source_rgb", "v", CAIRO_S "nnn", "\n"
	    " void set_source_rgb (cairo_t cr, real red, real green, real blue)\n"
	    "\n"
	    " Set solid color source\n" },
	{ do_Cairo_set_source_surface, "set_source_surface", "v", CAIRO_S SURFACE_S "nn", "\n"
	    " void set_source_surface (cairo_t cr, surface_t surface, real x, real y)\n"
	    "\n"
	    " Set the source to surface and offset by x,y\n" },
	{ do_Cairo_mask_surface, "mask_surface", "v", CAIRO_S SURFACE_S "nn", "\n"
	    " void mask_surface (cairo_t cr, surface_t surface, real x, real y)\n"
	    "\n"
	    " Fill cr with the source using surface, offset by x,y, as the mask\n" },
	{ do_Cairo_select_font_face, "select_font_face", "v", CAIRO_S "s" FONT_SLANT_S FONT_WEIGHT_S, "\n"
	    " void select_font_face (cairo_t cr, string family, font_slant_t slant, font_weight_t weight)\n"
	    "\n"
	    " Select a font face using the specified family, slant and weight\n" },
	{ 0 }
    };

    static const struct fbuiltin_5 funcs_5[] = {
	{ do_Cairo_rectangle, "rectangle", "v", CAIRO_S "nnnn", "\n"
	    " void rectangle (cairo_t cr, real x, real y, real width, real height)\n"
	    "\n"
	    " Adds the specified rectangle to the current path\n" },
	{ do_Cairo_set_source_rgba, "set_source_rgba", "v", CAIRO_S "nnnn", "\n"
	    " void set_source_rgba (cairo_t cr, real red, real green, real blue, real alpha)\n"
	    "\n"
	    " Set solid color source with alpha (premultiplied)\n" },
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
	{ do_Cairo_Surface_finish, "finish", "v", SURFACE_S, "\n"
	    " void finish (surface_t surface)\n"
	    "\n"
	    " Finish a surface. All output to the surface is finalized\n" },
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

    static const struct fbuiltin_2 surfuncs_2[] = {
	{ do_Cairo_Surface_write_to_png, "write_to_png", "v", SURFACE_S "s", "\n"
	    " void write_to_png (surface_t surface, string filename)\n"
	    "\n"
	    " Write a surface to filename in png format\n" },
	{ do_Cairo_Surface_write_to_png_file, "write_to_png_file", "v", SURFACE_S "f", "\n"
	    " void write_to_png (surface_t surface, file f)\n"
	    "\n"
	    " Write a surface to f in png format\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_3 surfuncs_3[] = {
	{ do_Cairo_Surface_create_window, "create_window", SURFACE_S, "snn", "\n"
	    " surface_t create_window (real width, real height)\n"
	    "\n"
	    " Create a window and return a surface pointer for it\n" },
	{ do_Cairo_Surface_create_similar, "create_similar", SURFACE_S, SURFACE_S "nn", "\n"
	    " surface_t create_similar (surface_t related, real width, real height)\n"
	    "\n"
	    " Create a similar surface related to another surface\n" },
	{ do_Cairo_Surface_set_device_offset, "set_device_offset", "v", SURFACE_S "nn", "\n"
	    " void set_device_offset (surface_t surface, real x, real y)\n"
	    "\n"
	    " Sets the device offset for surface to x,y\n" },
	{ 0 }
    };

    static const struct fbuiltin_1 patfuncs_1[] = {
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
	{ do_Cairo_Pattern_set_matrix, "set_matrix", "v", PATTERN_S MATRIX_S, "\n"
	    " void set_matrix (pattern_t pattern, matrix_t matrix)\n"
	    "\n"
	    " Set a transformation matrix for a pattern\n" },
	{ do_Cairo_Pattern_set_extend, "set_extend", "v", PATTERN_S EXTEND_S, "\n"
	    " void set_extend (pattern_t pattern, extend_t extend)\n"
	    "\n"
	    " Set a extend method for a pattern\n" },
	{ do_Cairo_Pattern_set_filter, "set_filter", "v", PATTERN_S FILTER_S, "\n"
	    " void set_filter (pattern_t pattern, filter_t filter)\n"
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

    static const struct fbuiltin_5 patfuncs_5[] = {
	{ do_Cairo_Pattern_add_color_stop_rgb, "add_color_stop_rgb", "v", PATTERN_S "nnnn", "\n"
	    " void add_color_stop_rgba (pattern_t cr, real offset, real red, real green, real blue)\n"
	    "\n"
	    " Add a color stop in a gradient pattern.\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_6 patfuncs_6[] = {
	{ do_Cairo_Pattern_create_radial, "create_radial", PATTERN_S, "nnnnnn", "\n"
	    " pattern_t create_radial (real cx0, real cy0, real radius0, real cx1, real cy1, real radius1)\n"
	    "\n"
	    " Create a radial gradient pattern\n" },
	{ do_Cairo_Pattern_add_color_stop_rgba, "add_color_stop_rgba", "v", PATTERN_S "nnnnn", "\n"
	    " void add_color_stop_rgba (pattern_t cr, real offset, real red, real green, real blue, real alpha)\n"
	    "\n"
	    " Add a color and opacity stop in a gradient pattern.\n" },
	{ 0 }
    };

    static const struct fbuiltin_1 imgfuncs_1[] = {
	{ do_Cairo_Image_surface_create_from_png, "surface_create_from_png", SURFACE_S, "s", "\n"
	    " surface_t surface_create_from_png (string filename)\n"
	    "\n"
	    " Create an image surface from the png image in filename\n" },
	{ do_Cairo_Image_surface_create_from_png_file, "surface_create_from_png_file", SURFACE_S, "f", "\n"
	    " surface_t surface_create_from_png_file (file f)\n"
	    "\n"
	    " Create an image surface from the png image in f\n" },
	{ 0 }
    };
					 
    static const struct fbuiltin_3 imgfuncs_3[] = {
	{ do_Cairo_Image_surface_create, "surface_create", SURFACE_S, "iii", "\n"
	    " surface_t surface_create (int format, int width, int height)\n"
	    "\n"
	    " Create an image surface of the specified size in pixels\n" },
	{ 0 }
    };
					 
    static const struct fbuiltin_3 pdffuncs_3[] = {
	{ do_Cairo_Pdf_surface_create, "surface_create", SURFACE_S, "snn", "\n"
	    " surface_t surface_create (string filename, real width, real height)\n"
	    "\n"
	    " Create a PDF surface of the specified size in points, written to filename\n" },
	{ do_Cairo_Pdf_surface_create, "surface_create_for_file", SURFACE_S, "snn", "\n"
	    " surface_t surface_create (file f, real width, real height)\n"
	    "\n"
	    " Create a PDF surface of the specified size in points, written to f\n" },
	{ 0 }
    };
					 
    static const struct fbuiltin_3 psfuncs_3[] = {
	{ do_Cairo_Ps_surface_create, "surface_create", SURFACE_S, "snn", "\n"
	    " surface_t surface_create (string filename, real width, real height)\n"
	    "\n"
	    " Create a PS surface of the specified size in points, written to filename\n" },
	{ do_Cairo_Ps_surface_create, "surface_create_for_file", SURFACE_S, "snn", "\n"
	    " surface_t surface_create (file f, real width, real height)\n"
	    "\n"
	    " Create a PS surface of the specified size in points, written to f\n" },
	{ 0 }
    };
					 
    init_types ();
    
    BuiltinFuncs1 (&CairoNamespace, funcs_1);
    BuiltinFuncs2 (&CairoNamespace, funcs_2);
    BuiltinFuncs3 (&CairoNamespace, funcs_3);
    BuiltinFuncs4 (&CairoNamespace, funcs_4);
    BuiltinFuncs5 (&CairoNamespace, funcs_5);
    BuiltinFuncs6 (&CairoNamespace, funcs_6);
    BuiltinFuncs7 (&CairoNamespace, funcs_7);

    BuiltinFuncs1 (&CairoSurfaceNamespace, surfuncs_1);
    BuiltinFuncs2 (&CairoSurfaceNamespace, surfuncs_2);
    BuiltinFuncs3 (&CairoSurfaceNamespace, surfuncs_3);
    
    BuiltinFuncs1 (&CairoPatternNamespace, patfuncs_1);
    BuiltinFuncs2 (&CairoPatternNamespace, patfuncs_2);
    BuiltinFuncs4 (&CairoPatternNamespace, patfuncs_4);
    BuiltinFuncs5 (&CairoPatternNamespace, patfuncs_5);
    BuiltinFuncs6 (&CairoPatternNamespace, patfuncs_6);

    BuiltinFuncs1 (&CairoImageNamespace, imgfuncs_1);
    BuiltinFuncs3 (&CairoImageNamespace, imgfuncs_3);
    
    BuiltinFuncs3 (&CairoPdfNamespace, pdffuncs_3);
    BuiltinFuncs3 (&CairoPsNamespace, psfuncs_3);

    RETURN(TrueVal);
}
