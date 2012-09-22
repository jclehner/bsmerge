#include <iostream>
#include <unistd.h>
#include "../config.hh"
using namespace std;

namespace bsmerge {
namespace {



struct CheckSlaveArg
{
	SlaveConfig slave;
	vector<string> ebuilds;
};

void check_slave(void* arg)
{
	const CheckSlaveArg* myArg = reinterpret_cast<const CheckSlaveArg*>(arg);

	if(!util::can_connect_passwordless(myArg->slave.hostspec, myArg->slave.port))
		return;

	const Config& config(Config::get());

	int type = myArg->slave.type;

	if(type != SlaveConfig::CHROOT && type != SlaveConfig::NATIVE)
	{
		config.warn() << "Ignoring " << myArg->slave.hostspec << ": unimplemented slave type." << endl;
		pthread_exit(NULL);
	}

	string emergeCmd("emerge");

	if(!myArg->slave.name.empty())
		config.get_value("EMERGE_CMD_" + myArg->slave.name, emergeCmd);

	vector<string> output = util::get_ssh_output("emerge -pv1 " + util::implode(myArg->ebuilds, ' '),
			myArg->slave.hostspec, myArg->slave.port);

	for(auto vi = output.begin(); vi != output.end(); ++vi)
	{
		if(strings::begins_with("[", *vi))
		{
			typedef string::size_type ST;
			ST pos = vi->find(']');
			if(pos == string::npos)
				continue;

			string line(vi->substr(pos + 1));
			strings::trim(line);
		}
	}



}



}}

using namespace bsmerge;

extern "C" int find_best_slave_main(int argc, char* argv[], const Config& config) 
{
	vector<string> slaves;
   	config.get_bash_array("SLAVES", slaves, true);
	if(slaves.empty())
	{
		config.err() << "No SLAVES in config file" << endl;
		return 1;
	}

	for(auto vi = slaves.begin(); vi != slaves.end(); ++vi)
	{
		SlaveConfig slave;
		if(!strings::convert(*vi, slave))
		{
			config.err() << "Invalid entry in SLAVES: " << *vi << endl;
			return 1;
		}

		cout << "# " << slave.hostspec << ": " << flush;
		cout << (util::can_connect_passwordless(slave.hostspec, slave.port) ? "UP" : "DOWN") << endl;
	}

	return 0;
}
