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

#include "config.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <cairo.h>
#include <cairo-ft.h>

#if HAVE_CAIRO_PDF_H
#include <cairo-pdf.h>
#endif

#if HAVE_CAIRO_SVG_H
#include <cairo-svg.h>
#endif

#if HAVE_CAIRO_PS_H
#include <cairo-ps.h>
#endif

#if HAVE_CAIRO_XLIB_H
#define Atom XAtom
#define HAVE_CAIRO_5C_WINDOW 1
#include <cairo-xlib.h>
#undef Atom
#undef True
#undef False
#undef Bool
#endif

#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>

#include <nickle/builtin.h>

typedef enum { CAIRO_5C_WINDOW, CAIRO_5C_IMAGE, CAIRO_5C_PDF, CAIRO_5C_SVG, CAIRO_5C_PS, CAIRO_5C_SCRATCH } cairo_5c_kind_t;

typedef struct _cairo_5c_gui	cairo_5c_gui_t;

typedef struct _cairo_5c_window_t {
    cairo_5c_gui_t  *gui;
} cairo_5c_window_t;

typedef struct _cairo_5c_pdf_t {
    Value	    file;
} cairo_5c_pdf_t;

typedef struct _cairo_5c_ps_t {
    Value	    file;
} cairo_5c_ps_t;

typedef struct _cairo_5c_svg_t {
    Value	    file;
} cairo_5c_svg_t;

typedef struct _cairo_5c_image_t {
    int		    unused;
} cairo_5c_image_t;

typedef struct _cairo_5c_t {
    DataType	    *data;
    cairo_t	    *cr;
    Value	    surface;
} cairo_5c_t;

typedef struct _cairo_5c_surface_t {
    DataType	    *data;
    cairo_5c_kind_t kind;
    cairo_surface_t *surface;
    double    	    width;
    double    	    height;
    Bool	    dirty;
    Value	    recv_events;
    Bool	    copied;
    union {
	cairo_5c_window_t   window;
	cairo_5c_pdf_t	    pdf;
	cairo_5c_ps_t	    ps;
	cairo_5c_svg_t	    svg;
	cairo_5c_image_t    image;
    } u;
} cairo_5c_surface_t;

typedef struct _cairo_5c_pattern_t {
    DataType	    *data;
    cairo_pattern_t *pattern;
} cairo_5c_pattern_t;

typedef struct _cairo_5c_scaled_font_t {
    DataType		*data;
    cairo_scaled_font_t	*scaled_font;
} cairo_5c_scaled_font_t;

typedef struct _cairo_5c_font_face_t {
    DataType		*data;
    cairo_font_face_t	*font_face;
} cairo_5c_font_face_t;

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
extern Type		*typeCairoRgbaColor;
extern Type		*typeCairoPattern;
extern Type		*typeCairoPath;
extern Type		*typeCairoCurveTo;
extern Type		*typeCairoFilter;
extern Type		*typeCairoScaledFont;
extern Type		*typeCairoFontFace;

extern Type		*typeCairoPatternExtend;
extern Type		*typeCairoPatternFilter;

extern Type		*typeCairoImageFormat;

extern Type		*typeRsvgDimensions;
extern Type		*typeRsvgPosition;

/* cairo.c */
    
cairo_5c_t *
cairo_5c_get (Value av);

void
cairo_5c_dirty (cairo_5c_t *c5c);

Value
do_Cairo_create (Value sv);

Value
do_Cairo_destroy (Value cv);

Value
do_Cairo_get_target (Value cv);

/* Error status queries */

Value
do_Cairo_status (Value cv);

Value
do_Cairo_status_to_string (Value sv);

Value
do_Cairo_enable (Value cv);

Value
do_Cairo_disable (Value cv);

/* surface.c */

cairo_5c_surface_t *
cairo_5c_surface_get (Value av);

#if HAVE_CAIRO_XLIB_H
Value
do_Cairo_Surface_create_window (Value namev, Value wv, Value hv);

Value
do_Cairo_Surface_resize_window (Value sv, Value wv, Value hv);
#endif

Value
do_Cairo_Image_surface_create (Value fv, Value wv, Value hv);

Value
do_Cairo_Image_surface_create_from_png (Value fnv);

Value
do_Cairo_Image_surface_create_from_png_file (Value fv);

Value
do_Cairo_Image_get_pixel (Value sv, Value xv, Value yv);

Value
do_Cairo_Image_put_pixel (Value sv, Value xv, Value yv, Value pv);

#if HAVE_CAIRO_PDF_H
Value
do_Cairo_Pdf_surface_create (Value fnv, Value wv, Value hv);

Value
do_Cairo_Pdf_surface_create_for_file (Value fv, Value wv, Value hv);
#endif

#if HAVE_CAIRO_SVG_H
Value
do_Cairo_Svg_surface_create (Value fnv, Value wv, Value hv);

Value
do_Cairo_Svg_surface_create_for_file (Value fv, Value wv, Value hv);
#endif

#if HAVE_CAIRO_PS_H
Value
do_Cairo_Ps_surface_create (Value fnv, Value wv, Value hv);

Value
do_Cairo_Ps_surface_create_for_file (Value fv, Value wv, Value hv);
#endif

Value
do_Cairo_Surface_write_to_png (Value sv, Value fnv);

Value
do_Cairo_Surface_write_to_png_file (Value sv, Value fv);

Value
do_Cairo_Surface_create_similar (Value sv, Value cv, Value wv, Value hv);

Value
do_Cairo_Surface_finish (Value sv);

Value
do_Cairo_Surface_status (Value sv);

Value
do_Cairo_Surface_destroy (Value sv);

Value
do_Cairo_Surface_write_to_png (Value sv, Value fnv);

Value
do_Cairo_Surface_write_to_png_file (Value sv, Value fv);

Value
do_Cairo_Surface_set_device_offset (Value sv, Value xv, Value yv);

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
do_Cairo_identity_matrix (Value cv);

Value
do_Cairo_set_source_rgb (Value cv, Value rv, Value gv, Value bv);

Value
do_Cairo_set_source_rgba (Value cv, Value rv, Value gv, Value bv, Value av);

Value
do_Cairo_set_source_surface (Value cv, Value sv, Value xv, Value yv);

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
do_Cairo_transform (Value cv, Value mv);

Value
do_Cairo_set_matrix (Value cv, Value mv);

Value
do_Cairo_identity_matrix (Value cv);

Value
do_Cairo_user_to_device (Value cv, Value pv);

Value
do_Cairo_user_to_device_distance (Value cv, Value pv);

Value
do_Cairo_device_to_user (Value cv, Value pv);

Value
do_Cairo_device_to_user_distance (Value cv, Value pv);

/* Query functions */

Value
do_Cairo_get_operator (Value cv);

Value
do_Cairo_get_tolerance (Value cv);

Value
do_Cairo_get_current_point (Value cv);

Value
do_Cairo_get_fill_rule (Value cv);

Value
do_Cairo_get_line_width (Value cv);

Value
do_Cairo_get_line_cap (Value cv);

Value
do_Cairo_get_line_join (Value cv);

Value
do_Cairo_get_miter_limit (Value cv);

Value
do_Cairo_get_matrix (Value cv);

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

/* Painting functions */
Value
do_Cairo_paint (Value cv);

Value
do_Cairo_paint_with_alpha (Value cv, Value av);

Value
do_Cairo_mask (Value cv, Value pv);

Value
do_Cairo_mask_surface (Value cv, Value sv, Value xv, Value yv);

Value
do_Cairo_stroke (Value cv);

Value
do_Cairo_stroke_preserve (Value cv);

Value
do_Cairo_fill (Value cv);

Value
do_Cairo_fill_preserve (Value cv);

Value
do_Cairo_copy_page (Value cv);

Value
do_Cairo_show_page (Value cv);

/* Insideness testing */
Value
do_Cairo_in_stroke (Value cv, Value xv, Value yv);

Value
do_Cairo_in_fill (Value cv, Value xv, Value yv);

/* Rectangular extents */
Value
do_Cairo_stroke_extents (Value cv);

Value
do_Cairo_fill_extents (Value cv);

/* Clipping */
Value
do_Cairo_reset_clip (Value cv);

Value
do_Cairo_clip (Value cv);

Value
do_Cairo_clip_preserve (Value cv);

Value
do_Cairo_copy_path (Value cv);

Value
do_Cairo_copy_path_flat (Value cv);

Value
do_Cairo_append_path (Value cv, Value pv);

/* matrix.c */
void
cairo_matrix_part (Value matrixv, cairo_matrix_t *matrix, char *err);

Value
new_cairo_matrix (cairo_matrix_t *matrix);

/* pattern.c */
cairo_pattern_t *
get_cairo_pattern (Value pv);
    
Value
do_Cairo_get_source (Value cv);

Value
do_Cairo_set_source (Value cv, Value patv);

Value
do_Cairo_Pattern_create_for_surface (Value sv);

Value
do_Cairo_Pattern_create_rgb (Value rv, Value gv, Value bv);

Value
do_Cairo_Pattern_create_rgba (Value rv, Value gv, Value bv, Value av);

Value
do_Cairo_Pattern_create_linear (Value x0v, Value y0v, Value x1v, Value y1v);

Value
do_Cairo_Pattern_create_radial (Value cx0v, Value cy0v, Value radius0v,
				Value cx1v, Value cy1v, Value radius1v);

Value
do_Cairo_Pattern_add_color_stop_rgb (Value patv, Value offsetv,
				     Value redv, Value greenv, Value bluev);

Value
do_Cairo_Pattern_add_color_stop_rgba (Value patv, Value offsetv,
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
do_Cairo_select_font_face (Value cv, Value fv, Value sv, Value wv);

Value
do_Cairo_set_font (Value cv, Value fnv);

Value
do_Cairo_set_font_size (Value cv, Value sv);

Value
do_Cairo_set_font_matrix (Value cv, Value mv);

Value
do_Cairo_get_font_matrix (Value cv);

Value
do_Cairo_show_text (Value cv, Value uv);

Value
do_Cairo_font_extents (Value cv);

Value
do_Cairo_set_font_face (Value cv, Value fv);

Value
do_Cairo_text_extents (Value cv, Value uv);

Value
do_Cairo_text_path (Value cv, Value uv);

/* Portable interface to general font features. */

#if 0

/* not actually useful until we expose freetype/fontconfig to nickle apps */
Value
do_Cairo_scaled_font_create (Value fv, Value mv, Value ctm);

Value
do_Cairo_scaled_font_extents (Value sfv);

Value
do_Cairo_scaled_text_extents (Value sfv, Value uv);

#endif

/* gtk.c */

#if HAVE_CAIRO_XLIB_H
Bool
cairo_5c_gui_create (cairo_5c_surface_t *c5s, char *name, int width, int height);

void
cairo_5c_gui_resize (cairo_5c_surface_t *c5s, int width, int height);

void
cairo_5c_gui_destroy (cairo_5c_surface_t *c5s);

void
cairo_5c_gui_free (cairo_5c_surface_t *c5s);

void
cairo_5c_gui_dirty (cairo_5c_surface_t *c5s);

Bool
cairo_5c_gui_disable (cairo_5c_surface_t *c5s);

Bool
cairo_5c_gui_enable (cairo_5c_surface_t *c5s);

void
cairo_5c_gui_check_size (cairo_5c_surface_t *c5s);

Value
cairo_5c_gui_open_event (cairo_5c_surface_t *c5s);

#endif

/* rsvg.c */
Value
do_Rsvg_new_from_string(Value sv);

Value
do_Rsvg_new_from_file(Value fv);

Value
do_Rsvg_render(Value cv, Value rv);

Value
do_Rsvg_render_sub(Value cv, Value rv, Value sv);

Value
do_Rsvg_set_dpi(Value rv, Value dv);

Value
do_Rsvg_set_dpi_x_y(Value rv, Value dx, Value dy);

Value
do_Rsvg_get_dimensions(Value rv);

Value
do_Rsvg_get_dimensions_sub(Value rv, Value sv);

Value
do_Rsvg_get_position_sub(Value rv, Value sv);

Value
do_Rsvg_get_title(Value rv);

Value
do_Rsvg_get_desc(Value rv);

Value
do_Rsvg_get_metadata(Value rv);

#endif /* _CAIRO_5C_H_ */
