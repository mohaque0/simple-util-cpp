cmake_minimum_required (VERSION 2.8.12)

message(STATUS "CMAKE_GENERATOR : ${CMAKE_GENERATOR}")

project(UtilTest)
set(CMAKE_BUILD_TYPE Debug)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)

# Load test list from file
file(READ ${CMAKE_BINARY_DIR}/testlist.txt TESTS)
string(REGEX REPLACE  "#.*$"  ""  TESTS  ${TESTS})
string(REPLACE        "\n"    ";" TESTS  ${TESTS})

message(STATUS "TESTS : ${TESTS}")

#
# Compilation
#

add_executable(tests ${TESTS})

set_target_properties(tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
)

#
# Linking
#

message(STATUS "LINKED_LIBRARIES : ${CONAN_LIBS}")

target_link_libraries(tests ${CONAN_LIBS})

if(THREADS_HAVE_PTHREAD_ARG)
	set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_OPTIONS "-pthread")
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
	target_link_libraries(tests "${CMAKE_THREAD_LIBS_INIT}")
endif()
