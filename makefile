CC = g++
CFLAGS = -Wall -I/usr/include/ncursesw -std=c++11
LDFLAGS = -lncurses -lcrypto

ALL = solisolv

all: $(ALL)

$(ALL): main.o CPlateau.o CSolver.o CCoup.o CPlateaux.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(ALL)
	rm -f *.o
