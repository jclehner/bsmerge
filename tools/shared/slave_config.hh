#ifndef BSMERGE_SLAVE_CONFIG
#define BSMERGE_SLAVE_CONFIG

namespace bsmerge {

struct SlaveConfig
{
	enum
	{
		NATIVE,
		QEMU,
		CHROOT,
		CROSSDEV,
		UNKNOWN
	};

	std::string hostspec;
	int type;
	unsigned short port;
	std::string name;

	friend std::istream& operator>>(std::istream& is, SlaveConfig& config);
};
}
#endif
