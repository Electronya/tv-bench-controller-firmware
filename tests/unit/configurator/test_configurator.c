/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_configurator.h
 * @author    jbacon
 * @date      2024-02-13
 * @brief     Configurator Module Test Cases
 *
 *            This file is the test cases of the configurator module.
 *
 * @ingroup  configurator
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "configurator.h"
#include "configurator.c"

DEFINE_FFF_GLOBALS;

/**
 * @brief The test maximum LED count
*/
#define TEST_MAX_LED_COUNT                  18

/**
 * @brief The test active LED count.
*/
#define TEST_ACTIVE_LED                     12

/**
 * @brief The test section count.
 */
#define TEST_SECTION_COUNT                  4

static void configuratorCaseSetup(void *f)
{
  size_t startLed;
  size_t ledPerSection;

  memset(&config, 0, sizeof(Configuration_t));
  config.maxLedCount = TEST_MAX_LED_COUNT;

  config.isReady = true;
  config.dynamicConfig.activeLedCount = TEST_ACTIVE_LED;
  config.dynamicConfig.sectionCount = TEST_SECTION_COUNT;
  ledPerSection = config.dynamicConfig.activeLedCount /
    config.dynamicConfig.sectionCount;
  for(size_t i = 0; i < config.dynamicConfig.sectionCount; ++i)
  {
    startLed = i * ledPerSection;
    config.dynamicConfig.sections[i].firstLed = startLed;
    config.dynamicConfig.sections[i].lastLed = startLed + ledPerSection - 1;
    config.dynamicConfig.sections[i].switchId = i;
    memset(&(config.dynamicConfig.sections[i].switchSeq), i,
      sizeof(LedSequence_t));
  }
}

ZTEST_SUITE(configurator_suite, NULL, NULL, configuratorCaseSetup, NULL, NULL);

/**
 * @test  configuratorIsReady must return false when the configuration was
 *        not yet loaded by the Pi and the configuration is not ready to
 *        be used.
*/
ZTEST(configurator_suite, test_configuratorIsReady_NotReady)
{
  config.isReady = false;

  zassert_false(configuratorIsReady());
}

/**
 * @test  configuratorIsReady must return true when the configuration was
 *        loaded by the Pi and the configuration is ready to be used.
*/
ZTEST(configurator_suite, test_configuratorIsReady_Ready)
{
  zassert_true(configuratorIsReady());
}

/**
 * @test  configuratorSetAsReady must set the configuration as ready.
*/
ZTEST(configurator_suite, test_configuratorSetAsReady_SetAsReady)
{
  config.isReady = false;

  configuratorSetAsReady();

  zassert_true(config.isReady);
}

/**
 * @test  configuratorGetMaxLedCount must return the maximal number of LED
 *        in the strip.
*/
ZTEST(configurator_suite, test_configuratorGetMaxLedCount_MaxLedCount)
{
  zassert_equal(TEST_MAX_LED_COUNT, configuratorGetMaxLedCount());
}

/**
 * @test  configuratorGetActiveLedCount must return an operation not permitted
 *        code if the configuration is not ready.
*/
ZTEST(configurator_suite, test_configuratorGetActiveLedCount_NotReady)
{
  int failRet = -EPERM;
  size_t activeLedCount;

  config.isReady = false;

  zassert_equal(failRet, configuratorGetActiveLedCount(&activeLedCount));
}

/**
 * @test  configuratorGetActiveLedCount must return the success code and the
 *        active LED count in the strip if the configuration is ready.
*/
ZTEST(configurator_suite, test_configuratorGetActiveLedCount_Ready)
{
  int successRet = 0;
  size_t activeLedCount = 0;

  zassert_equal(successRet, configuratorGetActiveLedCount(&activeLedCount));
  zassert_equal(TEST_ACTIVE_LED, activeLedCount);
}

#define ACTIVE_LED_COUNT_TEST_COUNT                   2
/**
 * @test  configuratorSetActiveLedCount must return an invalid parameter
 *        error when the requested active LED is greater than the
 *        max LED count or is 0.
*/
ZTEST(configurator_suite, test_configuratorSetActiveLedCount_BadLedCount)
{
  int failRet = -EINVAL;
  size_t ledCounts[ACTIVE_LED_COUNT_TEST_COUNT] = {0, TEST_MAX_LED_COUNT + 1};

  for(size_t i = 0; i < ACTIVE_LED_COUNT_TEST_COUNT; ++i)
    zassert_equal(failRet, configuratorSetActiveLedCount(ledCounts[i]));
}

/**
 * @test  configuratorSetActiveLedCount must return success and save the new
 *        active LED count when it's in range.
*/
ZTEST(configurator_suite, test_configuratorSetActiveLedCount_Success)
{
  int successRet = 0;
  size_t ledCounts[ACTIVE_LED_COUNT_TEST_COUNT] = {1, TEST_MAX_LED_COUNT};

  for(size_t i = 0; i < ACTIVE_LED_COUNT_TEST_COUNT; ++i)
  {
    zassert_equal(successRet, configuratorSetActiveLedCount(ledCounts[i]));
    zassert_equal(ledCounts[i], config.dynamicConfig.activeLedCount);
  }
}

/**
 * @test  configuratorGetMaxSectionCount must return the maximum allowed section
 *        count.
 */
ZTEST(configurator_suite, test_configuratorGetMaxSectionCount_MaxCount)
{
  zassert_equal(10, configuratorGetMaxSectionCount());
}

/**
 * @test  configuratorGetSectionCount must return an operation not permitted
 *        code if the configuration is not ready.
 */
ZTEST(configurator_suite, test_configuratorGetSectionCount_NotReady)
{
  int failRet = -EPERM;
  size_t sectionCount;

  config.isReady = false;

  zassert_equal(failRet, configuratorGetSectionCount(&sectionCount));
}

/**
 * @test  configuratorGetSectionCount must return the success code and the
 *        current section count when the operation succeeds.
 */
ZTEST(configurator_suite, test_configuratorGetSectionCount_SectionCount)
{
  int successRet = 0;
  size_t sectionCount;

  zassert_equal(successRet, configuratorGetSectionCount(&sectionCount));
  zassert_equal(TEST_SECTION_COUNT, sectionCount);
}

#define SECTION_COUNT_TEST_COUNT                      2
/**
 * @test  configuratorSetSectionCount must return an invalid parameter
 *        error when the new section count is 0 or greater than the max
 *        section count.
*/
ZTEST(configurator_suite, test_configuratorSetSectionCount_BadSectionCount)
{
  int failRet = -EINVAL;
  size_t sectionCounts[SECTION_COUNT_TEST_COUNT] = {0, MAX_SECTION_COUNT + 1};

  for(size_t i = 0; i < SECTION_COUNT_TEST_COUNT; ++i)
    zassert_equal(failRet, configuratorSetSectionCount(sectionCounts[i]));
}

/**
 * @test  configuratorSetSectionCount must return success and save the new
 *        section count when it's in range.
*/
ZTEST(configurator_suite, test_configuratorSetSectionCount_Success)
{
  int successRet = 0;
  size_t sectionCounts[SECTION_COUNT_TEST_COUNT] = {1, MAX_SECTION_COUNT};

  for(size_t i = 0; i < ACTIVE_LED_COUNT_TEST_COUNT; ++i)
  {
    zassert_equal(successRet, configuratorSetSectionCount(sectionCounts[i]));
    zassert_equal(sectionCounts[i], config.dynamicConfig.sectionCount);
  }
}

/**
 * @test  configuratorGetSection must return an operation not permitted
 *        if the configuration is not ready.
*/
ZTEST(configurator_suite, test_configuratorGetSection_NotReady)
{
  int failRet = -EPERM;
  LedSection_t *section = NULL;

  config.isReady = false;

  zassert_equal(failRet, configuratorGetSection(0, &section));
}

#define GET_SECTION_ERROR_TEST_CNT                  2
/**
 * @test  configuratorGetSection must return an invalid parameter error
 *        when the requested section does not exist.
*/
ZTEST(configurator_suite, test_configuratorGetSection_BadSection)
{
  int failRet = -EINVAL;
  LedSection_t *section = NULL;
  size_t sectionIdxes[GET_SECTION_ERROR_TEST_CNT] =
    {config.dynamicConfig.sectionCount, config.dynamicConfig.sectionCount + 10};

  for(size_t i = 0; i < GET_SECTION_ERROR_TEST_CNT; i++)
    zassert_equal(failRet, configuratorGetSection(sectionIdxes[i], &section));
}

/**
 * @test  configuratorGetSection must return the success code and the requested
 *        LED section configuration.
*/
ZTEST(configurator_suite, test_configuratorGetSection_Success)
{
  int successRet = 0;
  LedSection_t *section = NULL;

  for( size_t i = 0; i < config.dynamicConfig.sectionCount; i++)
  {
    zassert_equal(successRet, configuratorGetSection(i, &section));
    zassert_equal(config.dynamicConfig.sections + i, section);
  }
}

#define SET_SECTION_ERROR_TEST_COUNT                  3
/**
 * @test  configuratorSetSectionConfig must return an operation not
 *        permitted of the sextion count was not initialize first.
*/
ZTEST(configurator_suite, test_configuratorSetSectionConfig_CountNotInit)
{
  int failRet = -EPERM;
  size_t indexes[SET_SECTION_ERROR_TEST_COUNT] = {1, 6, MAX_SECTION_COUNT};
  LedSection_t section;

  config.dynamicConfig.sectionCount = 0;

  for(size_t i = 0; i < SET_SECTION_ERROR_TEST_COUNT; ++i)
  {
    zassert_equal(failRet, configuratorSetSectionConfig(indexes[i], &section));
  }
}

/**
 * @test  configuratorSetSectionConfig must return an invalid parameter of the
 *        section index is out of range.
*/
ZTEST(configurator_suite, test_configuratorSetSectionConfig_IndexOutOfRange)
{
  int failRet = -EINVAL;
  size_t sectionCounts[SET_SECTION_ERROR_TEST_COUNT] =
    {1, 4, MAX_SECTION_COUNT};
  LedSection_t section;
  size_t index;

  for(size_t i = 0; i < SET_SECTION_ERROR_TEST_COUNT; ++i)
  {
    config.dynamicConfig.sectionCount = sectionCounts[i];
    index = sectionCounts[i] + i;

    zassert_equal(failRet, configuratorSetSectionConfig(index, &section));
  }
}

#define SECTION_LED_LIMIT_TEST_CNT                    3
/**
 * @test  configuratorSetSectionConfig must return an invalid parameter if the
 *        section configuration use invalid first and last LED.
*/
ZTEST(configurator_suite, test_configuratorSetSectionConfig_BadLedLimit)
{
  int failRet = -EINVAL;
  size_t sectionIdx = 2;
  size_t firstLeds[SECTION_LED_LIMIT_TEST_CNT] =
    {config.dynamicConfig.activeLedCount + 1,
     config.dynamicConfig.sections[sectionIdx].firstLed, 10};
  size_t lastLeds[SECTION_LED_LIMIT_TEST_CNT] =
    {config.dynamicConfig.activeLedCount + 1,
     config.dynamicConfig.sections[sectionIdx].lastLed, 9};
  LedSection_t section;

  for(size_t i = 0; i < SECTION_LED_LIMIT_TEST_CNT; ++i)
  {
    section.firstLed = firstLeds[i];
    section.lastLed = lastLeds[i];

    zassert_equal(failRet, configuratorSetSectionConfig(sectionIdx, &section));
  }
}

/**
 * @test  configuratorSetSectionConfig must return an invalid parameter if the
 *        section configuration use a non existing switch.
*/
ZTEST(configurator_suite, test_configuratorSetSectionConfig_BadSwitch)
{
  int failRet = -EINVAL;
  LedSection_t section;

  section.switchId = MAX_SWITCH_COUNT;

  zassert_equal(failRet, configuratorSetSectionConfig(1, &section));
}

/**
 * @test  configuratorSetSectionConfig must return success and save the new
 *        section configuraton at the right index if the operation succeeds.
*/
ZTEST(configurator_suite, test_configuratorSetSectionConfig_Success)
{
  int successRet = 0;
  size_t sectionIdx = config.dynamicConfig.sectionCount - 1;
  LedSection_t section =
    {.firstLed = config.dynamicConfig.sections[sectionIdx].firstLed - 1,
     .lastLed = config.dynamicConfig.sections[sectionIdx].lastLed - 1,
     .switchId = 0, .switchSeq.seqType = SEQ_SOLID,
     .switchSeq.startColor.hexColor = 0xff00ff};

  zassert_equal(successRet, configuratorSetSectionConfig(sectionIdx, &section));
  zassert_equal(section.firstLed, config.dynamicConfig.sections[sectionIdx].firstLed);
  zassert_equal(section.lastLed, config.dynamicConfig.sections[sectionIdx].lastLed);
  zassert_equal(section.switchId, config.dynamicConfig.sections[sectionIdx].switchId);
  zassert_equal(section.switchSeq.seqType, config.dynamicConfig.sections[sectionIdx].switchSeq.seqType);
  zassert_equal(section.switchSeq.startColor.hexColor, config.dynamicConfig.sections[sectionIdx].switchSeq.startColor.hexColor);
}

/** @} */
