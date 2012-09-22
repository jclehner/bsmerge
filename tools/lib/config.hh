#ifndef BSMERGE_CONFIG_HH
#define BSMERGE_CONFIG_HH
#include <iostream>
#include "util.hh"

namespace bsmerge {

class Config
{
	public:	

	static const Config& get()
	{
		static Config* instance = NULL;
		if(!instance)
			instance = new Config;
		return *instance;
	}

	inline std::ostream& err() const {
		return mErrStream << "ERROR: ";
	}

	inline std::ostream& warn() const {
		return mErrStream << "WARNING: ";
	}

	inline std::ostream& verbose() const {
		return mOutStream << "# ";
	}

	inline std::string file() const {
		return "bsmerge.conf";
	}

	inline bool get_bash_array(const std::string& name, std::vector<std::string>& outArray, bool isTwoLevelArray = false) const {
		return util::get_bash_array(file(), name, outArray, isTwoLevelArray);
	}
	
	inline bool get_value(const std::string& name, std::string& outStr) const {
		return util::get_bash_value(file(), name, outStr);
	}

	private: 

	Config()
	: mOutStream(std::cout), mErrStream(std::cerr)
	{

	}

	std::ostream& mOutStream;
	std::ostream& mErrStream;
};
}
#endif
