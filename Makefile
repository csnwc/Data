CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

demo_neillsimplescreen : neillsimplescreen.c neillsimplescreen.h demo_neillsimplescreen.c
	$(CC) demo_neillsimplescreen.c neillsimplescreen.c -o demo_neillsimplescreen $(SANITIZE) $(LDLIBS)

demo_neillncurses : neillncurses.c neillncurses.h demo_neillncurses.c
	$(CC) demo_neillncurses.c neillncurses.c -o demo_neillncurses $(SANITIZE) $(LDLIBS) -lncurses`

demo_neillsdl2: demo_neillsdl2.c neillsdl2.c neillsdl2.h
	$(CC) demo_neillsdl2.c neillsdl2.c -o demo_neillsdl2 $(COMMON) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)

testrealloc_s : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../../ADTs/General/general.h ../../ADTs/General/general.c
	$(CC) testboolarr.c Realloc/realloc.c ../../ADTs/General/general.c -o testrealloc_s -I./Realloc $(SANITIZE) $(LDLIBS)

clean:
	rm -fr demo_neillsimplescreen demo_neillncurses demo_neillsdl2































DOT := $(shell command -v dot 2> /dev/null)

Linkedb.dot:  testlinked
	./testlinked
Reallocb.dot: testrealloc
	./testrealloc

Linkedb.pdf:  testbst.c testlinked Linkedb.dot
ifndef DOT
    $(error "dot is not available please install graphviz")
endif
	dot -Tpdf -o Linkedb.pdf Linkedb.dot
Reallocb.pdf:  testbst.c testrealloc Reallocb.dot
ifndef DOT
    $(error "dot is not available please install graphviz")
endif
	dot -Tpdf -o Reallocb.pdf Reallocb.dot

pdfs : Linkedb.pdf Reallocb.pdf
