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

#include "ConsumerImpl.h"
#include "common/Logger.h"
#include "common/UtilAll.h"
#include "rocketmq-ons-cpp-full.h"
#include "common/UtilAll.h"
#include <iostream>

using namespace ons;

ConsumerImpl::ConsumerImpl() {

}

ConsumerImpl::ConsumerImpl(const ons::ONSFactoryProperty &factoryProperty) throw(ons::ONSClientException) {
    std::string logPath(factoryProperty.getLogPath());
    rocketmq::spd_log::initLogger(logPath, rocketmq::LOGLEVEL_INFO);
    factory_property property;
    ons::FactoryPropertyConverter converter(factoryProperty, property, false);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    this->instanceIndex_ = create_consumer(thread_, &property);
    spdlog::info("Create Consumer OK, InstanceId:{}, ConsumerID:{}, NameServer:{}, Trace:{}, MessageModel:{}",
                 instanceIndex_, factoryProperty.getConsumerId(),
                 factoryProperty.getNameSrvAddr(), factoryProperty.getOnsTraceSwitch(),
                 factoryProperty.getMessageModel());
}

void ConsumerImpl::start() {
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    start_instance(thread_, instanceIndex_);
    spdlog::info("Start Consumer instance {} OK", instanceIndex_);
}

#ifdef __cplusplus
extern "C" {
#endif

int consumer_on_message(void *thread, void *opaque, char *topic, char *user_props, char *sys_props, char *body,
                        int body_len) {
    ons::MessageListener *listener = reinterpret_cast<ons::MessageListener *>(opaque);
    ons::Message message_;
    message_.setTopic(topic);
    ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
    ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
    message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
    ons::ConsumeContext ctx_;
    Action action = listener->consume(message_, ctx_);
    switch (action) {
        case CommitMessage:
            return 0;
        case ReconsumeLater:
            spdlog::info("Consume Message failed, Topic:{}, MessageId:{}, RecosumeTimes:{}",
                         message_.getTopic(), message_.getMsgID(), message_.getReconsumeTimes());
            return 1;
        default:
            return 1;
    }
}
#ifdef __cplusplus
}
#endif

void ConsumerImpl::subscribe(const char *topic, const char *subExpression, ons::MessageListener *listener) {
    subscription sub;
    memset(&sub, 0, sizeof(subscription));
    if (topic == NULL || 0 == strcasecmp(topic, "")) {
        std::string msg = "Topic cannot be NULL! Please check your ONS property set.";
        throw ONSClientException(msg, -1);
    }
    if (subExpression == NULL || 0 == strcasecmp(subExpression, "")) {
        std::string msg = "Sub Tag cannot be NULL! Please check your ONS property set.";
        throw ONSClientException(msg, -1);
    }
    if (listener == NULL) {
        std::string msg = "Message Listener cannot be NULL! Please check your ONS property set.";
        throw ONSClientException(msg, -1);
    }
    sub.topic = const_cast<char *>(topic);
    sub.sub_expression = const_cast<char *>(subExpression);
    sub.opaque = listener;
    sub.on_message = consumer_on_message;
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    ::subscribe(thread_, instanceIndex_, &sub);
    spdlog::info("Subscribe OK, InstanceID:{}, Topic:{}, SubExpression:{}", instanceIndex_, topic, subExpression);
}

void ConsumerImpl::shutdown() {
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    destroy_instance(thread_, instanceIndex_);
    spdlog::info("Destroy Consumer instance {} OK", instanceIndex_);
}

ConsumerImpl::~ConsumerImpl() {
    rocketmq::spd_log::destoryLogger();
}