iconsize=scalable
category=actions
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =						\
	activity-start.svg			\
	activity-stop.svg			\
	cell-format.svg				\
	cell-height.svg				\
	cell-size.svg				\
	cell-width.svg				\
	column-insert.svg			\
	column-remove.svg			\
	dialog-cancel.svg			\
	dialog-ok.svg				\
	document-save.svg			\
	edit-copy.svg				\
	edit-delete.svg				\
	edit-paste.svg				\
	edit-redo.svg				\
	edit-undo.svg				\
	format-columns-double.svg	\
	format-columns-single.svg	\
	format-columns-triple.svg	\
	format-justify-center.svg	\
	format-justify-fill.svg		\
	format-justify-left.svg		\
	format-justify-right.svg	\
	format-text-bold.svg		\
	format-text-italic.svg		\
	format-text-leading.svg		\
	format-text-size.svg		\
	format-text-underline.svg	\
	go-down.svg			\
	go-left.svg					\
	go-next-paired.svg			\
	go-next.svg					\
	go-previous-paired.svg		\
	go-previous.svg				\
	go-right.svg				\
	go-up.svg			\
	insert-image.svg			\
	insert-table.svg			\
	invite.svg					\
	list-add.svg				\
	list-remove.svg				\
	media-eject-insensitive.svg				\
	media-eject.svg							\
	media-playback-pause-insensitive.svg	\
	media-playback-pause.svg				\
	media-playback-start-insensitive.svg	\
	media-playback-start.svg				\
	media-playback-stop-insensitive.svg		\
	media-playback-stop.svg					\
	media-playlist-repeat-insensitive.svg	\
	media-playlist-repeat.svg				\
	media-playlist-shuffle-insensitive.svg	\
	media-playlist-shuffle.svg				\
	media-record-insensitive.svg			\
	media-record.svg						\
	media-seek-backward-insensitive.svg		\
	media-seek-backward.svg					\
	media-seek-forward-insensitive.svg		\
	media-seek-forward.svg		\
	row-insert.svg				\
	row-remove.svg				\
	system-restart.svg			\
	system-search.svg			\
	system-shutdown.svg			\
	tray-hide.svg				\
	tray-show.svg				\
	view-freeform.svg			\
	view-fullscreen.svg			\
	view-list.svg				\
	view-radial.svg				\
	view-refresh.svg			\
	view-return.svg				\
	zoom-activity.svg 			\
	zoom-best-fit.svg			\
	zoom-groups.svg				\
	zoom-home.svg				\
	zoom-in.svg					\
	zoom-neighborhood.svg		\
	zoom-original.svg			\
	zoom-out.svg

EXTRA_DIST = $(icon_DATA)

# install aliases for icons, and the use the icon-naming-utils to install
# further aliases for compatibility with gtk stock icon names.
install-data-local: install-iconDATA
	ln -s activity-start.svg $(DESTDIR)$(icondir)/document-open.svg
	ln -s activity-stop.svg $(DESTDIR)$(icondir)/application-exit.svg
	ln -s dialog-ok.svg $(DESTDIR)$(icondir)/dialog-apply.svg
	ln -s media-playback-stop.svg $(DESTDIR)$(icondir)/process-stop.svg
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
