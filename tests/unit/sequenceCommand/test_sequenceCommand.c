/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_sequenceCommand.h
 * @author    jbacon
 * @date      2023-11-20
 * @brief     Sequence Command Module Test Cases
 *
 *            This file is the test cases of the sequence command module.
 *
 * @ingroup  sequenceCommand
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include <zephyr/sys/util.h>

#include "sequenceCommand.c"

#include "appMsg.h"

DEFINE_FFF_GLOBALS;

static void seqCommandCaseSetup(void *f)
{

}

ZTEST_SUITE(seqCommand_suite, NULL, NULL, seqCommandCaseSetup, NULL, NULL);

#define SECTION_CONVERT_TEST_COUNT                  3
/**
 * @test  isSectionValid must return false if the convertion fails.
*/
ZTEST(seqCommand_suite, test_isSectionValid_convertFail)
{
  uint32_t section;
  char *args[SECTION_CONVERT_TEST_COUNT] = {"a", "1a", "a1"};

  for(uint8_t i = 0; i < SECTION_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isSectionValid(args[i], &section),
      "isSectionValid failed to flag the invalidity of the secion.");
  }
}

/**
 * @test  isSectionValid must return true if the convertion succeeds and
 *        the converted value is valid.
*/
ZTEST(seqCommand_suite, test_isSectionValid_success)
{
  uint32_t section;
  char *args[SECTION_CONVERT_TEST_COUNT] = {"1", "54", "100"};
  uint32_t expectedVals[SECTION_CONVERT_TEST_COUNT] = {1, 54, 100};

  for(uint8_t i = 0; i < SECTION_CONVERT_TEST_COUNT; ++i)
  {
    zassert_true(isSectionValid(args[i], &section),
      "isSectionValid failed to flag the validity of the secion.");
    zassert_equal(expectedVals[i], section,
      "isSectionValid failed to convert the section argument.");
  }
}

#define COLOR_CONVERT_TEST_COUNT                    3
/**
 * @test  isColorValid must return false if the convertion fails.
*/
ZTEST(seqCommand_suite, test_isColorValid_convertFail)
{
  Color_t color;
  char *args[COLOR_CONVERT_TEST_COUNT] = {"oiuj", "1kj", "lk1"};

  for(uint8_t i = 0; i < COLOR_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isColorValid(args[i], &color),
      "isColorValid failed to flag the invalidity of the color.");
  }
}

/**
 * @test  isColorValid must return false if the converted color value
 *        is out of range.
*/
ZTEST(seqCommand_suite, test_isColorValid_colorOutOfRange)
{
  Color_t color;
  char *args[COLOR_CONVERT_TEST_COUNT] = {"01000000", "55deff90", "ffffffff"};

  for(uint8_t i = 0; i < COLOR_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isColorValid(args[i], &color),
      "isColorValid failed to flag the invalidity of the color.");
  }
}

/**
 * @test  isColorValid must return true if the converted color value
 *        is valid.
*/
ZTEST(seqCommand_suite, test_isColorValid_colorValid)
{
  Color_t color;
  Color_t expectedColor[COLOR_CONVERT_TEST_COUNT] = {{.hexColor = 0xffffff},
                                                     {.hexColor = 0xdeff90},
                                                     {.hexColor = 0x000000}};
  char *args[COLOR_CONVERT_TEST_COUNT] = {"ffffff", "deff90", "000000"};

  for(uint8_t i = 0; i < COLOR_CONVERT_TEST_COUNT; ++i)
  {
    zassert_true(isColorValid(args[i], &color),
      "isColorValid failed to flag the validity of the color.");
    zassert_equal(expectedColor[i].hexColor, color.hexColor,
      "isColorValid failed to set the color to the argument value.");
  }
}

/** @} */
