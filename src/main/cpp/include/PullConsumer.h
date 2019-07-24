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

#ifndef __PULLCONSUMER_H__
#define __PULLCONSUMER_H__

#include <string>
#include <vector>
#include "MessageQueueONS.h"
#include "PullResultONS.h"

namespace ons {

    class ONSFactoryProperty;

    class ONSCLIENT_API PullConsumer {
    public:
        PullConsumer() {}

        virtual ~PullConsumer() {}

        virtual void start() = 0;

        virtual void shutdown() = 0;

        virtual void fetchSubscribeMessageQueues(
                const std::string &topic, std::vector<MessageQueueONS> &mqs) = 0;

        virtual PullResultONS pull(const MessageQueueONS &mq,
                                   const std::string &subExpression, long long offset,
                                   int maxNums) = 0;

        virtual long long searchOffset(const MessageQueueONS &mq, long long timestamp) = 0;

        virtual long long maxOffset(const MessageQueueONS &mq) = 0;

        virtual long long minOffset(const MessageQueueONS &mq) = 0;

        virtual void updateConsumeOffset(const MessageQueueONS &mq,
                                         long long offset) = 0;

        virtual void removeConsumeOffset(const MessageQueueONS &mq) = 0;

        virtual long long fetchConsumeOffset(const MessageQueueONS &mq,
                                             bool fromStore) = 0;

        virtual void persistConsumerOffset4PullConsumer(const MessageQueueONS &mq)
        throw(ons::ONSClientException) = 0;
    };

}
#endif
