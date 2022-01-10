# workbench

父进程fork若干个子进程，每个子进程在用户要求时间或默认的时间内对目标web循环发出实际访问请求，父子进程通过管道进行通信，子进程通过管道写端向父进程传递在若干次请求访问完毕后记录到的总信息，父进程通过管道读端读取子进程发来的相关信息，子进程在时间到后结束，父进程在所有子进程退出后统计并给用户显示最后的测试结果，然后退出。

> https://www.jianshu.com/p/dc1032b19c8d

## framework 测试

https://github.com/klen/py-frameworks-bench

https://github.com/nuxeo/FunkLoad

https://segmentfault.com/a/1190000038340980

## Python socket

https://kimmosaaskilahti.fi/blog/2021-01-03-asyncio-workers/

## WRK

https://github.com/wg/wrk/blob/master/src/wrk.c