project(Locus_database)

set(SOURCE_FILES
        Position.cpp Position.h
        Following.cpp Following.h
        ClientData.cpp ClientData.h
        ClientDataManager.cpp ClientDataManager.h
        Database.cpp Database.h)

add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
