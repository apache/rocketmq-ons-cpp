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

#include"ONSClientAbstract.h"
#include<string>

using namespace std;

namespace ons {
    const string FAQ::FIND_NS_FAILED = "https://github.com/alibaba/ons/issues/1";
    const string FAQ::CONNECT_BROKER_FAILED = "https://github.com/alibaba/ons/issues/2";
    const string FAQ::SEND_MSG_TO_BROKER_TIMEOUT = "https://github.com/alibaba/ons/issues/3";
    const string FAQ::SERVICE_STATE_WRONG = "https://github.com/alibaba/ons/issues/4";
    const string FAQ::BROKER_RESPONSE_EXCEPTION = "https://github.com/alibaba/ons/issues/5";
    const string FAQ::CLIENT_CHECK_MSG_EXCEPTION = "https://github.com/alibaba/ons/issues/6";
    const string FAQ::TOPIC_ROUTE_NOT_EXIST = "https://github.com/alibaba/ons/issues/7";
}