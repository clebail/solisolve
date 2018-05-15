CC = g++
CFLAGS = -Wall -I.. -std=c++11
LDFLAGS = 

ALL = anim

all: $(ALL)

$(ALL): main.o
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f $(ALL)
	rm -f *.o
