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

#include "CPushConsumer.h"
#include <map>
#include "CCommon.h"
#include "CMessageExt.h"
#include "ONSFactory.h"
#include "../common/UtilAll.h"

using namespace ons;
using namespace std;

class MessageListenerInner : public MessageListener {
public:
    MessageListenerInner() {}

    MessageListenerInner(CPushConsumer *consumer, MessageCallBack pCallback) {
        m_pconsumer = consumer;
        m_pMsgReceiveCallback = pCallback;
    }

    ~MessageListenerInner() {}

    Action consume(Message &message, ConsumeContext &context) {
        if (m_pMsgReceiveCallback == NULL) {
            return Action::ReconsumeLater;
        }
        CMessageExt *msg = (CMessageExt *) (&message);
        if (m_pMsgReceiveCallback(m_pconsumer, msg) != E_CONSUME_SUCCESS) {
            return Action::ReconsumeLater;
        }
        return Action::CommitMessage;
    }

private:
    MessageCallBack m_pMsgReceiveCallback;
    CPushConsumer *m_pconsumer;
};

map<CPushConsumer *, MessageListenerInner *> g_ListenerMap;

#ifdef __cplusplus
extern "C" {
#endif
#ifndef CAPI_MAX_SUB_EXPRESS_LEN
#define CAPI_MAX_SUB_EXPRESS_LEN 512
#endif

typedef struct __DefaultPushConsumer__ {
    ONSFactoryProperty factoryInfo;
    PushConsumer *innerConsumer;
    char expression[CAPI_MAX_SUB_EXPRESS_LEN];
} DefaultPushConsumer;

CPushConsumer *CreatePushConsumer(const char *groupId) {
    if (groupId == NULL) {
        return NULL;
    }
    DefaultPushConsumer *defaultPushConsumer = new DefaultPushConsumer();
    defaultPushConsumer->factoryInfo.setFactoryProperty(ONSFactoryProperty::GroupId, groupId);
    //defaultPushConsumer->factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "AK");
    //defaultPushConsumer->factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, "SK");
    defaultPushConsumer->factoryInfo.setOnsChannel(ONSChannel::LOCAL);
    memset(defaultPushConsumer->expression, 0, CAPI_MAX_SUB_EXPRESS_LEN);
    return (CPushConsumer *) defaultPushConsumer;
}
int DestroyPushConsumer(CPushConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    delete reinterpret_cast<DefaultPushConsumer *>(consumer);
    return OK;
}
int StartPushConsumer(CPushConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    DefaultPushConsumer *defaultPushConsumer = (DefaultPushConsumer *) consumer;
    try {
        defaultPushConsumer->innerConsumer = ONSFactory::getInstance()->createPushConsumer(
                defaultPushConsumer->factoryInfo);
        defaultPushConsumer->innerConsumer->subscribe(defaultPushConsumer->factoryInfo.getPublishTopics(),
                                                      defaultPushConsumer->expression, g_ListenerMap[consumer]);
        defaultPushConsumer->innerConsumer->start();
    } catch (exception &e) {
        return PULLCONSUMER_START_FAILED;
    }
    return OK;
}
int ShutdownPushConsumer(CPushConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    DefaultPushConsumer *defaultPushConsumer = (DefaultPushConsumer *) consumer;
    defaultPushConsumer->innerConsumer->shutdown();
    return OK;
}
int SetPushConsumerGroupID(CPushConsumer *consumer, const char *groupId) {
    if (consumer == NULL || groupId == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::GroupId, groupId);
    return OK;
}
const char *GetPushConsumerGroupID(CPushConsumer *consumer) {
    if (consumer == NULL) {
        return NULL;
    }
    return ((DefaultPushConsumer *) consumer)->factoryInfo.getGroupId();
}
int SetPushConsumerNameServerAddress(CPushConsumer *consumer, const char *namesrv) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::NAMESRV_ADDR, namesrv);
    return OK;
}
int SetPushConsumerNameServerDomain(CPushConsumer *consumer, const char *domain) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::ONSAddr, domain);
    return OK;
}
int Subscribe(CPushConsumer *consumer, const char *topic, const char *expression) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics, topic);
    memset(((DefaultPushConsumer *) consumer)->expression, 0, CAPI_MAX_SUB_EXPRESS_LEN);
    strncpy(((DefaultPushConsumer *) consumer)->expression, expression, CAPI_MAX_SUB_EXPRESS_LEN - 1);
    return OK;
}

int RegisterMessageCallback(CPushConsumer *consumer, MessageCallBack pCallback) {
    if (consumer == NULL || pCallback == NULL) {
        return NULL_POINTER;
    }

    DefaultPushConsumer *defaultPushConsumer = (DefaultPushConsumer *) consumer;
    MessageListenerInner *listenerInner = new MessageListenerInner(consumer, pCallback);
    g_ListenerMap[consumer] = listenerInner;
    return OK;
}

int RegisterMessageCallbackOrderly(CPushConsumer *consumer, MessageCallBack pCallback) {
    if (consumer == NULL || pCallback == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int UnregisterMessageCallbackOrderly(CPushConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int UnregisterMessageCallback(CPushConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    map<CPushConsumer *, MessageListenerInner *>::iterator iter;
    iter = g_ListenerMap.find(consumer);

    if (iter != g_ListenerMap.end()) {
        MessageListenerInner *listenerInner = iter->second;
        if (listenerInner != NULL) {
            delete listenerInner;
        }
        g_ListenerMap.erase(iter);
    }
    return OK;
}

int SetPushConsumerMessageModel(CPushConsumer *consumer, CMessageModel messageModel) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }

    DefaultPushConsumer *defaultPushConsumer = (DefaultPushConsumer *) consumer;
    switch (messageModel) {
        case BROADCASTING:
            defaultPushConsumer->factoryInfo.setFactoryProperty(ONSFactoryProperty::MessageModel,
                                                                ONSFactoryProperty::BROADCASTING);
            break;
        case CLUSTERING:
            defaultPushConsumer->factoryInfo.setFactoryProperty(ONSFactoryProperty::MessageModel,
                                                                ONSFactoryProperty::CLUSTERING);
            break;
        default:
            defaultPushConsumer->factoryInfo.setFactoryProperty(ONSFactoryProperty::MessageModel,
                                                                ONSFactoryProperty::CLUSTERING);
            break;
    }
    return OK;
}
int SetPushConsumerThreadCount(CPushConsumer *consumer, int threadCount) {
    if (consumer == NULL || threadCount == 0) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumeThreadNums,
                                                                       UtilAll::to_string(threadCount).c_str());
    return OK;
}
int SetPushConsumerMessageBatchMaxSize(CPushConsumer *consumer, int batchSize) {
    if (consumer == NULL || batchSize == 0) {
        return NULL_POINTER;
    }
    return OK;
}

int SetPushConsumerInstanceName(CPushConsumer *consumer, const char *instanceName) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumerInstanceName,
                                                                       instanceName);
    return OK;
}

int SetPushConsumerSessionCredentials(CPushConsumer *consumer,
                                      const char *accessKey,
                                      const char *secretKey,
                                      const char *channel) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, accessKey);
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, secretKey);
    return OK;
}

int SetPushConsumerLogPath(CPushConsumer *consumer, const char *logPath) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultPushConsumer *) consumer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::LogPath, logPath);
    return OK;
}

int SetPushConsumerLogFileNumAndSize(CPushConsumer *consumer, int fileNum, long fileSize) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return OK;
}

int SetPushConsumerLogLevel(CPushConsumer *consumer, CLogLevel level) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return OK;
}

#ifdef __cplusplus
};
#endif
