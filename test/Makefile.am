NICKLEFILES=\
	test.5c

REFPNG=\
	append-path-ref.png \
	box-ref.png \
	circle-ref.png \
	clip-ref.png \
	fill-extents-ref.png \
	fill-preserve-ref.png \
	line-ref.png \
	paint-ref.png \
	set-operator-ref.png \
	set-source-ref.png \
	set-tolerance-ref.png \
	stroke-extents-ref.png \
	stroke-preserve-ref.png

TESTS_ENVIRONMENT=SRCDIR="$(srcdir)" NICKLEPATH="$(top_srcdir):@NICKLE_PATH@" LD_LIBRARY_PATH=../src/.libs nickle

TESTS=test.5c

EXTRA_DIST=$(NICKLEFILES) $(REFPNG)
