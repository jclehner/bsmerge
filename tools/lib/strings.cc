#include "strings.hh"
using namespace std;

namespace bsmerge {
namespace strings {
namespace {

typedef string::size_type ST;

}

string& trim(string& str)
{
	ST pos = str.find_first_not_of(" \t");
	if(pos != string::npos)
		str.erase(0, pos);

	pos = str.find_last_not_of(" \t");
	if(pos != string::npos && (pos + 1) < str.size())
		str.erase(pos + 1);

	return str;
}

string escape(string str, bool useSingleQuotes)
{
	const char quote = useSingleQuotes ? '\'' : '\"';
	str.insert(0, 1, quote);

	ST pos = 1;

	while((pos = str.find(quote, pos)) != string::npos)
	{
		if(str[pos - 1] != '\\')
		{
			str.insert(pos, 1, '\\');
			pos += 2;
		}
		else
			++pos;
	}

	str.append(1, quote);

	return str;
}

}}
