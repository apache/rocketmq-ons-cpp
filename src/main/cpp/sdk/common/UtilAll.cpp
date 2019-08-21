/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "UtilAll.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace ons;

using namespace rapidjson;

std::string UtilAll::to_string(const std::map<std::string, std::string> &prop) {
    Value root(kObjectType);
    Value key(kStringType);
    Value value(kStringType);
    Document document;
    Document::AllocatorType &allocator = document.GetAllocator();
    for (auto i = prop.begin(); i != prop.end(); i++) {
        key.SetString(i->first.c_str(), allocator);
        value.SetString(i->second.c_str(), allocator);
        root.AddMember(key, value, allocator);
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    root.Accept(writer);
    return std::string(buffer.GetString());
}

void UtilAll::init_factory_property(const ons::ONSFactoryProperty &factoryProperty, factory_property &fp,
                                    bool is_producer) {
    memset(&fp, 0, sizeof(factory_property));
    string groupID(factoryProperty.getGroupId());
    if (!groupID.empty()) {
        fp.group_id = const_cast<char *>(factoryProperty.getGroupId());
    } else {
        if (is_producer) {
            fp.group_id = const_cast<char *>(factoryProperty.getProducerId());

        } else {
            fp.group_id = const_cast<char *>(factoryProperty.getConsumerId());

        }
    }
    fp.access_key = const_cast<char *>(factoryProperty.getAccessKey());
    fp.access_secret = const_cast<char *>(factoryProperty.getSecretKey());
    string domain(factoryProperty.getNameSrvDomain());
    if (!domain.empty()) {
        fp.use_domain = 1;
        fp.name_srv_domain = const_cast<char *>(factoryProperty.getNameSrvDomain());
    } else {
        fp.use_domain = 0;
        fp.name_srv_addr = const_cast<char *>(factoryProperty.getNameSrvAddr());
    }
    fp.message_model = const_cast<char *>(factoryProperty.getMessageModel());
    fp.consumer_instance_name = const_cast<char *>(factoryProperty.getConsumerInstanceName());

    string send_msg_timeout_millis = to_string(factoryProperty.getSendMsgTimeout());
    char *send_msg_timeout_millis_ = new char[send_msg_timeout_millis.length() + 1];
    strcpy(send_msg_timeout_millis_, send_msg_timeout_millis.c_str());
    fp.send_msg_timeout_millis = send_msg_timeout_millis_;

    string suspend_time_millis = to_string(factoryProperty.getSuspendTimeMillis());
    char *suspend_time_millis_ = new char[suspend_time_millis.length() + 1];
    strcpy(suspend_time_millis_, suspend_time_millis.c_str());
    fp.suspend_time_millis = suspend_time_millis_;

    string consume_thread_nums = to_string(factoryProperty.getConsumeThreadNums());
    char *consume_thread_nums_ = new char[consume_thread_nums.length() + 1];
    strcpy(consume_thread_nums_, consume_thread_nums.c_str());
    fp.consume_thread_nums = consume_thread_nums_;

    string ons_channel;
    switch (factoryProperty.getOnsChannel()) {
        case ALIYUN:
            ons_channel = "ALIYUN";
            break;
        case CLOUD:
        case ALL:
        case LOCAL:
        case INNER:
            ons_channel = "CLOUD";
            break;
        default:
            ons_channel = "ALIYUN";
            break;
    }
    char *ons_channel_ = new char[ons_channel.length() + 1];
    strcpy(ons_channel_, ons_channel.c_str());
    fp.ons_channel = ons_channel_;

    string max_msg_cache_size = to_string(factoryProperty.getMaxMsgCacheSize());
    char *max_msg_cache_size_ = new char[max_msg_cache_size.length() + 1];
    strcpy(max_msg_cache_size_, max_msg_cache_size.c_str());
    fp.max_msg_cache_size = max_msg_cache_size_;

    string max_msg_cache_size_in_mib = to_string(factoryProperty.getMaxMsgCacheSizeInMiB());
    char *max_msg_cache_size_in_mib_ = new char[max_msg_cache_size_in_mib.length() + 1];
    strcpy(max_msg_cache_size_in_mib_, max_msg_cache_size_in_mib.c_str());
    fp.max_msg_cache_size_in_mb = max_msg_cache_size_in_mib_;

    string ons_trace_switch = "true";
    if (!factoryProperty.getOnsTraceSwitch()) {
        ons_trace_switch = "false";
    }
    char *ons_trace_switch_ = new char[ons_trace_switch.length() + 1];
    strcpy(ons_trace_switch_, ons_trace_switch.c_str());
    fp.ons_trace_switch = ons_trace_switch_;

    string language_identifier = "1";
    char *language_identifier_ = new char[language_identifier.length() + 1];
    strcpy(language_identifier_, language_identifier.c_str());
    fp.language_identifier = language_identifier_;

    string instance_id = to_string(factoryProperty.getInstanceId());
    char *instance_id_ = new char[instance_id.length() + 1];
    strcpy(instance_id_, instance_id.c_str());
    fp.instance_id = instance_id_;
}

void UtilAll::fill_message_props(Message &msg, const std::string &json, bool sys) {
    Document document;
    document.Parse(json.c_str());
    for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        if (sys) {
            msg.putSystemProperties(itr->name.GetString(), itr->value.GetString());
        } else {
            msg.putUserProperties(itr->name.GetString(), itr->value.GetString());
        }
    }
}

thread_local int ThreadAttachment::isolate_thread_ref_cnt;