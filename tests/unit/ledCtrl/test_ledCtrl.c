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

#include "ledCtrl.h"
#include "ledCtrl.c"

#include "zephyrLedStrip.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, zephyrLedStripInit, ZephyrLedStrip_t*, uint32_t);
FAKE_VALUE_FUNC(uint32_t, zephyrLedStripGetPixelCnt, ZephyrLedStrip_t*);
FAKE_VALUE_FUNC(int, zephyrLedStripSetPixels, ZephyrLedStrip_t*, uint32_t,
  uint32_t, const ZephyrRgbLed_t*);
FAKE_VALUE_FUNC(int , zephyrLedStripUpdate, ZephyrLedStrip_t*);

/**
 * @brief The test max pixel count.
*/
#define TEST_MAX_PIXEL_COUNT            10

static void ledCtrlCaseSetup(void *f)
{
  RESET_FAKE(zephyrLedStripInit);
  RESET_FAKE(zephyrLedStripGetPixelCnt);
  RESET_FAKE(zephyrLedStripSetPixels);
  RESET_FAKE(zephyrLedStripUpdate);
}

ZTEST_SUITE(ledCtrl_suite, NULL, NULL, ledCtrlCaseSetup, NULL, NULL);

#define LED_CTRL_IS_DEV_READY_TEST_CNT  3
/**
 * @test  ledCtrlInit must try to initialize the LED strip and return
 *        the error code if the operation fails.
*/
ZTEST(ledCtrl_suite, test_ledCtrlInit_Fail)
{
  int returnVals[LED_CTRL_IS_DEV_READY_TEST_CNT] = {-ENODEV, -ENOSPC, -EINVAL};
  int pixelCounts[LED_CTRL_IS_DEV_READY_TEST_CNT] = {1, 24, 250};
  int result;
  for(uint8_t i = 0; i < LED_CTRL_IS_DEV_READY_TEST_CNT; ++i)
  {
    ledStrip.pixelCount = pixelCounts[i];
    RESET_FAKE(zephyrLedStripInit);
    zephyrLedStripInit_fake.return_val = returnVals[i];
    result = ledCtrlInit();
    zassert_equal(zephyrLedStripInit_fake.call_count, 1,
      "ledCtrlInit failed to initialize the LED strip");
    zassert_equal(zephyrLedStripInit_fake.arg0_val, &ledStrip,
      "ledCtrlInit failed to initialize the right LED strip");
    zassert_equal(zephyrLedStripInit_fake.arg1_val, pixelCounts[i],
      "ledCtrlInit failed to initialize the LED strip with the right pixel count");
    zassert_equal(result, returnVals[i],
      "ledCtrlInit failed to return the error code.");
  }
}

/**
 * @test  ledCtrlInit must initialize the LED strip and return
 *        the success code if the operation succeeds.
*/
ZTEST(ledCtrl_suite, test_ledCtrlInit_Success)
{
  int returnVals[LED_CTRL_IS_DEV_READY_TEST_CNT] = {0, 0, 0};
  int pixelCounts[LED_CTRL_IS_DEV_READY_TEST_CNT] = {1, 24, 250};
  int result;
  for(uint8_t i = 0; i < LED_CTRL_IS_DEV_READY_TEST_CNT; ++i)
  {
    ledStrip.pixelCount = pixelCounts[i];
    RESET_FAKE(zephyrLedStripInit);
    zephyrLedStripInit_fake.return_val = returnVals[i];
    result = ledCtrlInit();
    zassert_equal(zephyrLedStripInit_fake.call_count, 1,
      "ledCtrlInit failed to initialize the LED strip");
    zassert_equal(zephyrLedStripInit_fake.arg0_val, &ledStrip,
      "ledCtrlInit failed to initialize the right LED strip");
    zassert_equal(zephyrLedStripInit_fake.arg1_val, pixelCounts[i],
      "ledCtrlInit failed to initialize the LED strip with the right pixel count");
    zassert_equal(result, returnVals[i],
      "ledCtrlInit failed to return the success code.");
  }
}

#define GET_MAX_PIXEL_TEST_COUNT        3
/**
 * @test  ledCtrlGetMaxPixelCount must return the maximal pixel count.
*/
ZTEST(ledCtrl_suite, test_ledCtrlGetMaxPixelCount_MaxPixelCount)
{
  size_t expectedCounts[GET_MAX_PIXEL_TEST_COUNT] = {3, 100, 500};

  for(uint8_t i = 0; i < GET_MAX_PIXEL_TEST_COUNT; ++i)
  {
    zephyrLedStripGetPixelCnt_fake.return_val = expectedCounts[i];

    zassert_equal(expectedCounts[i], ledCtrlGetMaxPixelCount(),
      "ledCtrlGetMaxPixelCount failed to return the maximal pixel count.");
    zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
      "ledCtrlGetMaxPixelCount failed to return the maximal pixel count.");
    zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
      "ledCtrlGetMaxPixelCount failed to return the maximal pixel count.");

    RESET_FAKE(zephyrLedStripGetPixelCnt);
  }
}

#define INVALID_INDEXES_TEST_COUNT      3
/**
 * @test  ledCtrlSetToBaseColor must return the error code if the start or end
 *        index are not valid.
*/
ZTEST(ledCtrl_suite, test_ledCtrlSetToBaseColor_InvalidIndexes)
{
  int failRet = -EINVAL;
  ZephyrRgbLed_t pixels[TEST_MAX_PIXEL_COUNT];
  size_t startIndexes[INVALID_INDEXES_TEST_COUNT] = {5, 0, 10};
  size_t endIndexes[INVALID_INDEXES_TEST_COUNT] = {4, 11, 20};

  for(uint8_t i = 0; i < INVALID_INDEXES_TEST_COUNT; ++i)
  {
    zephyrLedStripGetPixelCnt_fake.return_val = TEST_MAX_PIXEL_COUNT;

    zassert_equal(failRet, ledCtrlSetToBaseColor(pixels, startIndexes[i],
      endIndexes[i], LED_COLOR_WHT),
      "ledCtrlSetToBaseColor failed to return the error code.");
    zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
      "ledCtrlSetToBaseColor failed to get the maximal pixel count.");
    zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
      "ledCtrlSetToBaseColor failed to get the maximal pixel count.");

    RESET_FAKE(zephyrLedStripGetPixelCnt);
  }
}

/**
 * @test  ledCtrlSetToBaseColor must set the given pixels to the desired base
 *        color and return the success code.
*/
ZTEST(ledCtrl_suite, test_ledCtrlSetToBaseColor_Success)
{
  int successRet = 0;
  size_t startIndex = 2;
  size_t endIndex = TEST_MAX_PIXEL_COUNT;
  ZephyrRgbLed_t pixels[TEST_MAX_PIXEL_COUNT];

  for(uint8_t i = 0; i < TEST_MAX_PIXEL_COUNT; ++i)
  {
    pixels[i].r = 0x00;
    pixels[i].g = 0x00;
    pixels[i].b = 0x00;
  }

  for(uint8_t i = 0; i < LED_COLOR_CNT; ++i)
  {
    zephyrLedStripGetPixelCnt_fake.return_val = TEST_MAX_PIXEL_COUNT;

    zassert_equal(successRet, ledCtrlSetToBaseColor(pixels, startIndex, endIndex, i),
      "ledCtrlSetToBaseColor failed to return the success code.");
    zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
      "ledCtrlSetToBaseColor failed to get the maximal pixel count.");
    zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
      "ledCtrlSetToBaseColor failed to get the maximal pixel count.");

    for(uint8_t j = startIndex; j < endIndex; ++j)
    {
      zassert_equal(baseColors[i].r, pixels[j].r);
      zassert_equal(baseColors[i].g, pixels[j].g);
      zassert_equal(baseColors[i].b, pixels[j].b);
    }

    RESET_FAKE(zephyrLedStripGetPixelCnt);
  }
}

/**
 * @test  ledCtrlUpdatePixels must return the error code if the start or end
 *        index are not valid.
*/
ZTEST(ledCtrl_suite, test_ledCtrlUpdatePixels_InvalidIndexes)
{
  int failRet = -EINVAL;
  ZephyrRgbLed_t pixels[TEST_MAX_PIXEL_COUNT];
  size_t startIndexes[INVALID_INDEXES_TEST_COUNT] = {5, 0, 10};
  size_t endIndexes[INVALID_INDEXES_TEST_COUNT] = {4, 11, 20};

  for(uint8_t i = 0; i < INVALID_INDEXES_TEST_COUNT; ++i)
  {
    zephyrLedStripGetPixelCnt_fake.return_val = TEST_MAX_PIXEL_COUNT;

    zassert_equal(failRet, ledCtrlUpdatePixels(pixels, startIndexes[i], endIndexes[i]),
      "ledCtrlUpdatePixels failed to return the error code.");
    zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
      "ledCtrlUpdatePixels failed to get the maximal pixel count.");
    zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
      "ledCtrlUpdatePixels failed to get the maximal pixel count.");

    RESET_FAKE(zephyrLedStripGetPixelCnt);
  }
}

/**
 * @test  ledCtrlUpdatePixels must return the error code if setting the led
 *        strip pixels fails.
*/
ZTEST(ledCtrl_suite, test_ledCtrlUpdatePixels_SetPixelFail)
{
  int failRet = -EINVAL;
  size_t startIndex = 2;
  size_t endIndex = TEST_MAX_PIXEL_COUNT;
  ZephyrRgbLed_t pixels[TEST_MAX_PIXEL_COUNT];

  zephyrLedStripGetPixelCnt_fake.return_val = TEST_MAX_PIXEL_COUNT;
  zephyrLedStripSetPixels_fake.return_val = failRet;

  zassert_equal(failRet, ledCtrlUpdatePixels(pixels, startIndex, endIndex),
    "ledCtrlUpdatePixels failed to return the error code.");
  zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(1, zephyrLedStripSetPixels_fake.call_count,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(&ledStrip, zephyrLedStripSetPixels_fake.arg0_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(startIndex, zephyrLedStripSetPixels_fake.arg1_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(endIndex, zephyrLedStripSetPixels_fake.arg2_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(pixels, zephyrLedStripSetPixels_fake.arg3_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
}

/**
 * @test  ledCtrlUpdatePixels must return the error code if updating the led
 *        strip pixels fails.
*/
ZTEST(ledCtrl_suite, test_ledCtrlUpdatePixels_UpdatePixelFail)
{
  int successRet = 0;
  int failRet = -EINVAL;
  size_t startIndex = 2;
  size_t endIndex = TEST_MAX_PIXEL_COUNT - 1;
  ZephyrRgbLed_t pixels[TEST_MAX_PIXEL_COUNT];

  zephyrLedStripGetPixelCnt_fake.return_val = TEST_MAX_PIXEL_COUNT;
  zephyrLedStripSetPixels_fake.return_val = successRet;
  zephyrLedStripUpdate_fake.return_val = failRet;

  zassert_equal(failRet, ledCtrlUpdatePixels(pixels, startIndex, endIndex),
    "ledCtrlUpdatePixels failed to return the error code.");
  zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(1, zephyrLedStripSetPixels_fake.call_count,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(&ledStrip, zephyrLedStripSetPixels_fake.arg0_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(startIndex, zephyrLedStripSetPixels_fake.arg1_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(endIndex, zephyrLedStripSetPixels_fake.arg2_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(pixels, zephyrLedStripSetPixels_fake.arg3_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(1, zephyrLedStripUpdate_fake.call_count,
    "ledCtrlUpdatePixels failed to update the led strip pixels.");
  zassert_equal(&ledStrip, zephyrLedStripUpdate_fake.arg0_val,
    "ledCtrlUpdatePixels failed to update the led strip pixels.");
}

/**
 * @test  ledCtrlUpdatePixels must update the led strip pixel and return
 *        the success code.
*/
ZTEST(ledCtrl_suite, test_ledCtrlUpdatePixels_Success)
{
  int successRet = 0;
  size_t startIndex = 2;
  size_t endIndex = TEST_MAX_PIXEL_COUNT - 1;
  ZephyrRgbLed_t pixels[TEST_MAX_PIXEL_COUNT];

  zephyrLedStripGetPixelCnt_fake.return_val = TEST_MAX_PIXEL_COUNT;
  zephyrLedStripSetPixels_fake.return_val = successRet;
  zephyrLedStripUpdate_fake.return_val = successRet;

  zassert_equal(successRet, ledCtrlUpdatePixels(pixels, startIndex, endIndex),
    "ledCtrlUpdatePixels failed to return the error code.");
  zassert_equal(1, zephyrLedStripGetPixelCnt_fake.call_count,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(&ledStrip, zephyrLedStripGetPixelCnt_fake.arg0_val,
    "ledCtrlUpdatePixels failed to get the maximal pixel count.");
  zassert_equal(1, zephyrLedStripSetPixels_fake.call_count,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(&ledStrip, zephyrLedStripSetPixels_fake.arg0_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(startIndex, zephyrLedStripSetPixels_fake.arg1_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(endIndex, zephyrLedStripSetPixels_fake.arg2_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(pixels, zephyrLedStripSetPixels_fake.arg3_val,
    "ledCtrlUpdatePixels failed to set the led strip pixels.");
  zassert_equal(1, zephyrLedStripUpdate_fake.call_count,
    "ledCtrlUpdatePixels failed to update the led strip pixels.");
  zassert_equal(&ledStrip, zephyrLedStripUpdate_fake.arg0_val,
    "ledCtrlUpdatePixels failed to update the led strip pixels.");
}

/** @} */
