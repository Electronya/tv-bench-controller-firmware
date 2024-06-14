/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_configCommand.c
 * @author    jbacon
 * @date      2024-02-13
 * @brief     Configuration Command Module Test Cases
 *
 *            This file is the test cases of the configuration command module.
 *
 * @ingroup  configCommand
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "configCommand.c"

#include "configurator.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(size_t, configuratorGetMaxLedCount);
FAKE_VALUE_FUNC(int, configuratorGetActiveLedCount, size_t*);
FAKE_VALUE_FUNC(int, configuratorSetActiveLedCount, size_t);
FAKE_VALUE_FUNC(size_t, configuratorGetMaxSectionCount);
FAKE_VALUE_FUNC(int, configuratorGetSectionCount, size_t*);
FAKE_VALUE_FUNC(int, configuratorSetSectionCount, size_t);

static void configCommandCaseSetup(void *f)
{
  // TODO: implement or cleanup.
}

ZTEST_SUITE(configCommand_suite, NULL, NULL,
  configCommandCaseSetup, NULL, NULL);

#define ACTIVE_LED_TEST_CNT       4
/**
 * @test  isActiveLedCountValid must return false when the active LED count is
 *        not a number.
 */
ZTEST(configCommand_suite, test_isActiveLedCountValid_NoNumber)
{
  char *arguments[ACTIVE_LED_TEST_CNT] = {"afefaf", "12afefwe", "3afe4", "afe12"};
  size_t ledCount;

  for(uint32_t i = 0; i < ACTIVE_LED_TEST_CNT; ++i)
  {
    zassert_false(isActiveLedCountValid(arguments[i], &ledCount));
  }
}

/**
 * @test  isActiveLedCountValid must return false if the active LED count is
 *        greater than the max LED count.
 */
ZTEST(configCommand_suite, test_isActiveLedCountValid_GreaterThanMaxLedCount)
{
  char *arguments[ACTIVE_LED_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxLedCounts[ACTIVE_LED_TEST_CNT] = {18, 30, 99, 148};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxLedCount, maxLedCounts, ACTIVE_LED_TEST_CNT);

  for(uint32_t i = 0; i < ACTIVE_LED_TEST_CNT; ++i)
  {
    zassert_false(isActiveLedCountValid(arguments[i], &ledCount));
  }
}

/**
 * @test  isActiveLedCountValid must return true and the converted LED count if
 *        this one is valid.
 */
ZTEST(configCommand_suite, test_isActiveLedCountValid_Valid)
{
  char *arguments[ACTIVE_LED_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxLedCounts[ACTIVE_LED_TEST_CNT] = {20, 50, 150, 151};
  size_t expectedCounts[ACTIVE_LED_TEST_CNT] = {19, 45, 100, 150};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxLedCount, maxLedCounts, ACTIVE_LED_TEST_CNT);

  for(uint32_t i = 0; i < ACTIVE_LED_TEST_CNT; ++i)
  {
    zassert_true(isActiveLedCountValid(arguments[i], &ledCount));
    zassert_equal(expectedCounts[i], ledCount);
  }
}

#define SECTION_COUNT_TEST_CNT      4
/**
 * @test  isSectionCountValid must return false when the section count is
 *        not a number.
 */
ZTEST(configCommand_suite, test_isSectionCountValid_NoNumber)
{
  char *arguments[SECTION_COUNT_TEST_CNT] = {"afefaf", "12afefwe", "3afe4", "afe12"};
  size_t sectionCount;

  for(uint32_t i = 0; i < SECTION_COUNT_TEST_CNT; ++i)
  {
    zassert_false(isSectionCountValid(arguments[i], &sectionCount));
  }
}

/**
 * @test  isSectionCountValid must return false if the section count is
 *        greater than the max section count.
 */
ZTEST(configCommand_suite, test_isSectionCountValid_GreaterThanMaxSectionCount)
{
  char *arguments[SECTION_COUNT_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxSectionCounts[SECTION_COUNT_TEST_CNT] = {18, 30, 99, 148};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxSectionCount, maxSectionCounts, SECTION_COUNT_TEST_CNT);

  for(uint32_t i = 0; i < SECTION_COUNT_TEST_CNT; ++i)
  {
    zassert_false(isSectionCountValid(arguments[i], &ledCount));
  }
}

/**
 * @test  isSectionCountValid must return true and the converted LED count if
 *        this one is valid.
 */
ZTEST(configCommand_suite, test_isSectionCountValid_Valid)
{
  char *arguments[SECTION_COUNT_TEST_CNT] = {"19", "45", "100", "150"};
  size_t maxSectionCounts[SECTION_COUNT_TEST_CNT] = {20, 50, 150, 151};
  size_t expectedCounts[SECTION_COUNT_TEST_CNT] = {19, 45, 100, 150};
  size_t ledCount;

  SET_RETURN_SEQ(configuratorGetMaxSectionCount, maxSectionCounts, SECTION_COUNT_TEST_CNT);

  for(uint32_t i = 0; i < SECTION_COUNT_TEST_CNT; ++i)
  {
    zassert_true(isSectionCountValid(arguments[i], &ledCount));
    zassert_equal(expectedCounts[i], ledCount);
  }
}

/** @} */
