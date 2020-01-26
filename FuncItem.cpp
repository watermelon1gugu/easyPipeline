//
// Created by archchen on 2020/1/25.
//

#include "FuncItem.h"

FuncItem::FuncItem(Context *(*func)(Context *), int wokerNum) {
    this->func = func;
    this->wokerNum = wokerNum;
}
