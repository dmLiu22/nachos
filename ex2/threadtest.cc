// threadtest.cc
//  Simple test case for the threads assignment.
//
//  Create two threads, and have them context switch
//  back and forth between themselves by calling Thread::Yield,
//  to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
#include<stdlib.h>
#include<time.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "synch.h"
#include "Table.h"
#include "BoundedBuffer.h"


extern void GenerateN(int which,int N, DLList *list);
extern void RemoveN(int which,int N, DLList *list);

// testnum is set in main.cc
int testnum = 1;
int T, N, E;
DLList *list;
Lock *lock;
Condition *cond;
BoundedBuffer *buffer;
Table *table;

//----------------------------------------------------------------------
// SimpleThread
//  Loop 5 times, yielding the CPU to another ready thread
//  each iteration.
//
//  "which" is simply a number identifying the thread, for debugging
//  purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;

    for (num = 0; num < 5; num++) {
    printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ConcurrentError1
//  insert N items -- switch threads -- remove N items -- switch threads
// Error phenomenon
//  thread may take out items that do not belong to itself
//----------------------------------------------------------------------

void
ConcurrentError1(int which)
{
    printf("*** thread %d\n", which);
    GenerateN(which,N, list);
    RemoveN(which,N, list);
    //currentThread->Yield();
}

void
ConcurrentError2(int which)
{
    int key[] = { 3,2,1,4,5,6};
    int item[] = {1,2,3,4,5,6};
    int i = 0;
    while (++i < 4) {
        printf("*** thread %d is going to insert an item with key: %d\n",
                                            which, key[(i - 1) * 2 + which]);
        list->SortedInsert(&item[(i - 1) * 2 + which], key[(i - 1) * 2 + which]);
        list->Show();
        currentThread->Yield();
    }
    printf("*** thread %d\n", which);
    RemoveN(which,3, list);
}

void
ConcurrentError3(int which)
{
    int key[] = {4,5,3,8,9,10};
    int item[] = {11,22,33,44,55,66};
    int i = 0;
    while (++i < 4) {
        printf("*** thread %d is going to insert an item with key: %d\n",
                                            which, key[(i - 1) * 2 + which]);
        list->SortedInsert(&item[(i - 1) * 2 + which], key[(i - 1) * 2 + which]);
        list->Show();
        currentThread->Yield();
    }
    //printf("*** thread %d\n", which);
    //currentThread->Yield();
    printf("*** thread %d\n", which);
    RemoveN(which,3, list);
}
void
ConcurrentError4(int which)
{
    int item[] = {0,0,0,0,0,0};
    int i = 0;
	if(which == 0)
	{
		printf("*** thread %d is going to insert the first element key = 10 \n",which);
		list->SortedInsert(&item[i], 10);
		list->Show();
		currentThread->Yield();
	    while (i++ < 3)
		{
			printf("***1 thread %d\n",which);
			list->Prepend(&item[i]);
			list->Show();
		}
				currentThread->Yield();
		while (i++ <=6)
		{
			printf("***1 thread %d\n",which);
			list->Prepend(&item[i]);
			list->Show();
		}

	}
	else
	{
		printf("*** thread %d remove\n",which);
		list->Remove();
		list->Show();
		currentThread->Yield();
	}


    //printf("*** thread %d\n", which);
    //currentThread->Yield();
    //printf("*** thread %d\n", which);
}
const int error_num = 4;    // total number of concurrent errors
typedef void (*func) (int);
func ConcurrentErrors[error_num] = {ConcurrentError1, ConcurrentError2, ConcurrentError3,ConcurrentError4};
                                    //ConcurrentError4, ConcurrentError5, ConcurrentError6};


void
SynThread(int which)
{
    printf("*** thread %d is to Wait\n", which);
    lock->Acquire();
    cond->Wait(lock);
    lock->Release();
    printf("*** thread %d is awakened\n", which);
}


//----------------------------------------------------------------------
// ThreadTest1
//  Set up a ping-pong between two threads, by forking a thread
//  to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
//ThreadTest2
//
//----------------------------------------------------------------------

void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

    lock = new Lock("ThreadTest2");
    cond = new Condition("ThreadTest2");



    list = new DLList(E);
    int i;

    for (i = 0; i < T; i++) {
        Thread *t = new Thread("forked thread");
        t->Fork(ConcurrentError1, i);
        //currentThread->Yield();
    }
    lock->Acquire();
    cond->Signal(lock);
    lock->Release();
}

void ThreadTest3()
{
    DEBUG('t', "Entering ThreadTest2");

    lock = new Lock("ThreadTest3");
    cond = new Condition("ThreadTest3");
    // verify that a Signal cannot affect a subsequent Wait
    lock->Acquire();
    cond->Signal(lock);
    lock->Release();
    printf("*** thread 0 Signal\n");

    // these threads will wait for cond
    for (int i = 1; i < 5; i++) {
        Thread *t = new Thread("forked thread");
        t->Fork(SynThread, i);
        currentThread->Yield();
    }

    printf("*** thread 0 Signal\n");
    lock->Acquire();
    cond->Signal(lock);
    lock->Release();

    currentThread->Yield();

    printf("*** thread 0 BroadCast\n");
    lock->Acquire();
    cond->Broadcast(lock);
    lock->Release();
}

//----------------------------------------------------------------------
//TableActions
//	Create a table according to parameter N.
//  Store, get and release elements while switching threads.
//----------------------------------------------------------------------
void
TableActions(int which)
{
    int indexArr[N];
    for(int i =0; i < N; i++) {
        void *obj = (void *)(Random() % 1000);
        indexArr[i] = table->Alloc(obj);
        if(indexArr[i]==-1)
        {
                printf("T_T线程 %d 试图存储数据 %d 到地址 [%d]，但是表格已满，插入失败 \n", which, (int)obj, indexArr[i]);
                i--;
        }
        else{
                printf("♥‿♥ 线程 %d 存储数据 %d 在地址 [%d] \n", which, (int)obj, indexArr[i]);
        }
        table->Print();
        currentThread->Yield();
    }
    for(int i =0; i < N; i++) {
        printf("^_^ 线程 %d 从地址 [%d] 得到 %d  \n", which, indexArr[i], (int)table->Get(indexArr[i]));
        table->Print();
        currentThread->Yield();
    }
    for(int i =0; i < N; i++) {
        table->Release(indexArr[i]);
        printf("o_O 线程 %d 释放地址 [%d] 的空间 \n", which, indexArr[i]);
        table->Print();
        currentThread->Yield();
    }
    printf("T_T 线程 %d 结束 \n", which);
}


void
TableTest()
{
    DEBUG('t', "Entering TableTest");

    int maxTableSize = E;
    table = new Table(maxTableSize);
    for(int i = 0; i < T; i ++) {
        Thread *t = new Thread("forked thread");
        t->Fork(TableActions, i);
        currentThread->Yield();
    }
}
//----------------------------------------------------------------------
//WriteBuffer
//	Create an pointer named 'data' that points to an area with
//'num' pieces of data and write these data to the buffer.
//----------------------------------------------------------------------

void
WriteBuffer(int num)
{
    printf("\n 现在的生产者是：生产者 %s\n", currentThread->getName());
    char* data = new char;
    int i;
    printf(" 请输入你要写入的数据。请严格按照你输入的大小\n");
         scanf("%s",data);
    //for (i = 0; i < num; i++)
    //{
     //scanf("%c",data+i);
        //*(data + i) = (Random() % 26) + 65;
   // }
    printf("生产者 %s 将把这些内容写入缓冲区:\n", currentThread->getName());
    for (i = 0; i < num-1 ; i++)
        printf("%c", *(data + i));
     printf("%c\n", *(data + i));
    buffer->Write((void*)data, num);
    printf("生产者 %s 完成生产.\n", currentThread->getName());
    buffer->Showbuffer();
}

//----------------------------------------------------------------------
//ReadBuffer
//	Read 'num' bytes of data from buffer to the area at the
//beginning of '* data'
//----------------------------------------------------------------------

void
ReadBuffer(int num)
{
    printf("\n 现在的消费者是：消费者 :%s\n", currentThread->getName());
    char* data = new char;
    buffer->Read((void*)data, num);
    printf("消费者 %s 消费完成,从缓冲区读出这些数据:", currentThread->getName());
    int i;
    for (i = 0; i < num - 1; i++)
        printf("%c", *(data + i));
    printf("%c\n", *(data + i));
    buffer->Showbuffer();
}

void
BufferTest_prior()
{
 int num1, num2, i;
    DEBUG('t', "Entering BufferTest");
    buffer = new BoundedBuffer(T);
    printf("\n输入消费者每次消费多少空间:");
    scanf("%d", &num1);
    printf("\n输入生产者每次生产多少空间:");
    scanf("%d", &num2);
    printf("\n");
    srand(time(NULL));
    Thread* a1[10];
    Thread* a2[10];
    for (int i = 0; i < E; i++) { //producer
        char* str = new char[20];
        sprintf(str, "WriteThread %d", i);
        int prior1;
        printf("\n输入该线程 %s 的优先级: ", str);
        scanf("%d", &prior1);
        a1[i] = new Thread(str, prior1);
    }
    for (int i = 0; i < N; i++) { //comsume
        char* str = new char[20];
        sprintf(str, "ReadThread %d", i);
        int prior1;
        printf("\n输入该线程 %s 的优先级: ", str);
        scanf("%d", &prior1);
        a2[i] = new Thread(str, prior1);
    }
    for (int i = 0; i < E; i++) {
        a1[i]->Fork(WriteBuffer, num2);
    }
    for (int i = 0; i < N; i++) {
        a2[i]->Fork(ReadBuffer, num1);
    }
}
//----------------------------------------------------------------------
//BufferTest
// 	Invoke a buffer test routine.In this test routine,we create
//       some read thread and wtire thread to test whether current
//       buffer is thread-safe.
//       Some Params:
//	T:maxsize of boundedbuffer
//  N:num of read threads(read from buffer)
//	E:num of write threads(write to buffer)
//	num1:num of read bytes
//  num2:num of write bytes
//----------------------------------------------------------------------
void
BufferTest()
{
     int num1, num2, i;
     DEBUG('t', "Entering BufferTest");
     buffer = new BoundedBuffer(T);
     printf("\n输入消费者每次消费多少空间:");
     scanf("%d", &num1);
     printf("\n输入生产者每次生产多少空间:");
     scanf("%d", &num2);
     printf("\n");
    int k, count1 = 0, count2 = 0;
     srand(time(NULL));
    for(i = 0; i < N + E; i++)
    {
        char *str = new char[20];
        if(i==0)
        {
                sprintf(str, "WriteThread %d", count2);
                Thread * t = new Thread(str);
                t -> Fork(WriteBuffer, num2);
                count2++;
        }
        else{
                k = (rand()+Random());
                 k%=2;
        if (k == 1)
        {
            if (count1 < N)
            {
                sprintf(str, "ReadThread %d", count1);
                Thread * t = new Thread(str);
                t -> Fork(ReadBuffer, num1);
                count1++;
            }
            else
            {
                sprintf(str, "WriteThread %d", count2);
                Thread * t = new Thread(str);
                t -> Fork(WriteBuffer, num2);
                count2++;
            }
        }
        else
        {
            if (count2 < E)
            {
                sprintf(str, "WriteThread %d", count2);
                Thread * t = new Thread(str);
                t -> Fork(WriteBuffer, num2);
                count2++;
            }
            else
            {
                sprintf(str, "ReadThread %d", count1);
                Thread * t = new Thread(str);
                t -> Fork(ReadBuffer, num1);
                count1++;
            }
        }
        }
    }
}

//----------------------------------------------------------------------
// ThreadTest
//  Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest(int t, int n, int e)
{
    switch (testnum) {
    case 1:
        ThreadTest1();
        break;
    case 2:
        T = t;
        N = n;
        E = e;
        ThreadTest2();
        break;
    case 3:
        ThreadTest3();
        break;
    case 4:
        T = t;
        N = n;
        E = e;
        TableTest();
        break;
    case 5:
        T = t;
        N = n;
        E = e;
        BufferTest();
        break;
         case 6:
        T = t;
        N = n;
        E = e;
        BufferTest_prior();
        break;
    default:
    printf("No test specified.\n");
    break;
    }
}