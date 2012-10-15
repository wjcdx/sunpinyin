#!/usr/bin/make -f
# -*- mode: makefile; indent-tabs-mode: t -*- vim:noet:ts=4

WGET = wget
TAR = tar
ENDIANNESS = le
DATA_DIR = '/usr/share/sunpinyin'

DL_LIST = 'http://code.google.com/p/open-gram/downloads/list'
DL_ROOT = 'http://open-gram.googlecode.com/files/'
DICT_PAT = 'dict\.utf8-[0-9]\+.tar.bz2'
TSLM_PAT = 'lm_sc\.t3g\.arpa-[0-9]\+.tar.bz2'

DICT_AR = $(shell w3m -dump ${DL_LIST} | grep -o ${DICT_PAT} | sort | tail -n 1)
TSLM_AR = $(shell w3m -dump ${DL_LIST} | grep -o ${TSLM_PAT} | sort | tail -n 1)

all: install

${DICT_AR}:
	${WGET} ${DL_ROOT}/$@

dict.utf8: ${DICT_AR}
	${TAR} xmf $^

${TSLM_AR}:
	${WGET} ${DL_ROOT}/$@

lm_sc.t3g.arpa: ${TSLM_AR}
	${TAR} xmf $^

lm_sc.t3g.orig: dict.utf8 lm_sc.t3g.arpa
	tslmpack lm_sc.t3g.arpa dict.utf8 $@

lm_sc.t3g: lm_sc.t3g.orig
	tslmendian -e ${ENDIANNESS} -i $^ -o $@

pydict3_sc.bin: dict.utf8 lm_sc.t3g
	genpyt -e ${ENDIANNESS} -i dict.utf8 -s lm_sc.t3g \
		-l lm_sc.t3g.log -t py -o $@

install: lm_sc.t3g pydict3_sc.bin
	install -d ${DATA_DIR}
	install -Dm644 $^ ${DATA_DIR}

clean:
	rm -rf ${DICT_AR} ${TSLM_AR} dict.utf8 lm_sc.t3g.arpa \
		lm_sc.t3g.orig lm_sc.t3g lm_sc.t3g.log pydict3_sc.bin

