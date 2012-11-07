
all:
	scons --prefix=/usr

install:
	scons --prefix=/usr install

clean:
	scons -c

uninstall:
	scons -c install

exec:
	/usr/lib/ibus-sunpinyin/ibus-engine-sunpinyin --ibus &

gdb:
	gdb --args /usr/lib/ibus-sunpinyin/ibus-engine-sunpinyin --ibus

strace:
	strace /usr/lib/ibus-sunpinyin/ibus-engine-sunpinyin --ibus

check:
	ps axu | grep sun

kill:
	killall -9 ibus-engine-sunpinyin
	ps axu | grep sun
	

