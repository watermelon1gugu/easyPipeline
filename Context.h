//
// Created by archchen on 2020/1/25.
//

#ifndef SCURMVISION_BASE_CONTEXT_H
#define SCURMVISION_BASE_CONTEXT_H

#include <unordered_map>

class Context {
    /*
     * @Description Context基类 在流水线各个阶段之间传递。
     *              在特定的生产线上需要继承Context并自定义属性添加直接所需要的参数以满足流水线各阶段需要
     */
public:
    Context()= default;
    bool isContinue();//判断该context是否继续传递
    void breakOff();//终止该context

private:
    bool _isContinue = true;
};


#endif //SCURMVISION_BASE_CONTEXT_H
