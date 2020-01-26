//
// Created by archchen on 2020/1/25.
//

#include "Pipeline.h"
#include <thread>
#include <utility>

namespace easyPipeline {

    int Pipeline::putTask(Context &context) {
        this->inputQueue.push(&context);
        return 0;
    }

    Pipeline::Pipeline(std::list<NormalWorkerFuncItem> &funcItems,
                       EndFileterFuncItem endFilterFuncItem = EndFileterFuncItem(_normalEndFilter, 2))
            : stepNum(funcItems.size()),
              queueNum(stepNum + 1),
              contextQueueList(std::list<BlockingQueue<Context &>>(queueNum, BlockingQueue<Context &>())),
              funcItems(funcItems),
              inputQueue(this->contextQueueList.front()),
              endProductQueue(this->contextQueueList.back()),
              endFilterFuncItem(std::move(endFilterFuncItem)) {
        std::function<Context(Context &)> func;
        this->_start();
    }


    void Pipeline::_worker(const std::function<Context(Context &)> &func, BlockingQueue<Context &> &inQueue,
                           BlockingQueue<Context &> &outQueue) {
        try {
            while (true) {
                Context context = inQueue.get();
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

    void Pipeline::_start() {
        std::list<std::thread> workerList = std::list<std::thread>();
        for (auto &funcItem:funcItems) {
            for (int i = 0; i < funcItem.getWorkerNum(); i++) {
                //可在此次对传入函数进行修饰
                workerList.emplace_back(std::thread(funcItem.getFunc()));
            }
        }
        for(int i =0;i<this->endFilterFuncItem.getWorkerNum();i++){
            workerList.emplace_back(std::thread(this->endFilterFuncItem.getFunc()));
        }
        for (auto &worker : workerList) {
            worker.detach();
        }
    }

    void Pipeline::endFilterWrap() {
        try {
            while (true) {
                Context context = this->endProductQueue.get();
                this->endFilterFuncItem.getFunc()(context);
            }
        } catch (...) {

        }
    }

    std::function<Context(Context &)> Pipeline::_normalEndFilter(Context &context) {
        context.free();
    }


}