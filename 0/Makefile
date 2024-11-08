FLAGS=$(shell pkg-config --libs --cflags r_util r_io)

all:
	gcc $(FLAGS) io_skel.c -fpic -shared -o libiombc2ram.so
