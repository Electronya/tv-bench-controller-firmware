# Include dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/listSourcesAndIncludes.cmake)

# Macro that generate the source and include files list for the desired test suite
macro(getFileListForSuite sourceList includeList)
  set(testSrc "")
  set(modSrc "")
  set(testInc "")
  set(mockInc "")
  set(modInc "")
  # List files and dirs for the test suite
  if(TEST_SUITE STREQUAL "ledCtrl")
    listSources(${CMAKE_CURRENT_SOURCE_DIR}/ledCtrl testSrc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/ledCtrl testInc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/../../src modInc)
  elseif(TEST_SUITE STREQUAL "appMsg")
    listSources(${CMAKE_CURRENT_SOURCE_DIR}/appMsg testSrc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/appMsg testInc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/../../src modInc)
  elseif(TEST_SUITE STREQUAL "colorMngr")
    listSources(${CMAKE_CURRENT_SOURCE_DIR}/colorManager testSrc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/colorManager testInc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/../../src modInc)
  elseif(TEST_SUITE STREQUAL "ledMngr")
    listSources(${CMAKE_CURRENT_SOURCE_DIR}/ledManager testSrc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/ledManager testInc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/../../src modInc)
  endif()

  # message("testSrc: ${testSrc}")
  # message("testInc: ${testInc}")
  # message("modSrc: ${modSrc}")
  # message("modInc: ${modInc}")

  list(APPEND testInc ${modInc})

  set(${sourceList} ${testSrc})
  set(${includeList} ${testInc})
endmacro()
