#pragma once
#include <TaskScheduler.h>
class PropegationManager
{
	Task t1(2000, 10, &t1Callback);
	Scheduler runner;
public:
	void t1Callback();
	PropegationManager(Task t1);
	~PropegationManager();
};

