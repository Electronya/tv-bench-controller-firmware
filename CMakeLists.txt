# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)

# Adding common sources files
list(APPEND SRC
  src/appCmd/appCmd.c
)

# Adding common include paths
list(APPEND INC
  src
)

if(NOT DEFINED ENYA_ENV)
  set(ENYA_ENV "prod")
endif()

# Setting up prod build environment
if(ENYA_ENV STREQUAL "prod")
  # Set board
  set(BOARD "stm32f072b_disco")

  list(APPEND SRC
    src/main.c
  )

# Setting up test build environment
elseif(ENYA_ENV STREQUAL "test")
  # Set board
  set(BOARD "native_posix")

  list(APPEND SRC
    test/unity/unity.c
    test/test_main.c
  )
  list(APPEND INC
    test
  )
  add_definitions(-DUNITY_EXCLUDE_SETJMP_H)
endif()

# Set Zephyr environment
find_package(Zephyr)
project(tv_bench_ctlr_coprocessor)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
target_sources(app PRIVATE ${SRC})
target_include_directories(app PRIVATE ${INC})
