#include "Alarm.h"

Alarm *Alarm::instance;

extern int testnum;

void Alarm::new_instance()
{
	instance=new Alarm();
}

Alarm::Alarm()
{
	queue = new DLList;
	//timer = new Timer(timerhandler, 0, false);
	waiters = 0;
}

Alarm::~Alarm()
{
	delete queue;
	//delete timer;
}

// 将事件插入中断队列，让其睡眠，等时间到了再让其进入就绪队列
void Alarm::Pause(int howLong)	// howlong单位为中断次数
{
	waiters++;
	Thread *loop_t;
	if(waiters==1)
	{
		loop_t = new Thread("loop thread");
		loop_t->Fork(check, 0);			//使至少有一个线程在运行，防止系统终止
	}
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	//必须关中断
	int Ticks = TimerTicks * howLong;  //TimerTicks默认为100，见stats.h
	queue->SortedInsert((void *)currentThread,stats->totalTicks + Ticks);	//按结束时间依序插入链表 // stats->totalTicks += SystemTicks
	if(testnum==2)
    printf("\033[40;35m %s \033[0m在时刻\033[40;32m %d \033[0m睡觉,睡\033[40;34m %d \033[0m个时间片,在\033[40;32m %d \033[0m时刻睡醒\n", currentThread->getName(), stats->totalTicks, howLong,stats->totalTicks+Ticks);
	currentThread->Sleep();
	(void)interrupt->SetLevel(oldLevel);
}

void Alarm::Pause1(int howLong)	// howlong单位为中断次数
{
	waiters++;
	Thread *loop_t;
	if(waiters==1)
	{
		loop_t = new Thread("loop thread");
		loop_t->Fork(check, 0);			//使至少有一个线程在运行，防止系统终止
	}
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	//必须关中断
	int Ticks = TimerTicks * howLong;  //TimerTicks默认为100，见stats.h
	queue->SortedInsert((void *)currentThread,stats->totalTicks + Ticks);	//按结束时间依序插入链表 // stats->totalTicks += SystemTicks
    printf("\033[40;35m 小车%s \033[0m在时刻\033[40;32m %d \033[0m上桥,至少行驶\033[40;34m %d \033[0m个时间片,最早在\033[40;32m %d \033[0m时刻下桥\n", currentThread->getName(), stats->totalTicks, howLong,stats->totalTicks+Ticks);
	currentThread->Sleep();
	(void)interrupt->SetLevel(oldLevel);
}


void check(int which)
{
	while (Alarm::instance->waiters!=0)
	{
		currentThread->Yield();
	}
	currentThread->Finish();
}

// 一个闹钟中断到来，唤醒目前应最早唤醒的线程，将其加入就绪队列
void Alarm::CheckIfDue()
{
	int duetime=-1;
	Thread *thread=NULL;
	IntStatus oldLevel = interrupt->SetLevel(IntOff);	//必须关中断
	thread = (Thread *)queue->Remove(&duetime);
	while (thread!= NULL)
	{
		if (duetime - stats->totalTicks <= 0)
		{
			waiters--;
//			printf("线程应该在 %d! 唤醒， 发出时间片切换的时刻是 %d\n",duetime,stats->totalTicks);
if(testnum==2)
            printf("在\033[40;36m %d \033[0m时刻调度器发现它苏醒了，放入就绪队列\n", stats->totalTicks);
			scheduler->ReadyToRun(thread);
			thread = (Thread *)queue->Remove(&duetime);
		}
		else
		{
			queue->SortedInsert((void *)thread,duetime);
			break;
		}
	}
	(void)interrupt->SetLevel(oldLevel);
}
