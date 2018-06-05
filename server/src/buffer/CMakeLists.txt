project(Locus_buffer)

set(SOURCE_FILES
        BlockingBuffer.cpp BlockingBuffer.h
        Converter.h
        Buffer.cpp Buffer.h
        BlockBuffer.cpp BlockBuffer.h PipeBuffer.cpp PipeBuffer.h RandomStringGenerator.cpp RandomStringGenerator.h)

add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
