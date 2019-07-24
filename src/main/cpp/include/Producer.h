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

#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include "Message.h"
#include "SendResultONS.h"
#include "ONSClientException.h"
#include "MessageQueueONS.h"
#include"ONSCallback.h"

namespace ons {

    class ONSCLIENT_API Producer {
    public:
        Producer() {}

        virtual ~Producer() {}

        // before send msg, start must be called to allocate resources.
        virtual void start() = 0;

        // before exit ons, shutdown must be called to release all resources allocated
        // by ons internally.
        virtual void shutdown() = 0;

        // retry max 3 times if send failed. if no exception throwed, it sends
        // success;
        virtual ons::SendResultONS send(Message &msg) throw(ons::ONSClientException) = 0;

        virtual ons::SendResultONS send(Message &msg,
                                        const MessageQueueONS &mq) throw(ons::ONSClientException) = 0;

        // async send
        virtual void sendAsync(Message &msg, ons::SendCallbackONS *callback) throw(ons::ONSClientException) = 0;

        // oneway send
        virtual void sendOneway(Message &msg) throw(ons::ONSClientException) = 0;
    };
}
#endif
