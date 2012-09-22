#include "slave_config.hh"
using namespace std;

namespace bsmerge {

std::istream& operator>>(std::istream& is, SlaveConfig& config)
{
	string type;

	if(is >> config.hostspec && is >> type)
	{
		if(type == "native")
			config.type = SlaveConfig::NATIVE;
		else if(type == "qemu")
			config.type = SlaveConfig::QEMU;
		else if(type == "chroot")
			config.type = SlaveConfig::CHROOT;
		else if(type == "crossdev")
			config.type = SlaveConfig::CROSSDEV;
		else
			config.type = SlaveConfig::UNKNOWN;

		if(!(is >> config.port))
		{
			is.clear();
			config.port = 22;
		}

		if(!(is >> config.name))
			is.clear();
	}

	return is;
}
}
