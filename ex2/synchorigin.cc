// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//  De-allocate semaphore, when no longer needed.  Assume no one
//  is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//  Wait until semaphore value > 0, then decrement.  Checking the
//  value and decrementing must be done atomically, so we
//  need to disable interrupts before checking the value.
//
//  Note that Thread::Sleep assumes that interrupts are disabled
//  when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts

    while (value == 0) {            // semaphore not available
    queue->Append((void *)currentThread);   // so go to sleep
    currentThread->Sleep();
    }
    value--;                    // semaphore available,
                        // consume its value

    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//  Increment semaphore value, waking up a waiter if necessary.
//  As with P(), this operation must be atomic, so we need to disable
//  interrupts.  Scheduler::ReadyToRun() assumes that threads
//  are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)    // make thread ready, consuming the V immediately
    scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}


Lock::Lock(char* debugName)
{
    name = debugName;
    isBusy = false;
    queue = new List;
    owner = NULL;
}

Lock::~Lock()
{
    delete this;
}

bool Lock::isHeldByCurrentThread()
{
    if (owner == currentThread && isBusy)
        return true;
    else
        return false;
}

void Lock::Acquire()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // 禁止中断
    while(isBusy) // 锁被占用
    {
        queue->Append((void *) currentThread);
        currentThread->Sleep();         // 阻塞
    }
    isBusy = true;           // 占用锁
    owner = currentThread;
    (void) interrupt->SetLevel(oldLevel);   // 允许中断
}

void Lock::Release()
{
    Thread* thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // 禁止中断
    thread = (Thread *)queue->Remove();
    if(thread != NULL)  //唤醒一个阻塞的进程
    {
        scheduler->ReadyToRun(thread);
    }
    isBusy = false;
    owner = NULL;
    (void) interrupt->SetLevel(oldLevel);   // 允许中断
}

Condition::Condition(char* debugName)
{
    name = debugName;
    queue = new List;
}

Condition::~Condition()
{
    delete this;
}

void Condition::Wait(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);//禁止中断
    queue->Append((void *)currentThread);
    conditionLock->Release();               // 释放条件锁
    (void) interrupt->SetLevel(oldLevel);   // 允许中断
    currentThread->Sleep();
    conditionLock->Acquire();               // 重新获得锁
}

void Condition::Signal(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    Thread* thread;
    thread = (Thread *)queue->Remove();
    if(thread != NULL)           // wake up next thread
        scheduler->ReadyToRun(thread);  // use mesa semantics
    (void) interrupt->SetLevel(oldLevel);
}

void Condition::Broadcast(Lock* conditionLock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    Thread* thread;
    while(!queue->IsEmpty())
    {
        thread = (Thread *)queue->Remove();
        scheduler->ReadyToRun(thread);
    }
    (void) interrupt->SetLevel(oldLevel);
}