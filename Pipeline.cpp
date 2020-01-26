//
// Created by archchen on 2020/1/25.
//

#include "Pipeline.h"



int Pipeline::putTask(Context context) {
    return 0;
}

Pipeline::Pipeline(std::list<FuncItem> funcItems) {
    int steps = funcItems.size();
    contextQueueList = std::list<std::queue<Context>>
    this->contextQueueList =
}

void Pipeline::endFilter(Context context) {

}


void Pipeline::_worker(std::function<Context (Context)> func, BlockingQueue<Context> &&inQueue, BlockingQueue<Context> &&outQueue) {
    try {
        while(true){
            Context context = inQueue.get();
            context = func(context);
            if(!context.isContinue()){
                continue;
            } else{
                outQueue.put(context);
            }
        }
    }catch(...) {

    }

}

void Pipeline::_start() {

}
