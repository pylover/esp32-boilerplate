cmake_minimum_required(VERSION 3.16)


if(ESPIDF_BUILDTYPE STREQUAL "debug")
  add_definitions("-DESPIDF_DEBUG")
endif()


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(
  boilerplate
  VERSION 0.1.0
)


