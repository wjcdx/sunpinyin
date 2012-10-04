#include "CUnitData.h"
#include "pinyin_data.h"
#include "xh_data.h"

TUnit
CUnitData::encode(const char *str)
{
	switch(m_type) {
	case PINYIN:
		return TUnit((unsigned int)CPinyinData::encode(str));
	case XINGHUA:
		return CXhData::encode(str);
	default:
		return TUnit(0);
	}
}

const char *
CUnitData::decode(TUnit u, const char **i,
		const char **f)
{
	switch(m_type) {
	case PINYIN:
		return CPinyinData::decode(TSyllable((unsigned int)u), i, f);
	case XINGHUA:
		return CXhData::decode(u, i, f);
	default:
		return NULL;
	}
}
