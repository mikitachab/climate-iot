cmake_minimum_required(VERSION 3.13)
include(FetchContent)


FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.7.3)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

find_package(httplib)

set(SOURCE_CLI
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Controller/Controller.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ClimateClient/ClimateClient.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(climate-cli ${SOURCE_CLI})
target_link_libraries(climate-cli
   boost_program_options
   httplib::httplib
   nlohmann_json::nlohmann_json
)
