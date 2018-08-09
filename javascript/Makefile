# Makefile
#
# lib378js

EXTRA_CFLAGS += -Wall -Werror

CC = gcc
HOSTCC = $(CC)

all: dictionary.js

ascii-compress: ascii-compress.c
	$(HOSTCC) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

dictionary.ascii:
	wget -N http://prdownloads.sourceforge.net/wordlist/ispell-enwl-3.1.20.tar.gz
	tar xvzf ./ispell-enwl-3.1.20.tar.gz english.0 -O | grep -v "'" | tr '[A-Z]' '[a-z]' | sort >dictionary.ascii

dictionary.js: ascii-compress dictionary.ascii
	echo -n "var dictionary = new Array();" >dictionary.js
	./ascii-compress a2d3g4j5m6p7t8w9 <dictionary.ascii | sed "s/^\(.\)=\(.*\)/dictionary\['\1\']='\2\';/" >>dictionary.js

clean:
	-rm ascii-compress *~ *dictionary.*