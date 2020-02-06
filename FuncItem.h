//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_FUNCITEM_H
#define SCURMVISION_BASE_FUNCITEM_H


#include "Context.h"

template<typename FUN_RESULT, typename ... FUN_ARGS>
class FuncItem {
public:
    explicit FuncItem(std::function<FUN_RESULT(FUN_ARGS ... args)> func, int workerNum) : func(func), workerNum(workerNum) {
    };

    std::function<FUN_RESULT(FUN_ARGS ... args)> getFunc() {
        return this->func;
    };

    int getWorkerNum() {
        return this->workerNum;
    };

private:
    int workerNum;
    std::function<FUN_RESULT(FUN_ARGS ... args)> func;
};


#endif //SCURMVISION_BASE_FUNCITEM_H
