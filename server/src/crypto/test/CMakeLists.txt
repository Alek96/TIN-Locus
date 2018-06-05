project(Locus_crypto_test)

include_directories(${gtest_SOURCE_DIR}/include &{gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        Crypto_test.cpp CryptoModule_test.cpp)

gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} Locus_crypto ${CMAKE_SOURCE_DIR}/lib/cryptopp/libcryptopp.a)