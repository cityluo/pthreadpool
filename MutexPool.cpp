#include "MutexPool.h"

pthread_mutex_t* MutexPool::_Add(const string& lock_name)
{
    pthread_mutex_t* pMutex = new pthread_mutex_t();
    if (pMutex == NULL)
        return pMutex;
    pthread_mutex_init(pMutex, NULL);
    _mutex_pool.insert(MUTEX_MAP::value_type(lock_name, pMutex));
    return pMutex;
}

pthread_mutex_t* MutexPool::_Get(const string& lock_name)
{
    if (_valid != 1)
        return (pthread_mutex_t*)0;

    pthread_mutex_t* pMutex = NULL;
    pthread_mutex_lock(&_mutex);
    
    if (_mutex_pool.size() >= _max_mutex){
        pthread_mutex_unlock(&_mutex);
        return (pthread_mutex_t*)0;
    }

    if (!Exists(lock_name)){ //不存在，创建
        pMutex = _Add(lock_name);
        pthread_mutex_unlock(&_mutex);
    } else{
        pMutex = _mutex_pool[lock_name];
        pthread_mutex_unlock(&_mutex);
    }
    return pMutex;
}


inline int MutexPool::Lock(const string& lock_name)
{
    return pthread_mutex_lock(_mutex_pool[lock_name]);
}

inline int MutexPool::Lock(const string& lock_name, const unsigned int second)
{
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME,&tp);
    tp.tv_sec += second;
    return pthread_mutex_timedlock(_mutex_pool[lock_name], &tp);
        
}

inline int MutexPool::UnLock(const string& lock_name)
{
    return pthread_mutex_unlock(_mutex_pool[lock_name]);
} 

int MutexPool::Remove(const string& lock_name)
{
    if (_valid != 1)
        return -1;
    pthread_mutex_lock(&_mutex);
    
    if (Exists(lock_name)){ //不存在，直接返回
        pthread_mutex_t* pMutex = NULL;
        pMutex = _mutex_pool[lock_name]; 
        _mutex_pool.erase(lock_name);
        pthread_mutex_unlock(&_mutex);
        pthread_mutex_destroy(pMutex);
    } else{
        pthread_mutex_unlock(&_mutex);
    }
    return 0;
}

int MutexPool::Add(const string& lock_name)
{
    if (_valid != 1)
        return -1;
    pthread_mutex_lock(&_mutex);

    if (_mutex_pool.size() >= _max_mutex){
        pthread_mutex_unlock(&_mutex);
        return -1;
    }

    if (!Exists(lock_name)){ //已经存在，直接返回
        if (!_Add(lock_name)){
            pthread_mutex_unlock(&_mutex);
            return -1;
        }
    }
    pthread_mutex_unlock(&_mutex);
    return 0;
}

bool MutexPool::Exists(const string& lock_name) const
{
   return _mutex_pool.end() == _mutex_pool.find(lock_name) ? false : true; 
}

unsigned int MutexPool::set_mutex_size(unsigned int mutex_size)
{
    if (mutex_size == 0 || mutex_size <= _mutex_pool.size())
        return _max_mutex;
    unsigned int tmp = _max_mutex;
    _max_mutex = mutex_size;
    return tmp; 
}
