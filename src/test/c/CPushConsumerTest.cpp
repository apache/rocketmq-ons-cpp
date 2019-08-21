#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include <mutex>
#include <thread>
#include <condition_variable>

#include "CPushConsumer.h"

int onMessages(CPushConsumer *consumer, CMessageExt *message) {
    std::cout << "Receive Message:" << GetMessageId(message) << std::endl;
    return E_CONSUME_SUCCESS;
}

class CPushConsumerTest : public testing::Test {
protected:
    void SetUp() override {
        consumer_ = CreatePushConsumer("GID_opensource_unit_test");
        SetPushConsumerNameServerAddress(consumer_, "47.100.33.127:9876");
        //SetPushConsumerSessionCredentials(consumer_, "AK", "SK", "LOCAL");
        //StartPushConsumer(consumer_);
    }

    void TearDown() override {
        //ShutdownPushConsumer(consumer_);
        DestroyPushConsumer(consumer_);
        consumer_ = NULL;
    }

    CPushConsumer *consumer_;
};

TEST_F(CPushConsumerTest, test_setUp) {
    ASSERT_TRUE(nullptr != consumer_);
}

TEST_F(CPushConsumerTest, test_ConsumeMessage) {
    Subscribe(consumer_, "t_opensource_unit_test", "*");
    RegisterMessageCallback(consumer_,onMessages);
    StartPushConsumer(consumer_);
    std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));
    ShutdownPushConsumer(consumer_);
    ASSERT_TRUE(nullptr != consumer_);
}

