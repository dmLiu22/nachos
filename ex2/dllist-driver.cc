#include<stdlib.h>
#include<time.h>
#include "dllist.h"
#include "system.h"
#include "copyright.h"

void  GenerateN(int t,int n, DLList *list) {
    int i,key;
    srand(time(NULL));
    for (i = 0; i < n; i++)
    {
        if(t==1)
            key=rand()%100;
        else
            key=rand()%50+Random()%50;
        printf("Thread %d : insert key =%d\n",t,key);
        list->SortedInsert(NULL,key);
        printf("Thread %d : inserted key =%d After:",t,key);
        list->Show();
    }
}

void RemoveN(int t, int n, DLList* dllist)
{
    int i, key;
    int* item_ptr;
    for (i = 0; i < n; i++)
    {
        item_ptr = (int*)dllist->Remove();
        key = *item_ptr;
        printf("Thread %d: remove success, key = %d, List: ", t, key);
        dllist->Show();
    }
}
