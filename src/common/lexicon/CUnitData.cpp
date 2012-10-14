#include "trie.h"
#include "CUnitData.h"
#include "pinyin_data.h"
#include "xh_data.h"

using namespace TrieThreadModel;

CTrie::TrieType CUnitData::m_type = CTrie::XINGHUA;

void
CUnitData::initialize(CTrie::TrieType t)
{
	m_type = t;
}

TUnit
CUnitData::encode(const char *str)
{
	switch(m_type) {
	case CTrie::PINYIN:
		return TUnit((unsigned int)CPinyinData::encode(str));
	case CTrie::XINGHUA:
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
	case CTrie::PINYIN:
		return CPinyinData::decode(TSyllable((unsigned int)u), i, f);
	case CTrie::XINGHUA:
		return CXhData::decode(u, i, f);
	default:
		return NULL;
	}
}
