include_directories(${CMAKE_SOURCE_DIR}/src/test/cpp
                    ${CMAKE_SOURCE_DIR}/src/main/c/include)

macro(add_gtest test_name test_source)
    add_executable(${test_name} ${test_source} TestCMain.cpp)
    target_link_libraries(${test_name} gtest ${LIBRARY_NAME})
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests)
endmacro()

add_gtest(cproducer_test CProducerTest.cpp)
add_gtest(cpushconsumer_test CPushConsumerTest.cpp)
