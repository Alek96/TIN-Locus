project(Locus_test_client)

find_package(Threads)

add_executable(${PROJECT_NAME}
        test_client.cpp
        signal_handlers.cpp signal_handlers.h)

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        )