#
#
#

CFLAGS = -Wall -std=gnu89 -Wextra -O2

all: gophermap.cgi

gopherblog: gopherblog.o

gophermap.cgi: gopherblog
	mv gopherblog gophermap.cgi
