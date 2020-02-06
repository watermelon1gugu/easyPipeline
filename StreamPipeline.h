//
// Created by archchen on 2020/2/1.
//

#ifndef EASYPIPELINE_STREAMPIPELINE_H
#define EASYPIPELINE_STREAMPIPELINE_H

#include "FuncItem.h"
#include "Context.h"
#include <typeinfo>
#include <list>
#include <queue>
#include <thread>
#include <utility>
#include <functional>
#include "BlockingCollection.h"

namespace easyPipeline {
    template<class C>
    class StreamPipeline {
    public:
        explicit StreamPipeline();

        void putTask(C *context);

        ~StreamPipeline();

    private:

        static std::function<C *(C *)> _normalEndFilter(C *context);

        static void _worker(const std::function<C *(C *)> &func, BlockingQueue<C *> *inQueue,
                            BlockingQueue<C *> *outQueue);

        static void
        _endFilterWorker(const std::function<void(C *)> &func, BlockingQueue<C *> *endProductQueue);

        void _start();


        unsigned long stepNum;
        unsigned long queueNum;
        std::vector<BlockingQueue<C *>> contextQueueList = std::vector<BlockingQueue<C *>>();
        FuncItem<std::function<void(C *)>> endFilterFuncItem = _normalEndFilter;        //默认调用context的析构函数以及清理所有垃圾
        std::vector<FuncItem<std::function<C *(C *)>>> &funcItems = std::vector<FuncItem<std::function<C *(C *)>>>();
        BlockingQueue<C *> &inputQueue;
        BlockingQueue<C *> &endProductQueue;

    };

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

    template<class C>
    void
    StreamPipeline<C>::_endFilterWorker(const std::function<void(C *)> &func, BlockingQueue<C *> *endProductQueue) {
        try {
            while (true) {
                C *context;
                endProductQueue->take(context);
                func(context);
                delete context;
            }
        } catch (...) {

        }
    }

    template<class C>
    StreamPipeline<C>::StreamPipeline() {

    }

    template<class C>
    void StreamPipeline<C>::putTask(C *context) {
        this->inputQueue.add(context);
    }

    template<class C>
    std::function<C *(C *)> StreamPipeline<C>::_normalEndFilter(C *context) {
        //TODO 写个默认filter
    }

    template<class C>
    StreamPipeline<C>::~StreamPipeline() {

    }
}
#endif //EASYPIPELINE_STREAMPIPELINE_H
