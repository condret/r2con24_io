FLAGS=$(shell pkg-config --libs --cflags r_util r_io)

all:
	gcc $(FLAGS) mbc2ram.c -fpic -shared -o libmbc2ram.so
