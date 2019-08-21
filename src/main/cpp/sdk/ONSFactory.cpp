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

#include "rocketmq.h"
#include "ONSFactory.h"
#include "ProducerImpl.h"
#include "OrderProducerImpl.h"
#include "ConsumerImpl.h"
#include "OrderConsumerImpl.h"
#include "common/UtilAll.h"
#include "common/ONSClientAbstract.h"
#include "TransactionProducerImpl.h"
#include <vector>

namespace ons {
    const char *ONSFactoryProperty::LogPath = "LogPath";
    const char *ONSFactoryProperty::ProducerId = "ProducerId";
    const char *ONSFactoryProperty::ConsumerId = "ConsumerId";
    const char *ONSFactoryProperty::GroupId = "GroupId";
    const char *ONSFactoryProperty::PublishTopics = "PublishTopics";
    const char *ONSFactoryProperty::MsgContent = "MsgContent";
    const char *ONSFactoryProperty::AccessKey = "AccessKey";
    const char *ONSFactoryProperty::SecretKey = "SecretKey";
    const char *ONSFactoryProperty::MessageModel = "MessageModel";
    const char *ONSFactoryProperty::BROADCASTING = "BROADCASTING";
    const char *ONSFactoryProperty::CLUSTERING = "CLUSTERING";
    const char *ONSFactoryProperty::SendMsgTimeoutMillis = "SendMsgTimeoutMillis";
    const char *ONSFactoryProperty::SuspendTimeMillis = "SuspendTimeMillis";
    const char *ONSFactoryProperty::SendMsgRetryTimes = "SendMsgRetryTimes";
    const char *ONSFactoryProperty::MaxMsgCacheSize = "MaxMsgCacheSize";
    const char *ONSFactoryProperty::MaxCachedMessageSizeInMiB = "MaxCachedMessageSizeInMiB";
    const char *ONSFactoryProperty::ONSAddr =
            "ONSAddr";  // name server domain name
    const char *ONSFactoryProperty::NAMESRV_ADDR =
            "NAMESRV_ADDR";  // name server ip addr
    const char *ONSFactoryProperty::ConsumeThreadNums = "ConsumeThreadNums";
    const char *ONSFactoryProperty::OnsChannel = "OnsChannel";
    const char *ONSFactoryProperty::OnsTraceSwitch = "OnsTraceSwitch";
    const char *ONSFactoryProperty::ConsumerInstanceName = "ConsumerInstanceName";
    const char *ONSFactoryProperty::InstanceId = "InstanceId";

    const char *NULL_STRING = "";
    const char *DEFAULT_CHANNEL = "ALIYUN";

    ONSFactoryProperty::ONSFactoryProperty() {}

    ONSFactoryProperty::~ONSFactoryProperty() {}

    void ONSFactoryProperty::setOnsTraceSwitch(bool onswitch) {
        if (onswitch)
            setFactoryProperty(OnsTraceSwitch, "true");
        elsejavascript:
        void(0);
        setFactoryProperty(OnsTraceSwitch, "false");
    }

    void ONSFactoryProperty::setMaxMsgCacheSize(int value) {
        char tmp[256] = {0};
        sprintf(tmp, "%d", value);
        setFactoryProperty(MaxMsgCacheSize, tmp);
    }

    void ONSFactoryProperty::setSendMsgTimeout(int value) {
        char tmp[16];
        sprintf(tmp, "%d", value);
        setFactoryProperty(SendMsgTimeoutMillis, tmp);
    }

    const char *ONSFactoryProperty::getLogPath() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(LogPath);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    void ONSFactoryProperty::setSendMsgRetryTimes(int value) {
        char tmp[16];
        sprintf(tmp, "%d", value);
        setFactoryProperty(SendMsgRetryTimes, tmp);
    }

    void ONSFactoryProperty::setOnsChannel(ONSChannel onsChannel) throw(ons::ONSClientException) {
        if (onsChannel == CLOUD) {
            setFactoryProperty(OnsChannel, "CLOUD");
        } else if (onsChannel == ALIYUN) {
            setFactoryProperty(OnsChannel, "ALIYUN");
        } else if (onsChannel == ALL) {
            setFactoryProperty(OnsChannel, "ALL");
        } else if (onsChannel == LOCAL) {
            setFactoryProperty(OnsChannel, "LOCAL");
        } else if (onsChannel == INNER) {
            setFactoryProperty(OnsChannel, "INNER");
        } else {
            throw ONSClientException(
                    FAQ::errorMessage("ONSChannel could only be setted to "
                                      "CLOUD/ALIYUN/ALL, please reset it.",
                                      FAQ::CLIENT_CHECK_MSG_EXCEPTION),
                    -1);
        }
    }

    bool ONSFactoryProperty::checkValidityOfFactoryProperties(const string &key,
                                                              const string &value) throw(ons::ONSClientException) {
        if (!key.compare(MessageModel)) {
            if (value.compare(BROADCASTING) && value.compare(CLUSTERING)) {
                throw ONSClientException(
                        FAQ::errorMessage("MessageModel could only be setted to BROADCASTING "
                                          "or CLUSTERING, please set it.",
                                          FAQ::CLIENT_CHECK_MSG_EXCEPTION),
                        -1);
            }
        }

        if (!key.compare(AccessKey)) {
            if (!value.compare("")) {
                throw ONSClientException(
                        FAQ::errorMessage("AccesKey must be setted, please set it.",
                                          FAQ::CLIENT_CHECK_MSG_EXCEPTION),
                        -1);
            }
        }

        if (!key.compare(SecretKey)) {
            if (!value.compare("")) {
                throw ONSClientException(
                        FAQ::errorMessage("SecretKey must be setted, please set it.",
                                          FAQ::CLIENT_CHECK_MSG_EXCEPTION),
                        -1);
            }
        }
        return true;
    }

    void ONSFactoryProperty::setFactoryProperty(const char *key,
                                                const char *value) throw(ons::ONSClientException) {

        string keyString = UtilAll::to_string(key);
        string valueString = UtilAll::to_string(value);

        try {
            if (checkValidityOfFactoryProperties(keyString, valueString)) {
                m_onsFactoryProperties[keyString] = valueString;
            }
        } catch (ONSClientException &e) {
            throw e;
        }
    }

    void ONSFactoryProperty::setFactoryProperties(
            map<string, string> factoryProperties) {
        m_onsFactoryProperties = factoryProperties;
    }

    map<string, string> ONSFactoryProperty::getFactoryProperties() const {
        return m_onsFactoryProperties;
    }

    bool ONSFactoryProperty::getOnsTraceSwitch() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(OnsTraceSwitch);
        if ((it != m_onsFactoryProperties.end())) {
            if ((*it).second == "false") {
                return false;
            }
        }
        return true;
    }

    const char *ONSFactoryProperty::getProducerId() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(ProducerId);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getConsumerId() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(ConsumerId);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getGroupId() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(GroupId);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getPublishTopics() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(PublishTopics);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getMessageModel() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(MessageModel);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getMessageContent() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(MsgContent);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const int ONSFactoryProperty::getSendMsgTimeout() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(SendMsgTimeoutMillis);
        if (it != m_onsFactoryProperties.end()) {
            return atoi((*it).second.c_str());
        }
        return -1;
    }

    const int ONSFactoryProperty::getSuspendTimeMillis() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(SuspendTimeMillis);
        if (it != m_onsFactoryProperties.end()) {
            return atoi((*it).second.c_str());
        }
        return -1;
    }

    const int ONSFactoryProperty::getSendMsgRetryTimes() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(SendMsgRetryTimes);
        if (it != m_onsFactoryProperties.end()) {
            return atoi((*it).second.c_str());
        }
        return -1;
    }

    const int ONSFactoryProperty::getMaxMsgCacheSize() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(MaxMsgCacheSize);
        if (it != m_onsFactoryProperties.end()) {
            return atoi((*it).second.c_str());
        }

        return -1;
    }

    const int ONSFactoryProperty::getMaxMsgCacheSizeInMiB() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(MaxCachedMessageSizeInMiB);
        if (it != m_onsFactoryProperties.end()) {
            return atoi((*it).second.c_str());
        }

        return -1;
    }

    const ONSChannel ONSFactoryProperty::getOnsChannel() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(OnsChannel);
        if (it != m_onsFactoryProperties.end()) {
            if (it->second == "CLOUD") {
                return CLOUD;
            } else if (it->second == "ALIYUN") {
                return ALIYUN;
            } else if (it->second == "ALL") {
                return ALL;
            } else if (it->second == "LOCAL") {
                return LOCAL;
            } else if (it->second == "INNER") {
                return INNER;
            } else {
                return ALIYUN;  // default value
            }
        } else {
            return ALIYUN;  // default value
        }
    }

    const int ONSFactoryProperty::getConsumeThreadNums() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(ConsumeThreadNums);
        if (it != m_onsFactoryProperties.end()) {
            return atoi((*it).second.c_str());
        }

        return -1;
    }

    const char *ONSFactoryProperty::getNameSrvAddr() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(NAMESRV_ADDR);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getNameSrvDomain() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(ONSAddr);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getAccessKey() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(AccessKey);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getSecretKey() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(SecretKey);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getChannel() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(OnsChannel);

        return (it == m_onsFactoryProperties.end()) ? DEFAULT_CHANNEL : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getConsumerInstanceName() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(ConsumerInstanceName);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    const char *ONSFactoryProperty::getInstanceId() const {
        map<string, string>::const_iterator it =
                m_onsFactoryProperties.find(InstanceId);
        return (it == m_onsFactoryProperties.end()) ? NULL_STRING : (*it).second.c_str();
    }

    class ONSFactoryInstance : public ONSFactoryAPI {
    public:

        ONSFactoryInstance();

        virtual ~ONSFactoryInstance();

        ons::Producer *createProducer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);

        ons::OrderProducer *createOrderProducer(ons::ONSFactoryProperty factoryProperty) throw(
        ons::ONSClientException);

        ons::OrderConsumer *createOrderConsumer(ons::ONSFactoryProperty factoryProperty) throw(
        ons::ONSClientException);

        ons::TransactionProducer *createTransactionProducer(
                ons::ONSFactoryProperty factoryProperty,
                ons::LocalTransactionChecker *checker) throw(ons::ONSClientException);

        ons::PullConsumer *createPullConsumer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);

        ons::PushConsumer *createPushConsumer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);

        vector<Producer *> producerTable;

        vector<OrderProducer *> orderProducerTable;

        vector<TransactionProducer *> transactionProducerTable;

        vector<PushConsumer *> consumerTable;

        vector<OrderConsumer *> orderConsumerTable;
    };

    static ONSFactoryAPI *instance() {
        static ONSFactoryInstance instance_;
        return &instance_;
    }
}

using namespace ons;

int validateProperty(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    if (ALIYUN == factoryProperty.getOnsChannel() && NULL_STRING == factoryProperty.getAccessKey()) {
        throw ONSClientException(
                FAQ::errorMessage("AccessKey must be set, please set it.",
                                  FAQ::CLIENT_CHECK_MSG_EXCEPTION),
                -1);
    }
    if (ALIYUN == factoryProperty.getOnsChannel() && NULL_STRING == factoryProperty.getSecretKey()) {
        throw ONSClientException(
                FAQ::errorMessage("SecretKey must be set, please set it.",
                                  FAQ::CLIENT_CHECK_MSG_EXCEPTION),
                -1);
    }
    return 0;
}

ons::ONSFactoryAPI *ONSFactory::onsFactoryInstance = ons::instance();

ons::ONSFactoryAPI *ONSFactory::getInstance() {
    return onsFactoryInstance;
}

ONSFactoryInstance::ONSFactoryInstance() {
}

ONSFactoryInstance::~ONSFactoryInstance() {
    for (auto it = producerTable.begin(); it != producerTable.end(); ++it) {
        delete *it;
    }
    for (auto it = orderProducerTable.begin(); it != orderProducerTable.end(); ++it) {
        delete *it;
    }
    for (auto it = transactionProducerTable.begin(); it != transactionProducerTable.end(); ++it) {
        delete *it;
    }
    for (auto it = consumerTable.begin(); it != consumerTable.end(); it++) {
        delete *it;
    }
    for (auto it = orderConsumerTable.begin(); it != orderConsumerTable.end(); it++) {
        delete *it;
    }
}

Producer *ONSFactoryInstance::createProducer(ons::ONSFactoryProperty factoryProperty) throw(ONSClientException) {
    if (ALIYUN != factoryProperty.getOnsChannel()) {
        factoryProperty.setOnsTraceSwitch(false);
    }
    validateProperty(factoryProperty);
    ProducerImpl *producerImpl = new ProducerImpl(factoryProperty);
    producerTable.push_back(producerImpl);
    return producerImpl;
}

OrderProducer *ONSFactoryInstance::createOrderProducer(ons::ONSFactoryProperty factoryProperty) throw(
ONSClientException) {
    if (ALIYUN != factoryProperty.getOnsChannel()) {
        factoryProperty.setOnsTraceSwitch(false);
    }
    validateProperty(factoryProperty);
    OrderProducerImpl *orderProducerImpl = new OrderProducerImpl(factoryProperty);
    orderProducerTable.push_back(orderProducerImpl);
    return orderProducerImpl;

}

OrderConsumer *ONSFactoryInstance::createOrderConsumer(ons::ONSFactoryProperty factoryProperty) throw(
ONSClientException) {
    if (ALIYUN != factoryProperty.getOnsChannel()) {
        factoryProperty.setOnsTraceSwitch(false);
    }
    validateProperty(factoryProperty);
    OrderConsumerImpl *consumer = new OrderConsumerImpl(factoryProperty);
    orderConsumerTable.push_back(consumer);
    return consumer;
}

TransactionProducer *ONSFactoryInstance::createTransactionProducer(
        ons::ONSFactoryProperty factoryProperty,
        ons::LocalTransactionChecker *checker) throw(ons::ONSClientException) {
    if (ALIYUN != factoryProperty.getOnsChannel()) {
        factoryProperty.setOnsTraceSwitch(false);
    }
    validateProperty(factoryProperty);
    if (checker == NULL) {
        std::string msg = "Transaction Checker cannot be NULL. Please check your ONS property set.";
        throw ONSClientException(msg, -1);
    }
    TransactionProducerImpl *transactionProducerImpl = new TransactionProducerImpl(factoryProperty, checker);
    transactionProducerTable.push_back(transactionProducerImpl);
    return transactionProducerImpl;
}

PullConsumer *
ONSFactoryInstance::createPullConsumer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    ons::ONSClientException unsupportedException("Pull is not supported any more", -1);
    throw unsupportedException;
}

PushConsumer *
ONSFactoryInstance::createPushConsumer(ons::ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    if (ALIYUN != factoryProperty.getOnsChannel()) {
        factoryProperty.setOnsTraceSwitch(false);
    }
    validateProperty(factoryProperty);
    ConsumerImpl *consumer = new ConsumerImpl(factoryProperty);
    consumerTable.push_back(consumer);
    return consumer;
}

ONSFactoryAPI::ONSFactoryAPI() {}

ONSFactoryAPI::~ONSFactoryAPI() {}

Producer *ONSFactoryAPI::createProducer(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    return NULL;
}

OrderProducer *ONSFactoryAPI::createOrderProducer(
        ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    return NULL;
}

OrderConsumer *ONSFactoryAPI::createOrderConsumer(
        ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    return NULL;
}

TransactionProducer *ONSFactoryAPI::createTransactionProducer(
        ONSFactoryProperty factoryProperty, LocalTransactionChecker *checker) throw(ons::ONSClientException) {
    return NULL;
}

PushConsumer *ONSFactoryAPI::createPushConsumer(
        ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    return NULL;
}

PullConsumer *ONSFactoryAPI::createPullConsumer(
        ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    return NULL;
}
