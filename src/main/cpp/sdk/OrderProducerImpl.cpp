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

#include "OrderProducerImpl.h"
#include "rocketmq.h"
#include "rocketmq-ons-cpp-full.h"
#include <iostream>
#include "SendResultONS.h"
#include "Logger.h"
#include "common/UtilAll.h"

using namespace ons;

OrderProducerImpl::OrderProducerImpl(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    std::string logPath(factoryProperty.getLogPath());
    rocketmq::spd_log::initLogger(logPath, rocketmq::LOGLEVEL_INFO);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    factory_property fp;
    FactoryPropertyConverter converter(factoryProperty, fp);
    instanceIndex_ = create_order_producer(thread_, &fp);
    rocketmq::spd_log::info("Create Order Producer OK, InstanceId:{}, ProducerID:{}, NameServer:{}",
                 instanceIndex_, factoryProperty.getProducerId(), factoryProperty.getNameSrvAddr());
}

OrderProducerImpl::~OrderProducerImpl() {
    rocketmq::spd_log::destoryLogger();
}

void OrderProducerImpl::start() {

}

void OrderProducerImpl::shutdown() {
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    destroy_instance(thread_, instanceIndex_);
    rocketmq::spd_log::info("Destroy Order Producer instance {} OK", instanceIndex_);
}

SendResultONS OrderProducerImpl::send(Message &msg, std::string shardingKey) throw(ons::ONSClientException) {
    message m;
    MessageConverter converter(m, msg);
    send_result sendResult;
    SendResultWrapper wrapper(sendResult);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    send_order_message(thread_, instanceIndex_, &m, &sendResult, (char *) shardingKey.c_str());
    if (sendResult.error_no) {
        ONSClientException clientException(std::string(sendResult.error_msg), sendResult.error_no);
        throw clientException;
    }
    rocketmq::spd_log::debug("Send message OK. MsgId: {}", sendResult.message_id);
    SendResultONS sendResultOns;
    sendResultOns.setMessageId(std::string(sendResult.message_id));
    return sendResultOns;

}