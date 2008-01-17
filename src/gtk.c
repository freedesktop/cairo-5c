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

#if HAVE_CAIRO_XLIB_H

#include <pthread.h>

typedef struct _gtk_global {
    DataType	*data;
    Display	*dpy;
    pthread_t	gtk_thread;
} gtk_global_t;

struct _cairo_5c_tool {
    DataType		*data;
    gtk_global_t	*global;
    int			dirty;
    int			disable;
    GtkWidget		*window;
    GtkWidget		*drawing_area;
};

static gtk_global_t *gtk_global;

static void
allocate_pixmap (cairo_5c_surface_t *c5s)
{
    Pixmap  pixmap;
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    Display	    *dpy = tool->global->dpy;
    GC		    gc = c5s->u.window.gc;
    int		    width = c5s->u.window.new_width;
    int		    height = c5s->u.window.new_height;
    int		    depth = c5s->u.window.depth;

    c5s->width = width;
    c5s->height = height;

    if (!width) width = 1;
    if (!height) height = 1;

    pixmap = XCreatePixmap (dpy, c5s->u.window.root, width, height, depth);
    XFillRectangle (dpy, pixmap, gc, 0, 0, width, height);
    if (c5s->u.window.pixmap)
    {
	XCopyArea (dpy, c5s->u.window.pixmap, pixmap, gc, 0, 0,
		   width, height, 0, 0);
	XFreePixmap (dpy, c5s->u.window.pixmap);
    }
    c5s->u.window.pixmap = pixmap;
    if (c5s->surface)
    {
	cairo_xlib_surface_set_drawable (c5s->surface,
					 pixmap,
					 width,
					 height);
    }
    else
    {
	c5s->surface = cairo_xlib_surface_create (dpy,
						  pixmap,
						  DefaultVisual (dpy, DefaultScreen (dpy)),
						  width,
						  height);
    }
}

/*
 * This part runs in the gtk thread, so it must not refer to or use
 * any memory managed by nickle.  Called from signal handler with
 * gdk lock held.
 */
static gboolean
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;

    c5s->u.window.new_width = event->width;
    c5s->u.window.new_height = event->height;
    if (c5s->u.window.send_events && event)
    {
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d configure %d %d %d %d\n",
		 0, event->x, event->y, event->width, event->height);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return TRUE;
}

/*
 * Called from signal handler with gdk lock held
 */
static gboolean
expose_event( GtkWidget *widget, GdkEventExpose *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    cairo_5c_tool_t	*tool = c5s->u.window.tool;

    if (tool->disable == 0) {
	Display	    *dpy = tool->global->dpy;
	Window	    xwin = GDK_WINDOW_XID (widget->window);
	GC	    gc = c5s->u.window.gc;

	XCopyArea (dpy, c5s->u.window.pixmap, xwin, gc,
			event->area.x, event->area.y,
			event->area.width, event->area.height,
			event->area.x, event->area.y);
	XFlush (dpy);
    }
    return FALSE;
}

/*
 * Called from delete_event with gdk lock held
 */
static void
delete_drawing_area (GtkWidget *widget, gpointer data)
{			   
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    cairo_5c_tool_t	*tool = c5s->u.window.tool;
    
    if (c5s->u.window.send_events)
    {
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d delete\n", 0);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    tool->drawing_area = NULL;
    tool->window = NULL;
}

/*
 * Called from signal handler with lock held
 */
static gboolean
delete_event( GtkWidget *widget, GdkEventAny *event )
{
    gtk_container_foreach (GTK_CONTAINER(widget), delete_drawing_area, NULL);
    return FALSE;
}

/*
 * Called from signal handler with lock held
 */
static gboolean
motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d motion %g %g\n",
		 event->time, event->x, event->y);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}


/*
 * Called from signal handler with lock held
 */
static gboolean
button_press_event( GtkWidget *widget, GdkEventButton *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	char	*extended_type;

	switch (event->type) {
	case GDK_2BUTTON_PRESS:
	    extended_type = "double-";
	    break;
	case GDK_3BUTTON_PRESS:
	    extended_type = "triple-";
	    break;
	default:
	    extended_type = "";
	    break;
	}
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d %spress %d %g %g\n",
		 event->time, extended_type,
		 event->button, event->x, event->y);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}

/*
 * Called from signal handler with lock held
 */
static gboolean
button_release_event( GtkWidget *widget, GdkEventButton *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d release %d %g %g\n",
		 event->time, event->button, event->x, event->y);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}

static gboolean
key_press_event( GtkWidget *widget, GdkEventKey *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	gchar	*string = event->string;
	int	n = event->length;
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d key-down %s ",
		 event->time, gdk_keyval_name (event->keyval));
	while (n--)
	    fprintf (c5s->u.window.send_events, "%02x", *string++);
        fprintf (c5s->u.window.send_events, "\n");
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}

static gboolean
key_release_event( GtkWidget *widget, GdkEventKey *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	gchar	*string = event->string;
	int	n = event->length;
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d key-up %s ",
		 event->time, gdk_keyval_name (event->keyval));
	while (n--)
	    fprintf (c5s->u.window.send_events, "%02x", *string++);
        fprintf (c5s->u.window.send_events, "\n");
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}

static gboolean
focus_in_event ( GtkWidget *widget, GdkEventFocus *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d focus-in\n",
		 0);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}

static gboolean
focus_out_event ( GtkWidget *widget, GdkEventFocus *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5s->u.window.send_events)
    {
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d focus-out\n",
		 0);
	fflush (c5s->u.window.send_events);
	gdk_threads_enter ();
    }
    return FALSE;
}

/*
 * Called from signal handler with lock held
 */
static void
gtk_repaint (cairo_5c_surface_t *c5s, int x, int y, int w, int h)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    GtkWidget	    *widget = tool->drawing_area;
    Pixmap	    pixmap;
    GC		    gc;
    
    if (widget && 
	(c5s->u.window.new_width != c5s->width ||
	 c5s->u.window.new_height != c5s->height))
    {
	allocate_pixmap (c5s);
    }
    
    pixmap = c5s->u.window.pixmap;
    gc = c5s->u.window.gc;

    if (widget && pixmap && gc)
    {
	Window	    xwin = GDK_WINDOW_XID (widget->window);
	Display	    *dpy = tool->global->dpy;
    
	if (xwin)
	{
	    if (w == 0)
		w = c5s->width - x;
	    if (h == 0)
		h = c5s->height - y;
	    tool->dirty = 0;
	    XCopyArea (dpy, pixmap, xwin, gc, x, y, w, h, x, y);
	    XFlush (dpy);
	}
    }
}

/*
 * Called from timeout with gdk lock not held
 */
static gboolean
gtk_repaint_timeout (gpointer data)
{
    gdk_threads_enter ();
    {
	cairo_5c_surface_t	*c5s = data;
	cairo_5c_tool_t	*tool = c5s->u.window.tool;
    
	if (tool->disable == 0 && tool->dirty)
	{
	    gtk_repaint (c5s, 0, 0, 0, 0);
	}
    }
    gdk_threads_leave ();
    return FALSE;
}

static void *
gtk_thread_main (void *closure)
{
    gdk_threads_enter ();
    gtk_main ();
    gdk_threads_leave ();
    return 0;
}

/*
 * Manage the gtk_global object, starting the thread and such
 */
static void
gtk_global_mark (void *object)
{
}

static int
gtk_global_free (void *object)
{
    gtk_global_t    *gg = object;

    gdk_threads_enter ();
    gtk_main_quit ();
    if (gg == gtk_global)
	gtk_global = NULL;
    gdk_threads_leave ();
    return 1;
}

static DataType	gtk_global_type = { gtk_global_mark, gtk_global_free, "GtkGlobal" };

static gtk_global_t *
create_gtk_global (void)
{
    ENTER ();
    static int	been_here = 0;
    static int	argc = 1;
    static char	*args[] = { "nickle", 0 };
    static char **argv = args;
    gtk_global_t    *gg;

    if (!been_here)
    {
	XInitThreads ();
	g_thread_init (NULL);
	gdk_threads_init ();
	been_here = 1;
    }
    gdk_threads_enter ();
    if (!gtk_init_check (&argc, &argv))
    {
	int err = errno;
	const char *display_name_arg = gdk_get_display_arg_name ();
	RaiseStandardException (exception_open_error,
				"cannot open X display",
				2, FileGetError (err), NewStrString (display_name_arg));
	RETURN (Void);
    }
	
    gg = ALLOCATE (&gtk_global_type, sizeof (gtk_global_t));
    
    gg->dpy = gdk_x11_get_default_xdisplay ();
    
    pthread_create (&gg->gtk_thread, 0, gtk_thread_main, gg);
    if (!gtk_global)
	gtk_global = gg;
    gdk_threads_leave ();
    RETURN (gg);
}

/*
 * manage a the gtk piece of an xlib surface
 */

static void
gtk_tool_mark (void *object)
{
    cairo_5c_tool_t *tool = object;

    MemReference (tool->global);
}

/*
 * Called from nickle with gdk lock not held
 */
static int
gtk_tool_free (void *object)
{
    cairo_5c_tool_t *tool = object;

    gdk_threads_enter ();
    if (tool->window)
    {
	gtk_widget_destroy (tool->window);
	tool->window = NULL;
	tool->drawing_area = NULL;
    }
    gdk_threads_leave ();
    return 1;
}

static DataType gtk_tool_type = { gtk_tool_mark, gtk_tool_free, "GtkTool" };

/*
 * Called from nickle with gdk lock not held
 */

Bool
cairo_5c_tool_create (cairo_5c_surface_t *c5s, char *name, int width, int height)
{
    ENTER ();
    gtk_global_t    *gg = gtk_global ? gtk_global : create_gtk_global ();
    cairo_5c_tool_t *tool;
    Display	    *dpy;
    XGCValues	    gcv;
    
    if (aborting)
    {
	EXIT ();
	return False;
    }
    tool = ALLOCATE (&gtk_tool_type, sizeof (cairo_5c_tool_t));
    
    gdk_threads_enter ();
    dpy = gg->dpy;
    
    tool->global = gg;
    tool->dirty = 0;
    tool->disable = 0;
    
    c5s->dirty = False;
    c5s->recv_events = Void;

    c5s->u.window.pixmap = None;
    c5s->u.window.send_events = 0;
    c5s->u.window.tool = tool;
    c5s->u.window.root = RootWindow (dpy, DefaultScreen (dpy));
    
    if (!width)
	width = XDisplayWidth (dpy, DefaultScreen (dpy)) / 3;
    if (!height)
	height = XDisplayWidth (dpy, DefaultScreen (dpy)) / 3;
    
    tool->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(tool->window),
				 width, height);
    gtk_window_set_title (GTK_WINDOW (tool->window), name);
    
    tool->drawing_area = gtk_drawing_area_new ();
    
    GTK_WIDGET_SET_FLAGS (tool->drawing_area, GTK_CAN_FOCUS);
    
    GTK_DRAWING_AREA (tool->drawing_area)->draw_data = c5s;
    
    gtk_container_add (GTK_CONTAINER(tool->window), tool->drawing_area);
    
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "expose_event",
		      (GtkSignalFunc) expose_event, NULL);
    g_signal_connect (GTK_OBJECT(tool->drawing_area),"configure_event",
		      (GtkSignalFunc) configure_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->window), "delete_event",
		      (GtkSignalFunc) delete_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "motion_notify_event",
		      (GtkSignalFunc) motion_notify_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "button_press_event",
		      (GtkSignalFunc) button_press_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "button_release_event",
		      (GtkSignalFunc) button_release_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "key_press_event",
		      (GtkSignalFunc) key_press_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "key_release_event",
		      (GtkSignalFunc) key_release_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "focus_in_event",
		      (GtkSignalFunc) focus_in_event, NULL);
    g_signal_connect (GTK_OBJECT (tool->drawing_area), "focus_out_event",
		      (GtkSignalFunc) focus_out_event, NULL);

    gtk_widget_set_double_buffered (tool->drawing_area, FALSE);
    
    gtk_widget_set_events (tool->drawing_area, GDK_EXPOSURE_MASK
			   | GDK_LEAVE_NOTIFY_MASK
			   | GDK_BUTTON_PRESS_MASK
			   | GDK_BUTTON_RELEASE_MASK
			   | GDK_POINTER_MOTION_MASK
			   | GDK_KEY_PRESS_MASK
			   | GDK_KEY_RELEASE_MASK 
			   | GDK_FOCUS_CHANGE_MASK );

    gtk_widget_realize (tool->window);
    gtk_widget_realize (tool->drawing_area);
    gdk_window_set_back_pixmap (GTK_WIDGET(tool->drawing_area)->window, NULL, FALSE);
    gtk_widget_show (tool->drawing_area);
    gtk_widget_show (tool->window);
    gtk_widget_grab_focus (tool->drawing_area);
    
    gcv.foreground = 0xffffffff;
    gcv.graphics_exposures = False;
    c5s->u.window.gc = XCreateGC (dpy, 
				  GDK_WINDOW_XID (tool->drawing_area->window),
				  GCForeground | GCGraphicsExposures,
				  &gcv);

    /* create the pixmap */
    c5s->u.window.new_width = tool->drawing_area->allocation.width;
    c5s->u.window.new_height = tool->drawing_area->allocation.height;
    c5s->u.window.depth = gdk_drawable_get_depth (GDK_DRAWABLE (tool->drawing_area->window));
    allocate_pixmap (c5s);
    
    gdk_threads_leave ();
    EXIT ();
    return True;
}

/*
 * called from nickle with the gdk lock not held
 */
Bool
cairo_5c_tool_destroy (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    
    gdk_threads_enter ();
    gtk_widget_hide (tool->window);
    if (c5s->u.window.pixmap)
    {
	XFreePixmap (tool->global->dpy, c5s->u.window.pixmap);
	c5s->u.window.pixmap = None;
    }
    if (c5s->u.window.gc)
    {
	XFreeGC (tool->global->dpy, c5s->u.window.gc);
	c5s->u.window.gc = NULL;
    }
    gdk_threads_leave ();
    /* let nickle allocator free it */
    return True;
}

void
cairo_5c_tool_mark (cairo_5c_surface_t *c5s)
{
    MemReference (c5s->u.window.tool);
}

void
cairo_5c_tool_dirty (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    
    gdk_threads_enter ();
    if (!tool->dirty)
    {
	tool->dirty = 1;
	if (tool->disable == 0)
	    g_timeout_add (16, gtk_repaint_timeout, c5s);
    }
    gdk_threads_leave ();
}

void
cairo_5c_tool_check_size (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    
    gdk_threads_enter ();
    if (tool->disable == 0 && 
	(c5s->u.window.new_width != c5s->width ||
	 c5s->u.window.new_height != c5s->height))
	allocate_pixmap (c5s);
    gdk_threads_leave ();
}

Bool
cairo_5c_tool_disable (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;

    gdk_threads_enter ();
    ++tool->disable;
    gdk_threads_leave ();
    return True;
}

/*
 * Called from nickle with gdk lock not held.  As no gtk/gdk
 * functions are called, I don't think we need to grab it.
 */
Bool
cairo_5c_tool_enable (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    Bool	    was_disabled;

    gdk_threads_enter ();
    was_disabled = tool->disable != 0;
    if (was_disabled)
    {
	--tool->disable;
	if (tool->disable == 0 && tool->dirty)
	{
	    gtk_repaint (c5s, 0, 0, 0, 0);
	    tool->dirty = 0;
//	    g_timeout_add (0, gtk_repaint_timeout, c5s);
	}
    }
    gdk_threads_leave ();
    return was_disabled;
}

Display *
cairo_5c_tool_display (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    
    return tool->global->dpy;
}

#endif /* HAVE_CAIRO_XLIB_H */
