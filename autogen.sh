#! /bin/sh
autoreconf -Wall -v --install || exit 1
./configure --enable-maintainer-mode "$@"
