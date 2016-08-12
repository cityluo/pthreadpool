#include <iostream>
#include "MutexPool.h"

int main(int argc, char* argv[])
{
    MutexPool *pmu = MutexPool::GetInstance();

    pmu->Lock("weqrew");
    pmu->UnLock("weqrew"); 
    pmu->Lock("sdafsda", 10);
    pmu->UnLock("sdafsda");
    pmu->Add("1");
    pmu->Lock("1");
    pmu->UnLock("1");
    pmu->Remove("1");
    pmu->Remove("sdafsda");
    pmu->set_mutex_size(12);
    
    return 0;
}
