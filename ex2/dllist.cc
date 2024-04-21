#include<stdio.h>
#include<iostream>
#include"dllist.h"
#include "system.h"

using namespace std;

class DLLElement;
class DLList;


DLLElement::DLLElement(void* itemPtr, int sortKey)
{
	next = prev = NULL;
	item = itemPtr;
	key = sortKey;
}


DLList::DLList()
{
	first = NULL;
	last = NULL;
	err_type = -1;
	lock = new Lock("list lock");
    listEmpty = new Condition("list empty cond");
}

DLList::DLList(int err_type)
{
    first = last = NULL;
    this->err_type = err_type;
    lock = new Lock("list lock");
    listEmpty = new Condition("list empty cond");
}

DLList::~DLList()
{
	delete lock;
    delete listEmpty;
	return;
}

void DLList::Prepend(void* item)
{
	lock->Acquire();
	DLLElement *new_item = new DLLElement(item,0);
	if(IsEmpty())
    {
        first=new_item;
        last=new_item;
        new_item->prev=NULL;
        new_item->next=NULL;

    }
    else
    {
        new_item->key=first->key-1;
        new_item->next=first;
        new_item->prev=NULL;
        first->prev=new_item;
        first=new_item;
    }
    lock->Release();
	return;
}

void DLList::Append(void* item)
{
	lock->Acquire();
	DLLElement* ele = new DLLElement(item, 0);
	if (IsEmpty())	// if list is empty
	{
		ele->key = 1;
		ele->prev = NULL;
		ele->next = NULL;
		first = ele;
		last = ele;
	}
	else 	// if list is not empty
	{
		DLLElement* temp;	// add element to tail
		temp = last;
		ele->key = temp->key + 1;
		ele->prev = temp;
		ele->next = NULL;
		last = ele;
		temp->next = ele;
	}
	listEmpty->Signal(lock);    // wake up a waiter, if any
    lock->Release();
	return;
}


void* DLList::Remove()
{
	lock->Acquire();
	while (IsEmpty())
        listEmpty->Wait(lock);

	DLLElement* p;
	p = first;

	if (IsEmpty())	// if list is empty
	{
	    printf("the list is empty!\n");
	    	lock->Release();
		return NULL;
	}
 else
    {
        void *remove_item;
        p=first;
        remove_item=p->item;
/*--------------------------------------------error 10-------------------------------------------------*/
    if (err_type ==10)
    {
        printf("----Switch thread------\n");
        currentThread->Yield();					//error destroy
    }
/*--------------------------------------------error 10-------------------------------------------------*/
        first=first->next;
        if(first==NULL)
        {
        	/*--------------------------------------------error 11-------------------------------------------------*/
        if (err_type ==11)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
            /*--------------------------------------------error 11-------------------------------------------------*/
            last = NULL;
        }
        else
        {
            /*--------------------------------------------error 12-------------------------------------------------*/
        if (err_type ==12)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();
        }
            /*--------------------------------------------error 12-------------------------------------------------*/
            first->prev=NULL;
        }
	lock->Release();
	return &(p->key);
}
}




bool DLList::IsEmpty()
{
    if((first==NULL) && (last ==NULL))
    {
        return true;
    }
    else if ((first!=NULL) && (last !=NULL))
    {
        return false;
    }
    else
        printf("error!first or last is NULL!");
    return false;
}

void DLList::SortedInsert(void* item, int sortKey)
{
	lock ->Acquire();
	DLLElement *new_item=new DLLElement(item,sortKey);
    if(this->IsEmpty())
    {
        first=new_item;
        /*--------------------------------------------error 1-------------------------------------------------*/
        if (err_type ==1)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 1-------------------------------------------------*/
        last=new_item;
        new_item->prev=NULL;
        new_item->next=NULL;
    }
    else
    {
        DLLElement *temp=first;
        while (temp!=NULL&&sortKey>=temp->key) //在find之前插入
        {
            temp=temp->next;
        }
/*--------------------------------------------error 9-------------------------------------------------*/
    if (err_type ==9)
    {
        printf("----Switch thread------\n");
        currentThread->Yield();					//error destroy
    }
/*--------------------------------------------error 9-------------------------------------------------*/
        if(temp==NULL)		//在表尾插入
        {
        	new_item->prev=last;
        /*--------------------------------------------error 7-------------------------------------------------*/
        if (err_type ==7)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 7-------------------------------------------------*/
        	last->next=new_item;
        /*--------------------------------------------error 8-------------------------------------------------*/
        if (err_type ==8)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 8-------------------------------------------------*/
        	last=new_item;
        	last->next=NULL;
        }
        else if(temp==first)	//在表头插入
        {
        	new_item->next=first;
        /*--------------------------------------------error 6-------------------------------------------------*/
        if (err_type ==6)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 6-------------------------------------------------*/
        	first->prev=new_item;
        /*--------------------------------------------error 2-------------------------------------------------*/
        if (err_type ==2)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 2-------------------------------------------------*/
        	first=new_item;
        	first->prev=NULL;
        }
        else //中间插入
        {
            temp->prev->next=new_item;
        /*--------------------------------------------error 4-------------------------------------------------*/
        if (err_type ==4)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 4-------------------------------------------------*/
            new_item->prev=temp->prev;
        /*--------------------------------------------error 3-------------------------------------------------*/
        if (err_type ==3)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 3-------------------------------------------------*/
            new_item->next=temp;
        /*--------------------------------------------error 5-------------------------------------------------*/
        if (err_type ==5)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 5-------------------------------------------------*/
            temp->prev=new_item;
        }
    }
            lock ->Release();
}


void* DLList::SortedRemove(int sortKey)
{
	lock->Acquire();
	while (IsEmpty())
        listEmpty->Wait(lock);

	DLLElement* p = first;
	while (p)
	{
		if (p->key == sortKey)
			break;
		p = p->next;
	}
	if (p == NULL)	// no such element
	{
		lock->Release();
		return NULL;
	}
	else
	{
		if (first == p)	// if remove from head
		{
			first = p->next;
			p->next->prev = NULL;
		}
		else if (last == p)	// if remove from tail
		{
			last = p->prev;
			p->prev->next = NULL;
		}
		else
		{
			p->prev->next = p->next;
			p->next->prev = p->prev;
		}
	}
	lock->Release();
	return p;
}




void DLList::Show()
{
    DLLElement *tmp;
    for (tmp=first;tmp!=NULL;tmp=tmp->next)
    {
        printf("%d ",tmp->key);
    }
    if(first!=NULL&&last!=NULL)
        printf("  -head = %d -tail = %d\n",first->key,last->key);
    else if(first==NULL&&last!=NULL)
        printf("  -head = NULL -tail = %d\n",last->key);
    else if(first!=NULL&&last==NULL)
        printf("  -head = %d -tail = NULL\n",first->key);
    else
    printf("NULL\n");
}
