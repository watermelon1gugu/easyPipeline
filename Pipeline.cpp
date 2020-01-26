//
// Created by archchen on 2020/1/25.
//

#include "Pipeline.h"
#include <thread>

namespace easyPipeline {

    int Pipeline::putTask(Context &context) {
        this->inputQueue.push(&context);
        return 0;
    }

    Pipeline::Pipeline(std::list<FuncItem> &funcItems)
            : stepNum(funcItems.size()),
              queueNum(stepNum + 1),
              contextQueueList(std::list<BlockingQueue<Context &>>(queueNum, BlockingQueue<Context &>())),
              funcItems(funcItems),
              inputQueue(this->contextQueueList.front()),
              endProductQueue(this->contextQueueList.back()) {
        this->_start();
    }

    void Pipeline::endFilter(Context &context) {
        //默认调用context的析构函数以及清理所有垃圾
    }


    void Pipeline::_worker(const std::function<Context(Context)> &func, BlockingQueue<Context> &inQueue,
                           BlockingQueue<Context> &outQueue) {
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
            for (int i = 0; i < funcItem.getWokerNum(); i++) {
                //可在此次对传入函数进行修饰
                workerList.emplace_back(std::thread(funcItem.getFunc()));
            }
        }

        for (auto &worker : workerList) {
            worker.detach();
        }
    }

    void Pipeline::endFilterWrapper(Context &context) {
        try {
            while (true) {
                Context context = this->endProductQueue.get();
                this->endFilter(context);
            }
        } catch (...) {

        }
    }
}