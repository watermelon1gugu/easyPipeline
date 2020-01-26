//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_FUNCITEM_H
#define SCURMVISION_BASE_FUNCITEM_H


#include "Context.h"

class FuncItem {
public:
    explicit FuncItem(int workerNum);

    int getWorkerNum();

private:
    int workerNum;
};

class NormalWorkerFuncItem : public FuncItem {
public:
    NormalWorkerFuncItem(std::function<Context(Context &)> func, int workerNum);

    std::function<Context(Context &)> getFunc();

private:
    std::function<Context(Context &)> func;
};

class EndFileterFuncItem : public FuncItem {
public:
    EndFileterFuncItem(std::function<void(Context &)> func, int workerNum);

    std::function<void(Context &)> getFunc();

private:
    std::function<void(Context &)> func;

};

#endif //SCURMVISION_BASE_FUNCITEM_H
