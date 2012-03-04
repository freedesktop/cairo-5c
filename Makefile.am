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

SUBDIRS = src nichrome test examples

BUILD_DATE=$(shell date +"%F")
NICKLE_VERSION=$(shell nickle -e version | sed 's/"//g')

RELEASE_UPLOAD_HOST =   cairographics.org
RELEASE_UPLOAD_BASE =	/srv/cairo.freedesktop.org/www
RELEASE_UPLOAD_DIR =	$(RELEASE_UPLOAD_BASE)/releases

NICKLEFILES= cairo.5c

DEBIAN = debian/cairo-5c.install debian/libcairo5c-0.install \
	 debian/changelog debian/compat \
	 debian/control debian/copyright debian/docs \
	 debian/rules

EXTRA_DIST = $(NICKLEFILES) ChangeLog autogen.sh cairo-5c.3.in $(DEBIAN)

MAINTAINERCLEANFILES=ChangeLog

.PHONY: ChangeLog

ChangeLog:
	(GIT_DIR=$(top_srcdir)/.git git log > .changelog.tmp && mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) || \
	(touch ChangeLog; echo 'git directory not found: installing possibly empty changelog.' >&2)

DISTCHECK_CONFIGURE_FLAGS="--with-nickle-libdir='$${datadir}/nickle'"

nicklelibdir= $(NICKLE_LIBDIR)

nicklelib_DATA = $(NICKLEFILES)

man_MANS = cairo-5c.3

cairo-5c.3: cairo-5c.3.in
	sed -e 's,@''NICKLE_VERSION@,$(NICKLE_ERSION),' \
	    -e 's,@''BUILD_DATE@,$(BUILD_DATE),' \
	    -e 's,@''pkgdatadir@,$(pkgdatadir),' $(top_srcdir)/cairo-5c.3.in > $@
	    
TARFILE=$(PACKAGE)-$(VERSION).tar.gz
DEBFILE=$(PACKAGE)_$(VERSION)-1_i386.deb
LIBDEBFILE=libcairo5c_$(VERSION)-1_i386.deb
SHA1_FILE=$(TARFILE).sha1
GPG_FILE=$(SHA1_FILE).asc

RELEASE_FILES = $(TARFILE) $(SHA1_FILE) $(GPG_FILE) $(DEBFILE) $(LIBDEBFILE)

debuild: $(DEBFILE) $(LIBDEBFILE)

$(LIBDEBFILE): $(DEBFILE)

$(DEBFILE): $(TARFILE)
	tar xzf $(TARFILE)
	(cd $(distdir) && pdebuild --buildresult `pwd`)

$(TARFILE): dist-gzip

clean-local:
	rm -f cairo-5c.3

$(SHA1_FILE): $(TAR_FILE)
	sha1sum $^ > $@

$(GPG_FILE): $(SHA1_FILE)
	@echo "Please enter your GPG password to sign the checksum."
	gpg --armor --sign $^ 

release: $(RELEASE_FILES)

release-upload: $(TARFILE) $(SHA1_FILE) $(GPG_FILE)
	mkdir -p releases
	scp $(TARFILE) $(SHA1_FILE) $(GPG_FILE) $(RELEASE_UPLOAD_HOST):$(RELEASE_UPLOAD_DIR) 
	mv $(TARFILE) $(SHA1_FILE) $(GPG_FILE) releases
