vpath %.h includes
vpath %.c src

CPPFLAGS=-Iincludes

asciify: main.o stb_image.o ansi_colours.o
	gcc -o asciify main.o stb_image.o ansi_colours.o -lncurses -lm

stb_image.o: stb_image.h stb_image.c

ansi_colours.o: ansi_colours.h ansi_colours.c

.PHONY: clean
clean:
	rm -f asciify stb_image.o ansi_colours.o main.o