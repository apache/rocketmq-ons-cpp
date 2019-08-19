add_library(${LIBRARY_NAME} SHARED
        ClientBase.cpp
        ONSFactory.cpp
        ProducerImpl.cpp
        ONSClientException.cpp
        SendResultONS.cpp
        Message.cpp
        OrderProducerImpl.cpp
        MessageQueueONS.cpp
        ConsumerImpl.cpp
        OrderConsumerImpl.cpp
        TransactionProducerImpl.cpp
        common/UtilAll.cpp
        common/Logger.cpp
        common/ONSClientAbstract.cpp
        extern/CBatchMessage.cpp
        extern/CErrorMessage.cpp
        extern/CMessage.cpp
        extern/CMessageExt.cpp
        extern/CProducer.cpp
        extern/CPullConsumer.cpp
        extern/CPushConsumer.cpp
        extern/CSendResult.cpp)
target_link_libraries(${LIBRARY_NAME} ${ROCKETMQ_CLIENT_CORE})
set_target_properties(${LIBRARY_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib VERSION ${CLIENT_VERSION} SOVERSION ${CLIENT_VERSION})