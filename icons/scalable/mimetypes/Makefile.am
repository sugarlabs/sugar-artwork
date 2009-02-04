iconsize=scalable
category=mimetypes
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =					\
	application-octet-stream.svg		\
	application-x-generic.svg		\
	application-x-squeak-project.svg	\
	audio-x-generic.svg			\
	image-x-generic.svg			\
	text-uri-list.svg			\
	text-x-generic.svg			\
	video-x-generic.svg

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
