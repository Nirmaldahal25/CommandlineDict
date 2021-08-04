cmake_minimum_required(VERSION 3.10)

project(dictionary C) 

include_directories(include)
include_directories(WebstersEnglishDictionary)

# add_subdirectory(argparser)
link_directories(argparser)
add_library(argparser STATIC argparser/c/argparser.h argparser/c/argparser.c)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(HEADER_FILE include/dictionary.h)
set(SOURCE_FILE dictionary.c)

add_executable(${PROJECT_NAME} main.c ${HEADER_FILE} ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} argparser)

# file(MAKE_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/WebstersEnglishDictionary)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/WebstersEnglishDictionary/dictionary.json
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)