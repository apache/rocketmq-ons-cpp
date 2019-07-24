/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <chrono>
#include "ONSFactory.h"
#include"LocalTransactionChecker.h"
#include "LocalTransactionExecuter.h"


using namespace std;
using namespace ons;

class LocalTransactionCheckerImpl : public LocalTransactionChecker {
    virtual TransactionStatus check(Message &msg) {
        cout << "checker::commit transaction" << endl;
        return CommitTransaction;
    }
};

class LocalTransactionExecuterImpl : public LocalTransactionExecuter {
    virtual TransactionStatus execute(Message &msg) {
        cout << "executer::commit transaction of msgid: " << msg.getMsgID() << endl;
        return CommitTransaction;
    }
};

int main() {
    std::cout << "=======Before sending message=======" << std::endl;
    ONSFactoryProperty factoryInfo;
    //Request from ONS console, this should be GroupID here.
    //请填写在阿里云ONS控制台上申请的GroupID，从实例化的版本开始，ProducerId和CounsumerId已经统一，此处设置是为了接口保持向前兼容。
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ProducerId, "GID_XXX");
    //Request from ONS console
    //请填写阿里云ONS控制台上申请的topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics, "Your topic");
    //Your Access Key from your account.
    //请填写你的账户的AK
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "Your Access Key");
    //Your Secret Key from your account.
    //请填写你的账户的SK
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, "Your Secret Key");
    //This is the endpoint from ONS console
    //请填写阿里云ONS控制台上对应实例的接入点
    factoryInfo.setFactoryProperty(ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://xxxxxxxxxxxxxxxx.aliyuncs.com:80");
    TransactionProducer *producer = nullptr;
    LocalTransactionCheckerImpl *checker = new LocalTransactionCheckerImpl();
    producer = ONSFactory::getInstance()->createTransactionProducer(factoryInfo, checker);
    producer->start();
    Message msg(
            factoryInfo.getPublishTopics(),
            "Your Tag",
            "Your Key",
            "This message body."
    );
    auto start = std::chrono::system_clock::now();
    int count = 32;
    LocalTransactionExecuterImpl executer;
    for (int i = 0; i < count; ++i) {
        try {
            SendResultONS sendResult = producer->send(msg, &executer);
            std::cout << "Message ID: " << sendResult.getMessageId() << std::endl;
        }
        catch (ONSClientException &e) {
            cout << e.GetMsg() << endl;
        }
        cout << endl;
    }
    auto interval = std::chrono::system_clock::now() - start;
    std::cout << "Send " << count << " messages OK, costs "
              << std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() << "ms" << std::endl;

//    producer->shutdown();
    std::cout << "=======After sending message=======" << std::endl;
    return 0;
}