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

#define GTK_DISABLE_DEPRECATED
#include "cairo-5c.h"
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <pthread.h>


typedef struct _cairo_5c_gtk {
    cairo_5c_x_t    x;
    int		    dirty;
    int		    disable;
    GtkWidget	    *window;
    GtkWidget	    *drawing_area;
    GdkPixmap	    *pixmap;
} cairo_5c_gtk_t;

static gboolean
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    GdkPixmap		*pixmap;
    cairo_5c_gtk_t	*c5cg = GTK_DRAWING_AREA (widget)->draw_data;

    c5cg->x.width = widget->allocation.width;
    c5cg->x.height = widget->allocation.height;
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
    if (c5cg->pixmap)
    {
	gdk_draw_drawable (pixmap, widget->style->white_gc,
			   c5cg->pixmap, 0, 0, 0, 0, 
			   widget->allocation.width,
			   widget->allocation.height);
	gdk_pixmap_unref (c5cg->pixmap);
    }
    c5cg->pixmap = pixmap;
    c5cg->x.pixmap = GDK_PIXMAP_XID (GDK_DRAWABLE(c5cg->pixmap));
    return TRUE;
}

static gboolean
expose_event( GtkWidget *widget, GdkEventExpose *event )
{
    cairo_5c_gtk_t	*c5cg = GTK_DRAWING_AREA (widget)->draw_data;
    
    gdk_draw_pixmap(widget->window,
		    widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		    c5cg->pixmap,
		    event->area.x, event->area.y,
		    event->area.x, event->area.y,
		    event->area.width, event->area.height);

    return FALSE;
}

static gboolean
motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
    cairo_5c_gtk_t	*c5cg = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5cg->x.send_events)
    {
	fprintf (c5cg->x.send_events, "%d motion %g %g\n",
		 event->time, event->x, event->y);
	fflush (c5cg->x.send_events);
    }
    return FALSE;
}


static gboolean
button_press_event( GtkWidget *widget, GdkEventButton *event )
{
    cairo_5c_gtk_t	*c5cg = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5cg->x.send_events)
    {
	fprintf (c5cg->x.send_events, "%d press %d %g %g\n",
		 event->time, event->button, event->x, event->y);
	fflush (c5cg->x.send_events);
    }
    return FALSE;
}

static gboolean
button_release_event( GtkWidget *widget, GdkEventButton *event )
{
    cairo_5c_gtk_t	*c5cg = GTK_DRAWING_AREA (widget)->draw_data;
    
    if (c5cg->x.send_events)
    {
	fprintf (c5cg->x.send_events, "%d release %d %g %g\n",
		 event->time, event->button, event->x, event->y);
	fflush (c5cg->x.send_events);
    }
    return FALSE;
}


static cairo_5c_gtk_t *
cairo_5c_window_new (int width, int height)
{
    cairo_5c_gtk_t	*c5cg = malloc (sizeof (cairo_5c_gtk_t));
    Display		*dpy;

    c5cg->pixmap = 0;
    c5cg->x.send_events = 0;
    c5cg->x.dpy = gdk_x11_get_default_xdisplay ();
    dpy = c5cg->x.dpy;
    if (!width)
	width = XDisplayWidth (dpy, DefaultScreen (dpy)) / 3;
    if (!height)
	height = XDisplayWidth (dpy, DefaultScreen (dpy)) / 3;
    c5cg->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(c5cg->window),
				 width, height);
    
    c5cg->drawing_area = gtk_drawing_area_new ();
    
    GTK_DRAWING_AREA (c5cg->drawing_area)->draw_data = c5cg;
    
    gtk_container_add (GTK_CONTAINER(c5cg->window), c5cg->drawing_area);
    
    g_signal_connect (GTK_OBJECT (c5cg->drawing_area), "expose_event",
		      (GtkSignalFunc) expose_event, NULL);
    g_signal_connect (GTK_OBJECT(c5cg->drawing_area),"configure_event",
		      (GtkSignalFunc) configure_event, NULL);
    g_signal_connect (GTK_OBJECT (c5cg->drawing_area), "motion_notify_event",
		      (GtkSignalFunc) motion_notify_event, NULL);
    g_signal_connect (GTK_OBJECT (c5cg->drawing_area), "button_press_event",
		      (GtkSignalFunc) button_press_event, NULL);
    g_signal_connect (GTK_OBJECT (c5cg->drawing_area), "button_release_event",
		      (GtkSignalFunc) button_release_event, NULL);

    gtk_widget_set_events (c5cg->drawing_area, GDK_EXPOSURE_MASK
			   | GDK_LEAVE_NOTIFY_MASK
			   | GDK_BUTTON_PRESS_MASK
			   | GDK_BUTTON_RELEASE_MASK
			   | GDK_POINTER_MOTION_MASK);

    gtk_widget_realize (c5cg->window);
    gtk_widget_realize (c5cg->drawing_area);
    gdk_window_set_back_pixmap (GTK_WIDGET(c5cg->drawing_area)->window, NULL, FALSE);
    gtk_widget_show (c5cg->drawing_area);
    gtk_widget_show (c5cg->window);
    configure_event (c5cg->drawing_area, 0);
    
    return c5cg;
}

void
repaint_x (cairo_5c_x_t *c5cx, int x, int y, int w, int h)
{
    cairo_5c_gtk_t  *c5cg = (cairo_5c_gtk_t *) c5cx;
    GtkWidget	    *widget = c5cg->drawing_area;
    Window	    xwin = GDK_WINDOW_XID (widget->window);
    GdkPixmap	    *pixmap = c5cg->pixmap;
    Pixmap	    xpix = GDK_PIXMAP_XID (pixmap);
    Display	    *dpy = gdk_x11_get_default_xdisplay ();
    GC		    xgc = GDK_GC_XGC(widget->style->white_gc);

    if (xwin && xpix && xgc)
    {
	if (w == 0)
	    w = c5cg->x.width - x;
	if (h == 0)
	    h = c5cg->x.height - y;
	XCopyArea (dpy, xpix, xwin, xgc, x, y, w, h, x, y);
	XFlush (dpy);
    }
}

static gboolean
gtk_repaint_timeout (gpointer data)
{
    cairo_5c_gtk_t  *c5cg = data;

    if (c5cg->disable == 0)
    {
	c5cg->dirty = 0;
	repaint_x (&c5cg->x, 0, 0, 0, 0);
    }
    return FALSE;
}

static void *
gtk_thread_main (void *closure)
{
    gtk_main ();
    return 0;
}

static cairo_5c_gtk_t *c5cg;

cairo_5c_x_t *
start_x (int width, int height)
{
    pthread_t	gtk_thread;
    static int	argc = 1;
    static char	*args[] = { "nickle", 0 };
    static char **argv = args;

    if (!c5cg)
    {
	XInitThreads ();
	g_thread_init (NULL);
	gdk_threads_init ();
	gtk_init (&argc, &argv);
	c5cg = cairo_5c_window_new (width, height);
    
	pthread_create (&gtk_thread, 0, gtk_thread_main, c5cg);
    }
    return &c5cg->x;
}

void
dirty_x (cairo_5c_x_t *c5cx, int x, int y, int w, int h)
{
    cairo_5c_gtk_t  *c5cg = (cairo_5c_gtk_t *) c5cx;

    if (!c5cg->dirty)
    {
	c5cg->dirty = 1;
	if (c5cg->disable == 0)
	{
	    gdk_threads_enter ();
	    g_timeout_add (16, gtk_repaint_timeout, c5cg);
	    gdk_threads_leave ();
	}
    }
}

Bool
disable_x (cairo_5c_x_t *c5cx)
{
    cairo_5c_gtk_t  *c5cg = (cairo_5c_gtk_t *) c5cx;

    ++c5cg->disable;
    return True;
}

Bool
enable_x  (cairo_5c_x_t *c5cx)
{
    cairo_5c_gtk_t  *c5cg = (cairo_5c_gtk_t *) c5cx;

    if (!c5cg->disable)
	return False;
    --c5cg->disable;
    if (!c5cg->disable && c5cg->dirty)
    {
	gdk_threads_enter ();
	g_timeout_add (0, gtk_repaint_timeout, c5cg);
	gdk_threads_leave ();
    }
    return True;
}
