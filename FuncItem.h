//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_FUNCITEM_H
#define SCURMVISION_BASE_FUNCITEM_H


#include "Context.h"

template<class T>
class FuncItem {
public:
    explicit FuncItem(int workerNum);

    virtual T getFunc() = 0;

    int getWorkerNum();

private:
    int workerNum;
};

class NormalWorkerFuncItem : public FuncItem<std::function<Context(Context &)>> {
public:
    NormalWorkerFuncItem(std::function<Context(Context &)> func, int workerNum);

    std::function<Context(Context &)> getFunc() override;

private:
    std::function<Context(Context &)> func;
};

class EndFileterFuncItem : public FuncItem<std::function<void(Context &)>> {
public:
    EndFileterFuncItem(std::function<void(Context &)> func, int workerNum);

    std::function<void(Context &)> getFunc() override;

private:
    std::function<void(Context &)> func;

};

#endif //SCURMVISION_BASE_FUNCITEM_H
