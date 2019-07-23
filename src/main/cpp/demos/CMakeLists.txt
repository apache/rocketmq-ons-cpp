macro(add_demo name source_file)
    add_executable(${name} ${source_file})
    target_link_libraries(${name} pthread ${CMAKE_PROJECT_NAME})
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/demos)
endmacro()

add_demo(producer_demo ProducerDemo.cpp)
add_demo(order_producer_demo OrderProducerDemo.cpp)
add_demo(multi_thread_producer_demo MultiThreadProducerDemo.cpp)
add_demo(producer_async_demo ProducerAsyncDemo.cpp)
add_demo(producer_oneway_demo ProducerOnewayDemo.cpp)
add_demo(consumer_demo ConsumerDemo.cpp)
add_demo(order_consumer_demo OrderConsumerDemo.cpp)
add_demo(transaction_producer_demo TransactionProducerDemo.cpp)



