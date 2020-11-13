#
# Gopherblog - a simple blog engine for gopher
# Copyright (C) 2020 Mateusz Viste
#

CFLAGS = -Wall -std=gnu89 -Wextra -O2

all: gophermap.cgi

gopherblog: gopherblog.o

gophermap.cgi: gopherblog
	mv gopherblog gophermap.cgi
