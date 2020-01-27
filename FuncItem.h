//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_FUNCITEM_H
#define SCURMVISION_BASE_FUNCITEM_H


#include "Context.h"

template<class F>
class FuncItem {
public:
    explicit FuncItem(F func,int workerNum):func(func),workerNum(workerNum){
    };
    F getFunc(){
        return this->func;
    };

    int getWorkerNum(){
        return this->workerNum;
    };

private:
    int workerNum;
    F func;
};


#endif //SCURMVISION_BASE_FUNCITEM_H
