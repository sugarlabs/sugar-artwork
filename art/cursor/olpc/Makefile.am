THEMEGEN = $(top_builddir)/art/cursor/cursorthemegen/cursorthemegen
THEME_DIR = $(top_builddir)/art/cursor/olpc/theme

all-local: olpc.stamp

olpc_images =			\
	olpc-0.png		\
	olpc-1.png		\
	olpc-2.png		\
	olpc-3.png		\
	olpc-4.png		\
	olpc-5.png		\
	olpc-6.png		\
	olpc-7.png		\
	olpc-8.png              \
	olpc-9.png              \
	olpc-hotspots.png

olpc.stamp: $(olpc_images) $(THEMEGEN) olpc.cursortheme
	rm -rf olpc &&     	                                \
	BDIR=`pwd` && cd $(srcdir) &&                           \
	$$BDIR/$(THEMEGEN) olpc.cursortheme $$BDIR/theme &&    	\
	cd $$BDIR && touch olpc.stamp

clean-local:
	rm -rf $(THEME_DIR)
	rm -rf $(top_builddir)/art/cursor/olpc/olpc.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/olpc/cursors/
	for i in `cd $(THEME_DIR) && echo *` ; do					\
	  $(INSTALL) $(THEME_DIR)/$$i $(DESTDIR)$(datadir)/icons/olpc/cursors/$$i ;	\
	done

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/icons/olpc/cursors

EXTRA_DIST=$(olpc_images) olpc.cursortheme
