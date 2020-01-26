//
// Created by archchen on 2020/1/25.
//

#include "FuncItem.h"

#include <utility>

NormalWorkerFuncItem::NormalWorkerFuncItem(std::function<Context(Context &)> func, int workerNum) : FuncItem(workerNum) {
    this->func = std::move(func);
}

std::function<Context(Context &)> NormalWorkerFuncItem::getFunc() {
    return this->func;
}


std::function<void(Context &)> EndFileterFuncItem::getFunc() {
    return this->func;
}

EndFileterFuncItem::EndFileterFuncItem(std::function<void(Context &)> func, int workerNum) : FuncItem(workerNum),func(std::move(func)) {

}

int FuncItem::getWorkerNum() {
    return this->workerNum;
}

FuncItem::FuncItem(int workerNum):workerNum(workerNum) {

}

