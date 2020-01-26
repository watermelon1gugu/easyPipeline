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

namespace easyPipeline {
    class Pipeline {
    public:
        explicit Pipeline(std::list<NormalWorkerFuncItem> &funcItems, EndFileterFuncItem  endFilterFuncItem);
        int putTask(Context &context);

    private:

        static std::function<Context(Context &)> _normalEndFilter(Context& context);
        static void _worker(const std::function<Context(Context&)> &func, BlockingQueue<Context&> &inQueue,
                            BlockingQueue<Context&> &outQueue);

        void endFilterWrap();

        void _start();


        unsigned long stepNum;
        unsigned long queueNum;
        EndFileterFuncItem endFilterFuncItem;        //默认调用context的析构函数以及清理所有垃圾
        std::list<NormalWorkerFuncItem> &funcItems;
        std::list<BlockingQueue<Context &>> contextQueueList;
        BlockingQueue<Context &> &inputQueue;
        BlockingQueue<Context &> &endProductQueue;
    };

//class PipelineWarp{
//public:
//    PipelineWarp firstStep();
//
//private:
//};
}
#endif //SCURMVISION_BASE_PIPELINE_H
