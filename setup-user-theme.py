#!/usr/bin/python

# FIXME Need to handle the case when symlinks already exists

import os

theme_dir = os.path.expanduser('~/.themes/olpc/gtk-2.0')
engine_dir = os.path.expanduser('~/.gtk-2.0/engines')
src_dir = os.path.abspath(os.path.dirname(__file__))

if not os.path.exists(theme_dir):
	os.makedirs(theme_dir)
	os.symlink(os.path.join(src_dir, 'gtk-engine/theme/gtkrc'),
			   os.path.join(theme_dir, 'gtkrc'))

engine_dest = os.path.join(engine_dir, 'libolpc.so')
if not os.path.exists(engine_dest):
	os.makedirs(engine_dir)
	engine_src = os.path.join(src_dir, 'gtk-engine/src/.libs/libolpc.so')
	os.symlink(engine_src, engine_dest)
