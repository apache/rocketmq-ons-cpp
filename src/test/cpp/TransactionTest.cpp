#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include "ClientBaseTest.h"

class LocalTransactionCheckerImpl : public ons::LocalTransactionChecker {
    virtual ons::TransactionStatus check(ons::Message &msg) {
        cout << "transaction checker::commit transaction" << endl;
        return ons::CommitTransaction;
    }
};

class LocalTransactionExecuterImpl : public ons::LocalTransactionExecuter {
    virtual ons::TransactionStatus execute(ons::Message &msg) {
        cout << "transaction executer::commit transaction of msgid: " << msg.getMsgID() << endl;
        return ons::CommitTransaction;
    }
};

class TransactionTest : public ClientBaseTest {
protected:
    void SetUp() override {
        ClientBaseTest::SetUp();
        checker_ = new LocalTransactionCheckerImpl();
        executor_ = new LocalTransactionExecuterImpl();
        producer_ = ons::ONSFactory::getInstance()->createTransactionProducer(factoryInfo, checker_);
        producer_->start();
    }

    void TearDown() override {
        ClientBaseTest::TearDown();
        producer_->shutdown();
        delete checker_;
        delete executor_;
    }

    ons::TransactionProducer *producer_;
    LocalTransactionCheckerImpl *checker_;
    LocalTransactionExecuterImpl *executor_;
};

TEST_F(TransactionTest, test_setUp) {
    ASSERT_TRUE(nullptr != producer_);
}

TEST_F(TransactionTest, testSendMessage_Normal) {
    ons::Message msg(
            "lingchu_test_0",
            "tagA",
            "ORDERID_100",
            "hello MQ_lingchu"
    );
    ons::SendResultONS sendResult = producer_->send(msg, executor_);
    ASSERT_TRUE(nullptr != sendResult.getMessageId());
}

TEST_F(TransactionTest, testSendMessage_ExcutorBeingNull) {
    ons::Message msg(
            "lingchu_test_0",
            "tagA",
            "ORDERID_100",
            "hello MQ_lingchu"
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg, NULL), ons::ONSClientException);
}

TEST_F(TransactionTest, testSendMessage_TopicBeingEmpty) {
    ons::Message msg(
            "",
            "tagA",
            "ORDERID_100",
            "hello MQ_lingchu"
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg, executor_), ons::ONSClientException);
}

TEST_F(TransactionTest, testSendMessage_TopicNotExist) {
    ons::Message msg(
            "topic_not_use_test",
            "tagA",
            "ORDERID_100",
            "hello MQ_lingchu"
    );
    EXPECT_THROW(ons::SendResultONS sendResult = producer_->send(msg, executor_), ons::ONSClientException);
}
