# rocketmq-ons-cpp
A full-fledged ONS C/C++ SDK out of its Java Counterpart taking advantage of GraalVM technology.

You can [quick start](https://github.com/ShannonDing/rocketmq-ons-cpp/blob/master/doc/QuickStart.md) by release binary, and also you can build it step by step as below.

----------
## Prerequisites
- g++ 4.8.x, and [4.8.5](https://gcc.gnu.org/gcc-4.8/) recommend
- cmake >= 3.0, and [3.14](https://cmake.org/download/) recommend
- Graalvm tool >= 19.0.0 and [GraalVM CE19.0.0](https://github.com/oracle/graal/releases/tag/vm-19.0.0) recommend
- Apache Maven >= 3.5 and [3.6.1](http://maven.apache.org/download.cgi) recommend

## Install build tools
Take `/root/tools` as the install path, you can install the build tools by below steps.
1. Graalvm: Download [19.0.0ce](https://github.com/oracle/graal/releases/tag/vm-19.0.0) according your OS platform to the install path and unpack it. then modify the ``.bash_profile`` to let it enabled.
    then install native-image by ``gu install native-image`` command.
    
    ```bash
    # .bash_profile
    
    if [ -f ~/.bashrc ]; then
        . ~/.bashrc
    fi
    
    # User specific environment and startup programs
    export MAVEN_HOME=/root/tools/apache-maven
    export JAVA_HOME=/root/tools/graalvm-ce-19.0.0
    export CMAKE_HOME=/root/tools/cmake
    export GRAALVM_HOME=/root/tools/graalvm-ce-19.0.0
    PATH=$JAVA_HOME/bin:CMAKE_HOME/bin:$GRAALVM_HOME/bin:$MAVEN_HOME/bin:$PATH:$HOME/.local/bin:$HOME/bin
    
    export PATH
    ```
    
2. Apache Maven: Download the binary[3.6.1](http://maven.apache.org/download.cgi) to the install path and unpack it, enable it by set runtime path to ``.bash_profile``.
3. cmake: Download the binary[3.14](https://cmake.org/download/) to the install path and unpack it, enable it by set runtime path to ``.bash_profile``.
4. g++: for centos7, the default gcc version is 4.8, you can install it by yum
    ```bash
    yum install gcc gcc-c++
    ```
    for centos6, you need update the yum repo first.
    ```bash
    curl -Lks http://www.hop5.in/yum/el6/hop5.repo > /etc/yum.repos.d/hop5.repo
    yum install gcc gcc-g++
    ```

## Check environment
1. Check graalvm
    ```bash
    [root@7a9d198e9877 ~]# native-image --version
    GraalVM Version 19.0.0 CE
    ```
2. Check Apache Maven
    ```bash
    [root@7a9d198e9877 ~]# mvn -v
    Apache Maven 3.6.1 (d66c9c0b3152b2e69ee9bac180bb8fcc8e6af555; 2019-04-04T19:00:29Z)
    Maven home: /root/tools/apache-maven
    Java version: 1.8.0_212, vendor: Oracle Corporation, runtime: /root/tools/graalvm-ee-19.0.0/jre
    Default locale: en_US, platform encoding: ANSI_X3.4-1968
    OS name: "linux", version: "4.9.93-linuxkit-aufs", arch: "amd64", family: "unix"
    ```
3. Check cmake
    ```bash
    [root@7a9d198e9877 ~]# cmake --version
    cmake version 3.14.5
    ```
4. Check g++
    ```bash
    [root@7a9d198e9877 ~]# g++ --version
    g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-36)
    Copyright (C) 2015 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```
## Build 
1. Build the cpp core by ***mvn install*** command
    ```bash
    mvn install
    ```
2. After finished, you can find the core libary in the build folder and the Head file in the graalvm_artifacts folder.
    ```bash
    root@974285cc0d03:~/OpenSource/rocketmq-ons-cpp/build# ll librocketmq_client_core.so
    -rwxr-xr-x  1 root root 30198632 Jul 19 07:43 librocketmq_client_core.so*
    
    root@974285cc0d03:~/OpenSource/rocketmq-ons-cpp/graalvm_artifacts# ll
    total 28
    drwxr-xr-x  2 root root 4096 Jul 19 07:03 ./
    drwxr-xr-x 14 root root 4096 Jul 25 06:05 ../
    -rw-r--r--  1 root root 4070 Jul 19 07:43 graal_isolate.h
    -rw-r--r--  1 root root 4182 Jul 19 07:43 graal_isolate_dynamic.h
    -rw-r--r--  1 root root 1278 Jul 19 07:43 rocketmq-ons-cpp-full.h
    -rw-r--r--  1 root root 1526 Jul 19 07:43 rocketmq-ons-cpp-full_dynamic.h
    ```
3. Build ONS library
    ```bash
    mkdir dist & cd dist
    cmake ../
    make
    ```
## Test
After building, the unit test case are installed in the dist/tests folder, you can the test case by:
```bash
make test
```
## Package
After building, you can package the library to a package and release it. the package contains the Head file,the lirary and the demos file.
Change to the project root path,then run the package script:
```bash
sh package.sh
```