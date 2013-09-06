#ifndef GVARIANT_H
#define GVARIANT_H

#include <vector>

class GVariant {

public:
	typedef enum {
		G_VARIANT_CLASS_EMPTY = 0,
		G_VARIANT_CLASS_BOOLEAN,
		G_VARIANT_CLASS_INT32,
		G_VARIANT_CLASS_STRING,
		G_VARIANT_CLASS_ARRAY,
	} EType;

	GVariant() {
		m_type = G_VARIANT_CLASS_EMPTY;
	}
	GVariant(bool value) {
		m_type = G_VARIANT_CLASS_BOOLEAN;
		m_bValue = value;
	}
	GVariant(int value) {
		m_type = G_VARIANT_CLASS_INT32;
		m_iValue = value;
	}
	GVariant(std::string value) {
		m_type = G_VARIANT_CLASS_STRING;
		m_sValue = value;
	}
	GVariant(std::vector<std::string> value) {
		m_type = G_VARIANT_CLASS_ARRAY;
		m_aryValue = value;
	}
	

	void SetType(EType type) { m_type = type; }
	EType GetType() { return m_type; }
	bool TypeEquals(EType type) { return m_type == type; }

	bool GetBool() { return m_bValue; }
	int GetInt() { return m_iValue; }
	std::string GetString() { return m_sValue; }
	std::vector<std::string> GetArray() { return m_aryValue; }


private:

	EType m_type;
	bool m_bValue;
	int m_iValue;
	std::string m_sValue;
	std::vector<std::string> m_aryValue;
	bool m_changed;
};



#endif