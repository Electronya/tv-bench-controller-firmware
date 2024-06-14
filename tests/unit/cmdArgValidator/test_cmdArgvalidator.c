/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_cmdArgValidator.h
 * @author    jbacon
 * @date      2024-06-14
 * @brief     Command Argument Validator Module Test Cases
 *
 *            This file is the test cases of the command argument
 *            validator module.
 *
 * @ingroup  cmdArgValidator
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "cmdArgValidator.h"
#include "cmdArgValidator.c"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(size_t, configuratorGetMaxLedCount);
FAKE_VALUE_FUNC(size_t, configuratorGetMaxSectionCount);
FAKE_VALUE_FUNC(size_t, configuratorGetSectionCount);

static void cmdArgValidatorCaseSetup(void *f)
{
  // TODO: clean up if not needed.
}

ZTEST_SUITE(cmdArgValidator_suite, NULL, NULL, cmdArgValidatorCaseSetup,
  NULL, NULL);

#define ACTIVE_LED_VALID_TEST_CNT                   4
/**
 * @test  isActiveLedCountValid must return false when the active LED count is
 *        not a number.
 */
ZTEST(cmdArgValidator_suite, test_isActiveLedCountValid_NoNumber)
{
  char *arguments[ACTIVE_LED_VALID_TEST_CNT] = {"afefaf", "12afefwe",
                                                "3afe4", "afe12"};
  size_t ledCount;

  for(uint32_t i = 0; i < ACTIVE_LED_VALID_TEST_CNT; ++i)
  {
    zassert_false(isActiveLedCountValid(arguments[i], &ledCount));
  }
}

/**
 * @test  isActiveLedCountValid must return false if the active LED count is
 *        greater than the max LED count.
 */
ZTEST(cmdArgValidator_suite, test_isActiveLedCountValid_GreaterThanMaxLedCount)
{
  char *arguments[ACTIVE_LED_VALID_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxLedCounts[ACTIVE_LED_VALID_TEST_CNT] = {18, 30, 99, 148};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxLedCount, maxLedCounts,
    ACTIVE_LED_VALID_TEST_CNT);

  for(uint32_t i = 0; i < ACTIVE_LED_VALID_TEST_CNT; ++i)
  {
    zassert_false(isActiveLedCountValid(arguments[i], &ledCount));
  }
}

/**
 * @test  isActiveLedCountValid must return true and the converted LED count if
 *        this one is valid.
 */
ZTEST(cmdArgValidator_suite, test_isActiveLedCountValid_Valid)
{
  char *arguments[ACTIVE_LED_VALID_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxLedCounts[ACTIVE_LED_VALID_TEST_CNT] = {20, 50, 150, 151};
  size_t expectedCounts[ACTIVE_LED_VALID_TEST_CNT] = {19, 45, 100, 150};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxLedCount, maxLedCounts,
    ACTIVE_LED_VALID_TEST_CNT);

  for(uint32_t i = 0; i < ACTIVE_LED_VALID_TEST_CNT; ++i)
  {
    zassert_true(isActiveLedCountValid(arguments[i], &ledCount));
    zassert_equal(expectedCounts[i], ledCount);
  }
}

#define SECTION_ID_VALID_TEST_COUNT                 3
/**
 * @test  isSectionIdValid must return false if the conversion fails.
*/
ZTEST(cmdArgValidator_suite, test_isSectionIdValid_convertFail)
{
  uint32_t section;
  char *args[SECTION_ID_VALID_TEST_COUNT] = {"a", "1a", "a1"};

  for(uint8_t i = 0; i < SECTION_ID_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isSectionIdValid(args[i], &section));
  }
}

/**
 * @test  isSectionIdValid must return false if the section ID is out of range.
 */
ZTEST(cmdArgValidator_suite, test_isSectionIdValid_idOutOfRange)
{
  uint32_t section;
  char *args[SECTION_ID_VALID_TEST_COUNT] = {"1", "14", "60"};
  size_t sectionCounts[SECTION_ID_VALID_TEST_COUNT] = {1, 10, 60};

  for(uint32_t i = 0; i < SECTION_ID_VALID_TEST_COUNT; ++i)
  {
    configuratorGetSectionCount_fake.return_val = sectionCounts[i];

    zassert_false(isSectionIdValid(args[i], &section));
  }
}

/**
 * @test  isSectionIdValid must return true if the conversion succeeds and
 *        the converted value is valid.
*/
ZTEST(cmdArgValidator_suite, test_isSectionIdValid_success)
{
  uint32_t section;
  char *args[SECTION_ID_VALID_TEST_COUNT] = {"1", "54", "100"};
  size_t sectionCounts[SECTION_ID_VALID_TEST_COUNT] = {2, 100, 101};
  uint32_t expectedVals[SECTION_ID_VALID_TEST_COUNT] = {1, 54, 100};

  for(uint8_t i = 0; i < SECTION_ID_VALID_TEST_COUNT; ++i)
  {
    configuratorGetSectionCount_fake.return_val = sectionCounts[i];

    zassert_true(isSectionIdValid(args[i], &section));
    zassert_equal(expectedVals[i], section);
  }
}

#define SECTION_CNT_VALID_TEST_CNT                  4
/**
 * @test  isSectionCountValid must return false when the section count is
 *        not a number.
 */
ZTEST(cmdArgValidator_suite, test_isSectionCountValid_NoNumber)
{
  char *arguments[SECTION_CNT_VALID_TEST_CNT] = {"afefaf", "12afefwe",
                                                 "3afe4", "afe12"};
  size_t sectionCount;

  for(uint32_t i = 0; i < SECTION_CNT_VALID_TEST_CNT; ++i)
  {
    zassert_false(isSectionCountValid(arguments[i], &sectionCount));
  }
}

/**
 * @test  isSectionCountValid must return false if the section count is
 *        greater than the max section count.
 */
ZTEST(cmdArgValidator_suite, test_isSectionCountValid_GreaterThanMaxSectionCount)
{
  char *arguments[SECTION_CNT_VALID_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxSectionCounts[SECTION_CNT_VALID_TEST_CNT] = {18, 30, 99, 148};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxSectionCount, maxSectionCounts,
    SECTION_CNT_VALID_TEST_CNT);

  for(uint32_t i = 0; i < SECTION_CNT_VALID_TEST_CNT; ++i)
  {
    zassert_false(isSectionCountValid(arguments[i], &ledCount));
  }
}

/**
 * @test  isSectionCountValid must return true and the converted LED count if
 *        this one is valid.
 */
ZTEST(cmdArgValidator_suite, test_isSectionCountValid_Valid)
{
  char *arguments[SECTION_CNT_VALID_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxSectionCounts[SECTION_CNT_VALID_TEST_CNT] = {20, 50, 150, 151};
  size_t expectedCounts[SECTION_CNT_VALID_TEST_CNT] = {19, 45, 100, 150};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxSectionCount, maxSectionCounts,
    SECTION_CNT_VALID_TEST_CNT);

  for(uint32_t i = 0; i < SECTION_CNT_VALID_TEST_CNT; ++i)
  {
    zassert_true(isSectionCountValid(arguments[i], &ledCount));
    zassert_equal(expectedCounts[i], ledCount);
  }
}

#define COLOR_VALID_TEST_COUNT                      3
/**
 * @test  isColorValid must return false if the conversion fails.
*/
ZTEST(cmdArgValidator_suite, test_isColorValid_convertFail)
{
  Color_t color;
  char *args[COLOR_VALID_TEST_COUNT] = {"oiuj", "1kj", "lk1"};

  for(uint8_t i = 0; i < COLOR_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isColorValid(args[i], &color));
  }
}

/**
 * @test  isColorValid must return false if the converted color value
 *        is out of range.
*/
ZTEST(cmdArgValidator_suite, test_isColorValid_colorOutOfRange)
{
  Color_t color;
  char *args[COLOR_VALID_TEST_COUNT] = {"01000000", "55deff90", "ffffffff"};

  for(uint8_t i = 0; i < COLOR_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isColorValid(args[i], &color));
  }
}

/**
 * @test  isColorValid must return true if the converted color value.
*/
ZTEST(cmdArgValidator_suite, test_isColorValid_colorValid)
{
  Color_t color;
  Color_t expectedColor[COLOR_VALID_TEST_COUNT] = {{.hexColor = 0xffffff},
                                                     {.hexColor = 0xdeff90},
                                                     {.hexColor = 0x000000}};
  char *args[COLOR_VALID_TEST_COUNT] = {"ffffff", "deff90", "000000"};

  for(uint8_t i = 0; i < COLOR_VALID_TEST_COUNT; ++i)
  {
    zassert_true(isColorValid(args[i], &color));
    zassert_equal(expectedColor[i].hexColor, color.hexColor);
  }
}

#define SEQ_LENGTH_VALID_TEST_COUNT                 3
/**
 * @test  isSeqLengthValid must return false if the conversion fails.
*/
ZTEST(cmdArgValidator_suite, test_isSeqLengthValid_convertFail)
{
  uint32_t length;
  char *args[SEQ_LENGTH_VALID_TEST_COUNT] = {"a", "1a", "a1"};

  for(uint8_t i = 0; i < SEQ_LENGTH_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isSeqLengthValid(args[i], &length));
  }
}

/**
 * @test  isSeqLengthValid must return true if the conversion succeeds and
 *        the converted value.
*/
ZTEST(cmdArgValidator_suite, test_isSeqLengthValid_success)
{
  uint32_t length;
  char *args[SEQ_LENGTH_VALID_TEST_COUNT] = {"1", "54", "100"};
  uint32_t expectedVals[SEQ_LENGTH_VALID_TEST_COUNT] = {1, 54, 100};

  for(uint8_t i = 0; i < SEQ_LENGTH_VALID_TEST_COUNT; ++i)
  {
    zassert_true(isSeqLengthValid(args[i], &length));
    zassert_equal(expectedVals[i], length);
  }
}

#define SEQ_DIRECTION_VALID_TEST_COUNT              2
/**
 * @test  isSeqDirectionValid must return false if the conversion fails.
*/
ZTEST(cmdArgValidator_suite, test_isSeqDirectionValid_convertFail)
{
  bool isInverted;
  char *args[SEQ_DIRECTION_VALID_TEST_COUNT] = {"norma", "inverte"};

  for(uint8_t i = 0; i < SEQ_DIRECTION_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isSeqDirectionValid(args[i], &isInverted));
  }
}

/**
 * @test  isSeqDirectionValid must return true if the conversion succeeds and
 *        the converted value.
*/
ZTEST(cmdArgValidator_suite, test_isSeqDirectionValid_success)
{
  bool isInverted;
  char *args[SEQ_DIRECTION_VALID_TEST_COUNT] = {"normal", "inverted"};
  bool expectedVals[SEQ_DIRECTION_VALID_TEST_COUNT] = {false, true};

  for(uint8_t i = 0; i < SEQ_DIRECTION_VALID_TEST_COUNT; ++i)
  {
    zassert_true(isSeqDirectionValid(args[i], &isInverted));
    zassert_equal(expectedVals[i], isInverted);
  }
}

/** @} */
