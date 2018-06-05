cmake_minimum_required(VERSION 3.10)
project(Locus)

find_package(Threads)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest)
enable_testing()

add_subdirectory(lib/googletest)
include_directories(lib)
include_directories(lib/cereal/include)

include_directories(src)

add_subdirectory(src/simple-client)
add_subdirectory(src/buffer)
add_subdirectory(src/client)
add_subdirectory(src/crypto)
add_subdirectory(src/database)
add_subdirectory(src/epoll)
add_subdirectory(src/log)
add_subdirectory(src/message)
add_subdirectory(src/packet)
add_subdirectory(src/server)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        Locus_server Locus_client
        Locus_crypto ${CMAKE_SOURCE_DIR}/lib/cryptopp/libcryptopp.a Locus_epoll
        Locus_database Locus_message Locus_buffer Locus_log Locus_packet
        ${CMAKE_THREAD_LIBS_INIT}
        )