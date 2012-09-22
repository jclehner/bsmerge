#ifndef DMERGE_UTIL_H
#define DMERGE_UTIL_H
#include <vector>
#include <string>
#include "strings.hh"



namespace bsmerge {
namespace util {

	std::vector<std::string> get_process_output(const std::string& command, 
			bool includeStderr = true);
	std::vector<std::string> get_process_output_unprivileged(const std::string& command, 
			bool includeStderr = true);

	std::vector<std::string> get_ssh_output(const std::string& command, const std::string& hostspec, unsigned short port = 22);

	bool get_bash_array(const std::string& file, const std::string& name, 
			std::vector<std::string>& outArray, bool isTwoLevelArray = false);

	bool get_bash_value(const std::string& file, const std::string& name, std::string& outString);

	template<typename T> bool get_bash_value(const std::string& file, const std::string& name, 
			T& outValue)
	{
		std::string str;
		if(!get_bash_value(file, name, str))
			return false;

		return bsmerge::strings::convert<std::string>(str, outValue);
	}

	std::vector<std::string> explode(const std::string& str, char delim);
	std::string implode(const std::vector<std::string>& vec, char delim);

	bool can_connect_passwordless(const std::string& hostspec, unsigned short port);

}}
#endif
