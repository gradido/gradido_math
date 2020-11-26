cmake_minimum_required (VERSION 2.8.11)
project (gradido_math C CXX)

if(WIN32)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
else()
find_path(MPFR_INCLUDES NAMES mpfr.h PATHS $ENV{GMPDIR} $ENV{MPFRDIR}  ${INCLUDE_INSTALL_DIR})
endif()

add_library (gradido_math STATIC src/gradidomath.c src/gradidomath.h)
target_link_libraries(gradido_math)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (gradido_math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --------------- tests ------------------------------------
if(WIN32)

else()
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

find_library(MPFR_LIBRARIES mpfr  PATHS $ENV{GMPDIR} $ENV{MPFRDIR} ${LIB_INSTALL_DIR})
#find_package(GTest CONFIG REQUIRED)
endif()



add_executable(test src/test.cpp)
target_compile_definitions(test PUBLIC "_TEST_BUILD")
if(WIN32)
target_link_libraries(test gradido_math ${CONAN_LIBS} ) 
else()
target_link_libraries(test gtest_main gradido_math  ${MPFR_LIBRARIES} )
endif()


add_test(NAME main COMMAND test)
