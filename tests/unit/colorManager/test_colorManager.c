/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_colorManager.h
 * @author    jbacon
 * @date      2023-12-08
 * @brief     Color Manager Module Test Cases
 *
 *            This file is the test cases of the color manager module.
 *
 * @ingroup  colorManager
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include <zephyr/sys/util.h>

#include "colorManager.h"
#include "colorManager.c"

#include "appMsg.h"
#include "ledCtrl.h"
#include "zephyrLedStrip.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(void*, k_malloc, size_t);
FAKE_VALUE_FUNC(int, ledCtrlUpdatePixels, ZephyrRgbLed_t*, size_t, size_t);

static void colorMngrCaseSetup(void *f)
{
  RESET_FAKE(k_malloc);
  RESET_FAKE(ledCtrlUpdatePixels);
}

ZTEST_SUITE(colorMngr_suite, NULL, NULL, colorMngrCaseSetup, NULL, NULL);

/**
 * @test  colorMngrSetSingle must return an error code when the temporary pixels
 *        allocation fails.
*/
ZTEST(colorMngr_suite, test_colorMngrSetSingle_PixelAllocationFail)
{
  int failRet = -ENOSPC;
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;

  k_malloc_fake.return_val = NULL;

  zassert_equal(failRet, colorMngrSetSingle(&color, firstLed, lastLed),
    "colorMngrSetSingle failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetSingle failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetSingle failed to allocate the required pixels.");
}

/**
 * @test  colorMngrSetSingle must return any error code raise by the pixel
 *        update operation.
*/
ZTEST(colorMngr_suite, test_colorMngrSetSingle_PixelUpdateFail)
{
  int failRet = -EIO;
  ZephyrRgbLed_t pixels[10];
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;

  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = failRet;

  zassert_equal(failRet, colorMngrSetSingle(&color, firstLed, lastLed),
    "colorMngrSetSingle failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetSingle failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetSingle failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "colorMngrSetSingle failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "colorMngrSetSingle failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "colorMngrSetSingle failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "colorMngrSetSingle failed to update the LED pixels.");
}

/**
 * @test  colorMngrSetSingle must set the allocated pixels to the sequence
 *        color, update the LED strip pixel and return the success code.
*/
ZTEST(colorMngr_suite, test_colorMngrSetSingle_Sucess)
{
  int successRet = 0;
  ZephyrRgbLed_t pixels[10];
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;

  color.hexColor = 0x00eeccff;
  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = successRet;

  zassert_equal(successRet, colorMngrSetSingle(&color, firstLed, lastLed),
    "colorMngrSetSingle failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetSingle failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetSingle failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "colorMngrSetSingle failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "colorMngrSetSingle failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "colorMngrSetSingle failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "colorMngrSetSingle failed to update the LED pixels.");

  for(uint8_t i = 0; i < lastLed - firstLed; i++)
  {
    zassert_equal(0xee, pixels[i].r,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(0xcc, pixels[i].g,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(0xff, pixels[i].b,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
  }
}

