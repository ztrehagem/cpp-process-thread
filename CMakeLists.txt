cmake_minimum_required(VERSION 3.7)

set (CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(capthread
  src/capthread.cpp
)
add_executable(capfork
  src/capfork.cpp
)
add_executable(capfork2
  src/capfork2.cpp
)
