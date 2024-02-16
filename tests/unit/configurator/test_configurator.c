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
 * @test  colorMngrSetSingle must set the pixels to the sequence color.
*/
ZTEST(configurator_suite, test_colorMngrSetSingle_SetColor)
{
  zassert_false(0);
}

/** @} */
