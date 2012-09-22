#include <string>
#include <vector>
using namespace std;

namespace bsmerge {
namespace {

vector<string> filter_ebuild_lines(const vector<string>& lines)
{
	vector<string> ret;

	for(auto vi = lines.begin(); vi != lines.end(); ++vi)
	{
		if(strings::begins_with("[", *vi))
		{
			typedef string::size_type ST;
			ST pos = vi->find(']');
			if(pos == string::npos)
				continue;

			string line(vi->substr(pos + 1));
			ret.push_back(strings::trim(line));
		}
	}

	return ret;
}

}

vector<string> get_ebuild_list_remote(const SlaveConfig& slave, const vector<string>& ebuilds)
{
	if(!util::can_connect_passwordless(myArg->slave.hostspec, myArg->slave.port))
			return;




}
}



