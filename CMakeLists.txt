# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)

# If no build environment is define, use the prod one
if(NOT DEFINED BUILD_ENV)
  set(BUILD_ENV "prod")
endif()

# Setting target board
if((BUILD_ENV STREQUAL "prod") OR (BUILD_ENV STREQUAL "dev"))
  set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
  set(BOARD "nucleo_f070rb")
elseif(BUILD_ENV STREQUAL "qemu")
  set(BOARD "qemu_cortex_m0")
elseif(BUILD_ENV STREQUAL "test")
  set(BOARD "qemu_cortex_m0")
else()
  message(FATAL_ERROR "Unsupported build environment!!!")
endif()

# Set Zephyr environment
find_package(Zephyr)
project(tv_bench_ctlr_coprocessor)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/listSourcesAndIncludes.cmake)

set(SRC "")
set(INC "")

if((BUILD_ENV STREQUAL "prod") OR (BUILD_ENV STREQUAL "dev")
  OR (BUILD_ENV STREQUAL "qemu"))
  listSources(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)
  listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/src INC)
elseif(BUILD_ENV STREQUAL "test")
  getFileListForSuite(SRC INC)
endif()

message("SRC: ${SRC}")
message("INC: ${INC}")

target_sources(app PRIVATE ${SRC})
target_include_directories(app PRIVATE ${INC})
