#
# Test main for testing.
#
cmake_minimum_required( VERSION 2.8.11 FATAL_ERROR )
project(autopilot_serial)

# Build the library with this application, otherwise we would need to make the 
# library for them...


# Add the exe
add_executable(autopilot_serial
 ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp
 autopilot_interface.cpp
 serial_port.cpp
 source.cpp
)

 #Mario? this is probably not right
if (CMAKE_VERSION VERSION_GREATER 2.8.11)  
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread")
else()
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11 -pthread")
endif()

# Property to set for C++ 11 standard.
set_property(TARGET autopilot_serial PROPERTY CXX_STANDARD 11)

