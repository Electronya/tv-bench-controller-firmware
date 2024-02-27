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

struct configurator_suite_fixture
{
  Configuration_t config;
};

static void *configurationSuiteSetup(void)
{
  struct configurator_suite_fixture *fixture =
    k_malloc(sizeof(struct configurator_suite_fixture));
    printk("fixture address: %p", fixture);
  zassume_not_null(fixture, NULL);

  return (void *)fixture;
}

static void configuratorUiteTeardown(void *f)
{
  k_free(f);
}

static void configuratorCaseSetup(void *f)
{
  uint8_t startLed;
  uint8_t ledPerSection;
  // TODO: clean up if not used.
  // struct configurator_suite_fixture *fixture =
  //   (struct configurator_suite_fixture *)f;

  memset(&config, 0, sizeof(Configuration_t));
  config.maxLedCount = TEST_MAX_LED_COUNT;

  config.dynamicConfig.activeLedCount = 12;
  config.dynamicConfig.sectionCount = 4;
  ledPerSection = config.dynamicConfig.activeLedCount /
    config.dynamicConfig.sectionCount;
  for(uint8_t i = 0; i < config.dynamicConfig.sectionCount; ++i)
  {
    startLed = i * ledPerSection;
    config.dynamicConfig.sections[i].firstLed = startLed;
    config.dynamicConfig.sections[i].lastLed = startLed + ledPerSection - 1;
    config.dynamicConfig.sections[i].switchId = i;
    memset(&(config.dynamicConfig.sections[i].switchSeq), i,
      sizeof(LedSequence_t));
  }
}

ZTEST_SUITE(configurator_suite, NULL, configurationSuiteSetup,
  configuratorCaseSetup, NULL, configuratorUiteTeardown);

/**
 * @test  configuratorIsReady must return false when the configuration was
 *        not yet loaded by the Pi and the configuration is not ready to
 *        be used.
*/
ZTEST(configurator_suite, test_configuratorIsReady_NotReady)
{
  zassert_false(configuratorIsReady());
}

/**
 * @test  configuratorIsReady must return true when the configuration was
 *        loaded by the Pi and the configuration is ready to be used.
*/
ZTEST(configurator_suite, test_configuratorIsReady_Ready)
{
  config.isReady = true;

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

#define ACTIVE_LED_COUNT_TEST_COUNT                   2
/**
 * @test  configuratorSetActiveLedCount must return an invalid parameter
 *        error when the requested active LED is greater than the
 *        max LED count or is 0.
*/
ZTEST(configurator_suite, test_configuratorSetActiveLedCount_BadLedCount)
{
  int failRet = -EINVAL;
  uint8_t ledCounts[ACTIVE_LED_COUNT_TEST_COUNT] = {0, TEST_MAX_LED_COUNT + 1};

  for(uint8_t i = 0; i < ACTIVE_LED_COUNT_TEST_COUNT; ++i)
    zassert_equal(failRet, configuratorSetActiveLedCount(ledCounts[i]));
}

/**
 * @test  configuratorSetActiveLedCount must return success and save the new
 *        active LED count when it's in range.
*/
ZTEST(configurator_suite, test_configuratorSetActiveLedCount_Success)
{
  int successRet = 0;
  uint8_t ledCounts[ACTIVE_LED_COUNT_TEST_COUNT] = {1, TEST_MAX_LED_COUNT};

  for(uint8_t i = 0; i < ACTIVE_LED_COUNT_TEST_COUNT; ++i)
  {
    zassert_equal(successRet, configuratorSetActiveLedCount(ledCounts[i]));
    zassert_equal(ledCounts[i], config.dynamicConfig.activeLedCount);
  }
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
  uint8_t sectionCounts[SECTION_COUNT_TEST_COUNT] = {0, MAX_SECTION_COUNT + 1};

  for(uint8_t i = 0; i < SECTION_COUNT_TEST_COUNT; ++i)
    zassert_equal(failRet, configuratorSetSectionCount(sectionCounts[i]));
}

/**
 * @test  configuratorSetSectionCount must return success and save the new
 *        section count when it's in range.
*/
ZTEST(configurator_suite, test_configuratorSetSectionCount_Success)
{
  int successRet = 0;
  uint8_t sectionCounts[SECTION_COUNT_TEST_COUNT] = {1, MAX_SECTION_COUNT};

  for(uint8_t i = 0; i < ACTIVE_LED_COUNT_TEST_COUNT; ++i)
  {
    zassert_equal(successRet, configuratorSetSectionCount(sectionCounts[i]));
    zassert_equal(sectionCounts[i], config.dynamicConfig.sectionCount);
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
  uint8_t indexes[SET_SECTION_ERROR_TEST_COUNT] = {1, 6, MAX_SECTION_COUNT};
  LedSection_t section;

  config.dynamicConfig.sectionCount = 0;

  for(uint8_t i = 0; i < SET_SECTION_ERROR_TEST_COUNT; ++i)
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
  uint8_t sectionCounts[SET_SECTION_ERROR_TEST_COUNT] =
    {1, 4, MAX_SECTION_COUNT};
  LedSection_t section;
  uint8_t index;

  for(uint8_t i = 0; i < SET_SECTION_ERROR_TEST_COUNT; ++i)
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
  uint8_t sectionIdx = 2;
  uint8_t firstLeds[SECTION_LED_LIMIT_TEST_CNT] =
    {config.dynamicConfig.activeLedCount + 1,
     config.dynamicConfig.sections[sectionIdx].firstLed, 10};
  uint8_t lastLeds[SECTION_LED_LIMIT_TEST_CNT] =
    {config.dynamicConfig.activeLedCount + 1,
     config.dynamicConfig.sections[sectionIdx].lastLed, 9};
  LedSection_t section;

  for(uint8_t i = 0; i < SECTION_LED_LIMIT_TEST_CNT; ++i)
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
  uint8_t sectionIdx = config.dynamicConfig.sectionCount - 1;
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
