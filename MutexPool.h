#ifndef   MUTEXPOOL_H
#define   MUTEXPOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string>
#include <map>
#include <algorithm>

#define MAX_MUTEX_COUNT 100

using namespace std;

class MutexPool
{
public:
    typedef map<string, pthread_mutex_t*> MUTEX_MAP;

    /* 使用单例模式，返回互斥锁池静态局部对象指针，保证系统内唯一
     * 定义构造、析构、拷贝构造、赋值为私有 */
    static MutexPool* GetInstance()
    {
        static MutexPool m_pInstance(MAX_MUTEX_COUNT);
        return &m_pInstance;
    }

    inline int Lock(const string& lock_name);  
    /* 获取名称为lock的锁，如果已经被锁，等待时间second秒，然后返回 */
    inline int Lock(const string& lock_name, const unsigned int second); 
    inline int UnLock(const string& lock_name);
    int Remove(const string& lock_name);
    int Add(const string& lock_name);
    bool Exists(const string& lock_name) const;
    /* 设置最大数量，返回原有最大数量，如果mutex_size为零，也返回原有数量
     * 如果当前数量大于utex_size，返回原有数量并保持不变 */ 
    unsigned int set_mutex_size(unsigned int mutex_size);

private:
    explicit MutexPool(unsigned int max_mutex = 0)
                       :_max_mutex(max_mutex), 
                        _valid(0),
                        _mutex_pool(MUTEX_MAP()) {
        if (pthread_mutex_init(&_mutex, NULL) == 0)
          _valid = 1; 
    }

    ~MutexPool(){
        if (_valid == 1){
            pthread_mutex_lock(&_mutex);
            _valid = 0;
            for (MUTEX_MAP::iterator it = _mutex_pool.begin(); 
                 it != _mutex_pool.end();  
                 ++it){
                pthread_mutex_destroy(it->second);
                delete it->second; 
            }
            pthread_mutex_unlock(&_mutex);
            pthread_mutex_destroy(&_mutex);
        }
    }
    
    //禁止拷贝和赋值
    MutexPool(MutexPool& rh){}
    const MutexPool& operator=(const MutexPool& rh) { return *this;}
    pthread_mutex_t* _Add(const string& lock_name); 
    pthread_mutex_t* _Get(const string& lock_name);

    unsigned int    _max_mutex; 
    unsigned int    _valid; //线程池是否有效， 0-无效 1-有效
    pthread_mutex_t _mutex;
    MUTEX_MAP       _mutex_pool; 
};

#endif
