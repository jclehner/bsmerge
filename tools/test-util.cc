#include <iostream>
#include "strings.hh"
#include "util.hh"
using namespace std;
using namespace bsmerge;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cerr << "usage: test-util [TEST] [ARGS ...]" << endl;
		return 1;
	}

	string test(argv[1]);

	argc -= 2;
	argv += 2;

	if(test == "get_process_output")
	{
		string command;

		for(int i = 0; i != argc; ++i)
			command += string(argv[i]) + " ";

		cout << "#HOST: cava.zapto.org" << endl;
		cout << "# " << endl;

		vector<string> lines = util::get_process_output(command, true);
		for(vector<string>::size_type i = 0; i != lines.size(); ++i)
		{
			if(strings::begins_with("[", lines[i]))
			{
				typedef string::size_type ST;
				ST pos = lines[i].find(']');
				if(pos == string::npos)
					continue;

				string line(lines[i].substr(pos + 1));
				strings::trim(line);

				cout << line <<  endl;
			}	
		}

		return 0;
	}
	else if(test == "get_bash_var")
	{
		if(argc != 3)
		{
			cerr << "usage: get_bash_var [value|array] [FILENAME] [VAR NAME]" << endl;
			return 1;
		}

		string type(argv[0]);
		if(type == "value")
		{
			string str;
			if(util::get_bash_value(argv[1], argv[2], str))
			{
				cout << str << endl;
				return 0;
			}
		}
		else if(type == "array")
		{
			vector<string> values;

			for(int i = 0; i != 2; ++i)
			{
				bool isTwoLevelArray = i == 1;

				if(util::get_bash_array(argv[1], argv[2], values, isTwoLevelArray))
				{
					cout << "# levels: " << (isTwoLevelArray ? "2" : "1") << endl;
					for(vector<string>::size_type k = 0; k != values.size(); ++k)
						cout << k << ": " << values[k] << endl;
					cout << endl;
				}
			}

			return 0;
		}
	}

	cerr << "ERROR (test=" << test << ")" << endl;
	return 1;
}
