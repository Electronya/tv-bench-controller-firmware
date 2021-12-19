# Macro that list all the source file in the project
macro(listSources rootDir result)
  file(GLOB_RECURSE sourceList ${rootDir}/*.c)
  set(${result} ${sourceList})
endmacro()

# Macro that list all the include directory in the project
macro(listIncludesDir rootDir result)
  file(GLOB children RELATIVE ${rootDir} ${rootDir}/*)
  set(dirList "")
  foreach(child ${children})
    if(IS_DIRECTORY ${rootDir}/${child})
      list(APPEND dirList ${rootDir}/${child})
    endif()
  endforeach()
  set(${result} ${dirList})
endmacro()

# Macro that generate the source and include files list for the desired test suite
macro(getFileListForSuite sourceList includeList)
  set(testSrc "")
  set(mockSrc "")
  set(modSrc "")
  set(testInc "")
  set(mockInc "")
  set(modInc "")
  # List files and dirs for the test suite
  if(TEST_SUITE STREQUAL "utils")
    listSources(${CMAKE_CURRENT_SOURCE_DIR}/tests/Utils testSrc)
    listSources(${CMAKE_CURRENT_SOURCE_DIR}/src/Utils modSrc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/tests testInc)
    listIncludesDir(${CMAKE_CURRENT_SOURCE_DIR}/src modInc)
  endif()

  list(APPEND testSrc ${mockSrc} ${modSrc})
  list(APPEND testInc ${modInc})

  set(${sourceList} ${testSrc})
  set(${includeList} ${testInc})
endmacro()
