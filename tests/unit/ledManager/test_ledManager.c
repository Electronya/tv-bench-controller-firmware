/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_ledManager.h
 * @author    jbacon
 * @date      2023-11-20
 * @brief     Led Manager Module Test Cases
 *
 *            This file is the test cases of the LED manager module.
 *
 * @ingroup  ledManager
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "ledManager.h"
#include "ledManager.c"

#include "zephyrCommon.h"
#include "zephyrThread.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(zephyrThreadCreate, ZephyrThread_t*, char*, uint32_t,
  ZephyrTimeUnit_t);

static void ledMngrCaseSetup(void *f)
{
  RESET_FAKE(zephyrThreadCreate);
}

ZTEST_SUITE(ledMngr_suite, NULL, NULL, ledMngrCaseSetup, NULL, NULL);

/**
 * @test  ledMngrInit must create the thread.
*/
ZTEST(ledMngr_suite, test_ledMngrInit_CreateThread)
{
  ledMngrInit();

  zassert_equal(1, zephyrThreadCreate_fake.call_count);
  zassert_equal(&thread, zephyrThreadCreate_fake.arg0_val);
  zassert_equal(LED_MNGR_THREAD_NAME, zephyrThreadCreate_fake.arg1_val);
  zassert_equal(ZEPHYR_TIME_NO_WAIT, zephyrThreadCreate_fake.arg2_val);
  zassert_equal(MILLI_SEC, zephyrThreadCreate_fake.arg3_val);
}

/** @} */
