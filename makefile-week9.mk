CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS =

all: testrealloc_s testrealloc_v testrealloc

testrealloc : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../../ADTs/General/general.h ../../ADTs/General/general.c
	$(CC) testboolarr.c Realloc/realloc.c ../../ADTs/General/general.c -o testrealloc -I./Realloc $(PRODUCTION) $(LDLIBS)

testrealloc_s : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../../ADTs/General/general.h ../../ADTs/General/general.c
	$(CC) testboolarr.c Realloc/realloc.c ../../ADTs/General/general.c -o testrealloc_s -I./Realloc $(SANITIZE) $(LDLIBS)

testrealloc_v : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../../ADTs/General/general.h ../../ADTs/General/general.c
	$(CC) testboolarr.c Realloc/realloc.c ../../ADTs/General/general.c -o testrealloc_v -I./Realloc $(VALGRIND) $(LDLIBS)


clean:
	rm -f testrealloc_s testrealloc_v testrealloc

run: all
	./testrealloc_s
	valgrind ./testrealloc_v
