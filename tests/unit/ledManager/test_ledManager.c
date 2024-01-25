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

#include <zephyr/sys/util.h>

#include "ledManager.h"
#include "ledManager.c"

#include "appMsg.h"
#include "sequenceManager.h"
#include "zephyrCommon.h"
#include "zephyrLedStrip.h"
#include "zephyrThread.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, appMsgPopLedSequence, LedSequence_t*);
FAKE_VOID_FUNC(seqMngrUpdateSolidFrame, Color_t*, ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(seqMngrUpdateSingleBreatherFrame, Color_t*, uint8_t, bool,
  ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(seqMngrUpdateFadeChaserFrame, Color_t*, bool, bool,
  ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(seqMngrUpdateColorRangeFrame, Color_t*, Color_t*, bool,
  ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(seqMngrUpdateColorRangeChaserFrame, Color_t*, Color_t*, bool,
  bool, ZephyrRgbPixel_t*, size_t);
FAKE_VALUE_FUNC(int, zephyrLedStripInit, ZephyrLedStrip_t*, const uint32_t);
FAKE_VOID_FUNC(zephyrThreadCreate, ZephyrThread_t*, char*, uint32_t,
  ZephyrTimeUnit_t);
FAKE_VALUE_FUNC(uint32_t, zephyrThreadSleep, uint32_t, ZephyrTimeUnit_t);

static void ledMngrCaseSetup(void *f)
{
  RESET_FAKE(zephyrLedStripInit);
  RESET_FAKE(zephyrThreadCreate);
}

ZTEST_SUITE(ledMngr_suite, NULL, NULL, ledMngrCaseSetup, NULL, NULL);

/**
 * @test  ledMngrInit must return the error code if the LED stip initalization
 *        fails.
*/
ZTEST(ledMngr_suite, test_ledMngrInit_LedStripInitFail)
{
  int failRet = -EIO;

  zephyrLedStripInit_fake.return_val = failRet;

  zassert_equal(failRet, ledMngrInit(),
    "ledMngrInit failed to return the error code.");
  zassert_equal(1, zephyrLedStripInit_fake.call_count,
    "ledMngrInit failed to initalize the LED strip.");
  zassert_equal(&ledStrip, zephyrLedStripInit_fake.arg0_val,
    "ledMngrInit failed to initalize the LED strip.");
  zassert_equal(ledStrip.pixelCount, zephyrLedStripInit_fake.arg1_val,
    "ledMngrInit failed to initalize the LED strip.");
}

/**
 * @test  ledMngrInit must create the thread and return the success code when
 *        the LED strip initialization succeeds.
*/
ZTEST(ledMngr_suite, test_ledMngrInit_CreateThread)
{
  int successRet = 0;

  zephyrLedStripInit_fake.return_val = successRet;

  zassert_equal(successRet, ledMngrInit(),
    "ledMngrInit failed to return the success code.");
  zassert_equal(1, zephyrLedStripInit_fake.call_count,
    "ledMngrInit failed to initalize the LED strip.");
  zassert_equal(&ledStrip, zephyrLedStripInit_fake.arg0_val,
    "ledMngrInit failed to initalize the LED strip.");
  zassert_equal(ledStrip.pixelCount, zephyrLedStripInit_fake.arg1_val,
    "ledMngrInit failed to initalize the LED strip.");
  zassert_equal(1, zephyrThreadCreate_fake.call_count,
    "ledMngrInit failed to create and start the thread.");
  zassert_equal(&thread, zephyrThreadCreate_fake.arg0_val,
    "ledMngrInit failed to create and start the thread.");
  zassert_equal(LED_MNGR_THREAD_NAME, zephyrThreadCreate_fake.arg1_val,
    "ledMngrInit failed to create and start the thread.");
  zassert_equal(ZEPHYR_TIME_NO_WAIT, zephyrThreadCreate_fake.arg2_val,
    "ledMngrInit failed to create and start the thread.");
  zassert_equal(MILLI_SEC, zephyrThreadCreate_fake.arg3_val,
    "ledMngrInit failed to create and start the thread.");
}

/** @} */
