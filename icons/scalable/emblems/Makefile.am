iconsize=scalable
category=emblems
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =			\
	emblem-busy.svg		\
	emblem-charging.svg	\
	emblem-downloads.svg	\
	emblem-favorite.svg	\
	emblem-locked.svg	\
	emblem-notification.svg	\
	emblem-outofrange.svg	\
	emblem-question.svg	\
	emblem-view-source.svg  \
	emblem-warning.svg	\
	emblem-checked.svg	\
	emblem-unchecked.svg

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
