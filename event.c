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
#include <errno.h>

Value
do_Cairo_open_event (Value cv)
{
    ENTER ();
    cairo_5c_t	*c5c = get_cairo_5c (cv);
    Value	read;
    int		fd[2];
    FILE	*write;

    if (aborting)
	RETURN(Void);
    if (!c5c->recv_events)
    {
	if (pipe (fd) < 0)
	{
	    int err = errno;
	    RaiseStandardException (exception_open_error,
				    FileGetErrorMessage (err),
				    2, FileGetError (err), Void);
	    RETURN (Void);
	}
	read = FileCreate (fd[0], FileReadable);
	if (aborting)
	    RETURN(Void);
	write = fdopen (fd[1], "w");
	c5c->recv_events = read;
	c5c->u.window.x->send_events = write;
    }
    RETURN (c5c->recv_events);
}
