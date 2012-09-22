#ifndef DMERGE_STRINGS_HH
#define DMERGE_STRINGS_HH
#include <sstream>
#include <string>
#include "exception.hh"
#include "static_assert.hh"

namespace bsmerge {
namespace strings {

inline bool begins_with(const std::string& begin, const std::string& str) {
	return str.substr(0, begin.size()) == begin;
}

inline bool begins_with(char begin, const std::string& str) {
	return str.empty() ? false : str[0] == begin;
}

template<typename T> std::string stringify(const T& t)
{
	std::ostringstream ostr;
	ostr << t;
	return ostr.str();	
}

//template<typename T> T unstringify(const std::string& str)
template<typename T> bool convert(const std::string& str, T& out)
{
	std::istringstream istr(str);
	return istr >> out;
}

std::string escape(std::string str, bool useSingleQuotes = true);

std::string& trim(std::string& str);

template<typename T> std::string ordinal(const T& num)
{
	// fail if T is a non-integer type
	BSMERGE_STATIC_ASSERT(T(4) / T(3) == T(1));

	const T ten = num - ((num / 100) * 100);
	if(ten >= 10 && ten < 20)
		return stringify(num) + "th";

	const T one = num - ((num / 10) * 10);
	if(one == 1)
		return stringify(num) + "st";
	else if(one == 2)
		return stringify(num) + "nd";
	else if(one == 3)
		return stringify(num) + "rd";
	
	return stringify(num) + "th";
}

}}
#endif
