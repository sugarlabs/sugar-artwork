iconsize=scalable
category=actions
icondir = $(datadir)/icons/sugar/$(iconsize)/$(category)

icon_DATA =						\
	activity-start.svg				\
	activity-stop.svg				\
	cell-format.svg					\
	cell-height.svg					\
	cell-size.svg					\
	cell-width.svg					\
	column-insert.svg				\
	column-remove.svg				\
	dialog-cancel.svg				\
	dialog-ok.svg					\
	document-save.svg				\
	document-send.svg				\
	document-open.svg				\
	edit-copy.svg					\
	edit-delete.svg					\
	edit-description.svg				\
	edit-duplicate.svg				\
	edit-paste.svg					\
	edit-redo.svg					\
	edit-undo.svg					\
	edit-clear.svg					\
	format-columns-double.svg			\
	format-columns-single.svg			\
	format-columns-triple.svg			\
	format-justify-center.svg			\
	format-justify-fill.svg				\
	format-justify-left.svg				\
	format-justify-right.svg			\
	format-text-bold.svg				\
	format-text-italic.svg				\
	format-text-leading.svg				\
	format-text-size.svg				\
	format-text-underline.svg			\
	go-down.svg					\
	go-home.svg					\
	go-left.svg					\
	go-next-paired.svg				\
	go-next.svg					\
	go-previous-paired.svg				\
	go-previous.svg					\
	go-right.svg					\
	go-up.svg					\
	insert-image.svg				\
	insert-table.svg				\
	invite.svg					\
	list-add.svg					\
	list-remove.svg					\
	media-eject-insensitive.svg			\
	media-eject.svg					\
	media-playback-pause-insensitive.svg		\
	media-playback-pause.svg			\
	media-playback-start-insensitive.svg		\
	media-playback-start.svg			\
	media-playback-stop-insensitive.svg		\
	media-playback-stop.svg				\
	media-playlist-repeat-insensitive.svg		\
	media-playlist-repeat.svg			\
	media-playlist-shuffle-insensitive.svg		\
	media-playlist-shuffle.svg			\
	media-record-insensitive.svg			\
	media-record.svg				\
	media-seek-backward-insensitive.svg		\
	media-seek-backward.svg				\
	media-seek-forward-insensitive.svg		\
	media-seek-forward.svg				\
	row-insert.svg					\
	row-remove.svg					\
	select-all.svg					\
	select-none.svg					\
	system-logout.svg				\
	system-restart.svg				\
	system-search.svg				\
	system-shutdown.svg				\
	toolbar-colors.svg				\
	toolbar-edit.svg				\
	toolbar-view.svg				\
	toolbar-help.svg				\
	toolbar-social-help.svg				\
	toolbar-social-help-animated.svg		\
	tray-hide.svg					\
	tray-show.svg					\
	tray-favourite.svg				\
	transfer-from-application-octet-stream.svg	\
	transfer-from-audio-x-generic.svg		\
	transfer-from-image-x-generic.svg		\
	transfer-from.svg				\
	transfer-from-text-uri-list.svg			\
	transfer-from-text-x-generic.svg		\
	transfer-from-video-x-generic.svg		\
	transfer-to-application-octet-stream.svg	\
	transfer-to-audio-x-generic.svg			\
	transfer-to-image-x-generic.svg			\
	transfer-to.svg					\
	transfer-to-text-uri-list.svg			\
	transfer-to-text-x-generic.svg			\
	transfer-to-video-x-generic.svg			\
	view-box.svg					\
	view-created.svg				\
	view-details.svg				\
	view-freeform.svg				\
	view-fullscreen.svg				\
	view-lastedit.svg				\
	view-list.svg					\
	view-radial.svg					\
	view-refresh.svg				\
	view-return.svg					\
	view-size.svg					\
	view-spiral.svg					\
	view-source.svg					\
	view-triangle.svg				\
	view-type.svg					\
	zoom-activity.svg 				\
	zoom-best-fit.svg				\
	zoom-groups.svg					\
	zoom-home.svg					\
	zoom-in.svg					\
	zoom-neighborhood.svg				\
	zoom-original.svg				\
	zoom-out.svg					\
	zoom-to-width.svg				\
	entry-cancel.svg				\
	entry-search.svg				\
	entry-stop.svg					\
	entry-refresh.svg


EXTRA_DIST = $(icon_DATA)

# install aliases for icons, and the use the icon-naming-utils to install
# further aliases for compatibility with gtk stock icon names.
install-data-local: install-iconDATA
	ln -sf activity-start.svg $(DESTDIR)$(icondir)/document-open.svg
	ln -sf activity-stop.svg $(DESTDIR)$(icondir)/application-exit.svg
	ln -sf dialog-ok.svg $(DESTDIR)$(icondir)/dialog-apply.svg
	ln -sf dialog-ok.svg $(DESTDIR)$(icondir)/gtk-ok.svg
	ln -sf media-playback-stop.svg $(DESTDIR)$(icondir)/process-stop.svg
	(cd $(DESTDIR)$(icondir)/.. && $(ICONMAP) -c $(category))
