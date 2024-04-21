//#include <cstdlib>
//#include <cstdio>
//#include<ctime>
#include "system.h"

#define START_KEY 100

class DLLElement {
public:
    DLLElement( void *itemPtr, int sortKey );   // initialize a list element
    DLLElement *next;
    DLLElement *prev;
    int key;
    void *item;
};

class DLList {
public:
    DLList();
    DLList(int type);
    ~DLList();

    void Prepend(void *item);
    void Append(void *item);
    void *Remove(int *keyPtr);


    bool IsEmpty();

    void SortedInsert(void *item, int sortKey);
    void *SortedRemove(int sortKey);
    void showlist();
private:
    DLLElement *first;
    DLLElement *last;
    int err_type;
};
void Insert(int t,int n,DLList *dllist);
void Remove(int t,int n,DLList *dllist);
