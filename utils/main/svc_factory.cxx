#include "svc_factory.h"
#include "socrule_svc_controller.h"

SvcFactory::SvcFactory() {
    _instance = NULL;
}

SvcFactory::~SvcFactory() {
    if (NULL != _instance) {
        delete _instance;
        _instance = NULL;
    }
}

// 在本身的目录下继承实现自己的类，然后这里创建即可。这里不用涉及到多线程
SvcController* SvcFactory::create() {
    if (_instance == NULL) {
        _instance = new SocRuleSvcController();
    }

    return _instance;
}
