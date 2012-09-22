#ifndef BSMERGE_THREAD_HH
#define BSMERGE_THREAD_HH
#include <pthread.h>

namespace bsmerge {

class Thread
{
	public:
	
	static const unsigned FLAG_DELAY_EXECUTION = 1;
	static const unsigned FLAG_JOIN_ON_DESTROY = 2;

	Thread(unsigned flags = 0, void* arg = NULL)
	: mFlags(flags), pmThread(NULL)
	{
		if(!(flags & FLAG_DELAY_EXECUTION))
			execute(arg);
	}

	inline void execute(void* arg = NULL)
	{
		if(pmThread)
			return;

		pmThread = new pthread_t;
		int status = pthread_create(pmThread, NULL, run, arg);
		if(status != 0)
			throw ErrnoException("pthread_create", status);
	}

	virtual void* run(void* arg) = 0;

	private:
	virtual ~Thread()
	{
		if(pmThread)
		{
			if(mFlags & FLAG_JOIN_ON_DESTROY)
				pthread_join(*pmThread, NULL);

			delete pmThread;
		}
	}


	Thread(const Thread& thread);
	Thread& operator=(const Thread& thread);

	unsigned mFlags;
	pthread_t* pmThread;
	pthread_attr_t mAttr;
};
}
#endif
