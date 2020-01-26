//
// Created by archchen on 2020/1/25.
//

#include "FuncItem.h"

#include <utility>

FuncItem::FuncItem(std::function<Context(Context)> func, int wokerNum) {
    this->func = std::move(func);
    this->wokerNum = wokerNum;
}

int FuncItem::getWokerNum() {
    return this->wokerNum;
}

std::function<Context(Context)> FuncItem::getFunc() {
    return this->func;
}
