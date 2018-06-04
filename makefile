CC = g++
CFLAGS = -Wall -I/usr/include/ncursesw -std=c++11
LDFLAGS = -lncurses

ALL = solisolv
SUBDIRS = anim
TOPTARGETS = all clean

$(TOPTARGETS): $(SUBDIRS)

all: $(ALL)

$(ALL): main.o CPlateau.o CSolver.o CCoup.o CPlateaux.o CGenetic.o
	$(CC) -o $@ $(LDFLAGS) $^
	
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(ALL)
	rm -f *.o
	
.PHONY: $(TOPTARGETS) $(SUBDIRS)
