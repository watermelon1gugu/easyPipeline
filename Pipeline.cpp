//
// Created by archchen on 2020/1/25.
//

#include "Pipeline.h"
#include <thread>
#include <utility>

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
              contextQueueList(std::vector<BlockingQueue<C &>>(queueNum, BlockingQueue<C &>())),
              funcItems(funcItems),
              inputQueue(this->contextQueueList.front()),
              endProductQueue(this->contextQueueList.back()),
              endFilterFuncItem(std::move(endFilterFuncItem)) {

        this->_start();
    }

    template<class C>
    void Pipeline<C>::_worker(const std::function<C(C &)> &func, BlockingQueue<C &> &inQueue,
                              BlockingQueue<C &> &outQueue) {
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
                                       BlockingQueue<C &> &endProductQueue) {
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