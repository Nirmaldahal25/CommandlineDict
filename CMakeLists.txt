cmake_minimum_required(VERSION 3.10)

project(dictionary C) 

include_directories(include)
include_directories(WebstersEnglishDictionary)

# add_subdirectory(argparser)
link_directories(argparser)
add_library(argparser STATIC argparser/c/argparser.h argparser/c/argparser.c)

set(HEADER_FILE include/dictionary.h)
set(SOURCE_FILE dictionary.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME} main.c ${HEADER_FILE} ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} argparser Threads::Threads )

# file(MAKE_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/WebstersEnglishDictionary)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/WebstersEnglishDictionary/dictionary.json
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)