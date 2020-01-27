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

    Pipeline::Pipeline(std::vector<NormalWorkerFuncItem> &funcItems,
                       EndFileterFuncItem endFilterFuncItem = EndFileterFuncItem(_normalEndFilter, 2))
            : stepNum(funcItems.size()),
              queueNum(stepNum + 1),
              contextQueueList(std::vector<BlockingQueue<Context &>>(queueNum, BlockingQueue<Context &>())),
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
                workerList.emplace_back(std::thread(_worker,funcItem.getFunc(),this->contextQueueList[i],this->contextQueueList[i+1]));
            }
        }
        for(int i =0;i<this->endFilterFuncItem.getWorkerNum();i++){
            workerList.emplace_back(std::thread(std::thread(_endFilterWorker,this->endFilterFuncItem.getFunc(),this->endProductQueue)));
        }
        for (auto &worker : workerList) {
            worker.detach();
        }
    }

     void Pipeline::_endFilterWorker(const std::function<Context(Context &)> &func, BlockingQueue<Context &> &endProductQueue) {
        try {
            while (true) {
                Context context = endProductQueue.get();
                func(context);
            }
        } catch (...) {

        }
    }

    std::function<Context(Context &)> Pipeline::_normalEndFilter(Context &context) {
        context.free();
    }


}