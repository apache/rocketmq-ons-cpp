#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include <mutex>
#include <thread>
#include <condition_variable>

#include "OrderClientBaseTest.h"

class OrderProducerTest : public OrderClientBaseTest {
protected:
    void SetUp() override {
        OrderClientBaseTest::SetUp();
        producer_ = ons::ONSFactory::getInstance()->createOrderProducer(factoryInfo);
        producer_->start();
    }

    void TearDown() override {
        OrderClientBaseTest::TearDown();
        producer_->shutdown();
    }

    ons::OrderProducer *producer_;
};

TEST_F(OrderProducerTest, test_setUp) {
    ASSERT_TRUE(nullptr != producer_);
}

TEST_F(OrderProducerTest, testSendMessage_Normal) {
    ons::Message msg(
            "t_opensource_unit_test_order",
            "tagA",
            "ORDERID_100",
            "hello, RocketMQ."
    );
    ons::SendResultONS sendResult = producer_->send(msg, "1");
    ASSERT_TRUE(nullptr != sendResult.getMessageId());
}

TEST_F(OrderProducerTest, testSendMessage_TopicBeingEmpty) {
    ons::Message msg(
            "",
            "tagA",
            "ORDERID_100",
            "hello Order Message."
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg, "1"), ons::ONSClientException);
}

TEST_F(OrderProducerTest, testSendMessage_TopicNotExist) {
    ons::Message msg(
            "topic_not_use_test",
            "tagA",
            "ORDERID_100",
            "hello Order Message."
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg, "1"), ons::ONSClientException);
}

TEST_F(OrderProducerTest, testSendMessage_BodyBeingEmpty) {
    ons::Message msg(
            "t_opensource_unit_test_order",
            "tagA",
            "ORDERID_100",
            ""
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg, "1"), ons::ONSClientException);
}