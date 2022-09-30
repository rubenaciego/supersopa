CPPFLAGS = -g
LDFLAGS =-g
LDLIBS =

SOURCES = main.cpp sopasolver.cpp sortedvec.cpp trie.cpp bloom.cpp hashmap.cpp
OBJS = $(subst .cpp,.o,$(SOURCES))

supersopa: $(OBJS)
	g++ $(LDFLAGS) -o supersopa $(OBJS) $(LDLIBS)
