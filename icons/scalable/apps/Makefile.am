iconsize=scalable
category=apps
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =			\
	activity-journal.svg

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
