cmake_minimum_required(VERSION 2.8)

project(runtime)

macro(use_cxx17)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 17)
  endif ()
endmacro(use_cxx17)

use_cxx17()

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(vendor/json/src)

set(SOURCE_FILES src/python_interface.c src/cpp_invoker.cpp src/lambda/context.cpp)

add_library(runtime SHARED ${SOURCE_FILES})

target_link_libraries(runtime ${PYTHON_LIBRARIES})

target_include_directories(runtime PRIVATE include)
target_include_directories(runtime PRIVATE src)
