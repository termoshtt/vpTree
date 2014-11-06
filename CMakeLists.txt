cmake_minimum_required(VERSION 2.8)

project(vpTree)
set(CMAKE_BUILD_TYPE Release)
add_definitions("-Wall -std=c++11")

enable_testing()
add_custom_target(short_test COMMAND "ctest" "-V" "-R" "TEST_.*_S")
add_custom_target(build_test)
add_dependencies(short_test build_test)
macro(mytest name type src)
    set(test_name TEST_${name}_${type})
    add_executable(${test_name} ${src})
    target_link_libraries(${test_name} ${ARGN})
    add_test(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
    add_dependencies(build_test ${test_name})
    unset(test_name)
endmacro(mytest)

add_custom_target(
    document
    COMMAND "doxygen" "doc/Doxyfile"
)

add_custom_target(
    ctags ALL
    COMMAND "ctags" "-R" "-f" ".tags" "--languages=C++,python" "--exclude='CMake*'"
)

include_directories(.)
add_subdirectory(test EXCLUDE_FROM_ALL)
