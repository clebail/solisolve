CC = g++
CFLAGS = -Wall -I/usr/include/ncursesw
LDFLAGS = -lncurses

ALL = solisolv

all: $(ALL)

$(ALL): main.o CPlateau.o CSolver.o CCoup.o CFeuille.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(ALL)
	rm -f *.o
