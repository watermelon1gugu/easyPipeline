//
// Created by archchen on 2020/2/1.
//

#ifndef EASYPIPELINE_STREAMPIPELINE_H
#define EASYPIPELINE_STREAMPIPELINE_H

#include "Context.h"
#include <typeinfo>
#include <list>
#include <queue>
#include <thread>
#include <utility>
#include <functional>
#include "BlockingCollection.h"
/*
 * TODO 编写doAndTransform方法和endPipeLine方法传入参数，接收参数为func和并发数
 * TODO 将FuncItem作为内部类使用 不对外显示
 * TODO 构造函数不再传入FuncItem
 */
namespace easyPipeline {
    template<class C>
    class StreamPipeline {
    private:
        template<typename FUN_RESULT, typename ... FUN_ARGS>
        class FuncItem {
        public:
            explicit FuncItem(std::function<FUN_RESULT(FUN_ARGS ... args)> func, int workerNum);

            std::function<FUN_RESULT(FUN_ARGS ... args)> getFunc() {
                return this->func;
            };

            int getWorkerNum() {
                return this->workerNum;
            };

        private:
            int workerNum;
            std::function<FUN_RESULT(FUN_ARGS ... args)> func;
        };

    public:
        StreamPipeline();

        int putTask(C *context);

        StreamPipeline& doAndTransform(const std::function<C *(C *)> &func, int wokerNum) {
            this->funcItems.push_back(FuncItem<C *, C *>(func, wokerNum));
            return *this;
        }

        StreamPipeline& endPipeLine(const std::function<void(C *)> &func, int wokerNum) {
            this->endFilterFuncItem = FuncItem<void, C *>(func, wokerNum);
            return *this;
        }


        void start();

        ~StreamPipeline();

    private:

        static std::function<C *(C *)> _normalEndFilter(C *context);

        static void _worker(const std::function<C *(C *)> &func, BlockingQueue<C *> *inQueue,
                            BlockingQueue<C *> *outQueue);

        static void
        _endFilterWorker(const std::function<void(C *)> &func, BlockingQueue<C *> *endProductQueue);


        std::vector<BlockingQueue < C * >>*
        contextQueueList;
        FuncItem<void, C *> endFilterFuncItem = FuncItem<void, C *>(_normalEndFilter,
                                                                    2);        //默认调用context的析构函数以及清理所有垃圾
        std::vector<FuncItem<C *, C *>> funcItems = std::vector<FuncItem<C *, C *>>();
        BlockingQueue<C *> *inputQueue;
        BlockingQueue<C *> *endProductQueue;

    };

    template<class C>
    template<typename FUN_RESULT, typename... FUN_ARGS>
    StreamPipeline<C>::FuncItem<FUN_RESULT, FUN_ARGS...>::FuncItem(std::function<FUN_RESULT(FUN_ARGS ... args)> func,
                                                                   int workerNum) : func(func),
                                                                                    workerNum(workerNum) {
    };
}


namespace easyPipeline {
    template<class C>
    int StreamPipeline<C>::putTask(C *context) {
        this->inputQueue->add(context);
        return 0;
    }

    template<class C>
    StreamPipeline<C>::StreamPipeline() {};

    template<class C>
    StreamPipeline<C>::~StreamPipeline() {
        delete this->contextQueueList;
    }

    template<class C>
    void StreamPipeline<C>::_worker(const std::function<C *(C *)> &func, BlockingQueue<C *> *inQueue,
                                    BlockingQueue<C *> *outQueue) {
        try {
            while (true) {
                C *context;
                inQueue->take(context);
                context = func(context);
                if (!context->isContinue()) {
                    continue;
                } else {
                    outQueue->add(context);
                }
            }
        } catch (...) {

        }

    }

    // TODO 在start中初始化任务queue
    template<class C>
    void StreamPipeline<C>::start() {
        this->contextQueueList = new std::vector<BlockingQueue<C * >>(funcItems.size() + 1);
        this->inputQueue = &(*this->contextQueueList)[0];
        this->endProductQueue = &(*this->contextQueueList)[this->contextQueueList->size() - 1];
        std::list<std::thread> workerList = std::list<std::thread>();
        for (int i = 0; i < funcItems.size(); i++) {
            for (int k = 0; k < funcItems[i].getWorkerNum(); k++) {
                //可在此次对传入函数进行修饰
                workerList.push_back(std::thread(_worker, funcItems[i].getFunc(), &(*this->contextQueueList)[i],
                                                 &(*this->contextQueueList)[i + 1]));
            }
        }
        std::list<std::thread> endFilterWorkerList = std::list<std::thread>();
        for (int i = 0; i < this->endFilterFuncItem.getWorkerNum(); i++) {
            workerList.push_back(std::thread(
                    _endFilterWorker, this->endFilterFuncItem.getFunc(), this->endProductQueue));
        }
        for (auto &worker : workerList) {
            worker.detach();
        }
    }

    template<class C>
    void StreamPipeline<C>::_endFilterWorker(const std::function<void(C *)> &func,
                                             BlockingQueue<C *> *endProductQueue) {
        try {
            while (true) {
                C *context;
                endProductQueue->take(context);
                func(context);
            }
        } catch (...) {

        }
    }

    template<class C>
    std::function<C *(C *)> StreamPipeline<C>::_normalEndFilter(C *context) {
        //TODO 写个默认filter
        delete context;
    }
}
#endif //EASYPIPELINE_STREAMPIPELINE_H
