//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_FUNCITEM_H
#define SCURMVISION_BASE_FUNCITEM_H


#include "Context.h"

class FuncItem {
public:
    FuncItem(std::function<Context (Context)> func,int wokerNum);
    int getWokerNum();
    std::function<Context (Context)> getFunc();
private:
    std::function<Context (Context)> func;
    int wokerNum;
};


#endif //SCURMVISION_BASE_FUNCITEM_H
