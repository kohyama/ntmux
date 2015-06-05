
OS := $(shell uname -s)
#
#	Linux:  $ uname -s 
#			  Linux
#	
#	OSX:	$ uname -s
#			  Darwin

all: ntmux

ntmux: ntmux.c
	gcc -o ntmux ntmux.c -lutil -D $(OS)

install:
	mv ntmux /usr/bin/

clean:
	rm -rf *.o ntmux
