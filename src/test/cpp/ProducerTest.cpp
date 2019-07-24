#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include <mutex>
#include <thread>
#include <condition_variable>

#include "ClientBaseTest.h"

class ProducerTest : public ClientBaseTest {
protected:
    void SetUp() override {
        ClientBaseTest::SetUp();
        producer_ = ons::ONSFactory::getInstance()->createProducer(factoryInfo);
        producer_->start();
    }

    void TearDown() override {
        ClientBaseTest::TearDown();
        producer_->shutdown();
    }

    ons::Producer *producer_;
};

TEST_F(ProducerTest, test_setUp) {
    ASSERT_TRUE(nullptr != producer_);
}

TEST_F(ProducerTest, testSendMessage_Normal) {
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    ons::SendResultONS sendResult = producer_->send(msg);
    ASSERT_TRUE(nullptr != sendResult.getMessageId());
}

TEST_F(ProducerTest, testSendMessage_TopicBeingEmpty) {
    ons::Message msg(
            "",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg), ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessage_TopicBeingNULL) {
    EXPECT_THROW(
            ons::Message msg(NULL, "tagA", "ORDERID_100", "hello RocketMQ.");,
            ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessage_TopicNotExist) {
    ons::Message msg(
            "topic_not_use_test",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg), ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessage_BodyBeingEmpty) {
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            ""
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg), ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessageOneway_Normal) {
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    EXPECT_NO_THROW(producer_->sendOneway(msg));
}

TEST_F(ProducerTest, testSendMessageOneway_TopicBeingEmpty) {
    ons::Message msg(
            "",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    EXPECT_THROW(producer_->sendOneway(msg), ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessageOneway_TopicNotExist) {
    ons::Message msg(
            "topic_not_use_test",
            "tagA",
            "ORDERID_100",
            "hello MQ_lingchu"
    );
    EXPECT_THROW(producer_->sendOneway(msg), ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessageOneway_BodyBeingEmpty) {
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            ""
    );
    EXPECT_THROW(producer_->sendOneway(msg), ons::ONSClientException);
}

class ExampleSendCallback : public ons::SendCallbackONS {
public:
    ExampleSendCallback(std::mutex &mtx, bool &complete, std::condition_variable &cv, bool &success)
            : mtx_(mtx), complete_(complete), cv_(cv), success_(success) {
    }

    void onSuccess(ons::SendResultONS &sendResult) override {
        success_ = true;
        {
            std::unique_lock<std::mutex> lk(mtx_);
            complete_ = true;
        }
        cv_.notify_all();
    };

    virtual void onException(ons::ONSClientException &e) override {
        {
            std::unique_lock<std::mutex> lk(mtx_);
            complete_ = true;
        }
        cv_.notify_all();
    };

    std::mutex &mtx_;
    bool &complete_;
    std::condition_variable &cv_;
    bool &success_;
};

TEST_F(ProducerTest, testSendMessageAsync_Normal) {
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    std::mutex mtx;
    std::condition_variable cv;
    bool complete = false;
    bool success = false;
    ExampleSendCallback cb(mtx, complete, cv, success);
    producer_->sendAsync(msg, &cb);

    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]() { return complete; });
    }

    ASSERT_TRUE(success);
}

TEST_F(ProducerTest, testSendMessageAsync_TopicBeingEmpty) {
    ons::Message msg(
            "",
            "tagA",
            "ORDERID_100",
            "hello RocketMQ."
    );
    std::mutex mtx;
    std::condition_variable cv;
    bool complete = false;
    bool success = false;
    ExampleSendCallback cb(mtx, complete, cv, success);
    EXPECT_THROW(producer_->sendAsync(msg, &cb);, ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessageAsync_BodyBeingEmpty) {
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            ""
    );
    std::mutex mtx;
    std::condition_variable cv;
    bool complete = false;
    bool success = false;
    ExampleSendCallback cb(mtx, complete, cv, success);
    EXPECT_THROW(producer_->sendAsync(msg, &cb);, ons::ONSClientException);
}

TEST_F(ProducerTest, testSendMessageAsync_TopicNotExist) {
    ons::Message msg(
            "topic_not_use_test",
            "tagA",
            "ORDERID_100",
            "Hello, RocketMQ"
    );
    std::mutex mtx;
    std::condition_variable cv;
    bool complete = false;
    bool success = false;
    ExampleSendCallback cb(mtx, complete, cv, success);
    EXPECT_THROW(producer_->sendAsync(msg, &cb);, ons::ONSClientException);
}