# include "Table.h"
# include "system.h"
Table::Table(int size)
{
	this->size = size;
    elem = new void *[this->size + 1]();
	lock1 = new Lock("TableLock1");
	tableEmpty = new Condition("tableEmpty condition");
}

Table::~Table()
{
	delete elem;
    delete lock1;

}

int Table::Alloc(void* object)
{
    int index = -1,ret;
    if(object == NULL)
        return index;
	lock1->Acquire();
	while(elem[index] != NULL && index < size) {
    index++;
    }
    if(index >= size)
    {
    ret = -1;
    }
    else
    {
    currentThread->Yield();
    elem[index] = object;
    ret = index;
    }
    //tableEmpty->Signal(lock1);
    lock1->Release();
    return ret;
}

void* Table::Get(int index)
{
	ASSERT(index >= 0 && index < size);
	return elem[index];
}


void Table::Release(int index)
{
    lock1->Acquire();
    while(elem[index] == NULL) tableEmpty->Wait(lock1);
    currentThread->Yield();
    elem[index] = NULL;
    lock1->Release();
}


void Table::Print() {
for(int i = 0; i < size; i++) {
if(elem[i] == NULL) printf("table[%d] : null\n", i);
else printf("table[%d] : %d\n", i, (int)elem[i]);
}
}