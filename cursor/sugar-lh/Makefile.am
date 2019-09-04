all-local: sugar.stamp

sugar_images =			\
	pngs/based_arrow_up.png	\
	pngs/left_ptr_watch_6.png	\
	pngs/top_side.png		\
	pngs/busy_8.png		\
	pngs/left_ptr_modify.png	\
	pngs/sb_up_arrow.png		\
	pngs/forbidden.png		\
	pngs/paintbrush.png		\
	pngs/busy_0.png		\
	pngs/left_ptr_watch_4.png	\
	pngs/question_arrow.png	\
	pngs/left_ptr_watch_1.png	\
	pngs/crosshair.png		\
	pngs/circle.png		\
	pngs/left_ptr_watch_5.png	\
	pngs/link.png		\
	pngs/left_ptr_watch_2.png	\
	pngs/based_arrow_left.png	\
	pngs/left_ptr_watch_9.png	\
	pngs/minus.png		\
	pngs/center_ptr.png		\
	pngs/busy_6.png		\
	pngs/xterm.png		\
	pngs/eraser.png		\
	pngs/left_ptr_watch_3.png	\
	pngs/bottom_right_corner.png	\
	pngs/sb_down_arrow.png	\
	pngs/busy_3.png		\
	pngs/busy_9.png		\
	pngs/hand1.png		\
	pngs/paint-bucket.png	\
	pngs/bottom_left_corner.png	\
	pngs/based_arrow_right.png	\
	pngs/left_ptr_watch_8.png	\
	pngs/busy_2.png		\
	pngs/fleur.png		\
	pngs/sb_right_arrow.png	\
	pngs/plus.png		\
	pngs/right_ptr.png		\
	pngs/based_arrow_down.png	\
	pngs/left_ptr_watch_0.png	\
	pngs/busy_5.png		\
	pngs/busy_11.png		\
	pngs/busy_7.png		\
	pngs/left_ptr_watch_10.png	\
	pngs/close.png		\
	pngs/sb_left_arrow.png	\
	pngs/exchange.png		\
	pngs/sb_h_double_arrow.png	\
	pngs/busy_10.png		\
	pngs/hand_closed.png		\
	pngs/hand2.png		\
	pngs/hand_open.png		\
	pngs/color-picker.png	\
	pngs/right_side.png		\
	pngs/left_ptr_watch_7.png	\
	pngs/left_ptr_watch_11.png	\
	pngs/pencil.png		\
	pngs/busy_4.png		\
	pngs/sb_v_double_arrow.png	\
	pngs/busy_1.png		\
	pngs/left_ptr.png

cursor_instructions =		\
	hand_closed.in		\
	sb_h_double_arrow.in	\
	top_side.in		\
	minus.in		\
	center_ptr.in		\
	plus.in			\
	forbidden.in		\
	busy.in			\
	left_ptr_watch.in	\
	paintbrush.in		\
	color-picker.in		\
	sb_up_arrow.in		\
	eraser.in		\
	crosshair.in		\
	sb_v_double_arrow.in	\
	right_ptr.in		\
	sb_down_arrow.in	\
	paint-bucket.in		\
	close.in		\
	pencil.in		\
	sb_left_arrow.in	\
	question_arrow.in	\
	based_arrow_right.in	\
	fleur.in		\
	hand_open.in		\
	sb_right_arrow.in	\
	hand2.in		\
	based_arrow_down.in	\
	bottom_right_corner.in	\
	right_side.in		\
	xterm.in		\
	link.in			\
	based_arrow_left.in	\
	based_arrow_up.in	\
	left_ptr_modify.in	\
	exchange.in		\
	left_ptr.in		\
	circle.in		\
	bottom_left_corner.in	\
	hand1.in

IMAGE_DIR = $(top_srcdir)/cursor/sugar-lh
OUTPUT_DIR = $(top_builddir)/cursor/sugar-lh
CURSORS_DIR = $(OUTPUT_DIR)/cursors

sugar.stamp: $(sugar_images) $(cursor_instructions) $(THEMEGEN)
	mkdir -p $(CURSORS_DIR)
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/hand_closed.in $(CURSORS_DIR)/hand_closed
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/sb_h_double_arrow.in $(CURSORS_DIR)/sb_h_double_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/top_side.in $(CURSORS_DIR)/top_side
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/minus.in $(CURSORS_DIR)/minus
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/center_ptr.in $(CURSORS_DIR)/center_ptr
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/plus.in $(CURSORS_DIR)/plus
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/forbidden.in $(CURSORS_DIR)/forbidden
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/busy.in $(CURSORS_DIR)/busy
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/left_ptr_watch.in $(CURSORS_DIR)/left_ptr_watch
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/paintbrush.in $(CURSORS_DIR)/paintbrush
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/color-picker.in $(CURSORS_DIR)/color-picker
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/sb_up_arrow.in $(CURSORS_DIR)/sb_up_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/eraser.in $(CURSORS_DIR)/eraser
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/crosshair.in $(CURSORS_DIR)/crosshair
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/sb_v_double_arrow.in $(CURSORS_DIR)/sb_v_double_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/right_ptr.in $(CURSORS_DIR)/right_ptr
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/sb_down_arrow.in $(CURSORS_DIR)/sb_down_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/paint-bucket.in $(CURSORS_DIR)/paint-bucket
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/close.in $(CURSORS_DIR)/close
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/pencil.in $(CURSORS_DIR)/pencil
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/sb_left_arrow.in $(CURSORS_DIR)/sb_left_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/question_arrow.in $(CURSORS_DIR)/question_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/based_arrow_right.in $(CURSORS_DIR)/based_arrow_right
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/fleur.in $(CURSORS_DIR)/fleur
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/hand_open.in $(CURSORS_DIR)/hand_open
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/sb_right_arrow.in $(CURSORS_DIR)/sb_right_arrow
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/hand2.in $(CURSORS_DIR)/hand2
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/based_arrow_down.in $(CURSORS_DIR)/based_arrow_down
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/bottom_right_corner.in $(CURSORS_DIR)/bottom_right_corner
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/right_side.in $(CURSORS_DIR)/right_side
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/xterm.in $(CURSORS_DIR)/xterm
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/link.in $(CURSORS_DIR)/link
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/based_arrow_left.in $(CURSORS_DIR)/based_arrow_left
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/based_arrow_up.in $(CURSORS_DIR)/based_arrow_up
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/left_ptr_modify.in $(CURSORS_DIR)/left_ptr_modify
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/exchange.in $(CURSORS_DIR)/exchange
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/left_ptr.in $(CURSORS_DIR)/left_ptr
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/circle.in $(CURSORS_DIR)/circle
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/bottom_left_corner.in $(CURSORS_DIR)/bottom_left_corner
	xcursorgen -p $(IMAGE_DIR) $(IMAGE_DIR)/hand1.in $(CURSORS_DIR)/hand1
	ln -s left_ptr $(CURSORS_DIR)/default
	ln -s left_ptr $(CURSORS_DIR)/top_left_arrow
	ln -s right_ptr $(CURSORS_DIR)/top_right_arrow
	ln -s right_ptr $(CURSORS_DIR)/arrow
	ln -s right_ptr $(CURSORS_DIR)/draft_small
	ln -s right_ptr $(CURSORS_DIR)/draft_large
	ln -s right_side $(CURSORS_DIR)/left_side
	ln -s top_side $(CURSORS_DIR)/bottom_side
	ln -s top_side $(CURSORS_DIR)/double_arrow
	ln -s bottom_left_corner $(CURSORS_DIR)/top_right_corner
	ln -s bottom_right_corner $(CURSORS_DIR)/top_left_corner
	ln -s forbidden $(CURSORS_DIR)/unavailable
	ln -s plus $(CURSORS_DIR)/dnd_copy
	ln -s left_ptr $(CURSORS_DIR)/dnd_move
	ln -s question_arrow $(CURSORS_DIR)/dnd_ask
	ln -s left_ptr $(CURSORS_DIR)/dnd_link
	ln -s left_ptr $(CURSORS_DIR)/dnd_none
	ln -s plus $(CURSORS_DIR)/dnd-copy
	ln -s left_ptr $(CURSORS_DIR)/dnd-move
	ln -s question_arrow $(CURSORS_DIR)/dnd-ask
	ln -s left_ptr $(CURSORS_DIR)/dnd-link
	ln -s left_ptr $(CURSORS_DIR)/dnd-none
	ln -s plus $(CURSORS_DIR)/copy
	ln -s plus $(CURSORS_DIR)/add
	ln -s minus $(CURSORS_DIR)/remove
	ln -s close $(CURSORS_DIR)/pirate
	ln -s close $(CURSORS_DIR)/destroy
	ln -s crosshair $(CURSORS_DIR)/cross
	ln -s color-picker $(CURSORS_DIR)/eyedropper
	ln -s left_ptr_watch $(CURSORS_DIR)/08e8e1c95fe2fc01f976f1e063a24ccd
	ln -s question_arrow $(CURSORS_DIR)/5c6cd98b3f3ebcb1f9c7f1c204630408
	ln -s hand_closed $(CURSORS_DIR)/208530c400c041818281048008011002
	ln -s hand_open $(CURSORS_DIR)/5aca4d189052212118709018842178c0
	ln -s plus $(CURSORS_DIR)/08ffe1cb5fe6fc01f906f1c063814ccf
	ln -s dnd_copy $(CURSORS_DIR)/1081e37283d90000800003c07f3ef6bf
	ln -s dnd_move $(CURSORS_DIR)/9081237383d90e509aa00f00170e968f
	ln -s dnd_none $(CURSORS_DIR)/1001208387f90000800003000700f6ff
	ln -s color-picker $(CURSORS_DIR)/90b3018312820b127e5a0b7c8034a301
	ln -s left_ptr_watch $(CURSORS_DIR)/watch
	touch sugar.stamp

clean-local:
	rm -rf cursors
	rm -f sugar.stamp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/icons/sugar-lh/cursors/
	for i in `cd $(CURSORS_DIR) && echo *` ; do						\
		$(INSTALL) -m u=rw,go=r $(CURSORS_DIR)/$$i $(DESTDIR)$(datadir)/icons/sugar-lh/cursors/$$i ;	\
	done

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/icons/sugar-lh/cursors

EXTRA_DIST=$(sugar_images) $(cursor_instructions)
