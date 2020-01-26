## 初步构想
pipeline对所有流程函数进行代理封装
流程封装：在开头声明接受context （尝试使用依赖注入方式传递参数）
初始化封装：初始化时希望可以实现采用声明式编程的方式实现。实现结果filter对结果进行异步回调
pipline.step1().step2().step3().resultFilter().start()
