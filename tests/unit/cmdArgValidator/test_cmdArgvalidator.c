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
FAKE_VALUE_FUNC(size_t, configuratorGetActiveLedCount);
FAKE_VALUE_FUNC(size_t, configuratorGetMaxSectionCount);
FAKE_VALUE_FUNC(size_t, configuratorGetSectionCount);
FAKE_VALUE_FUNC(int, configuratorGetSection, size_t, LedSection_t**);

/**
 * @brief The section count for the tests.
 */
#define TEST_SECTION_COUNT        4

struct cmdArgValidator_suite_fixture
{
  LedSection_t sections[TEST_SECTION_COUNT];
};

static void *cmArgValidatorSuiteSetup(void)
{
  struct cmdArgValidator_suite_fixture *fixture;

  fixture = k_malloc(sizeof(struct cmdArgValidator_suite_fixture));

  return (void *)fixture;
}

static void cmdArgValidatorCaseSetup(void *f)
{
  struct cmdArgValidator_suite_fixture *fixture =
    (struct cmdArgValidator_suite_fixture *)f;
  size_t ledCount = 3;

  for(uint32_t i = 0; i < TEST_SECTION_COUNT; ++i)
  {
    fixture->sections[i].firstLed = (i * ledCount) + (i + 1);
    fixture->sections[i].ledCount = ledCount;
  }
}

static void cmdArgValidatorCaseTeardown(void *f)
{
  RESET_FAKE(configuratorGetMaxLedCount);
  RESET_FAKE(configuratorGetActiveLedCount);
  RESET_FAKE(configuratorGetMaxSectionCount);
  RESET_FAKE(configuratorGetSectionCount);
  RESET_FAKE(configuratorGetSection);
}

static void cmdArgValidatorSuiteTeardown(void *f)
{
  k_free(f);
}

ZTEST_SUITE(cmdArgValidator_suite, NULL, cmArgValidatorSuiteSetup,
  cmdArgValidatorCaseSetup, cmdArgValidatorCaseTeardown,
  cmdArgValidatorSuiteTeardown);

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
    zassert_equal(0, configuratorGetMaxLedCount_fake.call_count);
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

  for(uint32_t i = 0; i < ACTIVE_LED_VALID_TEST_CNT; ++i)
  {
    configuratorGetMaxLedCount_fake.return_val = maxLedCounts[i];

    zassert_false(isActiveLedCountValid(arguments[i], &ledCount));
    zassert_equal(1, configuratorGetMaxLedCount_fake.call_count);

    RESET_FAKE(configuratorGetMaxLedCount);
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

  for(uint32_t i = 0; i < ACTIVE_LED_VALID_TEST_CNT; ++i)
  {
    configuratorGetMaxLedCount_fake.return_val = maxLedCounts[i];

    zassert_true(isActiveLedCountValid(arguments[i], &ledCount));
    zassert_equal(expectedCounts[i], ledCount);
    zassert_equal(1, configuratorGetMaxLedCount_fake.call_count);

    RESET_FAKE(configuratorGetMaxLedCount);
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
    zassert_equal(0, configuratorGetSectionCount_fake.call_count);
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
    zassert_equal(1, configuratorGetSectionCount_fake.call_count);

    RESET_FAKE(configuratorGetSectionCount);
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
    zassert_equal(1, configuratorGetSectionCount_fake.call_count);

    RESET_FAKE(configuratorGetSectionCount);
  }
}

#define SECTION_CNT_VALID_TEST_CNT                  4
/**
 * @test  isSectionCountValid must return false when the section count is
 *        not a number.
 */
ZTEST(cmdArgValidator_suite, test_isSectionCountValid_noNumber)
{
  char *arguments[SECTION_CNT_VALID_TEST_CNT] = {"afefaf", "12afefwe",
                                                 "3afe4", "afe12"};
  size_t sectionCount;

  for(uint32_t i = 0; i < SECTION_CNT_VALID_TEST_CNT; ++i)
  {
    zassert_false(isSectionCountValid(arguments[i], &sectionCount));
    zassert_equal(0, configuratorGetMaxSectionCount_fake.call_count);
  }
}

/**
 * @test  isSectionCountValid must return false if the section count is
 *        greater than the max section count.
 */
ZTEST(cmdArgValidator_suite, test_isSectionCountValid_greaterThanMaxSectionCount)
{
  char *arguments[SECTION_CNT_VALID_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxSectionCounts[SECTION_CNT_VALID_TEST_CNT] = {18, 30, 99, 148};
  size_t ledCount;

  for(uint32_t i = 0; i < SECTION_CNT_VALID_TEST_CNT; ++i)
  {
    configuratorGetMaxSectionCount_fake.return_val = maxSectionCounts[i];

    zassert_false(isSectionCountValid(arguments[i], &ledCount));
    zassert_equal(1, configuratorGetMaxSectionCount_fake.call_count);

    RESET_FAKE(configuratorGetMaxSectionCount);
  }
}

/**
 * @test  isSectionCountValid must return true and the converted LED count if
 *        this one is valid.
 */
ZTEST(cmdArgValidator_suite, test_isSectionCountValid_valid)
{
  char *arguments[SECTION_CNT_VALID_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxSectionCounts[SECTION_CNT_VALID_TEST_CNT] = {20, 50, 150, 151};
  size_t expectedCounts[SECTION_CNT_VALID_TEST_CNT] = {19, 45, 100, 150};
  size_t ledCount;

  for(uint32_t i = 0; i < SECTION_CNT_VALID_TEST_CNT; ++i)
  {
    configuratorGetMaxSectionCount_fake.return_val = maxSectionCounts[i];

    zassert_true(isSectionCountValid(arguments[i], &ledCount));
    zassert_equal(expectedCounts[i], ledCount);
    zassert_equal(1, configuratorGetMaxSectionCount_fake.call_count);

    RESET_FAKE(configuratorGetMaxSectionCount);
  }
}

#define SECTION_LED_VALID_TEST_COUNT                4
/**
 * @test  isSectionLedsValid must return false when the section LEDs info are
 *        not a number.
 */
ZTEST(cmdArgValidator_suite, test_isSectionLedsValid_noNumber)
{
  char *args[SECTION_LED_VALID_TEST_COUNT][2] = {{"afefaf", "10"},
                                                 {"10", "12afefwe"},
                                                 {"3afe4", "10"},
                                                 {"10", "afe12"}};
  size_t firstLed;
  size_t ledCount;

  for(uint32_t i = 0; i < SECTION_LED_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isSectionLedsValid(args[i], &firstLed, &ledCount));
    zassert_equal(0, configuratorGetActiveLedCount_fake.call_count);
    zassert_equal(0, configuratorGetSectionCount_fake.call_count);
    zassert_equal(0, configuratorGetSection_fake.call_count);
  }
}

/**
 * @test  isSectionLedsValid must return false when the get section operation
 *        fails.
 */
ZTEST_F(cmdArgValidator_suite, test_isSectionLedsValid_getSectionFail)
{
  char *args[2] = {"15", "2"};
  int retVals[TEST_SECTION_COUNT] = {0, 0, 0, 0};
  size_t activeLedCount = 20;
  size_t firstLed;
  size_t ledCount;

  int customFake(size_t idx, LedSection_t **section)
  {
    *section = fixture->sections + idx;
    return retVals[idx];
  }

  for(uint32_t i = 0; i < TEST_SECTION_COUNT; ++i)
  {
    configuratorGetActiveLedCount_fake.return_val = activeLedCount;
    configuratorGetSectionCount_fake.return_val = TEST_SECTION_COUNT;

    if(i > 0)
      retVals[i - 1] = 0;

    retVals[i] = -EINVAL;
    configuratorGetSection_fake.custom_fake = customFake;

    zassert_false(isSectionLedsValid(args, &firstLed, &ledCount));
    zassert_equal(1, configuratorGetActiveLedCount_fake.call_count);
    zassert_equal(1, configuratorGetSectionCount_fake.call_count);
    zassert_equal(i + 1, configuratorGetSection_fake.call_count);

    RESET_FAKE(configuratorGetActiveLedCount);
    RESET_FAKE(configuratorGetSectionCount);
    RESET_FAKE(configuratorGetSection);
  }
}

/**
 * @test  isSectionLedsValid must return false if the section would get out of
 *        the active LED range.
 */
ZTEST(cmdArgValidator_suite, test_isSectionLedsValid_outOfActiveLedRange)
{
  char *args[SECTION_LED_VALID_TEST_COUNT][2] = {{"3", "10"}, {"100", "1"},
                                                 {"50", "51"}, {"175", "100"}};
  size_t activeLedCounts[SECTION_LED_VALID_TEST_COUNT] = {12, 46, 100, 200};
  size_t firstLed;
  size_t ledCount;

  for(uint32_t i = 0; i < SECTION_LED_VALID_TEST_COUNT; ++i)
  {
    configuratorGetActiveLedCount_fake.return_val = activeLedCounts[i];

    zassert_false(isSectionLedsValid( args[i], &firstLed, &ledCount));
    zassert_equal(1, configuratorGetActiveLedCount_fake.call_count);
    zassert_equal(0, configuratorGetSectionCount_fake.call_count);
    zassert_equal(0, configuratorGetSection_fake.call_count);

    RESET_FAKE(configuratorGetActiveLedCount);
    RESET_FAKE(configuratorGetSectionCount);
    RESET_FAKE(configuratorGetSection);

  }
}

/**
 * @test  isSectionLedsValid must return false if the section would get inside
 *        another section.
 */
ZTEST_F(cmdArgValidator_suite, test_isSectionLedsValid_insideOtherSection)
{
  int successRet = 0;
  char *args[SECTION_LED_VALID_TEST_COUNT][2] = {{"1", "3"}, {"7", "3"},
                                                 {"1", "5"}, {"11", "5"}};
  size_t activeLedCount = 100;
  size_t getSectionCallCounts[SECTION_LED_VALID_TEST_COUNT] = {1, 2, 1, 3};
  size_t firstLed;
  size_t ledCount;

  int customFake(size_t idx, LedSection_t **section)
  {
    *section = fixture->sections + idx;
    return successRet;
  }

  for(uint32_t i = 0; i < SECTION_LED_VALID_TEST_COUNT; ++i)
  {
    configuratorGetActiveLedCount_fake.return_val = activeLedCount;
    configuratorGetSectionCount_fake.return_val = TEST_SECTION_COUNT;

    configuratorGetSection_fake.custom_fake = customFake;

    zassert_false(isSectionLedsValid(args[i], &firstLed, &ledCount));
    zassert_equal(1, configuratorGetActiveLedCount_fake.call_count);
    zassert_equal(1, configuratorGetSectionCount_fake.call_count);
    zassert_equal(getSectionCallCounts[i],
      configuratorGetSection_fake.call_count);

    RESET_FAKE(configuratorGetActiveLedCount);
    RESET_FAKE(configuratorGetSectionCount);
    RESET_FAKE(configuratorGetSection);
  }
}

/**
 * @test  isSectionLedsValid must return true when the section LED info is
 *        valid.
 */
ZTEST_F(cmdArgValidator_suite, test_isSectionLedsValid_success)
{
  int successRet = 0;
  char *args[SECTION_LED_VALID_TEST_COUNT][2] = {{"16", "2"}, {"4", "1"},
                                                 {"17", "3"}, {"8", "1"}};
  size_t activeLedCount = 20;
  size_t firstLed;
  size_t ledCount;

  int customFake(size_t idx, LedSection_t **section)
  {
    *section = fixture->sections + idx;
    return successRet;
  }

  for(uint32_t i = 0; i < SECTION_LED_VALID_TEST_COUNT; ++i)
  {
    configuratorGetActiveLedCount_fake.return_val = activeLedCount;
    configuratorGetSectionCount_fake.return_val = TEST_SECTION_COUNT;

    configuratorGetSection_fake.custom_fake = customFake;

    zassert_true(isSectionLedsValid(args[i], &firstLed, &ledCount));
    zassert_equal(1, configuratorGetActiveLedCount_fake.call_count);
    zassert_equal(1, configuratorGetSectionCount_fake.call_count);
    zassert_equal(TEST_SECTION_COUNT, configuratorGetSection_fake.call_count);

    RESET_FAKE(configuratorGetActiveLedCount);
    RESET_FAKE(configuratorGetSectionCount);
    RESET_FAKE(configuratorGetSection);
  }
}

#define SECTION_SW_VALID_TEST_COUNT                 4
/**
 * @test  isSectionSwitchValid must return false when the switch ID is
 *        not a number.
 */
ZTEST(cmdArgValidator_suite, test_isSectionSwitchValid_noNumber)
{
  char *args[SECTION_SW_VALID_TEST_COUNT] = {"afefaf", "12afefwe",
                                             "3afe4", "afe12"};
  size_t switchId;

  for(uint32_t i = 0; i < SECTION_SW_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isSectionSwitchValid(args[i], &switchId));
  }
}

/**
 * @test  isSectionSwitchValid must return false when the switch ID is
 *        out side of the switches range.
 */
ZTEST(cmdArgValidator_suite, test_isSectionSwitchValid_outOfRange)
{
  char *args[SECTION_SW_VALID_TEST_COUNT] = {"3", "12", "34", "120"};
  size_t switchId;

  for(uint32_t i = 0; i < SECTION_SW_VALID_TEST_COUNT; ++i)
  {
    zassert_false(isSectionSwitchValid(args[i], &switchId));
  }
}

/**
 * @test  isSectionSwitchValid must return true and the converted switch ID
 *        when it's a valid.
 */
ZTEST(cmdArgValidator_suite, test_isSectionSwitchValid_success)
{
  char *args[SECTION_SW_VALID_TEST_COUNT] = {"0", "1", "2", "0"};
  size_t expectedSwitchIds[SECTION_SW_VALID_TEST_COUNT] = {0, 1, 2, 0};
  size_t switchId;

  for(uint32_t i = 0; i < SECTION_SW_VALID_TEST_COUNT; ++i)
  {
    zassert_true(isSectionSwitchValid(args[i], &switchId));
    zassert_equal(expectedSwitchIds[i], switchId);
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
