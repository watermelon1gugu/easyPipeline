# -*- coding: utf-8 -*-
"""
@Author archchen
@Time 2019/12/19 3:19 下午
@Description 一个简单的流水线处理类
"""
import queue
import types
from inspect import isfunction
from threading import Thread


class Pipeline(object):
    def __init__(self, func_items: list, logger=None):
        self._func_items = func_items
        self._step_num = len(func_items)  # 5
        self._queue_num = self._step_num  # 5
        self._queue_list = [(queue.Queue(func_item.queue_len) if func_item.queue_len != -1 else queue.Queue()) for
                            func_item in self._func_items]
        self._input_queue = self._queue_list[0]
        self._output_queue = self._queue_list[-1]
        self._logger = logger
        self.__start_working_thread()

    @staticmethod
    def __worker(func: types.FunctionType, in_queue: queue.Queue, out_queue: queue.Queue, log_func=None,
                 logger=None):
        try:
            while True:
                if isfunction(log_func):
                    log_func()
                context = in_queue.get(block=True)
                context = func(context)
                # TODO 异常处理
                if isinstance(context, ErrorContext):
                    return
                if out_queue is not None:
                    out_queue.put(context, block=True)
                else:
                    del context
        except Exception as e:
            import traceback
            if logger is not None:
                logger.error("__processing_worker error,exception:{}", traceback.format_exc())
            else:
                raise PipelineError(e)

    def __start_working_thread(self):
        worker_list = []
        for i in range(self._step_num):
            func_item = self._func_items[i]
            worker_num = func_item.worker_num
            for _ in range(worker_num):  # 0 1 2 3 4
                worker_list.append(Thread(target=self.__worker,
                                          args=(func_item.func, self._queue_list[i],
                                                self._queue_list[i + 1] if i < self._queue_num - 1 else None,
                                                self._logger)))

        for worker in worker_list:
            worker.start()
        # for worker in worker_list:
        #     worker.join()

    def put_reqest(self, context):
        self._input_queue.put(context)

    def get_result(self):
        return self._output_queue.get(True)

    def get_all_queue_size(self):
        all_queue_size = {}
        for q, func_item in zip(self._queue_list, self._func_items):
            all_queue_size[func_item.name] = q.qsize()
        return all_queue_size


class FuncItem(object):
    def __init__(self, func, worker_num, name: str, queue_len: int):
        self.func = func
        self.worker_num = worker_num
        self.name = name
        self.queue_len = queue_len


class Context(object):
    """
    @Description 用于pipeline各环节之间传参的context类，temp_param存放下一个环节会用到的参数，global_param存放全局参数，即在各个环节都可能用到的参数
    """

    def __init__(self):
        self.__global_param = {}  # global只单增

    def add_single_global_param(self, key, value):
        """
        添加一个global键值对，如有重复将覆盖
        :param key: 键
        :param value: 值
        :return:
        """
        self.__global_param[key] = value

    def add_dict_global_param(self, param: dict):
        """
        添加一组global键值对，如有重复将覆盖
        :param param:
        :return:
        """
        self.__global_param.update(param)

    def get_global_param(self):
        """
        获得global_param副本
        :return:
        """
        return dict(self.__global_param.items())

    def get_request(self):
        """
        返回原本的调用请求，子类重载
        :return:
        """
        pass


class ErrorContext(Context):
    def __init__(self, msg):
        super(ErrorContext, self).__init__()
        self.msg = msg


class PipelineError(Exception):
    def __init__(self, ErrorInfo):
        super().__init__(self)  # 初始化父类
        self.errorinfo = ErrorInfo

    def __str__(self):
        return self.errorinfo
