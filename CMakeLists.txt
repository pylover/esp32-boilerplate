cmake_minimum_required(VERSION 3.16)


if(BUILD_TYPE STREQUAL "debug")
  add_definitions("-DDEBUG")
endif()


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(
  boilerplate
  VERSION 0.1.0
)
