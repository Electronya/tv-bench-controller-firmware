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
#include "ledCtrl.h"
#include "zephyrCommon.h"
#include "zephyrLedStrip.h"
#include "zephyrThread.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(void*, k_malloc, size_t);
FAKE_VALUE_FUNC(int, appMsgPopLedSequence, LedSequence_t*);
FAKE_VALUE_FUNC(int, ledCtrlUpdatePixels, ZephyrRgbLed_t*, size_t, size_t);
FAKE_VOID_FUNC(zephyrThreadCreate, ZephyrThread_t*, char*, uint32_t,
  ZephyrTimeUnit_t);
FAKE_VALUE_FUNC(uint32_t, zephyrThreadSleep, uint32_t, ZephyrTimeUnit_t);

static void ledMngrCaseSetup(void *f)
{
  RESET_FAKE(k_malloc);
  RESET_FAKE(ledCtrlUpdatePixels);
  RESET_FAKE(zephyrThreadCreate);
}

ZTEST_SUITE(ledMngr_suite, NULL, NULL, ledMngrCaseSetup, NULL, NULL);

/**
 * @test  processSolidSeq must return an error code when the temporary pixels
 *        allocation fails.
*/
ZTEST(ledMngr_suite, test_processSolidSeq_PixelAllocationFail)
{
  int failRet = -ENOSPC;
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  LedSequence_t seq;

  section.firstLed = firstLed;
  section.lastLed = lastLed;
  k_malloc_fake.return_val = NULL;

  zassert_equal(failRet, processSolidSeq(&seq),
    "processSolidSeq failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "processSolidSeq failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "processSolidSeq failed to allocate the required pixels.");
}

/**
 * @test  processSolidSeq must return any error code raise by the pixel
 *        update operation.
*/
ZTEST(ledMngr_suite, test_processSolidSeq_PixelUpdateFail)
{
  int failRet = -EIO;
  ZephyrRgbLed_t pixels[10];
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  LedSequence_t seq;

  section.firstLed = firstLed;
  section.lastLed = lastLed;
  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = failRet;

  zassert_equal(failRet, processSolidSeq(&seq),
    "processSolidSeq failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "processSolidSeq failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "processSolidSeq failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "processSolidSeq failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "processSolidSeq failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "processSolidSeq failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "processSolidSeq failed to update the LED pixels.");
}

/**
 * @test  processSolidSeq must set the allocated pixels to the sequence
 *        color, update the LED strip pixel and return the success code.
*/
ZTEST(ledMngr_suite, test_processSolidSeq_Sucess)
{
  int successRet = 0;
  ZephyrRgbLed_t pixels[10];
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  LedSequence_t seq;

  section.firstLed = firstLed;
  section.lastLed = lastLed;
  seq.startColor.hexColor = 0x00eeccff;
  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = successRet;

  zassert_equal(successRet, processSolidSeq(&seq),
    "processSolidSeq failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "processSolidSeq failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "processSolidSeq failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "processSolidSeq failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "processSolidSeq failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "processSolidSeq failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "processSolidSeq failed to update the LED pixels.");

  for(uint8_t i = 0; i < lastLed - firstLed; i++)
  {
    zassert_equal(0xee, pixels[i].r,
      "processSolidSeq failed to set the pixels to the sequence color.");
    zassert_equal(0xcc, pixels[i].g,
      "processSolidSeq failed to set the pixels to the sequence color.");
    zassert_equal(0xff, pixels[i].b,
      "processSolidSeq failed to set the pixels to the sequence color.");
  }
}

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
