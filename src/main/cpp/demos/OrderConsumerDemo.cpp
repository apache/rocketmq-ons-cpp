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

#include "ONSFactory.h"

#include <iostream>
#include <thread>
#include <mutex>

using namespace ons;

std::mutex console_mtx;

class ExampleMessageListener : public MessageOrderListener {
public:
    OrderAction consume(Message &message, ConsumeOrderContext &context) {
        //此处为具体的消息处理过程，确认消息被处理成功请返回 Success，
        //如果有消费异常，或者期望重新消费，可以返回 Suspend，消息将会在一段时间后重新投递
        std::lock_guard<std::mutex> lk(console_mtx);
        std::cout << "Received a message. Topic: " << message.getTopic() << ", MsgId: "
                  << message.getMsgID() << std::endl;
        return Success;
    }
};

int main(int argc, char *argv[]) {
    std::cout << "=======Before consuming messages=======" << std::endl;
    ONSFactoryProperty factoryInfo;
    //Request from ONS console, this should be GroupID here.
    //请填写在阿里云ONS控制台上申请的GroupID，从实例化的版本开始，ProducerId和CounsumerId已经统一，此处设置是为了接口保持向前兼容。
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumerId, "GID_XXX");
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
    OrderConsumer *consumer = ONSFactory::getInstance()->createOrderConsumer(factoryInfo);

    std::string topic(factoryInfo.getPublishTopics());
    std::string tag("Your Tag");
    ExampleMessageListener *messageListener = new ExampleMessageListener();
    consumer->subscribe(topic.c_str(), tag.c_str(), messageListener);

    //Start this consumer
    //准备工作完成，必须调用启动函数，才可以正常工作。
    consumer->start();

    //Keep main thread running until process finished.
    //请保持线程常驻，不要执行shutdown操作
    std::this_thread::sleep_for(std::chrono::milliseconds(60 * 1000));
    consumer->shutdown();
    std::cout << "=======After consuming messages======" << std::endl;
    return 0;
}