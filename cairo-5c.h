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

#ifndef _CAIRO_5C_H_
#define _CAIRO_5C_H_

#include <nickle/builtin.h>
#define Atom XAtom
#include <cairo.h>
#include <stdio.h>
#include <unistd.h>
#undef Atom

typedef enum { CAIRO_5C_WINDOW, CAIRO_5C_PNG, CAIRO_5C_PS, CAIRO_5C_SCRATCH } cairo_5c_kind_t;

typedef struct _cairo_5c_tool	cairo_5c_tool_t;

typedef struct _cairo_5c_window_t {
    Pixmap	    pixmap;
    Pixmap	    curpix;
    cairo_5c_tool_t *tool;
    FILE	    *send_events;
} cairo_5c_window_t;

typedef struct _cairo_png_t {
    FILE	*file;
} cairo_5c_png_t;

typedef struct _cairo_ps_t {
    FILE	*file;
} cairo_5c_ps_t;

typedef struct _cairo_5c_t {
    DataType	    *data;
    cairo_t	    *cr;
    Value	    surface;
} cairo_5c_t;

typedef struct _cairo_5c_surface_t {
    DataType	    *data;
    cairo_5c_kind_t kind;
    cairo_surface_t *surface;
    int		    width;
    int		    height;
    Bool	    dirty;
    Value	    recv_events;
    union {
	cairo_5c_window_t   window;
	cairo_5c_png_t	    png;
	cairo_5c_ps_t	    ps;
    } u;
} cairo_5c_surface_t;

extern Type		*typeCairo;
extern Type		*typeCairoSurface;
extern Type		*typeCairoStatus;
extern Type		*typeCairoOperator;
extern Type		*typeCairoFillRule;
extern Type		*typeCairoLineCap;
extern Type		*typeCairoLineJoin;
extern Type		*typeCairoFontSlant;
extern Type		*typeCairoFontWeight;
extern Type		*typeCairoTextExtents;
extern Type		*typeCairoFontExtents;
extern Type		*typeCairoMatrix;
extern Type		*typeCairoPoint;
extern Type		*typeCairoRect;
extern Type		*typeCairoRgbColor;
extern Type		*typeCairoPattern;
extern Type		*typeCairoPath;
extern Type		*typeCairoMoveTo;
extern Type		*typeCairoLineTo;
extern Type		*typeCairoCurveTo;
extern Type		*typeCairoClosePath;
extern Type		*typeCairoFilter;

extern Type		*typeCairoPatternExtend;
extern Type		*typeCairoPatternFilter;

/* cairo.c */
    
cairo_5c_t *
cairo_5c_get (Value av);

void
cairo_5c_dirty (cairo_5c_t *c5c);

Value
do_Cairo_create (void);

Value
do_Cairo_destroy (Value cv);

Value
do_Cairo_set_target_surface (Value cv, Value sv);
    
Value
do_Cairo_current_target_surface (Value cv);
    
Value
do_Cairo_copy (Value dstv, Value srcv);

Value
do_Cairo_status (Value cv);

Value
do_Cairo_status_string (Value cv);

Value
do_Cairo_enable (Value cv);

Value
do_Cairo_disable (Value cv);

/* surface.c */

cairo_5c_surface_t *
cairo_5c_surface_get (Value av);

Value
do_Cairo_Surface_create_window (Value wv, Value hv);

Value
do_Cairo_Surface_create_png (Value fv, Value wv, Value hv);

Value
do_Cairo_Surface_create_ps (Value fv, Value wv, Value hv, Value xppiv, Value yppiv);

Value
do_Cairo_Surface_create_similar (Value sv, Value wv, Value hv);

Value
do_Cairo_Surface_destroy (Value sv);

Value
do_Cairo_Surface_width (Value sv);

Value
do_Cairo_Surface_height (Value sv);

/* event.c */
Value
do_Cairo_Surface_open_event (Value sv);

/* init.c */
Value
nickle_init (void);

int
EnumIntPart (Value ev, char *err);
    
Value
IntToEnum (Type *type, int i);

/* gstate.c */

Value
do_Cairo_save (Value cv);

Value
do_Cairo_restore (Value cv);

Value
do_Cairo_set_operator (Value cv, Value ov);

Value
do_Cairo_default_matrix (Value cv);

Value
do_Cairo_identity_matrix (Value cv);

Value
do_Cairo_set_rgb_color (Value av, Value rv, Value gv, Value bv);

Value
do_Cairo_set_alpha (Value cv, Value av);

Value
do_Cairo_set_tolerance (Value cv, Value tv);

Value
do_Cairo_set_fill_rule (Value cv, Value fv);

Value
do_Cairo_set_line_width (Value cv, Value lv);

Value
do_Cairo_set_line_cap (Value cv, Value lv);

Value
do_Cairo_set_line_join (Value cv, Value lv);

Value
do_Cairo_set_dash (Value cv, Value dv, Value ov);

Value
do_Cairo_set_miter_limit (Value cv, Value mv);

Value
do_Cairo_translate (Value cv, Value xv, Value yv);

Value
do_Cairo_scale (Value cv, Value xv, Value yv);

Value
do_Cairo_rotate (Value cv, Value av);

Value
do_Cairo_current_matrix (Value cv);

Value
do_Cairo_concat_matrix (Value cv, Value mv);

Value
do_Cairo_set_matrix (Value cv, Value mv);

Value
do_Cairo_transform_point (Value cv, Value pv);

Value
do_Cairo_transform_distance (Value cv, Value pv);

Value
do_Cairo_inverse_transform_point (Value cv, Value pv);

Value
do_Cairo_inverse_transform_distance (Value cv, Value pv);

Value
do_Cairo_init_clip (Value cv);

Value
do_Cairo_clip (Value cv);

Value
do_Cairo_current_operator (Value cv);

Value
do_Cairo_current_rgb_color (Value cv);

Value
do_Cairo_current_alpha (Value cv);

Value
do_Cairo_current_tolerance (Value cv);

Value
do_Cairo_current_point (Value cv);

Value
do_Cairo_current_fill_rule (Value cv);

Value
do_Cairo_current_line_width (Value cv);

Value
do_Cairo_current_line_cap (Value cv);

Value
do_Cairo_current_line_join (Value cv);

Value
do_Cairo_current_miter_limit (Value cv);

/* draw.c */
Value
do_Cairo_new_path (Value cv);
    
Value
do_Cairo_move_to (Value cv, Value xv, Value yv);

Value
do_Cairo_line_to (Value cv, Value xv, Value yv);

Value
do_Cairo_curve_to (Value cv, 
		   Value x1v, Value y1v,
		   Value x2v, Value y2v,
		   Value x3v, Value y3v);

Value
do_Cairo_arc (Value cv, 
	      Value x1v, Value y1v,
	      Value x2v, Value y2v,
	      Value rv);

Value
do_Cairo_arc_negative (Value cv, 
		       Value x1v, Value y1v,
		       Value x2v, Value y2v,
		       Value rv);

Value
do_Cairo_rel_move_to (Value cv, Value xv, Value yv);

Value
do_Cairo_rel_line_to (Value cv, Value xv, Value yv);

Value
do_Cairo_rel_curve_to (Value cv, 
		   Value x1v, Value y1v,
		   Value x2v, Value y2v,
		   Value x3v, Value y3v);

Value
do_Cairo_rectangle (Value cv, Value xv, Value yv, Value wv, Value hv);

Value
do_Cairo_close_path (Value cv);

Value
do_Cairo_fill (Value cv);

Value
do_Cairo_stroke (Value cv);

Value
do_Cairo_in_stroke (Value cv, Value xv, Value yv);

Value
do_Cairo_in_fill (Value cv, Value xv, Value yv);

Value
do_Cairo_stroke_extents (Value cv);

Value
do_Cairo_fill_extents (Value cv);

Value
do_Cairo_current_path_list (Value cv);

Value
do_Cairo_current_path_flat_list (Value cv);

/* matrix.c */
cairo_matrix_t *
cairo_matrix_part (Value matrixv, char *err);

Value
new_cairo_matrix (cairo_matrix_t *matrix);

/* pattern.c */
cairo_pattern_t *
get_cairo_pattern (Value pv);
    
Value
do_Cairo_current_pattern (Value cv);

Value
do_Cairo_set_pattern (Value cv, Value patv);

Value
do_Cairo_Pattern_create_png (Value filenamev);

Value
do_Cairo_Pattern_create_linear (Value x0v, Value y0v, Value x1v, Value y1v);

Value
do_Cairo_Pattern_create_radial (Value cx0v, Value cy0v, Value radius0v,
				Value cx1v, Value cy1v, Value radius1v);

Value
do_Cairo_Pattern_create_for_surface (Value cv);

Value
do_Cairo_Pattern_add_color_stop (Value patv, Value offsetv,
				 Value redv, Value greenv, Value bluev,
				 Value alphav);

Value
do_Cairo_Pattern_set_matrix (Value patv, Value matrixv);

Value
do_Cairo_Pattern_get_matrix (Value patv);

Value
do_Cairo_Pattern_set_extend (Value patv, Value extendv);

Value
do_Cairo_Pattern_get_extend (Value patv);

Value
do_Cairo_Pattern_set_filter (Value patv, Value filterv);

Value
do_Cairo_Pattern_get_filter (Value patv);

/* text.c */
Value
do_Cairo_select_font (Value cv, Value fv, Value sv, Value wv);

Value
do_Cairo_set_font (Value cv, Value fv);

Value
do_Cairo_scale_font (Value cv, Value sv);

Value
do_Cairo_transform_font (Value cv, Value mv);

Value
do_Cairo_current_font_extents (Value cv);

Value
do_Cairo_show_text (Value cv, Value uv);

Value
do_Cairo_text_path (Value cv, Value uv);

Value
do_Cairo_text_extents (Value cv, Value uv);


/* gtk.c */

Bool
cairo_5c_tool_create (cairo_5c_surface_t *c5s, int width, int height);

Bool
cairo_5c_tool_destroy (cairo_5c_surface_t *c5s);

void
cairo_5c_tool_dirty (cairo_5c_surface_t *c5s);

Bool
cairo_5c_tool_disable (cairo_5c_surface_t *c5s);

Bool
cairo_5c_tool_enable (cairo_5c_surface_t *c5s);

Display *
cairo_5c_tool_display (cairo_5c_surface_t *c5s);

void
cairo_5c_tool_mark (cairo_5c_surface_t *c5s);

#endif /* _CAIRO_5C_H_ */
