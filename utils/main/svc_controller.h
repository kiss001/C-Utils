#pragma once

#include "singleton.h"
#include "rtn_code.h"

/**
 * @brief 服务控制的接口，需要继承这个抽象类，然后实现相关的功能。相关实现类需要在svc_factory中进行指定。
 * 
 */
class SvcController {
public:
    virtual ~SvcController() {}

    virtual RtnCode init() = 0;
    virtual RtnCode start() = 0;
    virtual void    stop() = 0;
    virtual void    uinit() = 0;

private:


};
