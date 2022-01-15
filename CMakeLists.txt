project(gameslib)
cmake_minimum_required(VERSION 3.18)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

add_definitions(-D__ARM_LINUX__)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ~/pigpio/util)
find_package(pigpio REQUIRED)

# Obtain u8g2 Library (REQUIRED)
include("./cmake/u8g2.cmake")

# Obtain joystick Library (REQUIRED)
include("./cmake/joystick.cmake")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB GAMESLIB_SRC_FILES "src/*.cpp")
file(GLOB GAMESLIB_HEADER_FILES "include/*.hpp")

add_library(${CMAKE_PROJECT_NAME} STATIC ${GAMESLIB_SRC_FILES} ${U8G2_SRC_FILES} ${U8G2_PORT_SRC_FILES} ${U8G2_DRIVERS_SRC_FILES} ${JOYSTICK_SRC_FILES}) # ${U8G2_HEADER_FILES} ${U8G2_PORT_HEADER_FILES} ${U8G2_DRIVERS_HEADER_FILES}
target_link_libraries(${CMAKE_PROJECT_NAME} 
  PRIVATE Threads::Threads
  PRIVATE pigpio
)
target_include_directories(${CMAKE_PROJECT_NAME}
  PUBLIC "include/"
  PUBLIC pigpio_INCLUDE_DIRS
  PUBLIC ${U8G2_INCLUDE_DIR}
  PUBLIC ${JOYSTICK_INCLUDE_DIR}
)

add_subdirectory("examples/snake")