PREVPATH:=${PATH}:.
export PATH=${PREVPATH}

DICTGEN = ./sunpinyin-dictgen

gen_all:
	${DICTGEN} dict3_sc.bin.xh

lm_sc:
	${DICTGEN} lm_sc.t3g

xhdict:
	${DICTGEN} xhdict3_sc.bin

gdb-py:
	gdb --args ./genpyt -e le -i dict.utf8.py \
		-s lm_sc.t3g.py -l lm_sc.t3g.py.log -t xh -o dict3_sc.bin.py
gdb-xh:
	gdb --args ./genpyt -e le -i dict.utf8.xh \
		-s lm_sc.t3g.xh -l lm_sc.t3g.xh.log -t xh -o dict3_sc.bin.xh

xh:
	./genpyt -e le -i dict.utf8.xh \
		-s lm_sc.t3g.xh -l lm_sc.t3g.xh.log -t xh -o dict3_sc.bin.xh

pydict:
	${DICTGEN} pydict3_sc.bin

install:
	${DICTGEN} all

clean:
	rm -rf *dict3_sc.bin

distclean:
	rm -rf lm_sc.t3g.orig lm_sc.t3g lm_sc.t3g.*.log *dict3_sc.bin

todbg:
	cp dict.utf8.xh.dbg dict.utf8.xh
	cp lm_sc.t3g.arpa.dbg lm_sc.t3g.arpa

touse:
	cp dict.utf8.xh.all dict.utf8.xh
	cp lm_sc.t3g.arpa.all lm_sc.t3g.arpa

dconv-dbg:
	gdb --args ./dconv dict.utf8.xh.oc dict.utf8.py dict.utf8.xh

dconv:
	./dconv dict.utf8.xh.oc dict.utf8.py dict.utf8.xh
	sed -i '/XXX/d' dict.utf8.xh

.PHONY: dconv

