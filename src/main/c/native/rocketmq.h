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