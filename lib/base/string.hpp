/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2017 Icinga Development Team (https://www.icinga.com/)  *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef STRING_H
#define STRING_H

#include "base/i2-base.hpp"
#include "base/object.hpp"
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/range/iterator.hpp>
#include <string.h>
#include <functional>
#include <string>
#include <iosfwd>

namespace icinga {

class Value;

/**
 * String class.
 *
 * Rationale for having this: The std::string class has an ambiguous assignment
 * operator when used in conjunction with the Value class.
 */
class I2_BASE_API String
{
public:
	typedef std::string::iterator Iterator;
	typedef std::string::const_iterator ConstIterator;

	typedef std::string::iterator iterator;
	typedef std::string::const_iterator const_iterator;

	typedef std::string::reverse_iterator ReverseIterator;
	typedef std::string::const_reverse_iterator ConstReverseIterator;

	typedef std::string::reverse_iterator reverse_iterator;
	typedef std::string::const_reverse_iterator const_reverse_iterator;

	typedef std::string::size_type SizeType;

	String(void)
		: m_Data()
	{ }

	String(const char *data)
		: m_Data(data)
	{ }

	String(const std::string& data)
		: m_Data(data)
	{ }

	String(std::string&& data)
		: m_Data(data)
	{ }

	String(String::SizeType n, char c)
		: m_Data(n, c)
	{ }

	String(const String& other)
		: m_Data(other)
	{ }

	String(String&& other)
		: m_Data(std::move(other.m_Data))
	{ }

#ifndef _MSC_VER
	String(Value&& other);
#endif /* _MSC_VER */

	~String(void)
	{ }

	template<typename InputIterator>
	String(InputIterator begin, InputIterator end)
		: m_Data(begin, end)
	{ }

	String& operator=(const String& rhs)
	{
		m_Data = rhs.m_Data;
		return *this;
	}

	String& operator=(String&& rhs)
	{
		m_Data = std::move(rhs.m_Data);
		return *this;
	}

	String& operator=(Value&& rhs);

	String& operator=(const std::string& rhs)
	{
		m_Data = rhs;
		return *this;
	}

	String& operator=(const char *rhs)
	{
		m_Data = rhs;
		return *this;
	}

	const char& operator[](SizeType pos) const
	{
		return m_Data[pos];
	}

	char& operator[](SizeType pos)
	{
		return m_Data[pos];
	}

	String& operator+=(const String& rhs)
	{
		m_Data += rhs.m_Data;
		return *this;
	}

	String& operator+=(const char *rhs)
	{
		m_Data += rhs;
		return *this;
	}

	String& operator+=(const Value& rhs);

	String& operator+=(char rhs)
	{
		m_Data += rhs;
		return *this;
	}

	bool IsEmpty(void) const
	{
		return m_Data.empty();
	}

	bool operator<(const String& rhs) const
	{
		return m_Data < rhs.m_Data;
	}

	operator const std::string&(void) const
	{
		return m_Data;
	}

	const char *CStr(void) const
	{
		return m_Data.c_str();
	}

	void Clear(void)
	{
		m_Data.clear();
	}

	SizeType GetLength(void) const
	{
		return m_Data.size();
	}

	std::string& GetData(void)
	{
		return m_Data;
	}

	const std::string& GetData(void) const
	{
		return m_Data;
	}

	SizeType Find(const String& str, SizeType pos = 0) const
	{
		return m_Data.find(str, pos);
	}

	SizeType RFind(const String& str, SizeType pos = NPos) const
	{
		return m_Data.rfind(str, pos);
	}

	SizeType FindFirstOf(const char *s, SizeType pos = 0) const
	{
		return m_Data.find_first_of(s, pos);
	}

	SizeType FindFirstOf(char ch, SizeType pos = 0) const
	{
		return m_Data.find_first_of(ch, pos);
	}

	SizeType FindFirstNotOf(const char *s, SizeType pos = 0) const
	{
		return m_Data.find_first_not_of(s, pos);
	}

	SizeType FindFirstNotOf(char ch, SizeType pos = 0) const
	{
		return m_Data.find_first_not_of(ch, pos);
	}

	SizeType FindLastOf(const char *s, SizeType pos = NPos) const
	{
		return m_Data.find_last_of(s, pos);
	}

	SizeType FindLastOf(char ch, SizeType pos = NPos) const
	{
		return m_Data.find_last_of(ch, pos);
	}

	String SubStr(SizeType first, SizeType len = NPos) const
	{
		return m_Data.substr(first, len);
	}

	std::vector<String> Split(const char *separators) const
	{
		std::vector<String> result;
		boost::algorithm::split(result, m_Data, boost::is_any_of(separators));
		return result;
	}

	void Replace(SizeType first, SizeType second, const String& str)
	{
		m_Data.replace(first, second, str);
	}

	String Trim(void) const
	{
		String t = m_Data;
		boost::algorithm::trim(t);
		return t;
	}

	String ToLower(void) const
	{
		String t = m_Data;
		boost::algorithm::to_lower(t);
		return t;
	}

	String ToUpper(void) const
	{
		String t = m_Data;
		boost::algorithm::to_upper(t);
		return t;
	}

	String Reverse(void) const
	{
		String t = m_Data;
		std::reverse(t.m_Data.begin(), t.m_Data.end());
		return t;
	}

	void Append(int count, char ch)
	{
		m_Data.append(count, ch);
	}

	bool Contains(const String& str) const
	{
		return (m_Data.find(str) != std::string::npos);
	}

	void swap(String& str)
	{
		m_Data.swap(str.m_Data);
	}

	Iterator erase(Iterator first, Iterator last)
	{
		return m_Data.erase(first, last);
	}

	template<typename InputIterator>
	void insert(Iterator p, InputIterator first, InputIterator last)
	{
		m_Data.insert(p, first, last);
	}

	Iterator Begin(void)
	{
		return m_Data.begin();
	}

	ConstIterator Begin(void) const
	{
		return m_Data.begin();
	}

	Iterator End(void)
	{
		return m_Data.end();
	}

	ConstIterator End(void) const
	{
		return m_Data.end();
	}

	ReverseIterator RBegin(void)
	{
		return m_Data.rbegin();
	}

	ConstReverseIterator RBegin(void) const
	{
		return m_Data.rbegin();
	}

	ReverseIterator REnd(void)
	{
		return m_Data.rend();
	}

	ConstReverseIterator REnd(void) const
	{
		return m_Data.rend();
	}

	static const SizeType NPos;

	static Object::Ptr GetPrototype(void);

private:
	std::string m_Data;
};

inline std::ostream& operator<<(std::ostream& stream, const String& str)
{
	stream << str.GetData();
	return stream;
}

inline std::istream& operator>>(std::istream& stream, String& str)
{
	std::string tstr;
	stream >> tstr;
	str = tstr;
	return stream;
}

inline String operator+(const String& lhs, const String& rhs)
{
	return lhs.GetData() + rhs.GetData();
}

inline String operator+(const String& lhs, const char *rhs)
{
	return lhs.GetData() + rhs;
}

inline String operator+(const char *lhs, const String& rhs)
{
	return lhs + rhs.GetData();
}

inline bool operator==(const String& lhs, const String& rhs)
{
	return lhs.GetData() == rhs.GetData();
}

inline bool operator==(const String& lhs, const char *rhs)
{
	return lhs.GetData() == rhs;
}

inline bool operator==(const char *lhs, const String& rhs)
{
	return lhs == rhs.GetData();
}

inline bool operator<(const String& lhs, const char *rhs)
{
	return lhs.GetData() < rhs;
}

inline bool operator<(const char *lhs, const String& rhs)
{
	return lhs < rhs.GetData();
}

inline bool operator>(const String& lhs, const String& rhs)
{
	return lhs.GetData() > rhs.GetData();
}

inline bool operator>(const String& lhs, const char *rhs)
{
	return lhs.GetData() > rhs;
}

inline bool operator>(const char *lhs, const String& rhs)
{
	return lhs > rhs.GetData();
}

inline bool operator<=(const String& lhs, const String& rhs)
{
	return lhs.GetData() <= rhs.GetData();
}

inline bool operator<=(const String& lhs, const char *rhs)
{
	return lhs.GetData() <= rhs;
}

inline bool operator<=(const char *lhs, const String& rhs)
{
	return lhs <= rhs.GetData();
}

inline bool operator>=(const String& lhs, const String& rhs)
{
	return lhs.GetData() >= rhs.GetData();
}

inline bool operator>=(const String& lhs, const char *rhs)
{
	return lhs.GetData() >= rhs;
}

inline bool operator>=(const char *lhs, const String& rhs)
{
	return lhs >= rhs.GetData();
}

inline bool operator!=(const String& lhs, const String& rhs)
{
	return lhs.GetData() != rhs.GetData();
}

inline bool operator!=(const String& lhs, const char *rhs)
{
	return lhs.GetData() != rhs;
}

inline bool operator!=(const char *lhs, const String& rhs)
{
	return lhs != rhs.GetData();
}

inline String::Iterator begin(String& x)
{
	return x.Begin();
}

inline String::ConstIterator begin(const String& x)
{
	return x.Begin();
}

inline String::Iterator end(String& x)
{
	return x.End();
}

inline String::ConstIterator end(const String& x)
{
	return x.End();
}
inline String::Iterator range_begin(String& x)
{
	return x.Begin();
}

inline String::ConstIterator range_begin(const String& x)
{
	return x.Begin();
}

inline String::Iterator range_end(String& x)
{
	return x.End();
}

inline String::ConstIterator range_end(const String& x)
{
	return x.End();
}

}

namespace boost
{

template<>
struct range_mutable_iterator<icinga::String>
{
	typedef icinga::String::Iterator type;
};

template<>
struct range_const_iterator<icinga::String>
{
	typedef icinga::String::ConstIterator type;
};

}

#endif /* STRING_H */
