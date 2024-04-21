
#include "dllist.h"

DLLElement::DLLElement(void *itemPtr, int sortKey)
{
	//currentThread->Yield();		//error
    item=itemPtr;
    //currentThread->Yield();		//error
    key=sortKey;
    //currentThread->Yield();		//error
    prev=NULL;
    //currentThread->Yield();		//error
    next=NULL;
    //currentThread->Yield();		//error
}

DLList::DLList()
{
    first=NULL;
    last=NULL;
    err_type = 0;
}

DLList::DLList(int type)
{
    first=NULL;
    last=NULL;
    err_type = type;
}

DLList::~DLList()
{
    while(!IsEmpty())
    {
        Remove(NULL);
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

void DLList::Prepend(void *item)
{
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
}

void DLList::Append(void *item)
{
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
        new_item->key=last->key+1;
        new_item->next=NULL;
        new_item->prev=last;
        last->next=new_item;
        last=new_item;
    }
}

void *DLList::Remove(int *keyPtr)
{
    DLLElement *p;
    if(IsEmpty())
    {
    	keyPtr=NULL;
        printf("the list is empty!\n");
        return NULL;
    }
    else
    {
        void *remove_item;
        p=first;
        remove_item=p->item;
        *keyPtr=first->key;
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
        delete p;
        return remove_item;
    }
}




void DLList::SortedInsert(void *item, int sortKey)
{
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
        //currentThread->Yield();
        new_item->prev=NULL;
        //currentThread->Yield();
        new_item->next=NULL;
        //currentThread->Yield();
    }
    else
    {
    	//currentThread->Yield();
        //currentThread->Yield();
        DLLElement *temp=first;
        //currentThread->Yield();					//error destroy
        while (temp!=NULL&&sortKey>=temp->key) //在find之前插入
        {
        	//currentThread->Yield();				//error destroy
            temp=temp->next;
            //currentThread->Yield();				//error destroy
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
        	//currentThread->Yield();
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
        	//currentThread->Yield();			//error destroy
        	last->next=NULL;
        }
        else if(temp==first)	//在表头插入
        {
        	//currentThread->Yield();
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
        	//currentThread->Yield();      		//error destroy
        	first=new_item;
        	//currentThread->Yield();
        	//currentThread->Yield();
        	first->prev=NULL;
        }
        else //中间插入
        {
        	//currentThread->Yield();				//error destroy
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
            //currentThread->Yield();				//error destroy
            new_item->next=temp;
        /*--------------------------------------------error 5-------------------------------------------------*/
        if (err_type ==5)
        {
            printf("----Switch thread------\n");
            currentThread->Yield();					//error destroy
        }
        /*--------------------------------------------error 5-------------------------------------------------*/
            temp->prev=new_item;
            //currentThread->Yield();
        }
    }
}

void *DLList::SortedRemove(int sortKey)
{
    void *remove_item;
    DLLElement *tmp=first;
    if(IsEmpty())
    {
        printf("the list is empty!\n");
        return NULL;
    }

    while (tmp!=NULL&&sortKey!=tmp->key)
    {
           tmp=tmp->next;
    }
    if(tmp==NULL)
    {
        printf("there is no an item with sortkey%d!\n",sortKey);
        return NULL;
    }
    remove_item=tmp->item;
    if(tmp==first)
    {
        first=first->next;
        first->prev=NULL;
    }
    else if(tmp==last)
    {
        last=last->prev;
        last->next=NULL;
    }
    else
    {
        tmp->prev->next=tmp->next;
        tmp->next->prev=tmp->prev;
    }
    delete tmp;
    return remove_item;
}


void DLList::showlist()
{
    DLLElement *tmp;
    for (tmp=first;tmp!=NULL;tmp=tmp->next)
    {
        printf("%d ",tmp->key);
    }
    if(first!=NULL&&last!=NULL)
        printf("---first=%d,last=%d\n",first->key,last->key);
    else if(first==NULL&&last!=NULL)
        printf("---first=NULL,last=%d\n",last->key);
    else if(first!=NULL&&last==NULL)
        printf("---first=%d,last=NULL\n",first->key);
    else
    printf("NULL!\n");
}
