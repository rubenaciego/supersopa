CPPFLAGS = -g -std=c++17 -O3
LDFLAGS = -g
LDLIBS =

SOURCES = src/main.cpp src/sopasolver.cpp src/sortedvec.cpp src/trie.cpp src/bloom.cpp src/hashmap.cpp src/murmurhash3.cpp
OBJS = $(subst .cpp,.o,$(SOURCES))

supersopa: $(OBJS)
	g++ $(LDFLAGS) -o supersopa $(OBJS) $(LDLIBS)

clean:
	rm src/*.o -f

distclean: clean
	rm supersopa -f
