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

#ifndef __ROCKETMQ_H__
#define __ROCKETMQ_H__
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct message_struct {
    char *topic;
    char *tags;
    char *body;
    unsigned int body_size;
    char *key;
    char *user_prop;
    char *system_prop;
} message;

typedef struct send_result_struct {
    char *message_id;
    int error_no;
    char *error_msg;
} send_result;

typedef struct factory_property_struct {
    char *group_id;
    char *access_key;
    char *access_secret;
    char *name_srv_addr;
    char *name_srv_domain;
    char *message_model;
    char *send_msg_timeout_millis;
    char *consume_thread_nums;
    char *ons_channel;
    char *max_msg_cache_size;
    char *ons_trace_switch;
    char *consumer_instance_name;
    char *language_identifier;
    char *instance_id;
    int use_domain;
} factory_property;

typedef struct callback_func_struct {
    char *send_callback_ons;

    void (*on_success)(void *thread, char *message_id, char *send_callback_ons);

    void (*on_exception)(void *thread, char *m_msg, int m_error, char *send_callback_ons);
} callback_func;

typedef struct subscription_struct {
    char *topic;
    char *sub_expression;

    int (*on_message)(void *thread, void *opaque, char *topic, char *user_props, char *sys_props, char *body, int len);

    void *opaque;
} subscription;

#ifdef __cplusplus
}
#endif

#endif //__ROCKETMQ_H__