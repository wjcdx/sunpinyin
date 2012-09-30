#ifndef _SHUANGPIN_DATA_H_
#define _SHUANGPIN_DATA_H_

#include <string>
#include <map>

#include "ime-core/utils.h"
#include "syllable.h"

enum EShuangpinType {
    MS2003,
    ABC,
    ZIRANMA,
    PINYINJIAJIA,
    ZIGUANG,
    XIAOHE,
    USERDEFINE,
};

typedef struct _TZeroinitial {
    const char  *syl;
    const char  *mapshp;
} TZeroInitial;

typedef struct _TShungpinPlan {
    EShuangpinType type;
    char *mapinitials;
    char *mapfinals;
    TZeroInitial *zeroinitals;
} TShuangpinPlan;


typedef std::vector<std::string>         CMappedYin;
typedef std::map<std::string, TSyllable> CEncodingMap;
typedef std::map <char, CMappedYin*>     CKeyMap;

class CShuangpinData : private CNonCopyable
{
public:
    CShuangpinData (EShuangpinType shpPlan = MS2003);
    ~CShuangpinData ();



    TSyllable encode(const char *);
    const char *decode(TSyllable,
                               const char **i = NULL,
                               const char **f = NULL);

    int getMapString(const char *ch, CMappedYin & syls);
    EShuangpinType getShuangpinType() const { return m_shuangpinPlan->type; }
    void setShuangpinType(EShuangpinType shpPlan);

protected:
    void  _genCodingMap();
    void  _genKeyboardMap(EShuangpinType shyType);

    CEncodingMap m_codingmap;
    TShuangpinPlan *m_shuangpinPlan;
    CKeyMap m_keymap;
};

#endif /* _PINYIN_DATA_H_ */

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
