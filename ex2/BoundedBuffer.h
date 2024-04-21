#include "synch.h"

class BoundedBuffer {
   public:
     BoundedBuffer(int maxsize);
     void Read(void *data, int size);
     void Write(void *data, int size);
     void Showbuffer();
   private:
       int sizeofbuffer;
       int wptr,rptr;
       int usedSize;
       Semaphore* lockofbuffer, *empty, *full;
       char *buffer;
};