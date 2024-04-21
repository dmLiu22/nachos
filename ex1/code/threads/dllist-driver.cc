#include<stdlib.h>
#include<time.h>
#include "dllist.h"
#include "system.h"
#include "copyright.h"

void Insert(int t,int n, DLList *dllist)
{
    int i,key;
    srand(time(NULL));
    for (i = 0; i < n; i++)
    {
        if(t==1)
            key=rand()%100;
        else
            key=rand()%50+Random()%50;  
        printf("Thread %d : insert key =%d\n",t,key);
        dllist->SortedInsert(NULL,key);
        printf("Thread %d : inserted key =%d After:",t,key);
        dllist->showlist();
    }
}

void Remove(int t,int n, DLList *dllist)
{
    int i,key;
    for (i = 0; i < n; i++)
    {
        
        dllist->Remove(&key);
        printf("Thread %d : removed key=%d After:",t,key);
        dllist->showlist();
        //currentThread->Yield();
    }
}