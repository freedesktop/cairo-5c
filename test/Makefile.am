NICKLEFILES=\
	test.5c

REFPNG=\
	append-path-ref.png \
	box-ref.png \
	circle-ref.png \
	clip-ref.png \
	create-rgba-ref.png \
	create-rgb-ref.png \
	fill-extents-ref.png \
	fill-preserve-ref.png \
	line-ref.png \
	mask-ref.png \
	paint-ref.png \
	paint-with-alpha-ref.png \
	rotate-ref.png \
	set-fill-rule-ref.png \
	set-font-ref.png \
	set-font-matrix-ref.png \
	set-font-size-ref.png \
	set-line-cap-ref.png \
	set-line-join-ref.png \
	set-line-width-ref.png \
	set-miter-limit-ref.png \
	set-operator-ref.png \
	set-source-ref.png \
	set-tolerance-ref.png \
	show-text-ref.png \
	stroke-extents-ref.png \
	stroke-preserve-ref.png \
	text-extents-ref.png \
	text-path-ref.png

TESTS_ENVIRONMENT=SRCDIR="$(srcdir)" NICKLEPATH="$(top_srcdir):$(top_srcdir)/chrome:@NICKLE_PATH@" LD_LIBRARY_PATH=../src/.libs

TESTS=test.5c

EXTRA_DIST=$(NICKLEFILES) $(REFPNG)

clean-local:
	rm -f *-out.png

set-ref: test-suite.log
	for ref in $(REFPNG); do \
		out=`echo $$ref | sed 's/-ref.png/-out.png/'`; \
		echo cp $$out $$ref; \
		cp $$out $$ref; \
	done
