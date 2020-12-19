cmake_minimum_required(VERSION 3.13)

project(climate)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Controller/Controller.cpp
)

add_executable(climate ${SOURCE})

target_link_libraries(climate PRIVATE boost_program_options)
