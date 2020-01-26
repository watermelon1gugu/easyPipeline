//
// Created by archchen on 2020/1/25.
//

#include "FuncItem.h"

FuncItem::FuncItem(std::function<Context (Context)> func, int wokerNum) {
    this->func = func;
    this->wokerNum = wokerNum;
}
