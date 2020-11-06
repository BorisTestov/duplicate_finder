# Set patch value. It's not 0 if build on Travis
set(PATCH "0")
if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(PATCH $ENV{TRAVIS_BUILD_NUMBER})
endif()

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (DEFINED ENV{PROJECT_NAME})
    set(PROJECT_NAME $ENV{PROJECT_NAME})
else()
    get_filename_component(PROJECT_NAME ${PROJECT_ROOT} ABSOLUTE NAME)
    string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
    string(REPLACE "/" ";" PROJECT_NAME ${PROJECT_NAME})
    list(GET PROJECT_NAME -1 PROJECT_NAME)
endif()

# Set automoc for QTest
set(CMAKE_AUTOMOC ON)
set(SRC_DIR ${PROJECT_ROOT}/src)
set(INCLUDE_DIR ${PROJECT_ROOT}/include)
set(TARGET_PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Set compile options
set(COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror)
if (MSVC)
    set(COMPILE_OPTIONS /W4)
endif()

set(CMAKE_BUILD_TYPE "Release")
