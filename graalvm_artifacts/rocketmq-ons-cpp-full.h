#ifndef __ROCKETMQ_ONS_CPP_FULL_H
#define __ROCKETMQ_ONS_CPP_FULL_H

#include <graal_isolate.h>


#if defined(__cplusplus)
extern "C" {
#endif

int create_producer(graal_isolatethread_t*, factory_property*);

int create_transaction_producer(graal_isolatethread_t*, factory_property*, void*, void *);

int create_consumer(graal_isolatethread_t*, factory_property*);

int create_order_consumer(graal_isolatethread_t*, factory_property*);

void subscribe(graal_isolatethread_t*, int, subscription*);

void subscribe_order_listener(graal_isolatethread_t*, int, subscription*);

void start_instance(graal_isolatethread_t*, int);

void destroy_instance(graal_isolatethread_t*, int);

int create_order_producer(graal_isolatethread_t*, factory_property*);

void send_message(graal_isolatethread_t*, int, message*, send_result*);

void send_message_oneway(graal_isolatethread_t*, int, message*, send_result*);

void send_message_async(graal_isolatethread_t*, int, message*, send_result*, callback_func*);

void send_message_transaction(graal_isolatethread_t*, int, message*, send_result*, void*, void *);

void send_order_message(graal_isolatethread_t*, int, message*, send_result*, char*);

int run_main(int paramArgc, char** paramArgv);

#if defined(__cplusplus)
}
#endif
#endif
