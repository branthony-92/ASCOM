#ifndef ALPACAUTILS_H
#define ALPACAUTILS_H

#include <string>

#define DEVICEPROP(type, name, member_name) \
protected: \
	type m_##member_name;\
public:\
	type get##name() const { return m_##member_name; }\
	void set##name(type val) { m_##member_name = val; }

namespace Alpaca {
	namespace Utils {
	
	}
}

#endif // !ALPACAUTILS_H