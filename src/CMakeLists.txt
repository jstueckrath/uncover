cmake_minimum_required(VERSION 2.8)
project(WSTSAnalysisProject)
set(CMAKE_VERBOSE_MAKEFILE on)

# configure build depending on compilers
if(CMAKE_COMPILER_IS_GNUCXX)

  message(STATUS "Compiler identified as GNU compiler.")
  set(MY_RELEASE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -std=c++0x")
  set(MY_DEBUG_FLAGS "-g -Wall -std=c++0x")

# WARNING the intel compiler is currently not capable of compiling uncover, because of C++11 compatibility
elseif(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") AND UNIX)

  message(STATUS "Compiler identified as Intel compiler on a UNIX-like OS.")
  # the gxx-name parameter is needed as a fix to prevent a problem of finding header files of higher g++ versions
  # the fix is by adding : -gxx-name=g++-4.6
  # also a possible fix could be: -I/usr/include/x86_64-linux-gnu/c++/4.8/
  set(MY_RELEASE_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/x86_64-linux-gnu/c++/4.8/ -O3 -Wall -std=c++0x")
  set(MY_DEBUG_FLAGS "-g -Wall -std=c++0x")

elseif(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND UNIX)

  message(STATUS "Compiler identified as Clang on a UNIX-like OS.")
  set(MY_RELEASE_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -std=c++11")
  set(MY_DEBUG_FLAGS "-g -O0 -Wall -std=c++0x")

elseif(MSVC)

  message(WARNING "Compiler identified as Microsoft Visual C++ compiler. No flags specified yet!")

else()

  message(WARNING "Unknown compiler! Build will fail if C++11 is not enabled.")

endif()

# remove all standard compiler flags
set(CMAKE_CXX_FLAGS "")

# search and load shared Boost libraries
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS system filesystem program_options regex unit_test_framework)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Could not find Boost libraries!")
endif()
set(BOOSTLIB_FOR_MAIN ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})
set(BOOSTLIB_FOR_TESTS ${BOOSTLIB_FOR_MAIN} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
message(STATUS "All shared Boost libraries found.")

# search and load static Boost libraries
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system filesystem program_options regex)
if(NOT Boost_FOUND)
  message(WARNING "Could not find static Boost libraries. Only shared libraries are useable!")
endif()
if(Boost_FOUND)
  message(STATUS "All static Boost libraries found.")
endif()
set(BOOSTLIB_FOR_MAIN_STATIC ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# search and load shared Xerces library
find_library(Xerces_LIB xerces-c)
if(Xerces_LIB)
  message(STATUS "Shared Xerces-C++ library was found properly.")
endif()
if(NOT Xerces_LIB)
  message(FATAL_ERROR "Shared Xerces-C++ library was not found!")
endif()

# search and load static Xerces library
find_library(Xerces_LIB_STATIC NAMES libxerces-c.a)
if(Xerces_LIB_STATIC)
  # check if Xerces dependencies also exist
  find_library(Xerces_STATIC_DEP NAMES pthread)
  set(Xerces_LIB_STATIC ${Xerces_LIB_STATIC} ${Xerces_STATIC_DEP})
    if(Xerces_STATIC_DEP)
    message(STATUS "Static Xerces-C++ library was found properly.")
  endif()
  if(NOT Xerces_STATIC_DEP)
    message(WARNING "Static Xerces-C++ library was found, but its (shared) dependency 'pthread' was not.")
  endif()
endif()
if(NOT Xerces_LIB_STATIC)
  message(WARNING "Static Xerces-C++ library was not found. Only shared library may be used!")
endif()

# this automatically includes all files, but cmake must be called manually if new files where added
file(GLOB_RECURSE SOURCES "*.h" "*.cpp")

# TODO Here the include path of Xerces-C is missing, but this seems to be no problem
include_directories(${Boost_INCLUDE_DIRS})

# generate main executable
set(EXECNAME_MAIN uncover)
add_executable(${EXECNAME_MAIN} ${SOURCES})
target_link_libraries(${EXECNAME_MAIN} ${BOOSTLIB_FOR_MAIN} ${Xerces_LIB})
set_target_properties(${EXECNAME_MAIN} PROPERTIES COMPILE_FLAGS ${MY_RELEASE_FLAGS})

# generate main executable, statically linking Boost and Xerces
set(EXECNAME_MAIN_STATIC uncover_s)
add_executable(${EXECNAME_MAIN_STATIC} EXCLUDE_FROM_ALL ${SOURCES})
target_link_libraries(${EXECNAME_MAIN_STATIC} ${BOOSTLIB_FOR_MAIN_STATIC} ${Xerces_LIB_STATIC})
set_target_properties(${EXECNAME_MAIN_STATIC} PROPERTIES COMPILE_FLAGS ${MY_RELEASE_FLAGS})

# generate executable for unit tests
set(EXECNAME_TEST uncover_tests)
add_executable(${EXECNAME_TEST} EXCLUDE_FROM_ALL ${SOURCES})
target_link_libraries(${EXECNAME_TEST} ${BOOSTLIB_FOR_TESTS} ${Xerces_LIB})
set_target_properties(${EXECNAME_TEST} PROPERTIES COMPILE_FLAGS ${MY_DEBUG_FLAGS})
set_target_properties(${EXECNAME_TEST} PROPERTIES COMPILE_DEFINITIONS "COMPILE_UNIT_TESTS")

# generate executable for debugging
set(EXECNAME_DEBUG uncover_debug)
add_executable(${EXECNAME_DEBUG} EXCLUDE_FROM_ALL ${SOURCES})
target_link_libraries(${EXECNAME_DEBUG} ${BOOSTLIB_FOR_MAIN} ${Xerces_LIB})
set_target_properties(${EXECNAME_DEBUG} PROPERTIES COMPILE_FLAGS ${MY_DEBUG_FLAGS})
