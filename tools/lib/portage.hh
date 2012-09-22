#ifndef DMERGE_PORTAGE_HH
#define DMERGE_PORTAGE_HH
#include <string>
#include <vector>

namespace bsmerge
{
	class PortageRequest
	{
		public:
		PortageRequest(const std::string& hostname)
		: mHostname(hostname)
		{
		
		}

		private:

		std::string mHostname;

	}

	




}
