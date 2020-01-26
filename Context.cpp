//
// Created by archchen on 2020/1/25.
//

#include "Context.h"

bool Context::isContinue() {
    return this->_isContinue;
}

void Context::breakOff() {
    this->_isContinue = false;
}

std::unordered_map<std::string, void *> *Context::getGlobalParam() {
    return &(this->_globalParam);
}
