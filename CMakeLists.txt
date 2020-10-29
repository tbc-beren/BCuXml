#
# === Coverage ===============================================================
# cmake .. -DBCUXML_TESTCOVERAGE=1
# rm *.info
# rm -rf cov-results
# lcov -t "result" -o coverage_01.info -c -d .
# lcov --remove coverage_01.info -o coverage.info \
#     '/usr/include/*' \
#     '*/googletest-src/*'
# genhtml -o cov-results coverage.info
#
#
# === Build Debug ============================================================
# cmake .. -DCMAKE_BUILD_TYPE=Debug
#
cmake_minimum_required (VERSION 3.8)

# Set coverage options when needed
if (DEFINED BCUXML_TESTCOVERAGE)
set(COVERAGE_OPTIONS "--coverage")
endif (DEFINED BCUXML_TESTCOVERAGE)


set (CMAKE_CXX_STANDARD 11)

project (BCuXml CXX)
add_library(BCuXml STATIC
  src/BCuXmlNode.cpp
  )
target_include_directories(BCuXml PUBLIC include/)
target_compile_options(BCuXml PRIVATE ${COVERAGE_OPTIONS})
target_link_libraries(BCuXml ${COVERAGE_OPTIONS})






# Download and unpack googletest at configure time
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


enable_testing()
add_executable(
    BCuXmlTest

    # Useful testing tools
    #test/testTools.cpp

    # The tests
    test/testNode.cpp
    )
target_link_libraries(BCuXmlTest gtest_main)
target_link_libraries(BCuXmlTest BCuXml ${COVERAGE_OPTIONS})
if(MSVC)
  target_compile_options(BCuXmlTest PRIVATE /W4 /WX)
else()
  target_compile_options(BCuXmlTest PRIVATE -Wall -Wextra -pedantic -Werror ${COVERAGE_OPTIONS})
endif()
add_test(NAME BCuXmlTestName COMMAND BCuXmlTest)

# Coverage Update target
if (DEFINED BCUXML_TESTCOVERAGE)
add_custom_command(
  OUTPUT BCCoverage
  COMMENT "BCCoverage Command"
  COMMAND rm -f *.info
  COMMAND rm -rf cov-results
  COMMAND ./BCuXmlTest
  COMMAND lcov -t "result" -o coverage_01.info -c -d .
  COMMAND lcov --remove coverage_01.info -o coverage.info '/usr/include/*' '*/googletest-src/*'
  COMMAND genhtml -o cov-results coverage.info
)

else ()

add_custom_command(
  OUTPUT BCCoverage
  COMMENT "BCCoverage Command"
  COMMAND echo "Coverage not set. Please rebuild with:"
  COMMAND echo "     cmake ${CMAKE_SOURCE_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBCUXML_TESTCOVERAGE=1"
)

endif (DEFINED BCUXML_TESTCOVERAGE)

add_custom_target(coverage
  DEPENDS
    BCCoverage
    BCuXmlTest
    )
