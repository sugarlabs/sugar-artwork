#!/bin/sh
autoreconf -i
./configure --enable-maintainer-mode "$@"
