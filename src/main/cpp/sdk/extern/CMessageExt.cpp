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

#include "CMessageExt.h"
#include "CCommon.h"
#include "CConst.h"
#include "ONSFactory.h"
#include "../common/UtilAll.h"

#ifdef __cplusplus
extern "C" {
#endif
using namespace ons;
const char *GetMessageTopic(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL;
    }
    return ((Message *) msg)->getTopic();
}
const char *GetMessageTags(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL;
    }
    return ((Message *) msg)->getTag();
}
const char *GetMessageKeys(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL;
    }
    return ((Message *) msg)->getKey();
}
const char *GetMessageBody(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL;
    }
    return ((Message *) msg)->getBody();
}
const char *GetMessageProperty(CMessageExt *msg, const char *key) {
    if (msg == NULL) {
        return NULL;
    }
    return ((Message *) msg)->getUserProperties(key);
}
const char *GetMessageId(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL;
    }
    return ((Message *) msg)->getMsgID();
}

int GetMessageDelayTimeLevel(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return atoi(((Message *) msg)->getUserProperties(CAPI_PROPERTY_DELAY_TIME_LEVEL));
}

int GetMessageQueueId(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return UtilAll::str2i(((Message *) msg)->getUserProperties(CAPI_PROPERTY_REAL_QUEUE_ID));
}

int GetMessageReconsumeTimes(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return ((Message *) msg)->getReconsumeTimes();
}

int GetMessageStoreSize(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return 0;
}

long long GetMessageBornTimestamp(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return 0;
}

long long GetMessageStoreTimestamp(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return ((Message *) msg)->getStoreTimestamp();
}

long long GetMessageQueueOffset(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return ((Message *) msg)->getQueueOffset();
}

long long GetMessageCommitLogOffset(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return 0;
}

long long GetMessagePreparedTransactionOffset(CMessageExt *msg) {
    if (msg == NULL) {
        return NULL_POINTER;
    }
    return UtilAll::str2ll(((Message *) msg)->getUserProperties(CAPI_PROPERTY_TRANSACTION_PREPARED_QUEUE_OFFSET));
}
#ifdef __cplusplus
};
#endif
