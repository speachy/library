#ifndef THREADUTIL_H
#define THREADUTIL_H

#include <iostream>

#include "thread.h"

// SystemCommand_Thread - an autonomous ThreadFunction for executing a command on
// a subthread.  Inherits from Thread as well as ThreadFunction, and automatically
// launches the thread.
// Use like this:
//
// SystemCommand_Thread mythread("/path/to/some_command_that_will_take_a_while arguments...");
// while(!mythread.TestFinished())
// {
//   do progress display or whatever here...
// }

class Thread_SystemCommand : public ThreadFunction, public Thread
{
	public:
	Thread_SystemCommand(const char *cmd) : ThreadFunction(), Thread(this), cmd(cmd)
	{
		Start();
		WaitSync();
	}
	virtual ~Thread_SystemCommand()
	{
	}
	virtual int Entry(Thread &t)
	{
		SendSync();
		try
		{
			if(system(cmd))
				throw "Command failed";
		}
		catch(const char *err)
		{
			std::cerr << "Subthread error: " << err << std::endl;
			returncode=-1;
		}
		return(returncode);
	}
	protected:
	const char *cmd;
};


// ThreadFunction for use with a Classic c-style callback function

class ThreadFunction_Callback : public ThreadFunction
{
	public:
	ThreadFunction_Callback(int (*entry)(Thread &t,void *ud),void *UserData)
		: ThreadFunction(), entry(entry), userdata(UserData)
	{
	}
	virtual ~ThreadFunction_Callback()
	{
	}
	virtual int Entry(Thread &t)
	{
		if(entry)
			return((*entry)(t,userdata));
		else
			return(0);
	}
	protected:
	int (*entry)(Thread &t,void *ud);
	void *userdata;
};


#endif

