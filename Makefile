
all:
	scons --prefix=/usr --enable-plugins

install:
	scons --prefix=/usr --enable-plugins install

clean:
	scons --prefix=/usr --enable-plugins -c

uninstall:
	scons --prefix=/usr --enable-plugins -c install

