/**
 * @file task.h
 * @author guoqinglong@baidu.com
 * @brief  此函数用于指定一个任务类，用户需要继承此类创建任务
 */
#ifndef BASE_THTREDPOOL_TASK_H
#define BASE_THTREDPOOL_TASK_H

// namespace scss {

class Task {
public:
    Task();
    virtual ~Task();
    virtual void run() = 0;
    virtual void cancel() = 0;
};

// } /* namespace scss */ 

#endif /* BASE_THTREDPOOL_TASK_H */
