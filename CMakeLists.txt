cmake_minimum_required (VERSION 3.18)
project(GPR5204)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.h)
add_library(CommonLib STATIC ${SRC_FILES})
target_include_directories(CommonLib PUBLIC include/)
if(MSVC)
target_compile_definitions(CommonLib PUBLIC "_USE_MATH_DEFINES")
    target_compile_options(CommonLib PUBLIC /arch:AVX2 /Oi /GR- /EHs-c- /FA /Oy-)
else()
    target_compile_options(CommonLib PUBLIC "-march=haswell" "-masm=intel" -fno-rtti -fno-exceptions -save-temps -fno-omit-frame-pointer)
endif()

file(GLOB_RECURSE TEST_FILES test/*.cpp)
add_library(CommonTest ${TEST_FILES})
target_link_libraries(CommonTest PRIVATE CommonLib GTest::gtest GTest::gtest_main)


file(GLOB BENCH_FILES bench/*.cpp)

foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_FILE})
    target_link_libraries(${BENCH_NAME} PRIVATE CommonLib benchmark::benchmark benchmark::benchmark_main)

    
    set_target_properties (${BENCH_NAME} PROPERTIES FOLDER Bench)
endforeach(BENCH_FILE ${BENCH_FILES})
