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

#include "CPullConsumer.h"
#include "CCommon.h"
#include "CMessageExt.h"
#include "ONSFactory.h"

using namespace ons;
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

CPullConsumer *CreatePullConsumer(const char *groupId) {
    if (groupId == NULL) {
        return NULL;
    }
    return NULL;
}
int DestroyPullConsumer(CPullConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}
int StartPullConsumer(CPullConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}
int ShutdownPullConsumer(CPullConsumer *consumer) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}
int SetPullConsumerGroupID(CPullConsumer *consumer, const char *groupId) {
    if (consumer == NULL || groupId == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}
const char *GetPullConsumerGroupID(CPullConsumer *consumer) {
    if (consumer == NULL) {
        return NULL;
    }
    return NULL;
}
int SetPullConsumerNameServerAddress(CPullConsumer *consumer, const char *namesrv) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}
int SetPullConsumerNameServerDomain(CPullConsumer *consumer, const char *domain) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}
int SetPullConsumerSessionCredentials(CPullConsumer *consumer,
                                      const char *accessKey,
                                      const char *secretKey,
                                      const char *channel) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int SetPullConsumerLogPath(CPullConsumer *consumer, const char *logPath) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    // Todo, This api should be implemented by core api.
    //((DefaultMQPullConsumer *) consumer)->setInstanceName(instanceName);
    return NOT_SUPPORT_NOW;
}

int SetPullConsumerLogFileNumAndSize(CPullConsumer *consumer, int fileNum, long fileSize) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int SetPullConsumerLogLevel(CPullConsumer *consumer, CLogLevel level) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

int FetchSubscriptionMessageQueues(CPullConsumer *consumer, const char *topic, CMessageQueue **mqs, int *size) {
    if (consumer == NULL) {
        return NULL_POINTER;
    }
    return PULLCONSUMER_FETCH_MQ_FAILED;
}
int ReleaseSubscriptionMessageQueue(CMessageQueue *mqs) {
    if (mqs == NULL) {
        return NULL_POINTER;
    }
    free((void *) mqs);
    mqs = NULL;
    return OK;
}
CPullResult Pull(CPullConsumer *consumer,
                 const CMessageQueue *mq,
                 const char *subExpression,
                 long long offset,
                 int maxNums) {
    CPullResult pullResult;
    memset(&pullResult, 0, sizeof(CPullResult));
    pullResult.pullStatus = E_NO_NEW_MSG;
    return pullResult;
}
int ReleasePullResult(CPullResult pullResult) {
    if (pullResult.size == 0 || pullResult.msgFoundList == NULL || pullResult.pData == NULL) {
        return NULL_POINTER;
    }
    return NOT_SUPPORT_NOW;
}

#ifdef __cplusplus
};
#endif
