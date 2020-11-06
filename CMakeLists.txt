cmake_minimum_required(VERSION 3.2)

# Set up parameters
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_config.cmake)

project(${PROJECT_NAME} VERSION 0.0.${PATCH})

# Set up and install executables
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/executables/executables.cmake)

# Set up CPack
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpack/cpack.cmake)

# Enable testing
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests/tests.cmake)
