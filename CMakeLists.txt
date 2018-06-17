cmake_minimum_required(VERSION 2.8)

# project name
project(TwiFetch-OMP)

# set include path variables
include_directories(include)

# add all .cpp files in src and SHA1
file(GLOB SOURCES "src/*.cpp" "src/python/*" "include/SHA1/*.cpp")

# forcing C++11 standard
set(CMAKE_CXX_STANDARD 11)

# Add the executable files
add_executable(${PROJECT_NAME} ${SOURCES})

# Add OpenMP flags
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# link the curl and python library to the executable
target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} python2.7)

add_library(twifetchomp STATIC ${SOURCES})
#add_library(twifetchomp SHARED ${SOURCES})
