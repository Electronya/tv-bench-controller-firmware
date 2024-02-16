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

static void configuratorCaseSetup(void *f)
{

}

ZTEST_SUITE(configurator_suite, NULL, NULL, configuratorCaseSetup,
  NULL, NULL);

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

/** @} */
