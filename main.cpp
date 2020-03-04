#include <iostream>
#include "Context.h"
#include "Pipeline.h"
#include <utility>
#include <zconf.h>
#include "StreamPipeline.h"

using namespace easyPipeline;
using namespace std;

class TestContext : public Context {
public:
    TestContext(int num) {
        this->temp = num;
    }

    void print() {
        std::cout << "result:" << this->getTemp() << " " << this->result << std::endl;
    }

    int getTemp() const {
        return temp;
    }

    void setTemp(int temp) {
        TestContext::temp = temp;
    }

    int getResult() const {
        return result;
    }

    void setResult(int result) {
        TestContext::result = result;
    }

private:
    int result = 0;
    int temp;
};

TestContext *func1(TestContext *c) {
    cout << "func1:" << c->getTemp() << endl;
    c->setResult(c->getResult() + 1);
    return c;
}

TestContext *func2(TestContext *c) {
    cout << "func2:" << c->getTemp() << endl;
    c->setResult(c->getResult() * 10 + 1);
    return c;
}

void endFilterFunc(TestContext *c) {
    c->print();
    delete c;
    //在此自定义内存回收
}

int main() {
    StreamPipeline<TestContext> sp;
    sp.doAndTransform(func1, 2)
            .doAndTransform(func2, 2)
            .doAndTransform(func2, 2)
            .endPipeLine(endFilterFunc, 2)
            .start();
    for (int i = 0; i < 1000; i++) {
        TestContext *context = new TestContext(i);
        sp.putTask(context);
    }
    usleep(100000000);
}
//int main() {
//    typedef FuncItem<TestContext *, TestContext *> NormalFuncItem;
//    typedef FuncItem<void, TestContext *> EndFilterFuncItem;
//
//    vector<FuncItem<TestContext *, TestContext *>> funcitems{
//            NormalFuncItem(func1, 2),
//            NormalFuncItem(func2, 2),
//            NormalFuncItem(func2, 2),
//            NormalFuncItem(func2, 2),
//    };
//    EndFilterFuncItem endFilterFuncitem = EndFilterFuncItem(endFilterFunc, 2);
//
//    Pipeline<TestContext> p(funcitems, endFilterFuncitem);
//
//    for (int i = 0; i < 1000; i++) {
//        TestContext *context = new TestContext(i);
//        p.putTask(context);
//    }
//    usleep(100000000);
//
//    return 0;
//}