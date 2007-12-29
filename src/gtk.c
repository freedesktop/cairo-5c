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

/*
 * This part runs in the gtk thread, so it must not refer to or use
 * any memory managed by nickle.  Called from signal handler with
 * gdk lock held.
 */
static gboolean
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    GdkPixmap		*pixmap;
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;

    c5s->width = widget->allocation.width;
    c5s->height = widget->allocation.height;
    pixmap = gdk_pixmap_new (widget->window,
			     widget->allocation.width,
			     widget->allocation.height,
			     -1);
    gdk_draw_rectangle (pixmap,
			widget->style->white_gc,
			TRUE,
			0, 0,
			widget->allocation.width,
			widget->allocation.height);
    if (c5s->u.window.pixmap)
    {
	gdk_draw_drawable (pixmap, widget->style->white_gc,
			   c5s->u.window.pixmap, 0, 0, 0, 0, 
			   widget->allocation.width,
			   widget->allocation.height);
	gdk_drawable_unref (c5s->u.window.pixmap);
    }
    c5s->u.window.pixmap = pixmap;
    return TRUE;
}

/*
 * Called from signal handler with gdk lock held
 */
static gboolean
expose_event( GtkWidget *widget, GdkEventExpose *event )
{
    cairo_5c_surface_t	*c5s = GTK_DRAWING_AREA (widget)->draw_data;

    
    gdk_draw_pixmap(widget->window,
		    widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		    c5s->u.window.pixmap,
		    event->area.x, event->area.y,
		    event->area.x, event->area.y,
		    event->area.width, event->area.height);
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
    if (c5s->u.window.pixmap)
    {
	gdk_drawable_unref (c5s->u.window.pixmap);
	c5s->u.window.pixmap = NULL;
    }
    c5s->u.window.pixmap = None;
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
	gdk_threads_leave ();
	fprintf (c5s->u.window.send_events, "%d press %d %g %g\n",
		 event->time, event->button, event->x, event->y);
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

/*
 * Called from signal handler with lock held
 */
static void
gtk_repaint (cairo_5c_surface_t *c5s, int x, int y, int w, int h)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    GtkWidget	    *widget = tool->drawing_area;
    GdkPixmap	    *pixmap = c5s->u.window.pixmap;
    
    if (widget && pixmap)
    {
	Window	    xwin = GDK_WINDOW_XID (widget->window);
	Pixmap	    xpix = GDK_PIXMAP_XID (pixmap);
	Display	    *dpy = tool->global->dpy;
	GC	    xgc = GDK_GC_XGC(widget->style->white_gc);
    
	if (xwin && xpix && xgc)
	{
	    if (w == 0)
		w = c5s->width - x;
	    if (h == 0)
		h = c5s->height - y;
	    XCopyArea (dpy, xpix, xwin, xgc, x, y, w, h, x, y);
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
    
	if (tool->disable == 0)
	{
	    tool->dirty = 0;
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

    c5s->u.window.curpix = 0;
    c5s->u.window.pixmap = 0;
    c5s->u.window.send_events = 0;
    c5s->u.window.tool = tool;
    
    if (!width)
	width = XDisplayWidth (dpy, DefaultScreen (dpy)) / 3;
    if (!height)
	height = XDisplayWidth (dpy, DefaultScreen (dpy)) / 3;
    
    tool->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(tool->window),
				 width, height);
    gtk_window_set_title (GTK_WINDOW (tool->window), name);
    
    tool->drawing_area = gtk_drawing_area_new ();
    
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

    gtk_widget_set_events (tool->drawing_area, GDK_EXPOSURE_MASK
			   | GDK_LEAVE_NOTIFY_MASK
			   | GDK_BUTTON_PRESS_MASK
			   | GDK_BUTTON_RELEASE_MASK
			   | GDK_POINTER_MOTION_MASK);

    gtk_widget_realize (tool->window);
    gtk_widget_realize (tool->drawing_area);
    gdk_window_set_back_pixmap (GTK_WIDGET(tool->drawing_area)->window, NULL, FALSE);
    gtk_widget_show (tool->drawing_area);
    gtk_widget_show (tool->window);
    /* create the pixmap */
    configure_event (tool->drawing_area, 0);
    
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
	gdk_drawable_unref (c5s->u.window.pixmap);
	c5s->u.window.pixmap = NULL;
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
    
    if (!tool->dirty)
    {
	tool->dirty = 1;
	if (tool->disable == 0)
	{
	    g_timeout_add (16, gtk_repaint_timeout, c5s);
	}
    }
}

Bool
cairo_5c_tool_disable (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;

    ++tool->disable;
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

    if (!tool->disable)
	return False;
    --tool->disable;
    if (!tool->disable && tool->dirty)
	g_timeout_add (0, gtk_repaint_timeout, c5s);
    return True;
}

Display *
cairo_5c_tool_display (cairo_5c_surface_t *c5s)
{
    cairo_5c_tool_t *tool = c5s->u.window.tool;
    
    return tool->global->dpy;
}

#endif /* HAVE_CAIRO_XLIB_H */
