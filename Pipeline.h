//
// Created by archchen on 2020/1/25.
//
// ## 初步构想
//pipeline对所有流程函数进行代理封装
//流程封装：在开头声明接受context （尝试使用依赖注入方式传递参数）
//初始化封装：初始化时希望可以实现采用声明式编程的方式实现。实现结果filter对结果进行异步回调
//pipline.step1().step2().step3().resultFilter().start()

#ifndef SCURMVISION_BASE_PIPELINE_H
#define SCURMVISION_BASE_PIPELINE_H

#include "FuncItem.h"
#include "Context.h"
#include "BlockingQueue.h"
#include <typeinfo>
#include <list>
#include <queue>
#include <thread>

namespace easyPipeline {
    template<class C>
    class Pipeline {
    public:
        explicit Pipeline(std::vector<FuncItem<std::function<C(C &)>>> &funcItems,
                          FuncItem<std::function<void(C &)>> endFilterFuncItem);

        int putTask(C &context);

    private:

        static std::function<C(C &)> _normalEndFilter(C &context);

        static void _worker(const std::function<C(C &)> &func, BlockingQueue<C &> &inQueue,
                            BlockingQueue<C &> &outQueue);

        static void
        _endFilterWorker(const std::function<C(C &)> &func, BlockingQueue<C &> &endProductQueue);

        void _start();


        unsigned long stepNum;
        unsigned long queueNum;
        FuncItem<std::function<void(C &)>> endFilterFuncItem;        //默认调用context的析构函数以及清理所有垃圾
        std::vector<FuncItem<std::function<C(C &)>>> &funcItems;
        std::vector<BlockingQueue<C &>> contextQueueList;
        BlockingQueue<C &> &inputQueue;
        BlockingQueue<C &> &endProductQueue;
    };

//class PipelineWarp{
//public:
//    PipelineWarp firstStep();
//
//private:
//};
}
#endif //SCURMVISION_BASE_PIPELINE_H
