//
// Created by archchen on 2020/1/25.
//

#include "Pipeline.h"



int Pipeline::putTask(Context context) {
    return 0;
}

Pipeline::Pipeline(std::list<FuncItem> funcItems) {

}

void Pipeline::resultFilter(Context context) {

}

void Pipeline::_worker(Context(*func)(Context), BlockingQueue<Context> &&inQueue, BlockingQueue<Context> &&outQueue) {
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
