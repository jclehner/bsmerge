#include <sys/types.h>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <pwd.h>
#include <grp.h>
#include "util.hh"
#include <iostream>
using namespace std;

namespace bsmerge {
namespace util {
namespace {

const string unprivilegedUser = "nobody";
const string unpriviledgedGroup = "nogroup";

const int connectTimeout = 5;

uid_t get_uid_from_name(const string& name)
{
	struct passwd* pw = getpwnam(name.c_str());
	if(!pw)
		throw ErrnoException("getpwnam");

	return pw->pw_uid;
}

gid_t get_gid_from_name(const string& name)
{
	struct group* gr = getgrnam(name.c_str());
	if(!gr)
		throw ErrnoException("getgrnam");

	return gr->gr_gid;
}

class UnprivilegedScope
{
	public:
	UnprivilegedScope()
	: mUid(getuid()), mGid(getgid())
	{
		if(mUid != 0)
			return;

		struct passwd* pw = getpwnam(unprivilegedUser.c_str());
		if(!pw)
			throw ErrnoException("getpwnam");

		if(seteuid(pw->pw_uid) != 0)
			throw ErrnoException("seteuid");

		if(setegid(pw->pw_gid) != 0)
			throw ErrnoException("setegid");
	}

	~UnprivilegedScope()
	{
		if(mUid != 0)
			return;

		seteuid(mUid);
		setegid(mGid);
	}

	private:
	uid_t mUid;
	gid_t mGid;
};

void verify_variable_name(const string& name)
{
	string::size_type pos = name.find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789_");

	if(pos != string::npos)
		throw Exception("Illegal variable name: " + name);
}
}

vector<string> get_process_output_privileged(const string& command, bool includeStderr)
{
	vector<string> ret;

	const char* cmd;
	if(!includeStderr)
		cmd = command.c_str();
	else
		cmd = (command + " 2>&1").c_str();

	FILE* p = popen(cmd, "r");
	if(!p)
		throw ErrnoException("popen");

	const size_t size = 1024;
	char buffer[size];

	string str;

	while(!feof(p))
	{
		size_t bytes = fread(buffer, sizeof(char), size, p);
		if(p != 0)
			str += string(buffer, bytes);
		else if(ferror(p))
			throw ErrnoException("fread");
	}

	pclose(p);

	istringstream istr(str);

	string line;

	while(getline(istr, line))
		ret.push_back(line);

	return ret;
}

inline vector<string> get_process_output(const string& command, bool includeStderr)
{
	UnprivilegedScope unprivilegedScope;
	return get_process_output_privileged(command, includeStderr);
}

vector<string> get_ssh_output(const string& command, const string& hostspec, unsigned short port)
{
	using strings::stringify;

	string sshCommand(
			"ssh -o PasswordAuthentication=no -o ChallengeResponseAuthentication=no "
			"-o ConnectTimeout=" + stringify(connectTimeout) + " "
			"-p " + stringify(port) + " -n " + hostspec + " " + strings::escape(command, true));

	return get_process_output(sshCommand, true);
}

bool get_bash_array(const string& filename, const string& name, vector<string>& outArray, bool isTwoLevelArray)
{
	verify_variable_name(name);

	string varStr = "${" + name + "[@]}";
	if(isTwoLevelArray)
	{
		varStr.insert(0, "\"");
		varStr.append("\"");
	}
	
	outArray = get_process_output(
		"bash -c 'source " + strings::escape(filename, false) + "; " 
		"for E__  in " + varStr + "; do echo ${E__}; done'", 
		false);

	return !outArray.empty();
}

bool get_bash_value(const string& filename, const string& name, string& outString)
{
	verify_variable_name(name);

	vector<string> lines = get_process_output(
		"bash -c 'source " + strings::escape(filename, false) + "; echo ${" + name + "}'", 
		false);

	if(lines.size() != 1 || lines[0].empty())
		return false;

	outString = lines[0];
	return true;
}

vector<string> explode(const string& str, char delim)
{
	vector<string> ret;
	istringstream istr(str);
	string token;
	while(getline(istr, token, delim))
		ret.push_back(token);

	return ret;
}

string implode(const vector<string>& vec, char delim)
{
	string ret;

	for(vector<string>::size_type i = 0; i != vec.size(); ++i)
	{
		if(i != 0)
			ret += delim;
		ret += vec[i];
	}

	return ret;
}

bool can_connect_passwordless(const string& hostspec, unsigned short port)
{
	using strings::stringify;

	string cmd(
		"ssh -q -o ChallengeResponseAuthentication=no "
		"-o ConnectTimeout=" + stringify(connectTimeout) + " " +
		"-o PasswordAuthentication=no -p " + stringify(port) + " -n " +
		hostspec + " >> /dev/null");

	int status = system(cmd.c_str());

	if(WIFEXITED(status))
		return WEXITSTATUS(status) == 0;
	else
		throw Exception("ssh killed/stopped");

	return true;
}

}}
