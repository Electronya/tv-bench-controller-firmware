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
  struct configurator_suite_fixture *fixture =
    (struct configurator_suite_fixture *)f;

  memset(&config, 0, sizeof(Configuration_t));
  config.maxLedCount = TEST_MAX_LED_COUNT;

  fixture->config.dynamicConfig.activeLedCount = 12;
  fixture->config.dynamicConfig.sectionCount = 4;
  ledPerSection = fixture->config.dynamicConfig.activeLedCount /
      fixture->config.dynamicConfig.sectionCount;
  for(uint8_t i = 0; i < fixture->config.dynamicConfig.sectionCount; ++i)
  {
    startLed = i * ledPerSection;
    fixture->config.dynamicConfig.sections[i].firstLed = startLed;
    fixture->config.dynamicConfig.sections[i].lastLed =
      startLed + ledPerSection - 1;
    fixture->config.dynamicConfig.sections[i].switchId = i;
    memset(&(fixture->config.dynamicConfig.sections[i].switchSeq), i,
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

/** @} */
