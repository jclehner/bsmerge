#ifndef BSMERGE_EXCEPTION_HH
#define BSMERGE_EXCEPTION_HH
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <string>

namespace bsmerge {

class Exception : public std::runtime_error
{
	public:
	Exception(const std::string& message)
	: std::runtime_error(message.c_str())
	{}

	virtual ~Exception() throw() {}

};

class ErrnoException : public Exception
{
	public:
	ErrnoException(const std::string& message, int error = errno)
	: Exception(message + ": " + strerror(error))
	{}

	virtual ~ErrnoException() throw() {}
};

}
#endif
