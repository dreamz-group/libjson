cmake_minimum_required(VERSION 3.5)

project(json)

file(GLOB SOURCE "*.cpp" )
add_library(json ${SOURCE})

if(${BUILD_SHARED_LIBS})
    install(TARGETS    json       DESTINATION /usr/local/lib )
endif()

target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

option(make_tests "Make test programs" OFF)

if(${make_tests})
    add_subdirectory (Tests)
endif()
  
   
