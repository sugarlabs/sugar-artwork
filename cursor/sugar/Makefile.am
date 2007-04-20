THEMEGEN = $(top_builddir)/cursor/cursorthemegen/cursorthemegen
THEME_DIR = $(top_builddir)/cursor/sugar/theme

all-local: sugar.stamp

sugar_images =			\
	sugar-0.png		\
	sugar-1.png		\
	sugar-2.png		\
	sugar-3.png		\
	sugar-4.png		\
	sugar-5.png		\
	sugar-6.png		\
	sugar-7.png		\
	sugar-8.png              \
	sugar-9.png              \
	sugar-10.png             \
	sugar-11.png             \
	sugar-hotspots.png

sugar.stamp: $(sugar_images) $(THEMEGEN) sugar.cursortheme
	rm -rf sugar &&     	                                \
	BDIR=`pwd` && cd $(srcdir) &&                           \
	$$BDIR/$(THEMEGEN) sugar.cursortheme $$BDIR/theme &&    	\
	cd $$BDIR && touch sugar.stamp

clean-local:
	rm -rf $(THEME_DIR)
	rm -rf $(top_builddir)/art/cursor/sugar/sugar.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/sugar/cursors/
	for i in `cd $(THEME_DIR) && echo *` ; do					\
	  $(INSTALL) $(THEME_DIR)/$$i $(DESTDIR)$(datadir)/icons/sugar/cursors/$$i ;	\
	done

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/icons/sugar/cursors

EXTRA_DIST=$(sugar_images) sugar.cursortheme
