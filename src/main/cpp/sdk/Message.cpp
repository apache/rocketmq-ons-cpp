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

#include "Message.h"
#include "ONSClientException.h"
#include "common/UtilAll.h"

namespace ons {

    const char *SystemPropKey::TAG = "__TAG";
    const char *SystemPropKey::KEY = "__KEY";
    const char *SystemPropKey::MSGID = "__MSGID";
    const char *SystemPropKey::RECONSUMETIMES = "__RECONSUMETIMES";
    const char *SystemPropKey::STARTDELIVERTIME = "__STARTDELIVERTIME";
    const char *EMPTY_STR = "";

    Message::Message() { Init(EMPTY_STR, EMPTY_STR, EMPTY_STR, EMPTY_STR); }

    Message::Message(const std::string &topic, const std::string &tags, const std::string &byte_body) {
        Init(topic, tags, EMPTY_STR, byte_body);
    }

    Message::Message(const char *topic, const char *tags, const char *byte_body) {
        if (topic == NULL || byte_body == NULL) {
            std::string msg = "Topic or Body cannot be NULL. Please check your message set.";
            throw ONSClientException(msg, -1);
        }
        Init(topic, EMPTY_STR, EMPTY_STR, byte_body);
        if (tags != NULL) {
            setTag(tags);
        }
    }

    Message::Message(const char *topic, size_t topic_size, const char *tags, size_t tags_size,
                     const char *body, size_t body_size) {
        if (topic == NULL || topic_size == 0 || body == NULL || body_size == 0) {
            std::string msg = "Topic or Body cannot be NULL. Please check your message set.";
            throw ONSClientException(msg, -1);
        }
        std::string topic_(topic, topic_size);
        std::string body_(body, body_size);
        Init(topic_, EMPTY_STR, EMPTY_STR, body_);
        if (tags != NULL && tags_size > 0) {
            std::string tags_(tags, tags_size);
            setTag(tags_.c_str());
        }
    }

    Message::Message(const char *topic, const char *tags, const char *keys, const char *body) {
        if (topic == NULL || body == NULL) {
            std::string msg = "Topic or Body cannot be NULL. Please check your message set.";
            throw ONSClientException(msg, -1);
        }
        Init(topic, EMPTY_STR, EMPTY_STR, body);
        if (tags != NULL) {
            setTag(tags);
        }
        if (keys != NULL) {
            setKey(keys);
        }
    }

    Message::~Message() {
        systemProperties.clear();
        userProperties.clear();
    }

    Message::Message(const Message &other) {
        body = other.body;
        topic = other.topic;
        m_storeTimestamp = other.m_storeTimestamp;
        m_queueOffset = other.m_queueOffset;
        systemProperties = other.systemProperties;
        userProperties = other.userProperties;
    }

    Message &Message::operator=(const Message &other) {
        if (this != &other) {
            body = other.body;
            topic = other.topic;
            m_storeTimestamp = other.m_storeTimestamp;
            m_queueOffset = other.m_queueOffset;
            systemProperties = other.systemProperties;
            userProperties = other.userProperties;
        }
        return *this;
    }

    void Message::putSystemProperties(const char *key, const char *value) {
        if (key == NULL || value == NULL) {
            std::string msg = "Key or Value cannot be NULL in SystemProperties! Please check your message set.";
            throw ONSClientException(msg, -1);
        }
        systemProperties[UtilAll::to_string(key)] = UtilAll::to_string(value);
    }

    const char *Message::getSystemProperties(const char *key) const {
        if (key == NULL) {
            return NULL;
        }
        std::map<std::string, std::string>::const_iterator it =
                systemProperties.find(UtilAll::to_string(key));
        return (it == systemProperties.end()) ? EMPTY_STR : (*it).second.c_str();
    }

    void Message::setSystemProperties(std::map<std::string, std::string> &systemProperty) {
        systemProperties = systemProperty;
    }

    std::map<std::string, std::string> Message::getSystemProperties() const {
        return systemProperties;
    }

    void Message::putUserProperties(const char *key, const char *value) {
        if (key == NULL || value == NULL) {
            std::string msg = "Key or Value cannot be NULL in UserProperties! Please check your message set.";
            throw ONSClientException(msg, -1);
        }
        userProperties[UtilAll::to_string(key)] = UtilAll::to_string(value);
    }

    const char *Message::getUserProperties(const char *key) const {
        if (key == NULL) {
            return NULL;
        }
        std::map<std::string, std::string>::const_iterator it =
                userProperties.find(UtilAll::to_string(key));
        return (it == userProperties.end()) ? EMPTY_STR : (*it).second.c_str();
    }

    void Message::setUserProperties(std::map<std::string, std::string> &userProperty) {
        userProperties = userProperty;
    }

    std::map<std::string, std::string> Message::getUserProperties() const {
        return userProperties;
    }

    const char *Message::getTopic() const {
        return topic.c_str();
    }

    void Message::setTopic(const char *msgtopic) {
        if (msgtopic == NULL) {
            topic = UtilAll::to_string(EMPTY_STR);
            return;
        }
        topic = UtilAll::to_string(msgtopic);
    }

    const char *Message::getTag() const {
        return getSystemProperties(SystemPropKey::TAG);
    }

    void Message::setTag(const char *tags) {
        if (tags == NULL) {
            putSystemProperties(SystemPropKey::TAG, EMPTY_STR);
            return;
        }
        putSystemProperties(SystemPropKey::TAG, tags);
    }

    const char *Message::getMsgID() const {
        return getSystemProperties(SystemPropKey::MSGID);
    }

    void Message::setMsgID(const char *msgId) {
        if (msgId == NULL) {
            putSystemProperties(SystemPropKey::MSGID, EMPTY_STR);
            return;
        }
        putSystemProperties(SystemPropKey::MSGID, msgId);
    }

    const char *Message::getKey() const {
        return getSystemProperties(SystemPropKey::KEY);
    }

    void Message::setKey(const char *keys) {
        if (keys == NULL) {
            putSystemProperties(SystemPropKey::KEY, EMPTY_STR);
            return;
        }
        putSystemProperties(SystemPropKey::KEY, keys);
    }

    const long long Message::getStartDeliverTime() const {
        const char *tmp = getSystemProperties(SystemPropKey::STARTDELIVERTIME);
        if (tmp && tmp[0] != '\0') {
            return UtilAll::str2ll(tmp);
        }
        return 0;
    }

    void Message::setStartDeliverTime(long long level) {
        putSystemProperties(SystemPropKey::STARTDELIVERTIME,
                            UtilAll::to_string(level).c_str());
    }

    const char *Message::getBody() const {
        return body.c_str();
    }

    const char *Message::getByteBody(int *len) const {
        if (len == NULL) {
            return NULL;
        }
        *len = body.length();
        return body.c_str();
    }

    void Message::setBody(unsigned char *msgbody, int len) {
        body.clear();
        if (msgbody == NULL || len == 0) {
            body = std::string(EMPTY_STR);
            return;
        }
        body = std::string(reinterpret_cast<char *>(msgbody), len);
    }

    void Message::setMsgBody(const std::string msgbody) {
        body = msgbody;
    }

    const std::string Message::getMsgBody() const {
        return body;
    }

    const size_t Message::getBodySize() const {
        return body.length();
    }

    const int Message::getReconsumeTimes() const {
        const char *reconsumeTime =
                getSystemProperties(SystemPropKey::RECONSUMETIMES);
        if (reconsumeTime && reconsumeTime[0] != '\0') return atoi(reconsumeTime);

        return 0;
    }

    void Message::setReconsumeTimes(int reconsumeTimes) {
        putSystemProperties(SystemPropKey::RECONSUMETIMES,
                            UtilAll::to_string(reconsumeTimes).c_str());
    }

    long long Message::getStoreTimestamp() const { return m_storeTimestamp; }

    void Message::setStoreTimestamp(long long storeTimestamp) {
        m_storeTimestamp = storeTimestamp;
    }

    long long Message::getQueueOffset() const {
        return m_queueOffset;;
    }

    void Message::setQueueOffset(long long queueOffset) {
        m_queueOffset = queueOffset;
    }

    const std::string Message::toString() const {
        std::stringstream ss;
        ss << "Message [topic=" << topic << ", body=" << body << "]";
        return ss.str();
    }

    const std::string Message::toSystemString() const {
        std::stringstream ss;
        std::map<std::string, std::string>::const_iterator it = systemProperties.begin();
        ss << "Message [";
        for (; it != systemProperties.end(); ++it) {
            ss << it->first << ": " << it->second << ", ";
        }
        ss << "]";
        return ss.str();
    }

    const std::string Message::toUserString() const {
        std::stringstream ss;
        std::map<std::string, std::string>::const_iterator it = userProperties.begin();
        ss << "Message [";
        for (; it != userProperties.end(); ++it) {
            ss << it->first << ": " << it->second << ", ";
        }
        ss << "]";
        return ss.str();
    }

    void Message::Init(const std::string &msgtopic, const std::string &msgtags,
                       const std::string &msgkeys, const std::string &msgbody) {
        topic = msgtopic;
        body = msgbody;
        if (msgtags.length() > 0) {
            setTag(msgtags.c_str());
        }

        if (msgkeys.length() > 0) {
            setKey(msgkeys.c_str());
        }
    }
}
