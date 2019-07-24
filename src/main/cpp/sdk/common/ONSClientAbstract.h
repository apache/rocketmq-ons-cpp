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

#ifndef MQ_ONSCLIENTABSTRACT_H
#define MQ_ONSCLIENTABSTRACT_H

#include <string>
#include <sstream>

namespace ons {
    class FAQ {
    public:
        FAQ() {}

        virtual ~FAQ() {}

        static const std::string FIND_NS_FAILED;
        static const std::string CONNECT_BROKER_FAILED;
        static const std::string SEND_MSG_TO_BROKER_TIMEOUT;
        static const std::string SERVICE_STATE_WRONG;
        static const std::string BROKER_RESPONSE_EXCEPTION;
        static const std::string CLIENT_CHECK_MSG_EXCEPTION;
        static const std::string TOPIC_ROUTE_NOT_EXIST;

        static std::string errorMessage(const std::string msgTopic,
                                        const std::string url) {
            std::stringstream ss;
            ss << msgTopic << "\nSee " << url << " for further details.";
            return ss.str();
        }
    };

}


#endif //MQ_ONSCLIENTABSTRACT_H
