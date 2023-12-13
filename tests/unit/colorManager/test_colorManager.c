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
FAKE_VOID_FUNC(k_free, void*);
FAKE_VALUE_FUNC(int, ledCtrlUpdatePixels, ZephyrRgbPixel_t*, size_t, size_t);

static void colorMngrCaseSetup(void *f)
{
  RESET_FAKE(k_malloc);
  RESET_FAKE(k_free);
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
  ZephyrRgbPixel_t pixels[10];
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
  zassert_equal(1, k_free_fake.call_count,
    "colorMngrSetSingle failed to deallocate the pixels.");
  zassert_equal(&pixels, k_free_fake.arg0_val,
    "colorMngrSetSingle failed to deallocate the pixels.");
}

/**
 * @test  colorMngrSetSingle must set the allocated pixels to the sequence
 *        color, update the LED strip pixel and return the success code.
*/
ZTEST(colorMngr_suite, test_colorMngrSetSingle_Sucess)
{
  int successRet = 0;
  ZephyrRgbPixel_t pixels[10];
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
  zassert_equal(1, k_free_fake.call_count,
    "colorMngrSetSingle failed to deallocate the pixels.");
  zassert_equal(&pixels, k_free_fake.arg0_val,
    "colorMngrSetSingle failed to deallocate the pixels.");

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

/**
 * @test  colorMngrSetFade must return an error code when the temporary pixels
 *        allocation fails.
*/
ZTEST(colorMngr_suite, test_colorMngrSetFade_PixelAllocationFail)
{
  int failRet = -ENOSPC;
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;

  k_malloc_fake.return_val = NULL;

  zassert_equal(failRet, colorMngrSetFade(&color, fadeLvl, fadeStart, firstLed, lastLed, true),
    "colorMngrSetFade failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetFade failed to allocate the required pixels.");
}

/**
 * @test  colorMngrSetFade must return an error code when the temporary pixels
 *        allocation fails.
*/
ZTEST(colorMngr_suite, test_colorMngrSetFade_PixelUpdateFail)
{
  int failRet = -EIO;
  ZephyrRgbPixel_t pixels[10];
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;

  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = failRet;

  zassert_equal(failRet, colorMngrSetFade(&color, fadeLvl, fadeStart, firstLed, lastLed, true),
    "colorMngrSetFade failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(1, k_free_fake.call_count,
    "colorMngrSetFade failed to deallocate the pixels.");
  zassert_equal(&pixels, k_free_fake.arg0_val,
    "colorMngrSetFade failed to deallocate the pixels.");
}

/**
 * @test  colorMngrSetFade must set the allocated pixels to the appropriate
 *        color in an ascending fashon, update the strip LED and return the
 *        success code.
*/
ZTEST(colorMngr_suite, test_colorMngrSetFade_AscendingSuccess)
{
  int successRet = 0;
  uint32_t pixelIdx;
  uint32_t pixelCntr = 0;
  ZephyrRgbPixel_t pixels[10];
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00eeccff;
  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = successRet;

  zassert_equal(successRet, colorMngrSetFade(&color, fadeLvl, fadeStart, firstLed, lastLed, true),
    "colorMngrSetFade failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(1, k_free_fake.call_count,
    "colorMngrSetFade failed to deallocate the pixels.");
  zassert_equal(&pixels, k_free_fake.arg0_val,
    "colorMngrSetFade failed to deallocate the pixels.");

  pixelIdx = fadeStart;
  while(pixelCntr < lastLed - firstLed)
  {
    expectedRed = (int32_t)(0xee - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xee - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(0xcc - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xcc - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(0xff - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xff - pixelCntr * fadeLvl;
    zassert_equal(expectedRed, pixels[pixelIdx].r,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, pixels[pixelIdx].g,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, pixels[pixelIdx].b,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    pixelCntr++;
    pixelIdx++;
    if(pixelIdx == lastLed + 1)
      pixelIdx = firstLed;
  }
}

/**
 * @test  colorMngrSetFade must set the allocated pixels to the appropriate
 *        color in a descending fashon, update the strip LED and return the
 *        success code.
*/
ZTEST(colorMngr_suite, test_colorMngrSetFade_DescendingSuccess)
{
  int successRet = 0;
  uint32_t pixelIdx;
  uint32_t pixelCntr = 0;
  ZephyrRgbPixel_t pixels[10];
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  uint32_t firstLed = 0;
  uint32_t lastLed = 10;
  size_t expectedSize = (lastLed - firstLed) * 3;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00eeccff;
  k_malloc_fake.return_val = &pixels;
  ledCtrlUpdatePixels_fake.return_val = successRet;

  zassert_equal(successRet, colorMngrSetFade(&color, fadeLvl, fadeStart, firstLed, lastLed, false),
    "colorMngrSetFade failed to retunr the error code.");
  zassert_equal(1, k_malloc_fake.call_count,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(expectedSize, k_malloc_fake.arg0_val,
    "colorMngrSetFade failed to allocate the required pixels.");
  zassert_equal(1, ledCtrlUpdatePixels_fake.call_count,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(pixels, ledCtrlUpdatePixels_fake.arg0_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(firstLed, ledCtrlUpdatePixels_fake.arg1_val,
    "colorMngrSetFade failed to update the LED pixels.");
  zassert_equal(lastLed, ledCtrlUpdatePixels_fake.arg2_val,
    "colorMngrSetFade failed to update the LED pixels.");

  pixelIdx = fadeStart;
  while(pixelCntr < lastLed - firstLed)
  {
    expectedRed = (int32_t)(0xee - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xee - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(0xcc - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xcc - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(0xff - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xff - pixelCntr * fadeLvl;
    zassert_equal(expectedRed, pixels[pixelIdx].r,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, pixels[pixelIdx].g,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, pixels[pixelIdx].b,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    pixelCntr++;
    pixelIdx--;
    if(pixelIdx == 0xffffffff)
      pixelIdx = lastLed;
  }
}

/** @} */
