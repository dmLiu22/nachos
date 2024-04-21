// synchlist.cc
//	Routines for synchronized access to a list.
//
//	Implemented by surrounding the List abstraction
//	with synchronization routines.
//
// 	Implemented in "monitor"-style -- surround each procedure with a
// 	lock acquire and release pair, using condition signal and wait for
// 	synchronization.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synchlist.h"

//----------------------------------------------------------------------
// SynchList::SynchList
//	Allocate and initialize the data structures needed for a 
//	synchronized list, empty to start with.
//	Elements can now be added to the list.
//----------------------------------------------------------------------

SynchList::SynchList()
{
    list = new List();
    lock = new Lock("list lock"); 
    listEmpty = new Condition("list empty cond");
}

//----------------------------------------------------------------------
// SynchList::~SynchList
//	De-allocate the data structures created for synchronizing a list. 
//----------------------------------------------------------------------

SynchList::~SynchList()
{ 
    delete list; 
    delete lock;
    delete listEmpty;
}

//----------------------------------------------------------------------
// SynchList::Append
//      Append an "item" to the end of the list.  Wake up anyone
//	waiting for an element to be appended.
//
//	"item" is the thing to put on the list, it can be a pointer to 
//		anything.
//----------------------------------------------------------------------

void
SynchList::Append(void *item)
{
    lock->Acquire();
    list->Append(item);
    listEmpty->Signal(lock);
    lock->Release();
}

//----------------------------------------------------------------------
// SynchList::Remove
//      Remove an "item" from the beginning of the list.  Wait if
//	the list is empty.
// Returns:
//	The removed item. 
//----------------------------------------------------------------------

void *
SynchList::Remove()
{
    void *item;

    lock->Acquire();			// 上锁
    while (list->IsEmpty())
	listEmpty->Wait(lock);		// wait 直到非空
    item = list->Remove();
    ASSERT(item != NULL);
    lock->Release();
    return item;
}

//----------------------------------------------------------------------
// SynchList::Mapcar
//      Apply function to every item on the list.  Obey mutual exclusion
//	constraints.
//
//	"func" is the procedure to be applied.
//----------------------------------------------------------------------

void
SynchList::Mapcar(VoidFunctionPtr func)
{ 
    lock->Acquire(); 
    list->Mapcar(func);
    lock->Release(); 
}
