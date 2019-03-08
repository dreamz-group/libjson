# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../ ./)

file(GLOB SOURCE "*.cpp")

add_executable(libjson_test ${SOURCE} )

target_link_libraries(libjson_test  json ${GTEST_LIBRARIES} pthread)

add_test (NAME SunLight COMMAND libjson_test)