file(GLOB TEST_SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/tests/*.cpp")
file(GLOB TEST_HEADERS
        "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp")
list(REMOVE_ITEM TEST_SOURCES ${SRC_DIR}/main.cpp)

add_executable(test_${PROJECT_NAME} ${TEST_SOURCES} ${TEST_HEADERS})
set_target_properties(test_${PROJECT_NAME} PROPERTIES ${TARGET_PROPERTIES})
target_link_libraries(test_${PROJECT_NAME} gtest pthread)
target_compile_options(test_${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})

enable_testing()
add_test(test_${PROJECT_NAME} ${BUILD_DIR}/test_${PROJECT_NAME})
