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
#include <utility>
#include <functional>

namespace easyPipeline {
    template<class C>
    class Pipeline {
    public:
        explicit Pipeline(std::vector<FuncItem<std::function<C(C &)>>> &funcItems,
                          FuncItem<std::function<void(C &)>> endFilterFuncItem);

        int putTask(C &context);

    private:

        static std::function<C(C &)> _normalEndFilter(C &context);

        static void _worker(const std::function<C(C &)> &func, BlockingQueue<C> &inQueue,
                            BlockingQueue<C> &outQueue);

        static void
        _endFilterWorker(const std::function<C(C &)> &func, BlockingQueue<C> &endProductQueue);

        void _start();


        unsigned long stepNum;
        unsigned long queueNum;
        FuncItem<std::function<void(C &)>> endFilterFuncItem;        //默认调用context的析构函数以及清理所有垃圾
        std::vector<FuncItem<std::function<C(C &)>>> &funcItems;
        std::vector<BlockingQueue<C>> contextQueueList;
        BlockingQueue<C> &inputQueue;
        BlockingQueue<C> &endProductQueue;
    };

//class PipelineWarp{
//public:
//    PipelineWarp firstStep();
//
//private:
//};
}


namespace easyPipeline {
    template<class C>
    int Pipeline<C>::putTask(C &context) {
        this->inputQueue.push(&context);
        return 0;
    }

    template<class C>
    Pipeline<C>::Pipeline(std::vector<FuncItem<std::function<C(C&)>>> &funcItems,
                          FuncItem<std::function<void(C&)>> endFilterFuncItem)
            : stepNum(funcItems.size()),
              queueNum(stepNum + 1),
              contextQueueList(std::vector<BlockingQueue<C>>(queueNum, BlockingQueue<C>())),
              funcItems(funcItems),
              inputQueue(this->contextQueueList.front()),
              endProductQueue(this->contextQueueList.back()),
              endFilterFuncItem(std::move(endFilterFuncItem)) {

        this->_start();
    }

    template<class C>
    void Pipeline<C>::_worker(const std::function<C(C &)> &func, BlockingQueue<C> &inQueue,
                              BlockingQueue<C> &outQueue) {
        try {
            while (true) {
                C context = inQueue.get();
                context = func(context);
                if (!context.isContinue()) {
                    continue;
                } else {
                    outQueue.put(context);
                }
            }
        } catch (...) {

        }

    }

    template<class C>
    void Pipeline<C>::_start() {
        std::list<std::thread> workerList = std::list<std::thread>();
        for (auto &funcItem:funcItems) {
            for (int i = 0; i < funcItem.getWorkerNum(); i++) {
                //可在此次对传入函数进行修饰
                workerList.emplace_back(std::thread(_worker, funcItem.getFunc(), this->contextQueueList[i],
                                                    this->contextQueueList[i + 1]));
            }
        }
        for (int i = 0; i < this->endFilterFuncItem.getWorkerNum(); i++) {
            workerList.emplace_back(std::thread(
                    std::thread(_endFilterWorker, this->endFilterFuncItem.getFunc(), this->endProductQueue)));
        }
        for (auto &worker : workerList) {
            worker.detach();
        }
    }

    template<class C>
    void Pipeline<C>::_endFilterWorker(const std::function<C(C &)> &func,
                                       BlockingQueue<C> &endProductQueue) {
        try {
            while (true) {
                C context = endProductQueue.get();
                func(context);
            }
        } catch (...) {

        }
    }

    template<class C>
    std::function<C(C &)> Pipeline<C>::_normalEndFilter(C &c) {
    }
}
#endif //SCURMVISION_BASE_PIPELINE_H
