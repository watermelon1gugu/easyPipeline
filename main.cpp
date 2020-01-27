#include <iostream>
#include "Context.h"
#include "Pipeline.h"
#include <utility>

using namespace easyPipeline;

class TestContext : public Context {
public:
    TestContext(int num) {
        this->temp = num;
    }

    void print() {
        std::cout << this->result << std::endl;
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
    int result;
    int temp;
};

TestContext func1(TestContext &c) {
    int temp = c.getTemp();
    c.setResult(temp);
    return c;
}

TestContext func2(TestContext &c) {
    int temp = c.getTemp();
    c.setResult(c.getResult() + temp * 10);
    return c;
}

void endFilterFunc(TestContext &c) {
    c.print();
}

int main() {
    typedef std::function<TestContext(TestContext &)> NormalFunc;
    typedef std::function<void(TestContext &)> EndFilterFunc;
    typedef FuncItem<NormalFunc> NormalFuncItem;
    typedef FuncItem<EndFilterFunc> EndFilterFuncItem;

    std::vector<NormalFuncItem> funcitems{
            NormalFuncItem(NormalFunc(func1), 2),
            NormalFuncItem(NormalFunc(func2), 2)};
    EndFilterFuncItem endFilterFuncitem = EndFilterFuncItem(EndFilterFunc(endFilterFunc), 2);

    Pipeline<TestContext> p(funcitems, endFilterFuncitem);
    return 0;
}