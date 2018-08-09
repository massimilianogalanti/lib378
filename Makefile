# Makefile
#
# lib378.so

EXTRA_CFLAGS += -Wall -Werror

CFLAGS += -fPIC -DUSE_BZ2
LDFLAGS += -lbz2

CC = gcc
HOSTCC = $(CC)

LIBNAME = lib378.so

all: compress $(LIBNAME) phone-example dictionary.bin.bz2

phone-example: phone-example.c $(LIBNAME)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -L./ -l378 $< -o $@

$(LIBNAME): 378.h 378.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) -shared 378.c -o $@

.depend:
	touch .depend
	makedepend *.[ch] -f.depend 2>/dev/null

dictionary.ascii:
	wget -N http://prdownloads.sourceforge.net/wordlist/ispell-enwl-3.1.20.tar.gz
	tar xvzf ./ispell-enwl-3.1.20.tar.gz --exclude README -O | tr '[A-Z]' '[a-z]' | sort >dictionary.ascii

dictionary.bin: compress dictionary.ascii
	./compress dictionary.ascii

dictionary.bin.bz2: dictionary.bin
	bzip2 -9 dictionary.bin

clean:
	rm -f .depend *.o $(LIBNAME) compress *example *~ dictionary.*

-include .depend
