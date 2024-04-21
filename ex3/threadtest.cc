// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "Table.h"
#include "BoundedBuffer.h"
#include "EventBarrier.h"
#include "Alarm.h"
#include "Bridge.h"
#include "Elevator.h"

int testnum = 1, threadnum = 1, err_type = 0, n;
extern int consumernum;
extern int producernum;
int floornum = 10;   // The number of the floor(-f)
int elevatornum = 1; // The number of the elevator(-l)
int capacity = 20;   // The number of the elevator'capacity(-c)
int time1[10] = {0, 1, 6, 3, 10, 5, 6, 7, 8, 9};
int cardir[20]={0,0,1,0,1,1,1,0,1,0,1};
EventBarrier *barr = new EventBarrier();
Alarm *alarm = new Alarm();
Alarm *alarm1 = new Alarm();
Bridge *bridge = new Bridge();
Building *building;

extern void rider(int id, int srcFloor, int dstFloor);

void EventBarrierThread(int Threadid)
{
    if (Threadid == 0)
    {
        currentThread->Yield();
        printf("===============线程 \033[40;34m%d\033[0m  signal====================\n", Threadid);
        barr->Signal();
        printf("===============线程 \033[40;34m%d\033[0m  finish====================\n", Threadid);
    }
    else
    {
        printf("线程 \033[40;35m%d\033[0m is \033[40;32mwaiting\033[0m\n", Threadid);
        printf("                         等待线程的数目：\033[40;31m%d\033[0m\n", barr->Waiters()+1);
        barr->Wait();
        printf("线程 \033[40;35m%d\033[0m is \033[40;32mcompleted\033[0m\n", Threadid);
        printf("                         等待线程的数目：\033[40;31m%d\033[0m\n", barr->Waiters()-1);
        barr->Complete();
    }
}





void EventBarrierTest()
{
    char name[25];
    int i;
    DEBUG('t', "Entering EventBarrierTest");
    for (i = 1; i <= threadnum; i++)
    {
        sprintf(name, "EventBarrier_Thread %d", i);
        Thread *t = new Thread(name);
        t->Fork(EventBarrierThread, i);
        currentThread->Yield();
    }
    EventBarrierThread(0);
}

void testAlarm(int id)
{
//	printf("%s ", currentThread->getName());
	Alarm::instance->Pause(time1[id]);
	printf("\033[40;35mThread %d\033[0m在时刻\033[40;32m %d \033[0m运行\n", id,stats->totalTicks);
}

void AlarmTest()
{
	DEBUG('t', "EventBarrier_test");
	char name[15];
	for (int i = 1; i <=threadnum; i++)
	{
		sprintf(name, "Thread %d", i);
		Thread *t = new Thread(name);
		t->Fork(testAlarm, i);
		currentThread->Yield();
	}
}
void BridgeThread(int id)
{
	int direct=cardir[id];
    bridge->ArriveBridge(direct);
    bridge->CrossBridge(direct);
    Alarm::instance->Pause1(3);
    bridge->ExitBridge(direct);
}

void BridgeTest()
{
    DEBUG('t', "Entering ThreadTest3");
    char **name;
    name = new char *[threadnum];
    for (int i = 1; i <= threadnum; i++)
    {
        name[i] = new char[5];
        sprintf(name[i], "%d", i);
        Thread *t = new Thread(name[i]);
        t->Fork(BridgeThread, i);
    }
}

void ElevatorThread(int id)
{
    int currentfloor = 0, goalfloor = 0;
    while (currentfloor == goalfloor || currentfloor == 0 || goalfloor == 0)
    {
        currentfloor = rand() % floornum;
        goalfloor = rand() % floornum;
    };
    printf("\033[40;35m乘客 %d 号\033[0m: 从第%d层 --> 第%d层\n",id,currentfloor,goalfloor);
    rider(id, currentfloor, goalfloor);
}

void ElevatorTest()
{
    DEBUG('t', "Entering ThreadTest4");
    char **name;
    building = new Building("building", floornum, elevatornum);
    printf("elevatornum = %d  ridernum = %d  floornum = %d capacity = %d \n\n", elevatornum, threadnum, floornum, capacity);
    name = new char *[threadnum];
    for (int i = 1; i <= threadnum; i++)
    {
        name[i] = new char[5];
        sprintf(name[i], "%d", i);
        Thread *t = new Thread(name[i]);
        t->Fork(ElevatorThread, i);
    }
    // ElevatorThread(0);
}




void ThreadTest()
{
    switch (testnum)
    {
        case 1:
        EventBarrierTest();
        break;
    case 2:
        AlarmTest();
        break;
    case 3:
        BridgeTest();
        break;
    case 4:
        ElevatorTest();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}
