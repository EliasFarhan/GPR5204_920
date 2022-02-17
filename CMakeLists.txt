cmake_minimum_required (VERSION 3.18)
project(GPR5204)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.h)
add_library(CommonLib STATIC ${SRC_FILES})
target_include_directories(CommonLib PUBLIC include/)
if(MSVC)
    target_compile_definitions(CommonLib PUBLIC "_USE_MATH_DEFINES")
    target_compile_options(CommonLib PUBLIC /arch:AVX2 /Oi /GR- /EHs-c- /FA /Oy- /GL)
    target_link_options(CommonLib PUBLIC /LTCG)
else()
    target_compile_options(CommonLib PUBLIC "-march=haswell" "-masm=intel" -fno-rtti -fno-exceptions -save-temps
            -fno-omit-frame-pointer -flto -ffast-math)
    target_link_options(CommonLib PUBLIC -flto)
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

file(GLOB TEST_FILES test/*.cpp)

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE CommonLib GTest::gtest GTest::gtest_main)
    #gtest_add_tests(TARGET ${TEST_NAME})
    
    set_target_properties (${TEST_NAME} PROPERTIES FOLDER Test)
endforeach()


file(GLOB MAIN_FILES main/*.cpp)

foreach(MAIN_FILE ${MAIN_FILES})
    get_filename_component(MAIN_NAME ${MAIN_FILE} NAME_WE)
    add_executable(${MAIN_NAME} ${MAIN_FILE})
    target_link_libraries(${MAIN_NAME} PRIVATE CommonLib)
    #gtest_add_tests(TARGET ${TEST_NAME})
    
    set_target_properties (${MAIN_NAME} PROPERTIES FOLDER Main)
endforeach()
