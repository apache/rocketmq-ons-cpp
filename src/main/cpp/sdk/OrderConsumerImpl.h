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

#ifndef MQ_ORDER_CONSUMER_IMPL_H
#define MQ_ORDER_CONSUMER_IMPL_H

#include "graal_isolate.h"

#include "OrderConsumer.h"
#include "MessageOrderListener.h"
#include "ONSFactory.h"
#include "ClientBase.h"

namespace ons {
    class OrderConsumerImpl : public ons::OrderConsumer, ClientBase {
    public:
        OrderConsumerImpl(const ons::ONSFactoryProperty &property) throw(ons::ONSClientException);

        virtual ~OrderConsumerImpl();

        void start();

        void shutdown();

        void subscribe(const char *topic, const char *subExpression,
                       ons::MessageOrderListener *listener);
    };
}

#endif //MQ_ORDER_CONSUMER_IMPL_H
