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

#ifndef __MESSAGEQUEUEONS_H__
#define __MESSAGEQUEUEONS_H__

#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

namespace ons {
    class MessageQueueONS {
    public:
        MessageQueueONS();

        MessageQueueONS(const string &topic, const string &brokerName, int queueId);

        MessageQueueONS(const MessageQueueONS &other);

        MessageQueueONS &operator=(const MessageQueueONS &other);

        string getTopic() const;

        void setTopic(const string &topic);

        string getBrokerName() const;

        void setBrokerName(const string &brokerName);

        int getQueueId() const;

        void setQueueId(int queueId);

        bool operator==(const MessageQueueONS &mq) const;

        bool operator<(const MessageQueueONS &mq) const;

        int compareTo(const MessageQueueONS &mq) const;

    private:
        string m_topic;
        string m_brokerName;
        int m_queueId;
    };

}  //<!end namespace;
#endif
