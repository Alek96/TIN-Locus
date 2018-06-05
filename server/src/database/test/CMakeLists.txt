project(Locus_database_test)

include_directories(${gtest_SOURCE_DIR}/include &{gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        Position_test.cpp
        Watcher_test.cpp
        ClientInfo_test.cpp
        ClientDataManager_test.cpp)

gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} Locus_database Locus_message Locus_buffer Locus_log)