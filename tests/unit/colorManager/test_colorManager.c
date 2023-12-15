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

/**
 * @brief The test max pixel count.
*/
#define TEST_MAX_PIXEL_COUNT            10

struct colorMngr_suite_fixture
{
  ZephyrRgbPixel_t pixels[TEST_MAX_PIXEL_COUNT];
};

static void *colorMngrSuiteSetup(void)
{
  struct colorMngr_suite_fixture *fixture =
    k_malloc(sizeof(struct colorMngr_suite_fixture));
  zassume_not_null(fixture, NULL);

  return (void *)fixture;
}

static void colorMngrSuiteTeardown(void *f)
{
  k_free(f);
}

static void colorMngrCaseSetup(void *f)
{
  memset(f, 0x00, TEST_MAX_PIXEL_COUNT);
}

ZTEST_SUITE(colorMngr_suite, NULL, colorMngrSuiteSetup, colorMngrCaseSetup,
  NULL, colorMngrSuiteTeardown);

/**
 * @test  colorMngrSetSingle must set the pixels to the sequence color.
*/
ZTEST_F(colorMngr_suite, test_colorMngrSetSingle_SetColor)
{
  Color_t color;

  color.hexColor = 0x00eeccff;

  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  for(uint8_t i = 0; i < TEST_MAX_PIXEL_COUNT; i++)
  {
    zassert_equal(0xee, fixture->pixels[i].r,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(0xcc, fixture->pixels[i].g,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
    zassert_equal(0xff, fixture->pixels[i].b,
      "colorMngrSetSingle failed to set the pixels to the sequence color.");
  }
}

/**
 * @test  colorMngrApplyFadeTrail must set the pixels to the appropriate color in an
 *        ascending fashon.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyFadeTrail_SetColorAscending)
{
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  ZephyrRgbPixel_t *pixelPntr = fixture->pixels + fadeStart;
  uint32_t pixelCntr = 0;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00eeccff;
  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  colorMngrApplyFadeTrail(fadeLvl, fadeStart, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  while(pixelCntr < TEST_MAX_PIXEL_COUNT)
  {
    expectedRed = (int32_t)(color.r - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xee - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(color.g - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xcc - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(color.b - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xff - pixelCntr * fadeLvl;
    zassert_equal(expectedRed, pixelPntr->r,
      "colorMngrApplyFadeTrail failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, pixelPntr->g,
      "colorMngrApplyFadeTrail failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, pixelPntr->b,
      "colorMngrApplyFadeTrail failed to set the pixels to the sequence color.");
    pixelCntr++;
    pixelPntr++;
    if(pixelPntr == fixture->pixels + TEST_MAX_PIXEL_COUNT)
      pixelPntr = fixture->pixels;
  }
}

/**
 * @test  colorMngrApplyFadeTrail must set the pixels to the appropriate color in a
 *        descending fashon.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyFadeTrail_SetColorDescending)
{
  uint32_t fadeLvl = 2;
  uint32_t fadeStart = 5;
  ZephyrRgbPixel_t *pixelPntr = fixture->pixels + fadeStart;
  uint32_t pixelCntr = 0;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00eeccff;
  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  colorMngrApplyFadeTrail(fadeLvl, fadeStart, false, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  while(pixelCntr < TEST_MAX_PIXEL_COUNT)
  {
    expectedRed = (int32_t)(color.r - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xee - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(color.g - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xcc - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(color.b - pixelCntr * fadeLvl) <= 0 ? 0 :
      0xff - pixelCntr * fadeLvl;
    zassert_equal(expectedRed, pixelPntr->r,
      "colorMngrApplyFadeTrail failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, pixelPntr->g,
      "colorMngrApplyFadeTrail failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, pixelPntr->b,
      "colorMngrApplyFadeTrail failed to set the pixels to the sequence color.");
    pixelCntr++;
    pixelPntr--;
    if(pixelPntr < fixture->pixels)
      pixelPntr = fixture->pixels + TEST_MAX_PIXEL_COUNT - 1;
  }
}

/** @} */
