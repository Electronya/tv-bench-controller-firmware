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

static void cmdArgValidatorCaseSetup(void *f)
{
  // TODO: clean up if not needed.
}

ZTEST_SUITE(configurator_suite, NULL, NULL, cmdArgValidatorCaseSetup,
  NULL, NULL);

/** @} */
