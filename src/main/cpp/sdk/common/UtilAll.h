#ifndef MQ_UTILALL_H
#define MQ_UTILALL_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Message.h"
#include "ONSFactory.h"
#include "ONSChannel.h"
#include "Common.h"

#include "graal_isolate.h"
#include "Logger.h"
#include "rocketmq.h"
#include "ONSClientException.h"

#include <string>
#include <map>
#include <iostream>
#include <rocketmq.h>
#include <thread>
#include <mutex>
#include <unordered_map>

namespace ons {

    class UtilAll {
    public:
        template<typename T>
        static std::string to_string(const T &n) {
            std::ostringstream stm;
            stm << n;
            return stm.str();
        }

        static long long str2ll(const char *str) {
            return atoll(str);
        }

        static std::string to_string(const std::map<std::string, std::string> &prop);

        static graal_isolate_t* get_isolate() {
            static graal_isolate_t* singleton = nullptr;
            if (nullptr == singleton) {
                if (graal_create_isolate(nullptr, &singleton, nullptr)) {
                    spdlog::error("Failed to create graal isolate");
                    return nullptr;
                }
            }
            return singleton;
        }

        static void init_message(message &m, const Message &msg) {
            m.topic = const_cast<char *>(msg.getTopic());
            m.tags = const_cast<char *>(msg.getTag());
            m.body = const_cast<char *>(msg.getBody());
            m.body_size = msg.getBodySize();
            m.key = const_cast<char *>(msg.getKey());
            std::string user_properties = to_string(msg.getUserProperties());
            char *user_props = new char[user_properties.length() + 1];
            strcpy(user_props, user_properties.c_str());
            m.user_prop = user_props;

            std::string sys_properties = to_string(msg.getSystemProperties());
            char *sys_props = new char[sys_properties.length() + 1];
            strcpy(sys_props, sys_properties.c_str());
            m.system_prop = sys_props;
        }

        static void release_message(message &m) {
            delete[](m.user_prop);
            delete[](m.system_prop);
        }

        static void fill_message_props(Message &msg, const std::string &json, bool sys);

        static void
        init_factory_property(const ONSFactoryProperty &factoryProperty, factory_property &fp, bool is_producer);

        static void release_factory_property(factory_property &fp) {
            delete[](fp.send_msg_timeout_millis);
            delete[](fp.consume_thread_nums);
            delete[](fp.ons_channel);
            delete[](fp.max_msg_cache_size);
            delete[](fp.ons_trace_switch);
            delete[](fp.language_identifier);
            delete[](fp.instance_id);
        }

        static void init_send_result(send_result &sendResult) {
            bzero(&sendResult, sizeof(send_result));
            sendResult.message_id = new char[ONS_SEND_RESULT_MSG_ID_LEN_MAX + 1];
            //sendResult.message_id[ONS_SEND_RESULT_MSG_ID_LEN_MAX] = 0;
            bzero(sendResult.message_id, ONS_SEND_RESULT_MSG_ID_LEN_MAX + 1);
            sendResult.error_msg = new char[ONS_SEND_RESULT_ERR_MSG_LEN_MAX + 1];
            //sendResult.error_msg[ONS_SEND_RESULT_ERR_MSG_LEN_MAX]=0;
            bzero(sendResult.error_msg, ONS_SEND_RESULT_ERR_MSG_LEN_MAX + 1);
        }

        static void release_send_result(send_result &sendResult) {
            delete[](sendResult.message_id);
            delete[](sendResult.error_msg);
        }
    };


    class MessageConverter {
    public:
        MessageConverter(message &m, const Message &msg) : m_(m), msg_(msg) {
            UtilAll::init_message(m, msg);
        }

        ~MessageConverter() {
            UtilAll::release_message(m_);
        }

    private:
        message &m_;
        const Message &msg_;
    };

    class FactoryPropertyConverter {
    public:
        FactoryPropertyConverter(const ONSFactoryProperty &factoryProperty, factory_property &fp,
                                 bool is_producer = true) : factoryProperty_(
                factoryProperty), fp_(fp) {
            UtilAll::init_factory_property(factoryProperty, fp, is_producer);
        }

        ~FactoryPropertyConverter() {
            UtilAll::release_factory_property(fp_);
        }

    private:
        const ONSFactoryProperty &factoryProperty_;
        factory_property &fp_;
    };

    class ThreadAttachment {
    public:
        ThreadAttachment(graal_isolatethread_t **thread) throw (ons::ONSClientException) {
            std::stringstream ss;
            ss << std::this_thread::get_id();

            if (isolate_thread_ref_cnt) {
                graal_isolatethread_t *prev = graal_get_current_thread(UtilAll::get_isolate());
                assert(nullptr != prev);
                *thread = prev;
                return;
            }

            ++isolate_thread_ref_cnt;

            if (graal_attach_thread(UtilAll::get_isolate(), thread)) {
                spdlog::error("Failed to attach native thread {} to graal isolate", ss.str());
                throw ons::ONSClientException("attach thread to isolate failed", -1);
            }

            pthread_key_t key;
            pthread_key_create(&key, &ThreadAttachment::detach);
            pthread_setspecific(key, &ThreadAttachment::isolate_thread_ref_cnt);

            spdlog::debug("Attach thread {} to isolate OK", ss.str());
        }

        ~ThreadAttachment() {
        }

        static void detach(void *) {
            graal_isolatethread_t *attached = graal_get_current_thread(UtilAll::get_isolate());
            if (attached) {
                graal_detach_thread(attached);
                std::stringstream ss;
                ss << std::this_thread::get_id();
                spdlog::debug("Detach thread {} from isolate OK", ss.str());
            }
        }

    private:
        static thread_local int isolate_thread_ref_cnt;
    };

    class SendResultWrapper {
    public:
        SendResultWrapper(send_result &sr) : sr_(sr){
            UtilAll::init_send_result(sr);
            sr_ = sr;
        }

        ~SendResultWrapper() {
            UtilAll::release_send_result(sr_);
        }

    private:
        send_result &sr_;
    };
}

#endif //MQ_UTILALL_H
