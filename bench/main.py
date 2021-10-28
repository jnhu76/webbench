# -*- coding:utf-8 -*-

import time
import socket
import argparse

import asyncio
from urllib import request, error
from typing import Any, Dict, Optional, List
from concurrent.futures import ThreadPoolExecutor, as_completed


helper = """
Usage: bench <operation> <url>
  Options:
    -c, --connections <N>   Connections to keep open，每个线程处理数 N = 连接/线程
    -d, --duration    <T>   Duration of test
    -t, --thread      <N>   Number of request
    
    -H, --header      <H>   Add header to request
        --timeout     <T>   Socket/request timeout
        --latency           Print latency statistics
    -v, --version           Print version details
"""


parser = argparse.ArgumentParser(description="Test")
VERSION = "0.1.0"

CONNECTION_TIMEOUT = 5
CHUNK_SIZE = 1024
HTTP_VERSION = 1.0
CLRF = '\r\n\r\n'


class Bench:
    _timeout: int
    _connection: int
    _duration: int
    _thread: int
    _header: Dict
    _latency: bool
    _version: str
    _data: str
    _url: str
    _arr: List[Any]
    _executor: ThreadPoolExecutor

    def __init__(self,
        url: str,
        connection: int = 100,
        duration: int = 20,
        thread: int = 1,
        timeout: int = 300,
        latency: bool = False,
        header: Dict = {}
    ) -> None:
        # self._connection = connection / thread
        self._connection = 12
        self._duration = duration
        self._timeout = timeout
        self._latency = latency
        self._header = header if header else {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.54 Safari/537.36 Edg/95.0.1020.30'}
        self._data = ''
        self._url = url
        self._thread = thread
        self._arr = []
        self._executor = ThreadPoolExecutor(max_workers=self._thread)

    @property
    def data(self):
        return self._data

    @data.setter
    def data(self):
        if self._latency:
           data = "" 

    
    def connection(self) -> Optional[Dict]:
        req = request.Request(self._url, headers=self._header)
        start_time = time.process_time_ns()
        end_time = 0
        try:
            if self._timeout:
                res = request.urlopen(req, timeout=self._timeout, data=None)
            else:
                res = request.urlopen(req, data=None)
            end_time = time.process_time_ns()
        except (error.URLError, error.HTTPError, error.ContentTooShortError,)as e:
            print("something error: {}".format(e.args))
        else:
            result = {
                'status_code': res.status,
                'time': (end_time - start_time) if end_time else end_time
            }
            result.update(res.getheaders())
            return result


    # def job(self):
    #     loop = asyncio.new_event_loop()
    #     asyncio.set_event_loop(loop)
    #     tasks = [self.connection for i in range(self._connection)]
    #     loop.run_until_complete(asyncio.wait(tasks))
    #     loop.close()

    def run(self):
        with ThreadPoolExecutor(max_workers=self._thread) as t:
            obj_list = []
            for i in range(self._thread):
                obj = t.submit(self.connection)
                obj_list.append(obj)

            for future in as_completed(obj_list):
                self._arr.append(future.result())

        self.printf()

    
    def printf(self):
        for i in range(self._thread):
            if self._arr[i]:
                print('==================================')
                for k, v in self._arr[i].items():
                    print(k, ": ", v)
          
        

if __name__ == "__main__":
    # bench = Bench("https://www.baidu.com")
    # res = bench.connection()
    # for k, v in res.items():
    #     print(k, ": ", v)

    # bench2 = Bench("https://www.www.baidu.com.com")
    # res2 = bench2.connection()
    # if res2:
    #     for k, v in res2.items():
    #         print(k, ": ", v)

    # bench3 = Bench("https://www.google.com")
    # res3 = bench2.connection()
    # if res3:
    #     for k, v in res3.items():
    #         print(k, ": ", v)

    bench = Bench("https://www.baidu.com", thread=3)
    bench.run()
    # bench.printf()
