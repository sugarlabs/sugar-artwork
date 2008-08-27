iconsize=scalable
category=categories
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =			\
	preferences-system.svg

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
