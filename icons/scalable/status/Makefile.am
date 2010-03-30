iconsize=scalable
category=status
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =			\
	audio-volume-high.svg	\
	audio-volume-low.svg	\
	audio-volume-medium.svg	\
	audio-volume-muted.svg	\
	data-download.svg	\
	data-upload.svg		\
	image-missing.svg

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
