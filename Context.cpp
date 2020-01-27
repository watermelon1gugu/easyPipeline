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

