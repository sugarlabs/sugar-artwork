#!/usr/bin/python

# FIXME Need to handle the case when symlinks already exists

import os

theme_dir = os.path.expanduser('~/.themes/olpc/gtk-2.0')
gtkrc_dest = os.path.join(theme_dir, 'gtkrc')

engine_dir = os.path.expanduser('~/.gtk-2.0/engines')
engine_dest = os.path.join(engine_dir, 'libolpc.so')

src_dir = os.path.abspath(os.path.dirname(__file__))

if not os.path.exists(theme_dir):
	try:
		os.makedirs(theme_dir)
	except OSError, exc:
		if exc[0] == 17:   # File exists
			pass
try:
	os.unlink(gtkrc_dest)
except OSError, exc:
	pass
os.symlink(os.path.join(src_dir, 'gtk-engine/theme/gtkrc'), gtkrc_dest)

if not os.path.exists(engine_dest):
	try:
		os.makedirs(engine_dir)
	except OSError, exc:
		if exc[0] == 17:   # File exists
			pass
engine_src = os.path.join(src_dir, 'gtk-engine/src/.libs/libolpc.so')
try:
	os.unlink(engine_dest)
except OSError, exc:
	pass
os.symlink(engine_src, engine_dest)
