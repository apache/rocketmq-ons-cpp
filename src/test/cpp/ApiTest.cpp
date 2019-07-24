#include <gtest/gtest.h>
#include <iostream>
#include "BaseTest.h"

namespace ons {
    class ApiTest : public BaseTest {
    };

    TEST_F(ApiTest, test_create_producer) {
        int instance_id = create_producer(thread_, &property);
        EXPECT_TRUE(instance_id >= 0) << "Instance Id should be non-negative";
        destroy_instance(thread_, instance_id);
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_create_order_producer) {
        int instance_id = create_order_producer(thread_, &property);
        EXPECT_TRUE(instance_id >= 0) << "Instance Id should be non-negative";
        destroy_instance(thread_, instance_id);
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_create_transaction_producer) {
        void *transaction_check = (void *) transaction_check_func;
        LocalTransactionCheckerImpl pTransactionCheckListener;
        void *checker = reinterpret_cast<void *>(&pTransactionCheckListener);
        int instance_id = create_transaction_producer(thread_, &property, checker, transaction_check);
        EXPECT_TRUE(instance_id >= 0) << "Instance Id should be non-negative";
        destroy_instance(thread_, instance_id);
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_create_consumer) {
        int consumer_id = create_consumer(thread_, &property);
        EXPECT_TRUE(consumer_id >= 0) << "Instance Id should be non-negative";
        destroy_instance(thread_, consumer_id);
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_create_order_consumer) {
        int consumer_id = create_order_consumer(thread_, &property);
        EXPECT_TRUE(consumer_id >= 0) << "Instance Id should be non-negative";
        destroy_instance(thread_, consumer_id);
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_send_message) {
        int instance_id = create_producer(thread_, &property);
        message m;
        message_creator creator(m);
        send_result sr;
        send_result_wrapper wrapper(sr);
        for (int i = 0; i < total; i++) {
            send_message(thread_, instance_id, &m, &sr);
            ASSERT_EQ(sr.error_no, 0);
            std::cout << "Send OK, MsgId: " << sr.message_id << std::endl;
        }
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_send_message_oneway) {
        int instance_id = create_producer(thread_, &property);
        message m;
        message_creator creator(m);
        send_result sendResult;
        send_result_wrapper wrapper(sendResult);
        bzero(&sendResult, sizeof(send_result));
        for (int i = 0; i < total; i++) {
            send_message_oneway(thread_, instance_id, &m, &sendResult);
        }
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
        ASSERT_EQ(0, sendResult.error_no);
    }

    TEST_F(ApiTest, test_send_order_message) {
        updatePropertyKeyValue("group_id", "GID_opensource_unit_test_order");
        int instance_id = create_order_producer(thread_, &property);
        message m;
        message_creator creator(m, true);
        send_result sr;
        send_result_wrapper wrapper(sr);
        char *sharding_key;
        string_wrapper stringWrapper(sharding_key, "1");
        for (int i = 0; i < total; i++) {
            send_order_message(thread_, instance_id, &m, &sr, sharding_key);
            ASSERT_EQ(sr.error_no, 0);
            std::cout << "Send OK, MsgId: " << sr.message_id << std::endl;
        }
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, test_send_message_async) {
        MyCallback pSendCallback;
        int instance_id = create_producer(thread_, &property);
        message m;
        message_creator creator(m);
        send_result sendResult;
        send_result_wrapper wrapper(sendResult);
        callback_func c_f;
        c_f.on_success = on_success_func;
        c_f.on_exception = on_exception_func;
        c_f.send_callback_ons = reinterpret_cast<char *>(&pSendCallback);
        for (int i = 0; i < MyCallback::message_num; i++) {
            send_message_async(thread_, instance_id, &m, &sendResult, &c_f);
        }
        {
            std::unique_lock<std::mutex> lk(m1);
            cv.wait(lk);
        }
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, send_message_transaction) {
        void *transaction_check = (void *) transaction_check_func;
        LocalTransactionCheckerImpl pTransactionCheckListener;
        void *checker = reinterpret_cast<void *>(&pTransactionCheckListener);
        int instance_id = create_transaction_producer(thread_, &property, checker, transaction_check);
        LocalTransactionExecuterImpl pTransactionExecutor;
        message m;
        message_creator creator(m);
        send_result sr;
        send_result_wrapper wrapper(sr);
        void *executor = reinterpret_cast<void *>(&pTransactionExecutor);
        void *transaction_execute = (void *) transaction_execute_func;
        for (int i = 0; i < total; i++) {
            send_message_transaction(thread_, instance_id, &m, &sr, executor, transaction_execute);
            ASSERT_EQ(sr.error_no, 0);
            std::cout << "Send OK, MsgId: " << sr.message_id << std::endl;
        }
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, subsribe) {
        int instance_id = create_consumer(thread_, &property);
        ExampleMessageListener messageListener;
        ons::MessageListener *listener = &messageListener;
        subscription sub;
        memset(&sub, 0, sizeof(subscription));
        subscribe_creator creator(sub, "t_opensource_unit_test", "*");
        sub.opaque = listener;
        sub.on_message = consumer_on_message;
        ::subscribe(thread_, instance_id, &sub);
        start_instance(thread_, instance_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(20 * 1000));
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

    TEST_F(ApiTest, subsribe_order) {
        updatePropertyKeyValue("group_id", "GID_opensource_unit_test_order");
        int instance_id = create_order_consumer(thread_, &property);
        ExampleMessageOrderListener messageOrderListener;
        ons::MessageOrderListener *listener = &messageOrderListener;
        subscription sub;
        memset(&sub, 0, sizeof(subscription));
        subscribe_creator creator(sub, "t_opensource_unit_test_order", "tagA");
        sub.opaque = listener;
        sub.on_message = order_consumer_on_message;
        subscribe_order_listener(thread_, instance_id, &sub);
        start_instance(thread_, instance_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(60 * 1000));
        destroy_instance(thread_, instance_id);
        std::cout << "Destroy instance OK" << std::endl;
        ASSERT_EQ(graal_detach_thread(thread_), 0) << "Detach graal thread failed";
    }

}  // namespace ons
