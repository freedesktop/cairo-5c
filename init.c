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
Type		*typeTextExtents;

Value
nickle_init (void)
{
    ENTER ();
    static const struct fbuiltin_v funcs_v[] = {
	{ do_Cairo_new, "new", "F", ".i", "\n"
	    " foreign new (int...)\n"
	    "\n"
	    " Create a cairo window. Optional arguments are width, height\n" },
	{ 0 }
    };
	
    static const struct fbuiltin_1 funcs_1[] = {
	{ do_Cairo_width, "width", "i", "F", "\n"
	    " void width (foreign cairo)\n"
	    "\n"
	    " Return the width of a cairo surface\n" },
	{ do_Cairo_height, "height", "i", "F", "\n"
	    " void height (foreign cairo)\n"
	    "\n"
	    " Return the height of a cairo surface\n" },
	{ do_Cairo_status, "status", "i", "F", "\n"
	    " void status (foreign cairo)\n"
	    "\n"
	    " Return the status of a cairo surface\n" },
	{ do_Cairo_status_string, "status_string", "s", "F", "\n"
	    " void status_string (foreign cairo)\n"
	    "\n"
	    " Return the status string of a cairo surface\n" },
	{ do_Cairo_dispose, "dispose", "v", "F", "\n"
	    " void dispose (foreign cairo)\n"
	    "\n"
	    " Dispose a cairo surface\n" },
	{ do_Cairo_save, "save", "v", "F", "\n"
	    " void save (foreign cairo)\n"
	    "\n"
	    " Save graphics state\n" },
	{ do_Cairo_restore, "restore", "v", "F", "\n"
	    " void restore (foreign cairo)\n"
	    "\n"
	    " Restore graphics state\n" },
	{ do_Cairo_identity_matrix, "identity_matrix", "v", "F", "\n"
	    " void identity_matrix (foreign cairo)\n"
	    "\n"
	    " Set transformation matrix to identity\n" },
	{ do_Cairo_new_path, "new_path", "v", "F", "\n"
	    " void new_path (foreign cairo)\n"
	    "\n"
	    " Starts a new path\n" },
	{ do_Cairo_close_path, "close_path", "v", "F", "\n"
	    " void close_path (foreign cairo)\n"
	    "\n"
	    " Closes the current path\n" },
	{ do_Cairo_fill, "fill", "v", "F", "\n"
	    " void fill (foreign cairo)\n"
	    "\n"
	    " Fill the current path\n" },
	{ do_Cairo_stroke, "stroke", "v", "F", "\n"
	    " void stroke (foreign cairo)\n"
	    "\n"
	    " Stroke the current path\n" },
	{ 0 }
    };
    
    static const struct fbuiltin_2 funcs_2[] = {
	{ do_Cairo_set_alpha, "set_alpha", "v", "Fn", "\n"
	    " void set_alpha (foreign cr, real alpha)\n"
	    "\n"
	    " Set current alpha\n" },
	{ do_Cairo_set_tolerance, "set_tolerance", "v", "Fn", "\n"
	    " void set_tolerance (foreign cr, real alpha)\n"
	    "\n"
	    " Set current tolerance\n" },
	{ do_Cairo_set_line_width, "set_line_width", "v", "Fn", "\n"
	    " void set_line_width (foreign cr, real alpha)\n"
	    "\n"
	    " Set current line width\n" },
	{ do_Cairo_set_line_cap, "set_line_cap", "v", "Fi", "\n"
	    " void set_line_cap (foreign cr, int cap)\n"
	    "\n"
	    " Set current line cap style\n" },
	{ do_Cairo_set_line_join, "set_line_join", "v", "Fi", "\n"
	    " void set_line_join (foreign cr, int join)\n"
	    "\n"
	    " Set current line join style\n" },
	{ do_Cairo_rotate, "rotate", "v", "Fn", "\n"
	    " void rotate (foreign cr, real radians)\n"
	    "\n"
	    " Rotate current transformation matrix by specified amount\n" },
	{ do_Cairo_scale_font, "scale_font", "v", "Fn", "\n"
	    " void scale_font (foreign cr, real scale)\n"
	    "\n"
	    " Scales current font by specified amount\n" },
	{ do_Cairo_show_text, "show_text", "v", "Fs", "\n"
	    " void show_text (foreign cr, string text)\n"
	    "\n"
	    " Shows text at current position\n" },
	{ do_Cairo_text_path, "text_path", "v", "Fs", "\n"
	    " void text_path (foreign cr, string text)\n"
	    "\n"
	    " Appends text to current path\n" },
	{ do_Cairo_text_extents, "text_extents", "u", "Fs", "\n"
	    " text_extents_t text_extents (foreign cr, string text)\n"
	    "\n"
	    " Appends text to current path\n" },
	{ 0 }
    };

    static const struct fbuiltin_3 funcs_3[] = {
	{ do_Cairo_new_png, "new_png", "F", "sii", "\n"
	    " foreign new_png (string filename, int width, int height)\n"
	    "\n"
	    " Create a cairo png file.\n" },
	{ do_Cairo_move_to, "move_to", "v", "Fnn", "\n"
	    " void move_to (foreign cr, real x, real y)\n"
	    "\n"
	    " Move to specified position\n" },
	{ do_Cairo_line_to, "line_to", "v", "Fnn", "\n"
	    " void line_to (foreign cr, real x, real y)\n"
	    "\n"
	    " Draw line to specified position\n" },
	{ do_Cairo_rel_move_to, "rel_move_to", "v", "Fnn", "\n"
	    " void rel_move_to (foreign cr, real x, real y)\n"
	    "\n"
	    " Move to specified offset from the current position\n" },
	{ do_Cairo_rel_line_to, "rel_line_to", "v", "Fnn", "\n"
	    " void rel_line_to (foreign cr, real x, real y)\n"
	    "\n"
	    " Draw line to specified offset from the current position\n" },
	{ do_Cairo_translate, "translate", "v", "Fnn", "\n"
	    " void translate (foreign cr, real x, real y)\n"
	    "\n"
	    " Offset current transformation matrix by specified amount\n" },
	{ do_Cairo_scale, "scale", "v", "Fnn", "\n"
	    " void scale (foreign cr, real x, real y)\n"
	    "\n"
	    " Scale current transformation matrix by specified amount\n" },
	{ 0 }
    };

    static const struct fbuiltin_4 funcs_4[] = {
	{ do_Cairo_set_rgb_color, "set_rgb_color", "v", "Fnnn", "\n"
	    " void set_rgb_color (foreign cr, real red, real green, real blue)\n"
	    "\n"
	    " Set current color\n" },
	{ do_Cairo_select_font, "select_font", "v", "Fsii", "\n"
	    " void select_font (foreign cr, string family, int slant, int weight)\n"
	    "\n"
	    " Set current font\n" },
	{ 0 }
    };

    static const struct fbuiltin_6 funcs_6[] = {
	{ do_Cairo_arc, "arc", "v", "Fnnnnn", "\n"
	    " void arc (foreign cr, real x1, real y1, real x2, real y2, real radius)\n"
	    "\n"
	    " Draw a clockwise arc from x1,y1 to x2,y2 with the given radius\n" },
	{ do_Cairo_arc_negative, "arc_negative", "v", "Fnnnnn", "\n"
	    " void arc (foreign cr, real x1, real y1, real x2, real y2, real radius)\n"
	    "\n"
	    " Draw a counter-clockwise arc from x1,y1 to x2,y2 with the given radius\n" },
	{ 0 }
    };

    static const struct fbuiltin_7 funcs_7[] = {
	{ do_Cairo_curve_to, "curve_to", "v", "Fnnnnnn", "\n"
	    " void curve_to (foreign cr, real x1, real y1, real x2, real y2, real x3, real y3)\n"
	    "\n"
	    " Draw Bézier spline to specified position\n" },
	{ do_Cairo_rel_curve_to, "rel_curve_to", "v", "Fnnnnnn", "\n"
	    " void curve_to (foreign cr, real x1, real y1, real x2, real y2, real x3, real y3)\n"
	    "\n"
	    " Draw Bézier spline to specified offset from the current position\n" },
	{ 0 }
    };

    static const struct ibuiltin ivars[] = {
	{ CAIRO_LINE_CAP_BUTT, "CAP_BUTT", &CairoNamespace },
	{ CAIRO_LINE_CAP_ROUND, "CAP_ROUND", &CairoNamespace },
	{ CAIRO_LINE_CAP_SQUARE, "CAP_SQUARE", &CairoNamespace },
	{ CAIRO_LINE_JOIN_MITER, "JOIN_MITER", &CairoNamespace },
	{ CAIRO_LINE_JOIN_ROUND, "JOIN_ROUND", &CairoNamespace },
	{ CAIRO_LINE_JOIN_BEVEL, "JOIN_BEVEL", &CairoNamespace },
	{ CAIRO_FILL_RULE_WINDING, "RULE_WINDING", &CairoNamespace },
	{ CAIRO_FILL_RULE_EVEN_ODD, "RULE_EVEN_ODD", &CairoNamespace },
	{ CAIRO_FONT_WEIGHT_NORMAL, "WEIGHT_NORMAL", &CairoNamespace },
	{ CAIRO_FONT_WEIGHT_BOLD, "WEIGHT_BOLD", &CairoNamespace },
	{ CAIRO_FONT_SLANT_NORMAL, "SLANT_NORMAL", &CairoNamespace },
	{ CAIRO_FONT_SLANT_ITALIC, "SLANT_ITALIC", &CairoNamespace },
	{ CAIRO_FONT_SLANT_OBLIQUE, "SLANT_OBLIQUE", &CairoNamespace },
	{ 0 }
    };

    SymbolPtr	sym;
    Type    *type;

    CairoNamespace = BuiltinNamespace (/* parent */ 0, "Cairo")->namespace.namespace;

    type = BuildStructType (6, 
			    typePrim[rep_float], "x_bearing",
			    typePrim[rep_float], "y_bearing",
			    typePrim[rep_float], "width",
			    typePrim[rep_float], "height",
			    typePrim[rep_float], "x_advance",
			    typePrim[rep_float], "y_advance");
    
    sym = NewSymbolType (AtomId ("text_extents_t"), type);

    typeTextExtents = NewTypeName (NewExprAtom (AtomId ("text_extents_t"), 0, False),
				   sym);
    
    BuiltinSetUserdefType (typeTextExtents);

    NamespaceAddName (CairoNamespace, sym, publish_public);

    BuiltinFuncsV (&CairoNamespace, funcs_v);
    BuiltinFuncs1 (&CairoNamespace, funcs_1);
    BuiltinFuncs2 (&CairoNamespace, funcs_2);
    BuiltinFuncs3 (&CairoNamespace, funcs_3);
    BuiltinFuncs4 (&CairoNamespace, funcs_4);
    BuiltinFuncs6 (&CairoNamespace, funcs_6);
    BuiltinFuncs7 (&CairoNamespace, funcs_7);

    BuiltinIntegers (ivars);

    RETURN(TrueVal);
}
