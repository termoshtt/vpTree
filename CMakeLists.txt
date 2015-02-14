cmake_minimum_required(VERSION 2.8)

project(vpTree)
set(CMAKE_BUILD_TYPE Release)
include_directories(.)
add_definitions("-Wall -std=c++1y")

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
include(NumTest/NumTest.cmake)
add_subdirectory(test)

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

