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

#ifndef __TRANSACTIONPRODUCER_H__
#define __TRANSACTIONPRODUCER_H__

#include "LocalTransactionExecuter.h"
#include "ONSClient.h"
#include "SendResultONS.h"

namespace ons {
    class ONSCLIENT_API TransactionProducer {
    public:
        TransactionProducer() {}

        virtual ~TransactionProducer() {}

        // before send msg, start must be called to allocate resources.
        virtual void start() = 0;

        // before exit ons, shutdown must be called to release all resources allocated
        // by ons internally.
        virtual void shutdown() = 0;

        // retry max 3 times if send failed. if no exception throwed, it sends
        // success;
        virtual SendResultONS send(Message &msg,
                                   LocalTransactionExecuter *executer) = 0;
    };
}

#endif
