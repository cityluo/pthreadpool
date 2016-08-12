#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

enum ThreadStatus {Tidle, Tbusy};

class Thread
{
private:
    string& _name;
    pthread_t _id;

#endif
