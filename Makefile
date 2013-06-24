
all:
	scons --prefix=/usr

install:
	scons --prefix=/usr install

clean:
	scons --prefix=/usr -c

uninstall:
	scons --prefix=/usr -c install

CXX = g++
CFLAGS = -I$(shell pwd)/src
SRCS = src/portability.cpp src/xh/lexicon/DictConv.cpp
OBJS = 

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

dconv: src/portability.o src/xh/lexicon/DictConv.o
	$(CXX) $(CFLAGS) $^ -o $@
	
