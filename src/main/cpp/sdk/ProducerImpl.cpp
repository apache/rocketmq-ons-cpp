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

#include "ProducerImpl.h"
#include "rocketmq.h"
#include "rocketmq-ons-cpp-full.h"
#include "SendResultONS.h"
#include "Logger.h"
#include "common/UtilAll.h"

using namespace ons;

ProducerImpl::ProducerImpl() throw(ONSClientException) {
}

ProducerImpl::ProducerImpl(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    std::string logPath(factoryProperty.getLogPath());
    rocketmq::spd_log::initLogger(logPath, rocketmq::LOGLEVEL_INFO);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    factory_property fp;
    FactoryPropertyConverter converter(factoryProperty, fp);
    instanceIndex_ = create_producer(thread_, &fp);
    rocketmq::spd_log::info("Create Producer OK, InstanceId:{}, ProducerID:{}, NameServer:{}",
                 instanceIndex_, factoryProperty.getProducerId(), factoryProperty.getNameSrvAddr());
}

ProducerImpl::~ProducerImpl() {
    rocketmq::spd_log::destoryLogger();
}

void ProducerImpl::start() {

}

void ProducerImpl::shutdown() {
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    destroy_instance(thread_, instanceIndex_);
    rocketmq::spd_log::info("Destroy Producer instance {} OK", instanceIndex_);
}

ons::SendResultONS ProducerImpl::send(Message &msg) throw(ONSClientException) {
    message m;
    MessageConverter converter(m, msg);
    send_result sendResult;
    SendResultWrapper wrapper(sendResult);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    send_message(thread_, instanceIndex_, &m, &sendResult);
    if (sendResult.error_no) {
        throw ONSClientException(std::string(sendResult.error_msg), sendResult.error_no);
    }
    rocketmq::spd_log::debug("Send message OK. MsgId: {}", sendResult.message_id);
    SendResultONS sendResultOns;
    sendResultOns.setMessageId(std::string(sendResult.message_id));
    return sendResultOns;
}

SendResultONS ProducerImpl::send(Message &msg, const MessageQueueONS &mq) throw(ons::ONSClientException) {
    return send(msg);
}

#ifdef __cplusplus
extern "C" {
#endif
void on_success_func(void *thread, char *message_id, char *send_callback_ons) {
    auto sendCallbackONS = reinterpret_cast<SendCallbackONS *>(send_callback_ons);
    SendResultONS sendResultOns;
    sendResultOns.setMessageId(message_id);
    sendCallbackONS->onSuccess(sendResultOns);
}

void on_exception_func(void *thread, char *m_msg, int m_error, char *send_callback_ons) {
    auto sendCallbackONS = reinterpret_cast<SendCallbackONS *>(send_callback_ons);
    ONSClientException onsClientException(m_msg, m_error);
    sendCallbackONS->onException(onsClientException);
}
#ifdef __cplusplus
}
#endif

void ProducerImpl::sendAsync(Message &msg, SendCallbackONS *pSendCallback) throw(ons::ONSClientException) {
    message m;
    MessageConverter converter(m, msg);
    send_result sendResult;
    SendResultWrapper wrapper(sendResult);
    callback_func c_f;
    if (pSendCallback == NULL) {
        std::string msg = "Send Callback cannot be NULL.";
        throw ONSClientException(msg, -1);
    }
    c_f.on_success = on_success_func;
    c_f.on_exception = on_exception_func;
    c_f.send_callback_ons = reinterpret_cast<char *>(pSendCallback);

    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    send_message_async(thread_, instanceIndex_, &m, &sendResult, &c_f);
    if (sendResult.error_no) {
        throw ONSClientException(std::string(sendResult.error_msg), sendResult.error_no);
    }
}

void ProducerImpl::sendOneway(Message &msg) throw(ons::ONSClientException) {
    message m;
    MessageConverter converter(m, msg);
    send_result sendResult;
    bzero(&sendResult, sizeof(send_result));
    SendResultWrapper wrapper(sendResult);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    send_message_oneway(thread_, instanceIndex_, &m, &sendResult);
    if (sendResult.error_no) {
        throw ONSClientException(std::string(sendResult.error_msg), sendResult.error_no);
    }
}
