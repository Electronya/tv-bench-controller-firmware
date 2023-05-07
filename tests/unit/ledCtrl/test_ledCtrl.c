/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_ledCtrl.h
 * @author    jbacon
 * @date      2023-05-06
 * @brief     Led Control Module Test Cases
 *
 *            This file is the test cases of the LED control module.
 *
 * @ingroup  ledCtrl
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "ledCtrlHw.h"

ZTEST_SUITE(ledCtrl_suite, NULL, NULL, NULL, NULL, NULL);

ZTEST(ledCtrl_suite, test_ledCtrlIsDeviceReady)
{
  zassert_true(true);
}


/** @} */
