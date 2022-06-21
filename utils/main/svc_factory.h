#pragma once

#include "singleton.h"
#include "svc_controller.h"

class SvcFactory : public Singleton<SvcFactory> {
public: 
    SvcController* create();

private:
    friend class Singleton<SvcFactory>;
    SvcFactory();
    ~SvcFactory();

    SvcController* _instance;
};