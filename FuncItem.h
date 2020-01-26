//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_FUNCITEM_H
#define SCURMVISION_BASE_FUNCITEM_H


#include "Context.h"

class FuncItem {
public:
    FuncItem(Context* (*func)(Context*),int wokerNum);
private:
    Context* (*func)(Context*);
    int wokerNum;
};


#endif //SCURMVISION_BASE_FUNCITEM_H
