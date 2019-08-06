#!/bin/bash

HOME_PATH=rocketmq-ons-cpp
CORE_LIBRARY_NAME=librocketmq_client_core
API_LIBRARY_NAME=libonsclient4cpp

if test "$(uname)" = "Linux"; then
    echo "========================start check package file for Linux======================================"
    LIBRARY_SUFFIX=so
    PACKAGE_NAME=aliyun-mq-linux-cpp-sdk
fi
if test "$(uname)" = "Darwin"; then
    echo "========================start check package file for Darwin======================================"
    LIBRARY_SUFFIX=dylib
    PACKAGE_NAME=aliyun-mq-darwin-cpp-sdk
fi

# this script must execute on graalvm-rocketmq-client4cpp dirname
full_path=`pwd`
base_dir_name=`basename $full_path`
if [[ ${base_dir_name} != ${HOME_PATH} ]];then
  echo "please execute this script on ${HOME_PATH} "
  exit -1
fi

# step 1 check license output

if [[ ! -e "licenses" ]]; then
  echo "Licenses path licenses does not exist, please check."
  exit -1
fi
# step 2 check headfile output

if [[ ! -e "src/main/cpp/include" ]]; then
  echo "Head file path src/main/cpp/include does not exist, please check."
  exit -2
fi

# step 3 check lib/libonsclient4cpp.so and build/librocketmq_client_core.so
if [[ ! -e "build/${CORE_LIBRARY_NAME}.${LIBRARY_SUFFIX}" ]];then
  echo "Core library build/${CORE_LIBRARY_NAME}.${LIBRARY_SUFFIX} not exits, please execute mvn install"
  exit -3
fi

if [[ ! -e "lib/${API_LIBRARY_NAME}.${LIBRARY_SUFFIX}" ]] ;then
echo "ONS API Library ${API_LIBRARY_NAME}.${LIBRARY_SUFFIX} not exits, please go to /dist and execute cmake ../ then  execute make."
exit -3
fi

# step 4, check demos
if [[ ! -e "src/main/cpp/demos" ]];then
  echo "Demo path src/main/cpp/demos does not exists, please check."
  exit -4
fi

# step 5, check docs
if [[ ! -e "doc" ]];then
  echo "doc path doc does not exists, please check."
  exit -5
fi

# step 6 remove old package path
if [[ -e ${PACKAGE_NAME} ]] || [[ -e ${PACKAGE_NAME}.tar.gz ]];then
  echo "find old package dir, we will delete this package file."
  rm -rf ${PACKAGE_NAME} &>/dev/null
  rm -rf ${PACKAGE_NAME}.tar.gz &>/dev/null
fi
echo "========================check package file success======================================"

echo "========================start packaging ================================================"
mkdir -v ${PACKAGE_NAME}
mkdir -v ${PACKAGE_NAME}/demos
mkdir -v ${PACKAGE_NAME}/doc

echo "Copy head files..."
cp -rf src/main/cpp/include ${PACKAGE_NAME}/include
echo "Copy library files......."
cp -rf lib ${PACKAGE_NAME}/lib
cp -rf build/${CORE_LIBRARY_NAME}.${LIBRARY_SUFFIX} ${PACKAGE_NAME}/lib/
echo "Copy demo files............"
cp -f src/main/cpp/demos/*.cpp ${PACKAGE_NAME}/demos/
echo "Copy demo make file................."
cp -f src/main/cpp/demos/CMakeLists.Release ${PACKAGE_NAME}/demos/CMakeLists.txt
echo "Copy changlog................."
cp -f doc/changelog ${PACKAGE_NAME}/doc/
echo "Copy license and notice file................."
cp -f licenses/LICENSE-BIN ${PACKAGE_NAME}/LICENSE
cp -f licenses/NOTICE-BIN ${PACKAGE_NAME}/NOTICE
echo "========================packaging end ================================================"


echo "========================start check ================================================"

cd ${PACKAGE_NAME}/demos

mkdir -v dist
cd dist
cmake ../ &>/dev/null
if [ $? -eq 0 ];then
  echo "cmake check success."
else
  echo "check failure, execute cmake build demo occur some error."
  exit -7
fi
make &>/dev/null
if [ $? -eq 0 ];then
  echo "check success"
  make clean &>/dev/null
else
  echo "check failure, execute make build demo occur some error."
  exit -7
fi
cd ../
if [ -e "bin" ];then
  echo "Remove excutable file."
  rm -rf bin &>/dev/null
fi
if [ -e "dist" ];then
  echo "Remove makefile file."
  rm -rf dist &>/dev/null
fi
cd $full_path
tar -czf ${PACKAGE_NAME}.tar.gz ${PACKAGE_NAME}/* &>/dev/null

echo "========================check  end ================================================"
