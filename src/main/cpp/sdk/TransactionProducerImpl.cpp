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

#include "TransactionProducerImpl.h"
#include "rocketmq-ons-cpp-full.h"
#include "Logger.h"

namespace ons {

#ifdef __cplusplus
    extern "C" {
#endif
    int transaction_check_func(void *thread, void *opaque, char *topic, char *user_props, char *sys_props,
                               char *body, int body_len) {
        LocalTransactionChecker *checker = reinterpret_cast<LocalTransactionChecker *>(opaque);
        ons::Message message_;
        message_.setTopic(topic);
        ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
        ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
        message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
        return checker->check(message_);
    }

    int transaction_execute_func(void *thread, void *opaque, char *topic, char *user_props, char *sys_props,
                                 char *body, int body_len) {
        LocalTransactionExecuter *executor = reinterpret_cast<LocalTransactionExecuter *>(opaque);
        ons::Message message_;
        message_.setTopic(topic);
        ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
        ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
        message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
        return executor->execute(message_);
    }
#ifdef __cplusplus
    }
#endif

    TransactionProducerImpl::TransactionProducerImpl(
            ONSFactoryProperty factoryProperty,
            LocalTransactionChecker *pTransactionCheckListener) {
        std::string logPath(factoryProperty.getLogPath());
        rocketmq::spd_log::initLogger(logPath, rocketmq::LOGLEVEL_INFO);
        graal_isolatethread_t *thread_;
        ThreadAttachment attachment(&thread_);
        factory_property fp;
        FactoryPropertyConverter converter(factoryProperty, fp);
        void *checker = reinterpret_cast<void *>(pTransactionCheckListener);
        void *transaction_check = (void *) transaction_check_func;
        instanceIndex_ = create_transaction_producer(thread_, &fp, checker, transaction_check);
        spdlog::info("Create Transaction Producer OK, InstanceId:{}, ProducerID:{}, NameServer:{}",
                     instanceIndex_, factoryProperty.getProducerId(), factoryProperty.getNameSrvAddr());
    }

    TransactionProducerImpl::~TransactionProducerImpl() {
        rocketmq::spd_log::destoryLogger();
    }

    void TransactionProducerImpl::start() {
    }

    void TransactionProducerImpl::shutdown() {
        graal_isolatethread_t *thread_;
        ThreadAttachment attachment(&thread_);
        destroy_instance(thread_, instanceIndex_);
        spdlog::info("Destroy Transaction Producer instance {} OK", instanceIndex_);
    }

    SendResultONS TransactionProducerImpl::send(Message &msg, LocalTransactionExecuter *pTransactionExecutor) {
        message m;
        MessageConverter converter(m, msg);
        send_result sendResult;
        SendResultWrapper wrapper(sendResult);
        graal_isolatethread_t *thread_;
        ThreadAttachment attachment(&thread_);
        if (pTransactionExecutor == NULL) {
            std::string msg = "Transaction Executor cannot be NULL.";
            throw ONSClientException(msg, -1);
        }
        void *executor = reinterpret_cast<void *>(pTransactionExecutor);
        void *transaction_execute = (void *) transaction_execute_func;
        send_message_transaction(thread_, instanceIndex_, &m, &sendResult, executor, transaction_execute);
        if (sendResult.error_no) {
            throw ONSClientException(std::string(sendResult.error_msg), sendResult.error_no);
        }
        SendResultONS sendResultOns;
        sendResultOns.setMessageId(std::string(sendResult.message_id));
        return sendResultOns;
    }

}