#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include <mutex>
#include <thread>
#include <condition_variable>

#include "CProducer.h"

class CProducerTest : public testing::Test {
protected:
    void SetUp() override {
        producer_ = CreateProducer("GID_opensource_unit_test");
        SetProducerNameServerAddress(producer_, "47.100.33.127:9876");
        SetProducerSessionCredentials(producer_, "AK", "SK", "LOCAL");
        StartProducer(producer_);
    }

    void TearDown() override {
        ShutdownProducer(producer_);
        DestroyProducer(producer_);
        producer_ = NULL;
    }

    CProducer *producer_;
};

TEST_F(CProducerTest, test_setUp) {
    ASSERT_TRUE(nullptr != producer_);
}

TEST_F(CProducerTest, testSendMessage_Normal) {
    CMessage *message = CreateMessage("t_opensource_unit_test");
    SetMessageKeys(message, "This is Key");
    SetMessageTags(message, "TAG");
    SetMessageBody(message, "This is Message Body.");
    CSendResult sendResult;
    SendMessageSync(producer_, message, &sendResult);
    std::cout<<"Send OK,MessageID:"<< sendResult.msgId<<std::endl;
    ASSERT_TRUE(E_SEND_OK == sendResult.sendStatus);
}