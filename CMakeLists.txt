cmake_minimum_required(VERSION 3.15)
project(Monte_Carlo_Ray_Tracer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
if (OpenCT_FOUND)
    message("OpenCV found")
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    message("OpenMP found")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(lib)

file(GLOB_RECURSE Monte_Carlo_Ray_Tracer_SRC "src/*.cpp" "src/*.hpp")

add_executable(Monte_Carlo_Ray_Tracer ${Monte_Carlo_Ray_Tracer_SRC})
target_link_libraries(Monte_Carlo_Ray_Tracer ${OpenCV_LIBS})

