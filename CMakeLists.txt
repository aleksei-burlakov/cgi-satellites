cmake_minimum_required(VERSION 2.8)

project(satellites)

include_directories("satellites.computation/include")
include_directories("satellites.misc/include")
include_directories("satellites.test/include")
include_directories("satellites.ui/include")

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_EXE_UI
  satellites.ui/src/main.cpp
)

set(SOURCE_LIB_COMPUTATION 
  satellites.computation/src/timeslots.cpp
)

set(SOURCE_LIB_MISC
  satellites.misc/src/misc.cpp
)

add_library(satellites.computation SHARED ${SOURCE_LIB_COMPUTATION})
add_library(satellites.misc SHARED ${SOURCE_LIB_MISC})

add_executable(satellites.ui ${SOURCE_EXE_UI})

target_link_libraries(satellites.ui satellites.computation satellites.misc)
