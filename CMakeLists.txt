cmake_minimum_required(VERSION 2.6.2)
if (NOT(CMAKE_MAJOR_VERSION LESS 3))
  cmake_policy(SET CMP0048 OLD)
endif ()
project(BeamLaser C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

enable_testing()
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include(CheckCCompilerFlag)

find_package(SPRNG REQUIRED)

option(BL_BUILD_TESTS "Whether to build tests" OFF)
option(BL_BUILD_DOCS "Whether to build documentation" OFF)
option(BL_ENABLE_COVERAGE "Instrument tests for coverage" OFF)
option(BL_WITH_ADDRESS_SANITIZER "Whether to build with address sanitizer enabled" OFF)
option(BL_WITH_WALL_FLAGS "Whether to compile with all warning flags." ON)
option(BL_WITH_PEDANTIC_FLAGS "Whether to compile with pedantic flags." OFF)
option(BL_WITH_WARNING_AS_ERRORS "Whether to treat compiler warnings as errors." ON)
option(BL_WITH_MPI "Whether we're building with MPI support" OFF)
option(BL_WITH_GUILE "Whether to build guile bindings" OFF)

if (BL_WITH_ADDRESS_SANITIZER)
  set(OLD_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
  set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
  CHECK_C_COMPILER_FLAG(-fsanitize=address HAVE_ADDRESS_SANITIZER)
  if (HAVE_ADDRESS_SANITIZER)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
  endif ()
  set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQUIRED_FLAGS})
endif ()

if (CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  if (BL_ENABLE_COVERAGE)
    add_definitions("-ftest-coverage -fprofile-arcs")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(COV_LIBRARIES gcov)
  endif ()
  if (BL_WITH_WALL_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
  endif ()
  if (BL_WITH_PEDANTIC_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
  endif ()
  if (BL_WITH_WARNING_AS_ERRORS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
  endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
endif ()

if (BL_WITH_MPI)
  find_package(MPI REQUIRED)
  message(STATUS " >>> MPI_C_FOUND == ${MPI_C_FOUND}")
  message(STATUS " >>> MPI_C_LIBRARIES == ${MPI_C_LIBRARIES}")
  message(STATUS " >>> MPI_C_INCLUDE_PATH == ${MPI_C_INCLUDE_PATH}")
  message(STATUS " >>> MPIEXEC == ${MPIEXEC}")
  message(STATUS " >>> MPIEXEC_NUMPROC_FLAG == ${MPIEXEC_NUMPROC_FLAG}")
endif ()

add_subdirectory(src)
add_subdirectory(examples)
if (BL_WITH_GUILE)
  add_subdirectory(guile)
endif ()
if (BL_BUILD_TESTS)
  add_subdirectory(cgreen)
  add_subdirectory(tests)
endif ()
if (BL_BUILD_DOCS)
  add_subdirectory(doc)
endif ()

configure_file(BeamLaserConfig.h.in ${PROJECT_BINARY_DIR}/BeamLaserConfig.h)

