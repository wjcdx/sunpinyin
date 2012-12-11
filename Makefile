
all:
	scons --prefix=/usr

install:
	scons --prefix=/usr install

clean:
	scons --prefix=/usr -c

uninstall:
	scons --prefix=/usr -c install

