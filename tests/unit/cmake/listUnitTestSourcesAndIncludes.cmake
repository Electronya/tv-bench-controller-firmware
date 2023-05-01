# Include dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/listSourcesAndIncludes.cmake)

# Macro that generate the source and include files list for the desired test suite
macro(getFileListForSuite sourceList includeList)
  set(testSrc "")
  set(mockSrc "")
  set(modSrc "")
  set(testInc "")
  set(mockInc "")
  set(modInc "")
  # List files and dirs for the test suite
  if(TEST_SUITE STREQUAL "dummy")
    listSources(${CMAKE_CURRENT_SOURCE_DIR} testSrc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR} testInc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/../../src modInc)
  endif()

  list(APPEND testSrc ${mockSrc} ${modSrc})
  list(APPEND testInc ${modInc})

  set(${sourceList} ${testSrc})
  set(${includeList} ${testInc})
endmacro()
