#ifndef MQ_BASE_TEST_H
#define MQ_BASE_TEST_H

#include <atomic>
#include <mutex>
#include <condition_variable>


#include "rocketmq.h"
#include "rocketmq-ons-cpp-full.h"
#include "ONSCallback.h"
#include "Message.h"
#include "UtilAll.h"
#include "Common.h"


namespace ons {
    class send_result_wrapper {
    public:
        send_result_wrapper(send_result &sendResult) : sendResult_(sendResult) {
            memset(&sendResult, 0, sizeof(sendResult));
            sendResult.message_id = new char[ONS_SEND_RESULT_MSG_ID_LEN_MAX + 1];
            memset(sendResult.message_id, 0, ONS_SEND_RESULT_MSG_ID_LEN_MAX + 1);
            sendResult.error_msg = new char[ONS_SEND_RESULT_ERR_MSG_LEN_MAX + 1];
            memset(sendResult.error_msg, 0, ONS_SEND_RESULT_ERR_MSG_LEN_MAX + 1);
        }

        ~send_result_wrapper() {
            delete[](sendResult_.message_id);
            delete[](sendResult_.error_msg);
        }

    private:
        send_result &sendResult_;
    };

    class message_creator {
    public:
        message_creator(message &m, bool is_order = false) : m_(m) {
            memset(&m, 0, sizeof(m));

            if (!is_order) {
                const char *topic = "t_opensource_unit_test";
                m.topic = new char[strlen(topic) + 1];
                strcpy(m.topic, topic);
            } else {
                const char *topic_ = "t_opensource_unit_test_order";
                m.topic = new char[strlen(topic_) + 1];
                strcpy(m.topic, topic_);
            }

            const char *tag = "tagA";
            m.tags = new char[strlen(tag) + 1];
            strcpy(m.tags, tag);

            const char *body = "Sample message body content";
            m.body = new char[strlen(body) + 1];
            strcpy(m.body, body);
            m.body_size = strlen(m.body);


            const char *key = "abc";
            m.key = new char[strlen(key) + 1];
            strcpy(m.key, key);

            const char *props = "{}";
            m.user_prop = new char[strlen(props) + 1];
            strcpy(m.user_prop, props);

            m.system_prop = new char[strlen(props) + 1];
            strcpy(m.system_prop, props);
        }

        ~message_creator() {
            delete[](m_.topic);
            delete[](m_.tags);
            delete[](m_.body);
            delete[](m_.key);
            delete[](m_.user_prop);
            delete[](m_.system_prop);
        }

    private:
        message &m_;
    };

    class subscribe_creator {
    public:
        subscribe_creator(subscription &sub,const char *topic, const char *sub_expression):sub_(sub) {
            sub.topic = new char[strlen(topic) + 1];
            strcpy(sub.topic, topic);

            sub.sub_expression = new char[strlen(sub_expression) + 1];
            strcpy(sub.sub_expression, sub_expression);
        }

        ~subscribe_creator() {
            delete[](sub_.topic);
            delete[](sub_.sub_expression);
        }

    private:
        subscription &sub_;
    };
    class string_wrapper {
    public:
        string_wrapper(char* &str,const char* value) : str_(str) {
            str = new char[strlen(value) + 1];
            strcpy(str, value);
        }

        ~string_wrapper() {
            delete str_;
        }

    private:
        char* &str_;
    };
    class BaseTest : public ::testing::Test {
    protected:
        BaseTest() {}

        void SetUp() override {
            int create_isolate_status = graal_create_isolate(NULL, &isolate_, &thread_);
            ASSERT_EQ(create_isolate_status, 0) << "error on isolate creation or attach";

            memset(&property, 0, sizeof(factory_property));
            const char *group_id = "GID_opensource_unit_test";
            const char *access_key = "AK";
            const char *access_secret = "SK";
            const char *name_srv_addr = "47.100.33.127:9876";
            group_id_ = new char[strlen(group_id) + 1];
            strcpy(group_id_, group_id);
            property.group_id = group_id_;

            access_key_ = new char[strlen(access_key) + 1];
            strcpy(access_key_, access_key);
            property.access_key = access_key_;

            access_secret_ = new char[strlen(access_secret) + 1];
            strcpy(access_secret_, access_secret);
            property.access_secret = access_secret_;

            name_srv_addr_ = new char[strlen(name_srv_addr) + 1];
            strcpy(name_srv_addr_, name_srv_addr);
            property.name_srv_addr = name_srv_addr_;

            total = 32;
        }

        void TearDown() override {
            delete group_id_;
            group_id_ = NULL;
            delete access_key_;
            access_key_ = NULL;
            delete access_secret_;
            access_secret_ = NULL;
            delete name_srv_addr_;
            name_srv_addr_ = NULL;
        }

        void updatePropertyKeyValue(const char *key, const char *value) {
            if (0 == strcasecmp(key, "group_id")) {
                if (group_id_ != NULL) {
                    delete group_id_;
                    group_id_ = NULL;
                }
                group_id_ = new char[strlen(value) + 1];
                strcpy(group_id_, value);
                property.group_id = group_id_;
            }
            if (0 == strcasecmp(key, "access_key")) {
                if (access_key_ != NULL) {
                    delete access_key_;
                    access_key_ = NULL;
                }
                access_key_ = new char[strlen(value) + 1];
                strcpy(access_key_, value);
                property.access_key = access_key_;
            }
            if (0 == strcasecmp(key, "access_secret")) {
                if (access_secret_ != NULL) {
                    delete access_secret_;
                    access_secret_ = NULL;
                }
                access_secret_ = new char[strlen(value) + 1];
                strcpy(access_secret_, value);
                property.access_secret = access_secret_;
            }
            if (0 == strcasecmp(key, "name_srv_addr")) {
                if (name_srv_addr_ != NULL) {
                    delete name_srv_addr_;
                    name_srv_addr_ = NULL;
                }
                name_srv_addr_ = new char[strlen(value) + 1];
                strcpy(name_srv_addr_, value);
                property.name_srv_addr = name_srv_addr_;
            }
        }

        graal_isolate_t *isolate_;
        graal_isolatethread_t *thread_;
        char *group_id_;
        char *access_key_;
        char *access_secret_;
        char *name_srv_addr_;
        factory_property property;
        int total;
    };

    std::mutex m1;
    std::mutex m2;
    std::condition_variable cv;

    class MyCallback : public ons::SendCallbackONS {
    public:
        void onSuccess(ons::SendResultONS &sendResult) override {
            std::lock_guard<std::mutex> lg(m2);
            success_num++;
            std::cout << "send success, message_id: " << sendResult.getMessageId() << ", total: " << success_num
                      << std::endl;
            if (success_num + failed_num == message_num) {
                cv.notify_all();
            }
        }

        void onException(ons::ONSClientException &e) override {
            std::lock_guard<std::mutex> lg(m2);
            failed_num++;
            std::cout << "send failure, total: " << failed_num << std::endl;
            std::cout << e.what() << std::endl;
            if (success_num + failed_num == message_num) {
                cv.notify_all();
            }
        }

        static int success_num;
        static int failed_num;
        static int message_num;
    };

    int MyCallback::success_num = 0;
    int MyCallback::failed_num = 0;
    int MyCallback::message_num = 32;

    class LocalTransactionCheckerImpl : public ons::LocalTransactionChecker {
        virtual ons::TransactionStatus check(ons::Message &msg) {
            cout << "checker::commit transaction" << endl;
            return ons::CommitTransaction;
        }
    };

    class LocalTransactionExecuterImpl : public ons::LocalTransactionExecuter {
        virtual ons::TransactionStatus execute(ons::Message &msg) {
            cout << "executer::commit transaction of msgid: " << msg.getMsgID() << endl;
            return ons::CommitTransaction;
        }
    };

    std::mutex console_mtx;

    class ExampleMessageListener : public ons::MessageListener {
    public:
        Action consume(ons::Message &message, ons::ConsumeContext &context) {
            std::lock_guard<std::mutex> lk(console_mtx);
            std::cout << "Received a message. Topic: " << message.getTopic() << ", MsgId: "
                      << message.getMsgID() << std::endl;
            return CommitMessage;
        }
    };

    class ExampleMessageOrderListener : public ons::MessageOrderListener {
    public:
        OrderAction consume(ons::Message &message, ons::ConsumeOrderContext &context) {
            std::lock_guard<std::mutex> lk(console_mtx);
            std::cout << "Received a message. Topic: " << message.getTopic() << ", MsgId: "
                      << message.getMsgID() << std::endl;
            return Success;
        }
    };

#ifdef __cplusplus
    extern "C" {
#endif
    void on_success_func(void *thread, char *message_id, char *send_callback_ons) {
        auto sendCallbackONS = reinterpret_cast<ons::SendCallbackONS *>(send_callback_ons);
        ons::SendResultONS sendResultOns;
        sendResultOns.setMessageId(message_id);
        sendCallbackONS->onSuccess(sendResultOns);
    }

    void on_exception_func(void *thread, char *m_msg, int m_error, char *send_callback_ons) {
        auto sendCallbackONS = reinterpret_cast<ons::SendCallbackONS *>(send_callback_ons);
        ons::ONSClientException onsClientException(m_msg, m_error);
        sendCallbackONS->onException(onsClientException);
    }

    int transaction_check_func(void *thread, void *opaque, char *topic, char *user_props, char *sys_props,
                               char *body, int body_len) {
        ons::LocalTransactionChecker *checker = reinterpret_cast<ons::LocalTransactionChecker *>(opaque);
        ons::Message message_;
        message_.setTopic(topic);
        ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
        ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
        message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
        return checker->check(message_);
    }

    int transaction_execute_func(void *thread, void *opaque, char *topic, char *user_props, char *sys_props,
                                 char *body, int body_len) {
        ons::LocalTransactionExecuter *executor = reinterpret_cast<ons::LocalTransactionExecuter *>(opaque);
        ons::Message message_;
        message_.setTopic(topic);
        ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
        ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
        message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
        return executor->execute(message_);
    }

    int consumer_on_message(void *thread, void *opaque, char *topic, char *user_props, char *sys_props, char *body,
                            int body_len) {
        ons::MessageListener *listener = reinterpret_cast<ons::MessageListener *>(opaque);
        ons::Message message_;
        message_.setTopic(topic);
        ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
        ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
        message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
        ons::ConsumeContext ctx_;
        Action action = listener->consume(message_, ctx_);
        switch (action) {
            case CommitMessage:
                return 0;
            case ReconsumeLater:
                return 1;
            default:
                return 1;
        }
    }

    int
    order_consumer_on_message(void *thread, void *opaque, char *topic, char *user_props, char *sys_props, char *body,
                              int body_len) {
        ons::MessageOrderListener *listener = reinterpret_cast<ons::MessageOrderListener *>(opaque);
        ons::Message message_;
        message_.setTopic(topic);
        ons::UtilAll::fill_message_props(message_, std::string(user_props), false);
        ons::UtilAll::fill_message_props(message_, std::string(sys_props), true);
        message_.setBody(reinterpret_cast<unsigned char *>(body), body_len);
        ons::ConsumeOrderContext ctx_;
        OrderAction action = listener->consume(message_, ctx_);
        switch (action) {
            case Success:
                return 0;
            case Suspend:
                return 1;
            default:
                return 1;
        }
    }
#ifdef __cplusplus
    }
#endif

}  // namespace ons

#endif  // MQ_BASE_TEST_H
