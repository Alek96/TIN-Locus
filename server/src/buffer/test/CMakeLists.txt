project(Locus_buffer_test)

include_directories(${gtest_SOURCE_DIR}/include &{gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        BlockingBuffer_test.cpp
        Buffer_test.cpp
        Converter_test.cpp)

gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} Locus_buffer)