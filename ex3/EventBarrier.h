#pragma once
#include "synch.h"
#include "system.h"

class EventBarrier
{
public:
	EventBarrier();
	~EventBarrier();

	void Wait();	
	void Signal();	
	void Complete();
	int	Waiters();	
private:
	int waiternum;
	Lock *lock;
	Condition *event, *comp;	//two conditions
	bool signaled;
};