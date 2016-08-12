#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <memory>

using namespace std;

enum TaskStatus {task_new, task_assigned, task_finished};

struct Task_Data
{
    Task_Data() {}
    virtual ~Task_Data() {}
}

class Task
{
public:
    typedef void *(*task_routine) (void *) ;

    Task(const string&          name,
         shared_ptr<Task_Data>& task_data,
         task_routine           mission,
         unsigned int           nice = 0,    
         unsigned int           timeout = 10000)
        :_name(name),
         _task_data(task_data),
         _mission(mission),
         _nice(nice),
         _time_out(time_out),
         _status(task_new) {} 
    
    virtual ~Task() { }

    const string& get_name() const
    {
        return _name;
    }

    const shared_ptr<Task_Data>& get_task_data() const
    {
        return _task_data;
    }

    const task_routine get_mission() const
    {
        return _mission;
    }

    unsigned int get_nice() const
    {
        return _nice;
    }

    unsigned int get_timeout() const
    {
        return _timeout;
    } 
 
    TaskStatus get_status() const
    {
         return _status;
    }
    
private:
    string                  _name;       //任务名称
    shared_ptr<Task_Data>   _task_data;
    task_routine            _mission;
    unsigned int            _nice;       //任务优先级
    unsigned int            _timeout;    //超时时间 
    TaskStatus              _status;
}

#endif
