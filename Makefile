
all:
	scons --prefix=/usr --enable-plugins

install:
	scons --prefix=/usr install

clean:
	scons --prefix=/usr --enable-plugins -c

uninstall:
	scons --prefix=/usr install

