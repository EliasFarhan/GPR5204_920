cmake_minimum_required (VERSION 3.18)
project(GPR5204)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark CONFIG REQUIRED)

file(GLOB BENCH_FILES bench/*.cpp)

foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_FILE})
    target_link_libraries(${BENCH_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main)
    target_include_directories(${BENCH_NAME} PRIVATE include/)
    if(MSVC)
        target_compile_options(${BENCH_NAME} PRIVATE /arch:AVX2 /Oi /GR- /EHs-c- /FA)
    else()
        target_compile_options(${BENCH_NAME} PRIVATE "-march=haswell")
    endif()
endforeach(BENCH_FILE ${BENCH_FILES})
