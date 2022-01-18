# workbench

主线程创建若干个工作线程，每个工作线程在用户要求时间或默认的时间内对目标web循环发出实际访问请求，主线程通过`atomic_bool`变量来控制工作线程的运行时间。工作线程将结果写入到全局`atomic_int`变量中，主线程在所有工作线程退出后统计并给用户显示最后的测试结果，然后退出。

程序参数参考[WebBench](https://www.jianshu.com/p/dc1032b19c8d)的结构。

## 第三方库

1. [cmdline](https://github.com/tanakh/cmdline)

2. bare Socket
