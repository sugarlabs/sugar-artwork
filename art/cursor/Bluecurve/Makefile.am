THEMEGEN = $(top_builddir)/art/cursor/cursorthemegen/cursorthemegen

all-local: Bluecurve.stamp

bluecurve_images =				\
	Bluecurve-24-hotspot.png		\
	Bluecurve-24-1.png			\
	Bluecurve-24-2.png			\
	Bluecurve-24-3.png			\
	Bluecurve-24-4.png			\
	Bluecurve-24-5.png			\
	Bluecurve-24-6.png			\
	Bluecurve-24-7.png			\
	Bluecurve-24-8.png			\
	Bluecurve-24-9.png			\
	Bluecurve-24-10.png			\
	Bluecurve-24-11.png			\
	Bluecurve-24-12.png			\
	Bluecurve-36-hotspot.png		\
	Bluecurve-36-1.png			\
	Bluecurve-48-hotspot.png		\
	Bluecurve-48-1.png

Bluecurve.stamp: $(bluecurve_images) $(THEMEGEN) Bluecurve.cursortheme
	rm -rf Bluecurve &&					\
	BDIR=`pwd` && cd $(srcdir) &&				\
	$$BDIR/$(THEMEGEN) Bluecurve.cursortheme $$BDIR/Bluecurve &&	\
	cd $$BDIR && touch Bluecurve.stamp

clean:
	rm -rf Bluecurve

install-data-local:
	rm -rf $(datadir)/icons/Bluecurve/cursors
	mkdir -p $(datadir)/icons/Bluecurve/cursors
	for i in `cd Bluecurve && echo *` ; do						\
	  if test -L Bluecurve/$$i ; then						\
	    cp -d Bluecurve/$$i $(datadir)/icons/Bluecurve/cursors/$$i ;		\
	  else										\
	    install -m 0644 Bluecurve/$$i $(datadir)/icons/Bluecurve/cursors/$$i ;	\
          fi ;										\
	done

EXTRA_DIST=$(bluecurve_images) Bluecurve.cursortheme
