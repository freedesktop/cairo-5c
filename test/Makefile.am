NICKLEFILES=\
	test.5c

REFPNG=\
	box-ref.png \
	circle-ref.png \
	line-ref.png

TESTS_ENVIRONMENT=SRCDIR="$(srcdir)" NICKLEPATH="$(top_srcdir):@NICKLE_PATH@" LD_LIBRARY_PATH=../src/.libs nickle

TESTS=test.5c

EXTRA_DIST=$(NICKLEFILES) $(REFPNG)
