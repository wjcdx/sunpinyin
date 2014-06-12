
all: win32 win64

win32:
	scons.bat --prefix=/usr --target_arch=x86

win64:
	scons.bat --prefix=/usr --target_arch=x86_64

install:
	scons.bat --prefix=/usr install

clean: clean32 clean64

clean32:
	scons.bat --prefix=/usr --target_arch=x86 -c

clean64:
	scons.bat --prefix=/usr --target_arch=x86_64 -c

uninstall:
	scons.bat --prefix=/usr -c install

CXX = g++
CFLAGS = -I$(shell pwd)/src
SRCS = src/portability.cpp src/xh/lexicon/DictConv.cpp
OBJS = 

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

dconv: src/portability.o src/xh/lexicon/DictConv.o
	$(CXX) $(CFLAGS) $^ -o $@
	
