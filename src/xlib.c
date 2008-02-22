/*
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

#include <poll.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <X11/Xutil.h>

typedef struct _x_repaint {
    struct _x_repaint	*next;
    int			when;
    cairo_5c_surface_t	*c5s;
} x_repaint_t;

typedef struct _x_global {
    Display	*dpy;
    int		ref_count;
    int		running;
    int		pipe[2];
    pthread_t	x_thread;
    XContext	context;
    x_repaint_t	*repaint;
    XAtom	wm_delete_window;
    XAtom	wm_protocols;
} x_global_t;

struct _cairo_5c_gui {
    x_global_t	    *global;
    Pixmap	    pixmap;
    Window	    window;
    GC		    gc;
    Window	    root;
    int		    dirty;
    int		    disable;
    int		    depth;
    int		    new_width;
    int		    new_height;
    FILE	    *send_events;
};

static x_global_t *x_global;

static void
allocate_pixmap (cairo_5c_surface_t *c5s)
{
    Pixmap  pixmap;
    cairo_5c_gui_t *gui = c5s->u.window.gui;
    Display	    *dpy = gui->global->dpy;
    GC		    gc = gui->gc;
    int		    width = gui->new_width;
    int		    height = gui->new_height;
    int		    depth = gui->depth;

    c5s->width = width;
    c5s->height = height;

    if (!width) width = 1;
    if (!height) height = 1;

    pixmap = XCreatePixmap (dpy, gui->root, width, height, depth);
    XFillRectangle (dpy, pixmap, gc, 0, 0, width, height);
    if (gui->pixmap)
    {
	XCopyArea (dpy, gui->pixmap, pixmap, gc, 0, 0,
		   width, height, 0, 0);
	XFreePixmap (dpy, gui->pixmap);
    }
    gui->pixmap = pixmap;
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

static cairo_5c_surface_t *
get_window_surface (x_global_t *xg, Window wid)
{
    XPointer	pointer;
    if (XFindContext (xg->dpy, wid, xg->context, &pointer) == 0)
	return (cairo_5c_surface_t *) pointer;
    return NULL;
}

static void
set_window_surface (x_global_t *xg, Window wid, cairo_5c_surface_t *c5s)
{
    if (c5s == NULL)
	XDeleteContext (xg->dpy, wid, xg->context);
    else
	XSaveContext (xg->dpy, wid, xg->context, (XPointer) c5s);
}

/*
 * This part runs in the X thread, so it must not refer to or use
 * any memory managed by nickle.
 */
static void
configure_event (x_global_t *xg, XConfigureEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window); 
    cairo_5c_gui_t	*gui;

    if (!c5s)
	return;
    gui = c5s->u.window.gui;
    gui->new_width = event->width;
    gui->new_height = event->height;
    if (gui->send_events)
    {
	fprintf (gui->send_events, "%d configure %d %d %d %d\n",
		 0, event->x, event->y, event->width, event->height);
	fflush (gui->send_events);
    }
}

/*
 * Called from signal handler with gdk lock held
 */
static void
expose_event (x_global_t *xg, XExposeEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window);
    cairo_5c_gui_t	*gui;
    
    if (!c5s)
	return;
    gui = c5s->u.window.gui;

    if (gui->disable == 0) {
	Display	    *dpy = gui->global->dpy;

	XCopyArea (dpy, gui->pixmap, gui->window,
		   gui->gc,
		   event->x, event->y,
		   event->width, event->height,
		   event->x, event->y);
	XFlush (dpy);
    }
}

/*
 * Called from delete_event with gdk lock held
 */
static void
delete_drawing_area (x_global_t *xg, cairo_5c_surface_t *c5s)
{			   
    cairo_5c_gui_t	*gui = c5s->u.window.gui;
    if (gui->send_events)
    {
	fprintf (gui->send_events, "%d delete\n", 0);
	fflush (gui->send_events);
    }
}

static void
client_message_event (x_global_t *xg, XClientMessageEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window);
    
    if (!c5s)
	return;
    if (event->message_type == xg->wm_protocols && event->format == 32)
    {
	if (event->data.l[0] == xg->wm_delete_window)
	    delete_drawing_area (xg, c5s);
    }
}

/*
 * Called from signal handler with lock held
 */
static void
motion_notify_event (x_global_t *xg, XMotionEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window);
    cairo_5c_gui_t	*gui;
    
    if (!c5s)
	return;
    gui = c5s->u.window.gui;
    if (gui->send_events)
    {
	fprintf (gui->send_events, "%d motion %d %d\n",
		 (int) event->time, event->x, event->y);
	fflush (gui->send_events);
    }
}


/*
 * Called from signal handler with lock held
 */
static void
button_event (x_global_t *xg, XButtonEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window);
    cairo_5c_gui_t	*gui;
    
    if (!c5s)
	return;
    gui = c5s->u.window.gui;
    if (gui->send_events)
    {
	char	*type = event->type == ButtonPress ? "press" : "release";
	char	*extended_type = "";

	fprintf (gui->send_events, "%d %s%s %d %d %d\n",
		 (int) event->time, extended_type, type,
		 event->button, event->x, event->y);
	fflush (gui->send_events);
    }
}

static void
key_event (x_global_t *xg, XKeyEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window);
    cairo_5c_gui_t	*gui;
    
    if (!c5s)
	return;
    gui = c5s->u.window.gui;
    if (gui->send_events)
    {
	char	*type = event->type == KeyPress ? "down" : "up";
	char	buffer[128];
	int	n, i;
	KeySym	sym;

	n = XLookupString (event, buffer, sizeof (buffer), &sym, NULL);
	fprintf (gui->send_events, "%d key-%s %s ",
		 (int) event->time, type, XKeysymToString (sym));
	for (i = 0; i < n; i++)
	    fprintf (gui->send_events, "%02x", buffer[i]);
        fprintf (gui->send_events, "\n");
	fflush (gui->send_events);
    }
}

static void
focus_change_event (x_global_t *xg, XFocusChangeEvent *event)
{
    cairo_5c_surface_t	*c5s = get_window_surface (xg, event->window);
    cairo_5c_gui_t	*gui;
    
    if (!c5s)
	return;
    gui = c5s->u.window.gui;
    if (gui->send_events)
    {
	char	*type = event->type == FocusIn ? "in" : "out";
	fprintf (gui->send_events, "%d focus-%s\n",
		 0, type);
	fflush (gui->send_events);
    }
}

/*
 * Called from signal handler with lock held
 */
static void
repaint (cairo_5c_surface_t *c5s, int x, int y, int w, int h)
{
    cairo_5c_gui_t  *gui = c5s->u.window.gui;
    if (gui->window && 
	(gui->new_width != c5s->width ||
	 gui->new_height != c5s->height))
    {
	allocate_pixmap (c5s);
    }
    
    if (gui->window && gui->pixmap && gui->gc)
    {
	Display	    *dpy = gui->global->dpy;
    
	if (w == 0)
	    w = c5s->width - x;
	if (h == 0)
	    h = c5s->height - y;
	gui->dirty = 0;
	XCopyArea (dpy, gui->pixmap, gui->window, gui->gc,
		   x, y, w, h, x, y);
	XFlush (dpy);
    }
}

static void
repaint_timeout (x_global_t *xg, int when)
{
    x_repaint_t	*xr;
    
    while ((xr = xg->repaint) && xr->when - when <= 0)
    {
	cairo_5c_surface_t  *c5s = xr->c5s;
	cairo_5c_gui_t	    *gui = c5s->u.window.gui;
	if (gui->disable == 0 && gui->dirty)
	    repaint (c5s, 0, 0, 0, 0);
	xg->repaint = xr->next;
	free (xr);
    }
}

static int
now (void)
{
#if HAVE_CLOCK_GETTIME
    struct timespec tp;

    clock_gettime (CLOCK_MONOTONIC, &tp);
    return (int) (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
#else
    struct timeval  tv;

    gettimeofday (&tv, NULL);
    return (int) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

static void *
x_thread_main (void *closure)
{
    x_global_t	    *xg = closure;
    XEvent	    event;
    struct pollfd   fds[2];
    int		    timeout;
    sigset_t	    mask;

    sigemptyset (&mask);
    sigaddset (&mask, SIGALRM);
    sigaddset (&mask, SIGIO);
    sigaddset (&mask, SIGCHLD);
    sigaddset (&mask, SIGINT);
    pthread_sigmask (SIG_BLOCK, &mask, NULL);
    
    fds[0].fd = ConnectionNumber (xg->dpy);
    fds[0].events = POLLIN;
    fds[1].fd = xg->pipe[0];
    fds[1].events = POLLIN;
    while (xg->running) {
	while (XPending (xg->dpy))
	{
	    XNextEvent (xg->dpy, &event);
	    switch (event.type) {
	    case ConfigureNotify:
		configure_event (xg, &event.xconfigure);
		break;
	    case Expose:
		expose_event (xg, &event.xexpose);
		break;
	    case MotionNotify:
		motion_notify_event (xg, &event.xmotion);
		break;
	    case ButtonPress:
	    case ButtonRelease:
		button_event (xg, &event.xbutton);
		break;
	    case KeyPress:
	    case KeyRelease:
		key_event (xg, &event.xkey);
		break;
	    case ClientMessage:
		client_message_event (xg, &event.xclient);
		break;
	    case FocusIn:
	    case FocusOut:
		focus_change_event (xg, &event.xfocus);
		break;
	    }
	}
	timeout = -1;
	while (xg->repaint)
	{
	    int when = now ();
	    timeout = xg->repaint->when - when;
	    if (timeout > 0)
		break;
	    timeout = -1;
	    repaint_timeout (xg, when);
	}
	poll (fds, 2, timeout);
	if (fds[1].revents & POLLIN) {
	    char    stuffed[128];
	    read (fds[1].fd, stuffed, sizeof (stuffed));
	}
    }
    close (xg->pipe[0]);
    close (xg->pipe[1]);
    XCloseDisplay (xg->dpy);
    free (xg);
    return 0;
}

static void
x_thread_wakeup (x_global_t *xg)
{
    write (xg->pipe[1], "\n", 1);
}

static void
x_schedule_repaint (cairo_5c_surface_t *c5s, int delta)
{
    cairo_5c_gui_t  *gui = c5s->u.window.gui;
    x_global_t	    *xg = gui->global;
    x_repaint_t	    *xr, **prev;

    for (xr = xg->repaint; xr; xr = xr->next)
	if (xr->c5s == c5s)
	    return;
    
    xr = malloc (sizeof (x_repaint_t));
    xr->when = now () + delta;
    xr->c5s = c5s;
    for (prev = &xg->repaint; (*prev); prev = &(*prev)->next)
    {
	if ((*prev)->when > xr->when)
	    break;
    }
    xr->next = *prev;
    *prev = xr;
    x_thread_wakeup (xg);
}

/*
 * Manage the x_global object, starting the thread and such
 */

static int
x_global_unref (x_global_t *xg)
{
    if (--xg->ref_count <= 0)
    {
	xg->running = 0;
	x_thread_wakeup (xg);
	if (xg == x_global)
	    x_global = NULL;
    }
    return 1;
}

static x_global_t *
x_global_create (void)
{
    static int	been_here = 0;
    Display	*dpy;
    x_global_t	*xg;

    if (!been_here)
    {
	XInitThreads ();
	been_here = 1;
    }
    
    if (x_global)
	return x_global;

    dpy = XOpenDisplay (NULL);
    
    if (!dpy)
    {
	int err = errno;
	const char *display_name_arg = XDisplayName (NULL);
	RaiseStandardException (exception_open_error,
				"cannot open X display",
				2, FileGetError (err), NewStrString (display_name_arg));
	return NULL;
    }
	
    xg = malloc (sizeof (x_global_t));
    
    xg->ref_count = 0;
    xg->dpy = dpy;
    xg->running = 1;
    xg->repaint = NULL;
    pipe (xg->pipe);
    
    pthread_create (&xg->x_thread, 0, x_thread_main, xg);
    x_global = xg;
    return xg;
}

/*
 * create the underlying gui object for a window surface
 */

Bool
cairo_5c_gui_create (cairo_5c_surface_t *c5s, char *name, int width, int height)
{
    ENTER ();
    x_global_t		    *xg;
    cairo_5c_gui_t	    *gui;
    Display		    *dpy;
    XGCValues		    gcv;
    int			    screen;
    XSizeHints		    sizeHints;
    XWMHints		    wmHints;
    XClassHint		    classHints;
    XSetWindowAttributes    attr;
    
    xg = x_global_create ();

    if (aborting)
    {
	EXIT ();
	return False;
    }

    dpy = xg->dpy;
    screen = DefaultScreen (dpy);
    
    if (!width)
	width = XDisplayWidth (dpy, screen) / 3;
    if (!height)
	height = XDisplayWidth (dpy, screen) / 3;

    gui = malloc (sizeof (cairo_5c_gui_t));
    
    xg->ref_count++;
    gui->global = xg;
    gui->pixmap = None;

    gui->root = RootWindow (dpy, screen);
    gui->dirty = 0;
    gui->disable = 0;
    gui->depth = DefaultDepth (dpy, screen);
    
    gui->new_width = width;
    gui->new_height = height;
    
    gui->send_events = NULL;
    
    attr.background_pixmap = None;
    attr.event_mask = (KeyPressMask|
		       KeyReleaseMask|
		       ButtonPressMask|
		       ButtonReleaseMask|
		       PointerMotionMask|
		       EnterWindowMask|
		       LeaveWindowMask|
		       ExposureMask|
		       StructureNotifyMask|
		       FocusChangeMask);
    
    gui->window = XCreateWindow (dpy, gui->root,
				 0, 0, gui->new_width, gui->new_height, 0,
				 gui->depth,
				 InputOutput, CopyFromParent,
				 CWEventMask|CWBackPixmap, &attr);

    gcv.foreground = 0xffffffff;
    gcv.graphics_exposures = False;
    gui->gc = XCreateGC (dpy, gui->window,
			 GCForeground | GCGraphicsExposures,
			 &gcv);
    
    set_window_surface (xg, gui->window, c5s);
    
    sizeHints.flags = 0;
    wmHints.flags = InputHint;
    wmHints.input = True;
    classHints.res_name = name;
    classHints.res_class = name;
    
    xg->wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    xg->wm_protocols = XInternAtom (dpy, "WM_PROTOCOLS", False);

    Xutf8SetWMProperties (dpy, gui->window, name, name,
			  NULL, 0, &sizeHints, &wmHints, &classHints);
    XSetWMProtocols (dpy, gui->window, &xg->wm_delete_window, 1);
			  
    XMapWindow (dpy, gui->window);

    c5s->u.window.gui = gui;
    
    /* create the pixmap */
    allocate_pixmap (c5s);
    
    EXIT ();
    return True;
}

/*
 * called when the client destroys a gui surface
 */
void
cairo_5c_gui_destroy (cairo_5c_surface_t *c5s)
{
    cairo_5c_gui_t *gui = c5s->u.window.gui;

    if (!gui)
	return;

    if (gui->window)
    {
	Window	wid = gui->window;
	gui->window = None;
	XDestroyWindow (gui->global->dpy, wid);
	XFlush (gui->global->dpy);
    }
}

/*
 * Called when a gui surface is freed
 */

void
cairo_5c_gui_free (cairo_5c_surface_t *c5s)
{
    cairo_5c_gui_t *gui = c5s->u.window.gui;
    cairo_5c_gui_destroy (c5s);

    if (!gui)
	return;
    if (gui->pixmap)
    {
	Pixmap pid = gui->pixmap;
	gui->pixmap = None;
	XFreePixmap (gui->global->dpy, pid);
    }
    if (gui->gc)
    {
	GC  gc = gui->gc;
	gui->gc = NULL;
	XFreeGC (gui->global->dpy, gc);
    }
    if (gui->global)
    {
	x_global_unref (gui->global);
	gui->global = NULL;
    }
    if (gui->send_events)
    {
	fclose (gui->send_events);
	gui->send_events = NULL;
    }
    free (gui);
}

void
cairo_5c_gui_dirty (cairo_5c_surface_t *c5s)
{
    cairo_5c_gui_t  *gui = c5s->u.window.gui;

    if (!gui->dirty)
    {
	gui->dirty = 1;
	if (gui->disable == 0)
	    x_schedule_repaint (c5s, 16);
    }
}

void
cairo_5c_gui_check_size (cairo_5c_surface_t *c5s)
{
    cairo_5c_gui_t *gui = c5s->u.window.gui;
    
    if (gui->disable == 0 && 
	(gui->new_width != c5s->width ||
	 gui->new_height != c5s->height))
	allocate_pixmap (c5s);
}

Bool
cairo_5c_gui_disable (cairo_5c_surface_t *c5s)
{
    cairo_5c_gui_t *gui = c5s->u.window.gui;

    ++gui->disable;
    return True;
}

/*
 * Called from nickle with gdk lock not held.  As no gtk/gdk
 * functions are called, I don't think we need to grab it.
 */
Bool
cairo_5c_gui_enable (cairo_5c_surface_t *c5s)
{
    cairo_5c_gui_t *gui = c5s->u.window.gui;
    Bool	    was_disabled;

    was_disabled = gui->disable != 0;
    if (was_disabled)
    {
	--gui->disable;
	if (gui->disable == 0 && gui->dirty)
	{
	    repaint (c5s, 0, 0, 0, 0);
	    gui->dirty = 0;
//	    g_timeout_add (0, gtk_repaint_timeout, c5s);
	}
    }
    return was_disabled;
}

Value
cairo_5c_gui_open_event (cairo_5c_surface_t *c5s)
{
    ENTER ();
    cairo_5c_gui_t *gui = c5s->u.window.gui;
    int		    fd[2];
    Value	    read;
    int		    err;

    if (pipe (fd) < 0)
    {
	err = errno;
	RaiseStandardException (exception_open_error,
				FileGetErrorMessage (err),
				2, FileGetError (err), Void);
	RETURN (Void);
    }
    read = FileCreate (fd[0], FileReadable);
    if (aborting)
	RETURN(Void);
    gui->send_events = fdopen (fd[1], "w");
    RETURN(read);
}

#endif /* HAVE_CAIRO_XLIB_H */
