iconsize=scalable
category=device
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =				\
	battery-000.icon		\
	battery-000.svg			\
	battery-010.icon		\
	battery-010.svg			\
	battery-020.icon		\
	battery-020.svg			\
	battery-030.icon		\
	battery-030.svg			\
	battery-040.icon		\
	battery-040.svg			\
	battery-050.icon		\
	battery-050.svg			\
	battery-060.icon		\
	battery-060.svg			\
	battery-070.icon		\
	battery-070.svg			\
	battery-080.icon		\
	battery-080.svg			\
	battery-090.icon		\
	battery-090.svg			\
	battery-100.icon		\
	battery-100.svg			\
	battery-charging-100.svg	\
	battery-charging-000.svg	\
	battery-charging-010.svg	\
	battery-charging-020.svg	\
	battery-charging-030.svg	\
	battery-charging-040.svg	\
	battery-charging-050.svg	\
	battery-charging-060.svg	\
	battery-charging-070.svg	\
	battery-charging-080.svg	\
	battery-charging-090.svg	\
	battery-charging-100.svg	\
	camera-external.svg		\
	camera.svg			\
	computer-xo.svg			\
	media-disk.svg			\
	media-flash-sd-mmc.svg		\
	media-flash-usb.svg		\
	microphone.svg			\
	network-mesh.svg		\
	network-mesh.icon		\
	network-wired.svg		\
	network-wireless-000.svg	\
	network-wireless-000.icon	\
	network-wireless-020.svg	\
	network-wireless-020.icon	\
	network-wireless-040.svg	\
	network-wireless-040.icon	\
	network-wireless-060.svg	\
	network-wireless-060.icon	\
	network-wireless-080.svg	\
	network-wireless-080.icon	\
	network-wireless-100.svg	\
	network-wireless-100.icon	\
	printer.svg			\
	speaker-000.svg			\
	speaker-033.svg			\
	speaker-066.svg			\
	speaker-100.svg			\
	speaker-muted-000.svg		\
	speaker-muted-033.svg		\
	speaker-muted-066.svg		\
	speaker-muted-100.svg		

EXTRA_DIST = $(icon_DATA)

install-data-local: install-iconDATA
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
