#include "trie.h"
#include "CUnitData.h"
#include "pinyin/input/pinyin_data.h"
#include "xh/input/xh_data.h"
#include "xh/input/xhoc_data.h"

using namespace TrieThreadModel;

CTrie::TrieType CUnitData::m_type = CTrie::XINGHUA;

void
CUnitData::initialize(CTrie::TrieType t)
{
	m_type = t;
}

TrieThreadModel::TUnit
CUnitData::encode(const char *str)
{
	switch(m_type) {
	case CTrie::PINYIN:
		return TUnit((unsigned int)CPinyinData::encode(str));
	case CTrie::XINGHUA:
		return CXhData::encode(str);
	case CTrie::XINGHUA_OC:
		return CXhocData::encode(str);
	default:
		return TUnit(0);
	}
}

const char *
CUnitData::decode(TrieThreadModel::TUnit u, const char **i,
		const char **f)
{
	switch(m_type) {
	case CTrie::PINYIN:
		return CPinyinData::decode(TSyllable((unsigned int)u), i, f);
	case CTrie::XINGHUA:
		return CXhData::decode(u, i, f);
	case CTrie::XINGHUA_OC:
		return CXhocData::decode(u, i, f);
	default:
		return NULL;
	}
}
