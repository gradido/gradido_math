cmake_minimum_required (VERSION 2.8.11)
project (gradido_math C CXX)

find_path(MPFR_INCLUDES NAMES mpfr.h PATHS $ENV{GMPDIR} $ENV{MPFRDIR}  ${INCLUDE_INSTALL_DIR})

add_library (gradido_math STATIC src/gradidomath.c src/gradidomath.h)
target_link_libraries(gradido_math)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (gradido_math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --------------- tests ------------------------------------
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_library(MPFR_LIBRARIES mpfr  PATHS $ENV{GMPDIR} $ENV{MPFRDIR} ${LIB_INSTALL_DIR})

add_executable(test src/test.cpp)
target_compile_definitions(test PUBLIC "_TEST_BUILD")

target_link_libraries(test ${CONAN_LIBS} gradido_math  ${MPFR_LIBRARIES} )

add_test(NAME main COMMAND test)