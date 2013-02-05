
all: clean
	scons

go:
	./sunpinyin -p x 2>/dev/null

go-with-info:
	./sunpinyin -p x

clean:
#	rm -f *.o sunpinyin
	rm -f sunpinyin

gdb:
	gdb --args ./sunpinyin -p x 2>/dev/null

prof:
	gprof sunpinyin gmon.out -p > a
	gprof sunpinyin gmon.out -q > b

.PHONY: prof

