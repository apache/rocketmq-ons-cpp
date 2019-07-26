#RocketMQ ONS CPP 使用说明中文版
-------------
#### 1. 版本包介绍

下载对应操作系统内核的[版本](https://github.com/ShannonDing/rocketmq-ons-cpp/releases)后进行解压，会有如下目录结构: 
```
demos/
include/
lib/
changelog
```
上面的目录和文件的作用如下: 
1. demos：包含了普通消息发送、Oneway 消息发送、顺序消息发送、普通消息消费、顺序消息消费等例子，还包含了 CMakeList.txt 用于 demo 的编译和管理。
2. include：用户自己编写的程序需要 include 的头文件。 
3. lib：包含基于x86_64的动态库，分别为接口库和内核库。
      libonsclient4cpp.so
      librocketmq_client_core.so
4. changelog： 新版本发布解决的问题和引入的新特性列表。

#### 2. Demos的使用
此版本的SDK只提供动态库方案。消息队列 RocketMQ 的库文件在 lib/目录下，生成可执行文件时需要链接 librocketmq_client_core.so和 libonsclient4cpp.so。由于demo引入了C++11的特性和使用cmake来管理，需要提前安装cmake3.0以上版本和g++ 4.8及以上版本。 

*注意*：由于 GCC 5.x 引入 [Dual ABI](https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html)，编译链接时，请添加 -D_GLIBCXX_USE_CXX11_ABI=0 编译选项。
```
cd aliyun-mq-linux-cpp-sdk  //下载的 SDK 解压后的路径
cd demos //进入 demos 目录，修改 demo 文件，填入自己在消息队列 RocketMQ 控制台创建的 Topic，key 相关的信息
cmake  .   //检测依赖和生成编译脚本
make  执行编译操作
cd bin //到生成的可执行文件目录下运`
```