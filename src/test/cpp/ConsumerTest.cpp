#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include "ClientBaseTest.h"

using namespace ons;

class UnitTestMessageListener : public MessageListener {
public:
    Action consume(Message &message, ConsumeContext &context) {
        return CommitMessage;
    }
};

class ConsumerTest : public ClientBaseTest {
protected:
    void SetUp() override {
        ClientBaseTest::SetUp();
        consumer_ = ons::ONSFactory::getInstance()->createPushConsumer(factoryInfo);
        messageListener_ = new UnitTestMessageListener();
    }

    void TearDown() override {
        ClientBaseTest::TearDown();
        //consumer_->shutdown();
        delete messageListener_;
    }

    ons::PushConsumer *consumer_;
    const char *topic_ = "Topic";
    const char *tag_ = "Tag";
    UnitTestMessageListener *messageListener_;
};

TEST_F(ConsumerTest, test_setUp) {
    ASSERT_TRUE(nullptr != consumer_);
}

TEST_F(ConsumerTest, testSubscriber_topicBeingNull) {
    EXPECT_THROW(consumer_->subscribe(NULL, tag_, messageListener_);, ons::ONSClientException);
}

TEST_F(ConsumerTest, testSubscriber_tagBeingNull) {
    EXPECT_THROW(consumer_->subscribe(topic_, NULL, messageListener_);, ons::ONSClientException);
}

TEST_F(ConsumerTest, testSubscriber_ListenerBeingNull) {
    EXPECT_THROW(consumer_->subscribe(topic_, tag_, NULL);, ons::ONSClientException);
}
