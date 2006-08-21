THEMEGEN = $(top_builddir)/art/cursor/cursorthemegen/cursorthemegen
THEME_DIR = $(top_builddir)/art/cursor/Bluecurve/theme

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
	rm -rf Bluecurve &&                                     	\
	BDIR=`pwd` && cd $(srcdir) &&                           	\
	$$BDIR/$(THEMEGEN) Bluecurve.cursortheme $$BDIR/theme &&    	\
	cd $$BDIR && touch Bluecurve.stamp

clean-local:
	rm -rf $(THEME_DIR)
	rm -rf $(top_builddir)/art/cursor/Bluecurve/Bluecurve.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/Bluecurve/cursors/
	for i in `cd Bluecurve && echo *` ; do							\
	  $(INSTALL) $(THEME_DIR)/$$i $(DESTDIR)$(datadir)/icons/Bluecurve/cursors/$$i ;	\
	done

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/icons/Bluecurve/cursors

EXTRA_DIST=$(bluecurve_images) Bluecurve.cursortheme
