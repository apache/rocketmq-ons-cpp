#ifndef __ROCKETMQ_ONS_CPP_FULL_H
#define __ROCKETMQ_ONS_CPP_FULL_H

#include <graal_isolate_dynamic.h>


#if defined(__cplusplus)
extern "C" {
#endif

typedef int (*create_producer_fn_t)(graal_isolatethread_t*, factory_property*);

typedef int (*create_transaction_producer_fn_t)(graal_isolatethread_t*, factory_property*, void*, void *);

typedef int (*create_consumer_fn_t)(graal_isolatethread_t*, factory_property*);

typedef int (*create_order_consumer_fn_t)(graal_isolatethread_t*, factory_property*);

typedef void (*subscribe_fn_t)(graal_isolatethread_t*, int, subscription*);

typedef void (*subscribe_order_listener_fn_t)(graal_isolatethread_t*, int, subscription*);

typedef void (*start_instance_fn_t)(graal_isolatethread_t*, int);

typedef void (*destroy_instance_fn_t)(graal_isolatethread_t*, int);

typedef int (*create_order_producer_fn_t)(graal_isolatethread_t*, factory_property*);

typedef void (*send_message_fn_t)(graal_isolatethread_t*, int, message*, send_result*);

typedef void (*send_message_oneway_fn_t)(graal_isolatethread_t*, int, message*, send_result*);

typedef void (*send_message_async_fn_t)(graal_isolatethread_t*, int, message*, send_result*, callback_func*);

typedef void (*send_message_transaction_fn_t)(graal_isolatethread_t*, int, message*, send_result*, void*, void *);

typedef void (*send_order_message_fn_t)(graal_isolatethread_t*, int, message*, send_result*, char*);

typedef int (*run_main_fn_t)(int paramArgc, char** paramArgv);

#if defined(__cplusplus)
}
#endif
#endif
