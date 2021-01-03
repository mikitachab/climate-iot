cmake_minimum_required(VERSION 3.13)

find_package(httplib)

set(SOURCE_CLI
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Controller/Controller.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(climate-cli ${SOURCE_CLI})
target_link_libraries(climate-cli PRIVATE boost_program_options httplib::httplib)
