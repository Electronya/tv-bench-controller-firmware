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

#include "colorManager.h"
#include "colorManager.c"

#include "appMsg.h"
#include "ledCtrl.h"
#include "zephyrLedStrip.h"

DEFINE_FFF_GLOBALS;

static void colorMngrCaseSetup(void *f)
{

}

ZTEST_SUITE(colorMngr_suite, NULL, NULL, colorMngrCaseSetup, NULL, NULL);

#define TEST_MAX_PIXEL_COUNT                10
/**
 * @test  colorMngrSetSingle must set the pixels to the sequence color.
*/
ZTEST(colorMngr_suite, test_colorMngrSetSingle_SetColor)
{
  ZephyrRgbPixel_t pixels[TEST_MAX_PIXEL_COUNT];
  size_t pixelCnt = TEST_MAX_PIXEL_COUNT;
  Color_t color;

  color.hexColor = 0x00eeccff;

  colorMngrSetSingle(&color, pixels, pixelCnt);

  for(uint8_t i = 0; i < pixelCnt; i++)
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
 * @test  colorMngrSetFade must set the pixels to the appropriate color in an
 *        ascending fashon.
*/
ZTEST(colorMngr_suite, test_colorMngrSetFade_SetColorAscending)
{
  ZephyrRgbPixel_t pixels[TEST_MAX_PIXEL_COUNT];
  uint32_t pixelCnt = TEST_MAX_PIXEL_COUNT;
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  ZephyrRgbPixel_t *pixelPntr = pixels + fadeStart;
  uint32_t pixelCntr = 0;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00eeccff;

  colorMngrSetFade(&color, fadeLvl, fadeStart, true, pixels, pixelCnt);

  while(pixelCntr < pixelCnt)
  {
    expectedRed = (int32_t)(0xee - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xee - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(0xcc - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xcc - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(0xff - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xff - pixelCntr * fadeLvl;
    zassert_equal(expectedRed, pixelPntr->r,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, pixelPntr->g,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, pixelPntr->b,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    pixelCntr++;
    pixelPntr++;
    if(pixelPntr > pixels + pixelCnt)
      pixelPntr = pixels;
  }
}

/**
 * @test  colorMngrSetFade must set the pixels to the appropriate color in a
 *        descending fashon.
*/
ZTEST(colorMngr_suite, test_colorMngrSetFade_SetColorDescending)
{
  ZephyrRgbPixel_t pixels[TEST_MAX_PIXEL_COUNT];
  uint32_t pixelCnt = TEST_MAX_PIXEL_COUNT;
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  ZephyrRgbPixel_t *pixelPntr = pixels + fadeStart;
  uint32_t pixelCntr = 0;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00eeccff;

  colorMngrSetFade(&color, fadeLvl, fadeStart, false, pixels, pixelCnt);

  while(pixelCntr < pixelCnt)
  {
    expectedRed = (int32_t)(0xee - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xee - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(0xcc - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xcc - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(0xff - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xff - pixelCntr * fadeLvl;
    zassert_equal(expectedRed, pixelPntr->r,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, pixelPntr->g,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, pixelPntr->b,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    pixelCntr++;
    pixelPntr--;
    if(pixelPntr < pixels)
      pixelPntr = pixels + pixelCnt;
  }
}

/** @} */
