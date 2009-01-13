iconsize=scalable
category=control
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =				\
	control-popup-arrow.svg		\
	color-preview.svg

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
