cmake_minimum_required(VERSION 2.8)

project(satellites)

include_directories("../satellites.computation/include")
include_directories("../satellites.misc/include")
include_directories("../satellites.test/include")
include_directories("../satellites.ui/include")
include_directories("../googletest/googletest/include")
include_directories("../googletest/googletest")
include_directories("../googletest/googlemock/include")
include_directories("../googletest/googlemock")

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_LIB_COMPUTATION 
  ../satellites.computation/src/timeslots.cpp
)

set(SOURCE_LIB_MISC
  ../satellites.misc/src/misc.cpp
)

set(SOURCE_EXE_TEST
  src/main.cpp
  src/test-computation.cpp
  src/test-misc.cpp
)
 
add_library(satellites.computation SHARED ${SOURCE_LIB_COMPUTATION})
add_library(satellites.misc SHARED ${SOURCE_LIB_MISC})

add_subdirectory("../googletest/googlemock" binary_dir)

add_executable(satellites.test ${SOURCE_EXE_TEST})

target_link_libraries(satellites.test satellites.computation satellites.misc gmock)
