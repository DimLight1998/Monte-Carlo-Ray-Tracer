cmake_minimum_required(VERSION 3.15)
project(MonteCarloRayTracer)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH "bin")

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

file(GLOB_RECURSE MonteCarloRayTracerSRC "src/*.cpp" "src/*.hpp")

add_executable(MonteCarloRayTracer ${MonteCarloRayTracerSRC})
target_link_libraries(MonteCarloRayTracer ${OpenCV_LIBS})

