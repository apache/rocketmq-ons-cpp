#!/bin/bash
if [ -d build ]; then
	rm -fr build
fi
mkdir build

native-image --shared  -H:Path=./build \
                       --no-server \
                       -H:ReflectionConfigurationFiles=./tools/graal/reflection_config.json \
                       -H:Name=rocketmq_client_core \
                       -H:CLibraryPath=./src/main/c/native \
                       -jar ./target/rocketmq-ons-cpp-full.jar \
                       -Dio.netty.noUnsafe=true \
                       -DOPEN_DUAL_ABI=false \
                       --report-unsupported-elements-at-runtime \
                       --allow-incomplete-classpath \
                       -H:+ReportExceptionStackTraces \
                       --enable-all-security-services \
                       --enable-url-protocols=https \
                       -H:EnableURLProtocols=http \
                       --initialize-at-build-time \
                       --initialize-at-run-time=io.netty.handler.ssl.util.BouncyCastleSelfSignedCertGenerator,io.netty.handler.ssl.ReferenceCountedOpenSslClientContext,io.netty.handler.ssl.ReferenceCountedOpenSslServerContext,io.netty.handler.ssl.JdkNpnApplicationProtocolNegotiator,io.netty.handler.ssl.JdkAlpnApplicationProtocolNegotiator,io.netty.handler.ssl.util.ThreadLocalInsecureRandom,io.netty.handler.ssl.JettyNpnSslEngine,io.netty.handler.ssl.ReferenceCountedOpenSslEngine,io.netty.handler.ssl.ConscryptAlpnSslEngine,io.netty.handler.ssl.JettyAlpnSslEngine$ClientEngine,io.netty.handler.ssl.JettyAlpnSslEngine$ServerEngine,io.netty.util.internal.ObjectCleaner,io.netty.handler.ssl.util.InsecureTrustManagerFactory

mkdir -p graalvm_artifacts/
cp build/*.h  graalvm_artifacts/

if test "$(uname)" = "Linux"; then
    mv build/rocketmq-ons-cpp-full.so build/librocketmq_client_core.so
fi

if test "$(uname)" = "Darwin"; then
    mv build/rocketmq-ons-cpp-full.dylib build/librocketmq_client_core.dylib
    install_name_tool -id "@rpath/librocketmq_client_core.dylib" build/librocketmq_client_core.dylib
fi
