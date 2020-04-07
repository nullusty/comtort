# CMakeLists.txt
# --------------
# Top-level CMake project file. 

cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(comtort)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
