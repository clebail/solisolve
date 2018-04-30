CC = gcc
CFLAGS = -Wall -I/usr/include/ncursesw
LDFLAGS = -lncurses

ALL = solisolv

all: $(ALL)

$(ALL): main.o TPlateau.o SListCoup.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(ALL)
	rm -f *.o