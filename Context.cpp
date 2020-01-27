//
// Created by archchen on 2020/1/25.
//

#include <iostream>
#include "Context.h"

bool Context::isContinue() {
    return this->_isContinue;
}

void Context::breakOff() {
    this->_isContinue = false;
}

void Context::free() {
    //TODO 释放globalParam内存
    std::cout<<""<<std::endl;
}

void *Context::getParam(std::string key) {
    return this->_globalParam.
}
