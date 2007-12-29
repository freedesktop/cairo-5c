#  $Id$
# 
#  Copyright © 2004 Keith Packard
# 
#  This library is free software; you can redistribute it and/or
#  modify it either under the terms of the GNU Lesser General Public
#  License version 2.1 as published by the Free Software Foundation
#  (the "LGPL") or, at your option, under the terms of the Mozilla
#  Public License Version 1.1 (the "MPL"). If you do not alter this
#  notice, a recipient may use your version of this file under either
#  the MPL or the LGPL.
# 
#  You should have received a copy of the LGPL along with this library
#  in the file COPYING-LGPL-2.1; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#  You should have received a copy of the MPL along with this library
#  in the file COPYING-MPL-1.1
# 
#  The contents of this file are subject to the Mozilla Public License
#  Version 1.1 (the "License"); you may not use this file except in
#  compliance with the License. You may obtain a copy of the License at
#  http://www.mozilla.org/MPL/
# 
#  This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
#  OF ANY KIND, either express or implied. See the LGPL or the MPL for
#  the specific language governing rights and limitations.
# 
#  The Original Code is the cairo graphics library.
# 
#  The Initial Developer of the Original Code is Keith Packard
# 
#  Contributor(s):
#       Keith Packard <keithp@keithp.com>
#

SUBDIRS = examples test

BUILD_DATE=$(shell date +"%F")
NICKLE_VERSION=$(shell nickle -e version | sed 's/"//g')

libcairo5c_la_SOURCES = \
	cairo-5c.h \
	init.c \
	cairo.c \
	draw.c \
	event.c \
	gstate.c \
	gtk.c \
	matrix.c \
	pattern.c \
	surface.c \
	text.c

NICKLEFILES= cairo.5c

DEBIAN = debian/cairo-5c.install debian/libcairo-5c0.install \
	 debian/changelog debian/compat \
	 debian/control debian/copyright debian/docs \
	 debian/rules debian/lintian.override

EXTRA_DIST = $(NICKLEFILES) ChangeLog autogen.sh cairo-5c.3.in $(DEBIAN)

nicklelibdir=$(datadir)/nickle

nicklelib_DATA = $(NICKLEFILES)

lib_LTLIBRARIES = libcairo5c.la

libcairo5c_la_LIBADD = $(CAIRO_5C_LIBS)

AM_CFLAGS=$(WARN_CFLAGS) $(CAIRO_5C_CFLAGS)

man_MANS = cairo-5c.3

cairo-5c.3: cairo-5c.3.in
	sed -e 's,@''NICKLE_VERSION@,$(NICKLE_ERSION),' \
	    -e 's,@''BUILD_DATE@,$(BUILD_DATE),' \
	    -e 's,@''pkgdatadir@,$(pkgdatadir),' $(top_srcdir)/cairo-5c.3.in > $@
	    
TARFILE=$(PACKAGE)-$(VERSION).tar.gz
DEBFILE=$(PACKAGE)_$(VERSION)-1_i386.deb
LIBDEBFILE=libcairo5c_$(VERSION)-1_i386.deb

RELEASE_FILES = $(TARFILE) $(DEBFILE) $(LIBDEBFILE)

debuild: $(DEBFILE) $(LIBDEBFILE)

$(LIBDEBFILE): $(DEBFILE)

$(DEBFILE): $(TARFILE)
	tar xzf $(TARFILE)
	(cd $(distdir) && debuild)

$(TARFILE): dist-gzip

clean-local:
	rm -f cairo-5c.3
