cmake_minimum_required(VERSION 2.8)

project(vpTree)
set(CMAKE_BUILD_TYPE Release)
include_directories(.)
add_definitions("-Wall -std=c++11")

# Shared Library
# ==============
set(MAKE_SHARED_LIB CACHE BOOL "if true, shared libs are compiled")
if(MAKE_SHARED_LIB)
  find_package(PythonLibs REQUIRED)
  if(${PYTHONLIBS_VERSION_STRING} GREATER 3.0)
    find_package(Boost COMPONENTS python3 REQUIRED)
  else()
    find_package(Boost COMPONENTS python REQUIRED)
  endif()
  include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

  add_library(vpTree SHARED python.cpp)
  target_link_libraries(vpTree ${Boost_LIBRARIES} ${PYTHON_LIBRARY} boost_numpy)
  set_target_properties(vpTree PROPERTIES PREFIX "")
  install(TARGETS vpTree LIBRARY DESTINATION "lib")
endif(MAKE_SHARED_LIB)

# Testing
# ======
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
add_subdirectory(test EXCLUDE_FROM_ALL)

# utility
# =======
add_custom_target(
    document
    COMMAND "doxygen" "doc/Doxyfile"
)
add_custom_target(
    ctags ALL
    COMMAND "ctags" "-R" "-f" ".tags" "--languages=C++,python" "--exclude='CMake*'"
)

