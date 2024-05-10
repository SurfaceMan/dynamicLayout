#include "testwindow1.h"
#include "windowfactory.h"

class RegisterWindow {
public:
    RegisterWindow() {
        WindowFactory::Instance()->registerWindow("test1", []() { return new TestWindow1(); });
        WindowFactory::Instance()->registerWindow("test2", []() { return new TestWindow2(); });
    }
};

static RegisterWindow tmp;