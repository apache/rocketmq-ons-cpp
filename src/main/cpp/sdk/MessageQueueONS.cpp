#include "MessageQueueONS.h"
#include "ONSClientException.h"
#include "UtilAll.h"

namespace ons {
    MessageQueueONS::MessageQueueONS() {
        m_queueId = -1;//invalide mq
        m_topic.clear();
        m_brokerName.clear();
    }

    MessageQueueONS::MessageQueueONS(const string &topic,
                                     const string &brokerName,
                                     int queueId) :
            m_topic(topic),
            m_brokerName(brokerName),
            m_queueId(queueId) {

    }

    MessageQueueONS::MessageQueueONS(const MessageQueueONS &other) :
            m_topic(other.m_topic),
            m_brokerName(other.m_brokerName),
            m_queueId(other.m_queueId) {
    }

    MessageQueueONS &MessageQueueONS::operator=(const MessageQueueONS &other) {
        if (this != &other) {
            m_brokerName = other.m_brokerName;
            m_topic = other.m_topic;
            m_queueId = other.m_queueId;
        }
        return *this;
    }

    string MessageQueueONS::getTopic() const {
        return m_topic;
    }

    void MessageQueueONS::setTopic(const string &topic) {
        m_topic = topic;
    }

    string MessageQueueONS::getBrokerName() const {
        return m_brokerName;
    }

    void MessageQueueONS::setBrokerName(const string &brokerName) {
        m_brokerName = brokerName;
    }

    int MessageQueueONS::getQueueId() const {
        return m_queueId;
    }

    void MessageQueueONS::setQueueId(int queueId) {
        m_queueId = queueId;
    }

    bool MessageQueueONS::operator==(const MessageQueueONS &mq) const {
        if (this == &mq) {
            return true;
        }

        if (m_brokerName != mq.m_brokerName) {
            return false;
        }

        if (m_queueId != mq.m_queueId) {
            return false;
        }

        if (m_topic != mq.m_topic) {
            return false;
        }

        return true;
    }

    int MessageQueueONS::compareTo(const MessageQueueONS &mq) const {
        int result = m_topic.compare(mq.m_topic);
        if (result != 0) {
            return result;
        }

        result = m_brokerName.compare(mq.m_brokerName);
        if (result != 0) {
            return result;
        }

        return m_queueId - mq.m_queueId;
    }

    bool MessageQueueONS::operator<(const MessageQueueONS &mq) const {
        return compareTo(mq) < 0;
    }

//<!***************************************************************************
} //<!end namespace;

