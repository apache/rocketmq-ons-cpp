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

#include "CProducer.h"
#include <string.h>
#include <typeindex>
#include <string.h>
#include <typeinfo>
#include "CBatchMessage.h"
#include "CCommon.h"
#include "CMQException.h"
#include "CMessage.h"
#include "CSendResult.h"
#include "ONSFactory.h"

#ifdef __cplusplus
extern "C" {
#endif
using namespace ons;
using namespace std;

class CSendCallback : public SendCallbackONS {
public:
    CSendCallback(CSendSuccessCallback cSendSuccessCallback, CSendExceptionCallback cSendExceptionCallback) {
        m_cSendSuccessCallback = cSendSuccessCallback;
        m_cSendExceptionCallback = cSendExceptionCallback;
    }

    virtual ~CSendCallback() {}

    virtual void onSuccess(SendResultONS &sendResult) {
        CSendResult result;
        result.sendStatus = E_SEND_OK;
        result.offset = 0;
        strncpy(result.msgId, sendResult.getMessageId(), MAX_MESSAGE_ID_LENGTH - 1);
        result.msgId[MAX_MESSAGE_ID_LENGTH - 1] = 0;
        if (m_cSendSuccessCallback != NULL) {
            m_cSendSuccessCallback(result);
        }
    }

    virtual void onException(ONSClientException &e) {
        CMQException exception;
        exception.error = e.GetError();
        exception.line = e.GetError();
        strncpy(exception.msg, e.GetMsg(), MAX_EXEPTION_MSG_LENGTH - 1);
        strncpy(exception.file, e.what(), MAX_EXEPTION_FILE_LENGTH - 1);
        if (m_cSendExceptionCallback != NULL) {
            m_cSendExceptionCallback(exception);
        }
    }

public:
    void setM_cSendSuccessCallback(CSendSuccessCallback callback) {
        m_cSendSuccessCallback = callback;
    }

    void setM_cSendExceptionCallback(CSendExceptionCallback callback) {
        m_cSendExceptionCallback = callback;
    }

private:
    CSendSuccessCallback m_cSendSuccessCallback;
    CSendExceptionCallback m_cSendExceptionCallback;
};

typedef struct __DefaultProducer__ {
    ONSFactoryProperty factoryInfo;
    Producer *innerProducer;
    CSendCallback *cSendCallback;
} DefaultProducer;

CProducer *CreateProducer(const char *groupId) {
    if (groupId == NULL) {
        return NULL;
    }
    DefaultProducer *defaultMQProducer = new DefaultProducer();
    defaultMQProducer->factoryInfo.setFactoryProperty(ONSFactoryProperty::GroupId, groupId);
    defaultMQProducer->factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "AK");
    defaultMQProducer->factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, "SK");
    defaultMQProducer->factoryInfo.setOnsChannel(ONSChannel::LOCAL);
    defaultMQProducer->cSendCallback = new CSendCallback(NULL, NULL);
    return (CProducer *) defaultMQProducer;
}
int DestroyProducer(CProducer *pProducer) {
    if (pProducer == NULL) {
        return NULL_POINTER;
    }
    DefaultProducer *defaultProducer = (DefaultProducer *) pProducer;
    if (defaultProducer->cSendCallback != NULL) {
        delete defaultProducer->cSendCallback;
    }
    delete reinterpret_cast<DefaultProducer *>(pProducer);
    return OK;
}
int StartProducer(CProducer *producer) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    DefaultProducer *defaultProducer = (DefaultProducer *) producer;
    try {
        defaultProducer->innerProducer = ONSFactory::getInstance()->createProducer(defaultProducer->factoryInfo);
        defaultProducer->innerProducer->start();
    } catch (exception &e) {
        return PRODUCER_START_FAILED;
    }
    return OK;
}
int ShutdownProducer(CProducer *producer) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->innerProducer->shutdown();
    return OK;
}
int SetProducerNameServerAddress(CProducer *producer, const char *namesrv) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::NAMESRV_ADDR, namesrv);
    return OK;
}
int SetProducerNameServerDomain(CProducer *producer, const char *domain) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::ONSAddr, domain);
    return OK;
}
int SendMessageSync(CProducer *producer, CMessage *msg, CSendResult *result) {
    // CSendResult sendResult;
    if (producer == NULL || msg == NULL || result == NULL) {
        return NULL_POINTER;
    }
    try {
        DefaultProducer *defaultProducer = (DefaultProducer *) producer;
        if (defaultProducer->innerProducer == NULL){
            return  NULL_POINTER;
        }
        Message *message = (Message *) msg;
        SendResultONS sendResult = defaultProducer->innerProducer->send(*message);
        result->sendStatus = E_SEND_OK;
        result->offset = 0;
        strncpy(result->msgId, sendResult.getMessageId(), MAX_MESSAGE_ID_LENGTH - 1);
        result->msgId[MAX_MESSAGE_ID_LENGTH - 1] = 0;
    } catch (exception &e) {
        return PRODUCER_SEND_SYNC_FAILED;
    }
    return OK;
}

int SendBatchMessage(CProducer *producer, CBatchMessage *batcMsg, CSendResult *result) {
    // CSendResult sendResult;
    if (producer == NULL || batcMsg == NULL || result == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int SendMessageAsync(CProducer *producer,
                     CMessage *msg,
                     CSendSuccessCallback cSendSuccessCallback,
                     CSendExceptionCallback cSendExceptionCallback) {
    if (producer == NULL || msg == NULL || cSendSuccessCallback == NULL || cSendExceptionCallback == NULL) {
        return NULL_POINTER;
    }
    DefaultProducer *defaultProducer = (DefaultProducer *) producer;
    if (defaultProducer->innerProducer == NULL){
        return  NULL_POINTER;
    }
    Message *message = (Message *) msg;
    SendResultONS sendResult = defaultProducer->innerProducer->send(*message);

    try {
        defaultProducer->cSendCallback->setM_cSendSuccessCallback(cSendSuccessCallback);
        defaultProducer->cSendCallback->setM_cSendExceptionCallback(cSendExceptionCallback);
        defaultProducer->innerProducer->sendAsync(*message, defaultProducer->cSendCallback);
    } catch (exception &e) {
        if (defaultProducer->cSendCallback != NULL) {
            if (std::type_index(typeid(e)) == std::type_index(typeid(ONSClientException))) {
                ONSClientException &mqe = (ONSClientException &) e;
                defaultProducer->cSendCallback->onException(mqe);
            }
        }
        return PRODUCER_SEND_ASYNC_FAILED;
    }
    return OK;
}

int SendMessageOneway(CProducer *producer, CMessage *msg) {
    if (producer == NULL || msg == NULL) {
        return NULL_POINTER;
    }
    DefaultProducer *defaultProducer = (DefaultProducer *) producer;
    if (defaultProducer->innerProducer == NULL){
        return  NULL_POINTER;
    }
    Message *message = (Message *) msg;
    try {
        defaultProducer->innerProducer->sendOneway(*message);
    } catch (exception &e) {
        return PRODUCER_SEND_ONEWAY_FAILED;
    }
    return OK;
}

int SendMessageOnewayOrderly(CProducer *producer, CMessage *msg, QueueSelectorCallback selector, void *arg) {
    if (producer == NULL || msg == NULL) {
        return NULL_POINTER;
    }
    DefaultProducer *defaultProducer = (DefaultProducer *) producer;
    if (defaultProducer->innerProducer == NULL){
        return  NULL_POINTER;
    }
    Message *message = (Message *) msg;
    try {
        //defaultProducer->innerProducer->sendOneway(*message);
    } catch (exception &e) {
        return PRODUCER_SEND_ONEWAY_FAILED;
    }
    return NOT_SUPPORT_NOW;
}

int SendMessageOrderlyAsync(CProducer *producer,
                            CMessage *msg,
                            QueueSelectorCallback callback,
                            void *arg,
                            CSendSuccessCallback cSendSuccessCallback,
                            CSendExceptionCallback cSendExceptionCallback) {
    if (producer == NULL || msg == NULL || callback == NULL || cSendSuccessCallback == NULL ||
        cSendExceptionCallback == NULL) {
        return NULL_POINTER;
    }

    return NOT_SUPPORT_NOW;
}

int SendMessageOrderly(CProducer *producer,
                       CMessage *msg,
                       QueueSelectorCallback callback,
                       void *arg,
                       int autoRetryTimes,
                       CSendResult *result) {
    if (producer == NULL || msg == NULL || callback == NULL || arg == NULL || result == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int SetProducerGroupName(CProducer *producer, const char *groupName) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::GroupId, groupName);
    return OK;
}
int SetProducerInstanceName(CProducer *producer, const char *instanceName) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::ConsumeThreadNums, instanceName);
    return OK;
}
int SetProducerSessionCredentials(CProducer *producer,
                                  const char *accessKey,
                                  const char *secretKey,
                                  const char *onsChannel) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, accessKey);
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, secretKey);
    if (onsChannel != NULL){
        ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::OnsChannel, onsChannel);
    }
    return OK;
}
int SetProducerLogPath(CProducer *producer, const char *logPath) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::LogPath, logPath);
    return OK;
}

int SetProducerLogFileNumAndSize(CProducer *producer, int fileNum, long fileSize) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    return OK;
}

int SetProducerLogLevel(CProducer *producer, CLogLevel level) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    //((DefaultProducer *) producer)->factoryInfo.setFactoryProperty(ONSFactoryProperty::LogPath, level);
    return OK;
}

int SetProducerSendMsgTimeout(CProducer *producer, int timeout) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    ((DefaultProducer *) producer)->factoryInfo.setSendMsgTimeout(timeout);
    return OK;
}

int SetProducerCompressMsgBodyOverHowmuch(CProducer *producer, int howmuch) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    return OK;
}

int SetProducerCompressLevel(CProducer *producer, int level) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    return OK;
}

int SetProducerMaxMessageSize(CProducer *producer, int size) {
    if (producer == NULL) {
        return NULL_POINTER;
    }
    return OK;
}
#ifdef __cplusplus
};
#endif
