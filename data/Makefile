PREVPATH:=${PATH}:../src
export PATH=${PREVPATH}

DICTGEN = ../src/sunpinyin-dictgen

gen_all:
	${DICTGEN} bin_data

lm_sc:
	${DICTGEN} lm_sc.t3g

xhdict:
	${DICTGEN} xhdict3_sc.bin

xhdict-gdb:
	gdb --args ../src/genpyt -e le -i dict.utf8.xh \
		-s lm_sc.t3g -l lm_sc.t3g.xh.log -t xh -o xhdict3_sc.bin

pydict:
	${DICTGEN} pydict3_sc.bin

install:
	${DICTGEN} all

clean:
	rm -rf *dict3_sc.bin

distclean:
	rm -rf lm_sc.t3g.orig lm_sc.t3g lm_sc.t3g.*.log *dict3_sc.bin
