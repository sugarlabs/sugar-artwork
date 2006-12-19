THEMEGEN = $(top_builddir)/art/cursor/cursorthemegen/cursorthemegen
THEME_DIR = $(top_builddir)/art/cursor/olpc/theme

all-local: Bluecurve.stamp

bluecurve_images =				\
	Bluecurve-48-hotspot.png		\
	Bluecurve-48-1.png

Bluecurve.stamp: $(bluecurve_images) $(THEMEGEN) Bluecurve.cursortheme
	rm -rf olpc &&     	                                	\
	BDIR=`pwd` && cd $(srcdir) &&                           	\
	$$BDIR/$(THEMEGEN) Bluecurve.cursortheme $$BDIR/theme &&    	\
	cd $$BDIR && touch Bluecurve.stamp

clean-local:
	rm -rf $(THEME_DIR)
	rm -rf $(top_builddir)/art/cursor/olpc/Bluecurve.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/olpc/cursors/
	for i in `cd $(THEME_DIR) && echo *` ; do					\
	  $(INSTALL) $(THEME_DIR)/$$i $(DESTDIR)$(datadir)/icons/olpc/cursors/$$i ;	\
	done

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/icons/olpc/cursors

EXTRA_DIST=$(bluecurve_images) Bluecurve.cursortheme
