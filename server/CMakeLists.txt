cmake_minimum_required(VERSION 3.13)
include(FetchContent)

find_package(Pistache 0.0.2 REQUIRED)

find_path(HIREDIS_HEADER hiredis)
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
find_package (SQLite3)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
add_executable(climate-server)
target_include_directories(climate-server
    PUBLIC
      ${HIREDIS_HEADER}
      ${REDIS_PLUS_PLUS_HEADER}
)
target_link_libraries(climate-server
  pistache_shared
  ${REDIS_PLUS_PLUS_LIB}
  SQLite::SQLite3
)
target_sources(climate-server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ClimateEndpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Repository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SQLite.cpp
)
