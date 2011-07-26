all-local: sugar.stamp

sugar_images =			\
	sugar-00.png		\
	sugar-01.png		\
	sugar-02.png		\
	sugar-03.png		\
	sugar-04.png		\
	sugar-05.png		\
	sugar-06.png		\
	sugar-07.png		\
	sugar-08.png            \
	sugar-09.png            \
	sugar-10.png            \
	sugar-11.png            \
	sugar-hotspots.png

IMAGE_DIR = $(top_srcdir)/cursor/sugar
OUTPUT_DIR = $(top_builddir)/cursor/sugar
CURSORS_DIR = $(OUTPUT_DIR)/cursors

sugar.stamp: $(sugar_images) $(THEMEGEN) sugar.cursortheme
	$(ICON_SLICER) --image-dir=$(IMAGE_DIR) --output-dir=$(OUTPUT_DIR) $(IMAGE_DIR)/sugar.cursortheme
	touch sugar.stamp

clean-local:
	rm -rf cursors
	rm -f sugar.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/sugar/cursors/
	for i in `cd $(CURSORS_DIR) && echo *` ; do						\
		$(INSTALL) -m u=rw,go=r $(CURSORS_DIR)/$$i $(DESTDIR)$(datadir)/icons/sugar/cursors/$$i ;	\
	done

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/icons/sugar/cursors

EXTRA_DIST=$(sugar_images) sugar.cursortheme
