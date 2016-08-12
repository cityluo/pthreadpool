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

    /* ʹ�õ���ģʽ�����ػ������ؾ�̬�ֲ�����ָ�룬��֤ϵͳ��Ψһ
     * ���幹�졢�������������졢��ֵΪ˽�� */
    static MutexPool* GetInstance()
    {
        static MutexPool m_pInstance(MAX_MUTEX_COUNT);
        return &m_pInstance;
    }

    inline int Lock(const string& lock_name);  
    /* ��ȡ����Ϊlock����������Ѿ��������ȴ�ʱ��second�룬Ȼ�󷵻� */
    inline int Lock(const string& lock_name, const unsigned int second); 
    inline int UnLock(const string& lock_name);
    int Remove(const string& lock_name);
    int Add(const string& lock_name);
    bool Exists(const string& lock_name) const;
    /* �����������������ԭ��������������mutex_sizeΪ�㣬Ҳ����ԭ������
     * �����ǰ�������ڣmutex_size������ԭ�����������ֲ��� */ 
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
    
    //��ֹ�����͸�ֵ
    MutexPool(MutexPool& rh){}
    const MutexPool& operator=(const MutexPool& rh) { return *this;}
    pthread_mutex_t* _Add(const string& lock_name); 
    pthread_mutex_t* _Get(const string& lock_name);

    unsigned int    _max_mutex; 
    unsigned int    _valid; //�̳߳��Ƿ���Ч�� 0-��Ч 1-��Ч
    pthread_mutex_t _mutex;
    MUTEX_MAP       _mutex_pool; 
};

#endif
