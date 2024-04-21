#include "synch.h"
#include "dllist.h"
#include "BoundedBuffer.h"
#include "stdio.h"
#include "list.h"
# include "system.h"
using namespace std;
char name1[5] = "lock",name2[6]="empty",name3[5]="full";
BoundedBuffer::BoundedBuffer(int sizebuffer)
{
	sizeofbuffer = sizebuffer;
	wptr = rptr = 0;
	usedSize=0;
	lockofbuffer = new Semaphore(name1,1);
	empty = new Semaphore(name2,sizeofbuffer);
	full = new Semaphore(name3,0);
	buffer = new char[sizeofbuffer];
}

void BoundedBuffer::Read(void* data, int size)
{
	int i;
	if (usedSize<size)
	{
		printf("没有可供读出的内容\n");
	}
			lockofbuffer->P();
	for (i = 0; i < size; i++)
	{
		full->P();

		*((char*)data + i) = *((char*)buffer + (rptr + i) % sizeofbuffer);
		empty->V();


	}
	rptr=(rptr+size)%sizeofbuffer;
	usedSize -= size;
			lockofbuffer->V();
}

void BoundedBuffer::Write(void* data, int size)
{
	int i;
		lockofbuffer->P();
	if (sizeofbuffer- usedSize < size)
	{
		printf("没有可供写入的空间\n");
	}
	for (i = 0; i < size; i++)
	{
		empty->P();

		*((char*)buffer + (wptr + i) % sizeofbuffer) = *((char*)data + i);
		full->V();

	}
	wptr=(wptr+size)%sizeofbuffer;
		usedSize += size;
			lockofbuffer->V();
}



void BoundedBuffer::Showbuffer()
{
if(usedSize==sizeofbuffer)
{
printf("缓冲区已满\n");
for(int i = 0; i< sizeofbuffer;i++)
		printf("%c", buffer[i]);
	printf("\n");
}
else{
printf("缓冲区的内容是：");
for(int i = rptr; i != wptr; i = (i + 1) % sizeofbuffer) {
printf("%c", buffer[i]);
}
printf("\n");
}
}
