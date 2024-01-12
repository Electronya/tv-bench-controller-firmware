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
 * @test  colorMngrApplyFade must apply a constant fade to the given pixels.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyFade_SetFade)
{
  uint8_t fadeLvl = 2;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00ee00ff;
  expectedRed = (int32_t)(color.r - fadeLvl) <= 0 ? 0 : color.r - fadeLvl;
  expectedGrn = (int32_t)(color.g - fadeLvl) <= 0 ? 0 : color.g - fadeLvl;
  expectedBlu = (int32_t)(color.b - fadeLvl) <= 0 ? 0 : color.b - fadeLvl;
  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  colorMngrApplyFade(fadeLvl, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  for(uint8_t i = 0; i < TEST_MAX_PIXEL_COUNT; ++i)
  {
    zassert_equal(expectedRed, fixture->pixels[i].r,
      "colorMngrApplyFade failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, fixture->pixels[i].g,
      "colorMngrApplyFade failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, fixture->pixels[i].b,
      "colorMngrApplyFade failed to set the pixels to the sequence color.");
  }
}

/**
 * @test  colorMngrApplyUnfade must apply a constant unfade to the given pixels.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyFade_SetUnfade)
{
  uint8_t unfadeLvl = 2;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00aa00ff;
  expectedRed = (uint32_t)(color.r + unfadeLvl) >= 255 ? 255 : color.r + unfadeLvl;
  expectedGrn = (uint32_t)(color.g + unfadeLvl) >= 255 ? 255 : color.g + unfadeLvl;
  expectedBlu = (uint32_t)(color.b + unfadeLvl) >= 255 ? 255 : color.b + unfadeLvl;
  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  colorMngrApplyUnfade(unfadeLvl, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  for(uint8_t i = 0; i < TEST_MAX_PIXEL_COUNT; ++i)
  {
    zassert_equal(expectedRed, fixture->pixels[i].r,
      "colorMngrApplyFade failed to set the pixels to the sequence color.");
    zassert_equal(expectedGrn, fixture->pixels[i].g,
      "colorMngrApplyFade failed to set the pixels to the sequence color.");
    zassert_equal(expectedBlu, fixture->pixels[i].b,
      "colorMngrApplyFade failed to set the pixels to the sequence color.");
  }
}

/**
 * @test  colorMngrApplyFadeTrail must apply an ascending fading trail to the
 *        given pixels.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyFadeTrail_ApplyAscendingTrail)
{
  uint8_t fadeLvl = 2;
  uint32_t trailStart = 5;
  ZephyrRgbPixel_t *pixelPntr = fixture->pixels + trailStart;
  uint32_t pixelCntr = 0;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00ee08ff;
  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  colorMngrApplyFadeTrail(fadeLvl, trailStart, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  while(pixelCntr < TEST_MAX_PIXEL_COUNT)
  {
    expectedRed = (int32_t)(color.r - pixelCntr * fadeLvl) <= 0 ? 0 :
      color.r - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(color.g - pixelCntr * fadeLvl) <= 0 ? 0 :
      color.g - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(color.b - pixelCntr * fadeLvl) <= 0 ? 0 :
      color.b - pixelCntr * fadeLvl;
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
 * @test  colorMngrApplyFadeTrail must apply a descending fading trail to the
 *        given pixels.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyFadeTrail_ApplyDescendingTrail)
{
  uint32_t fadeLvl = 2;
  uint32_t trailStart = 5;
  ZephyrRgbPixel_t *pixelPntr = fixture->pixels + trailStart;
  uint32_t pixelCntr = 0;
  Color_t color;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  color.hexColor = 0x00ee08ff;
  colorMngrSetSingle(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  colorMngrApplyFadeTrail(fadeLvl, trailStart, false, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  while(pixelCntr < TEST_MAX_PIXEL_COUNT)
  {
    expectedRed = (int32_t)(color.r - pixelCntr * fadeLvl) <= 0 ? 0 :
      color.r - pixelCntr * fadeLvl;
    expectedGrn = (int32_t)(color.g - pixelCntr * fadeLvl) <= 0 ? 0 :
      color.g - pixelCntr * fadeLvl;
    expectedBlu = (int32_t)(color.b - pixelCntr * fadeLvl) <= 0 ? 0 :
      color.b - pixelCntr * fadeLvl;
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

#define COLOR_RANGE_TEST_COUNT                3
/**
 * @test  colorMngrUpdateRange must reset the color wheel when the flag is set
 *        and update the pixel color to the wheel start.
*/
ZTEST_F(colorMngr_suite, test_colorMngrUpdateRange_ResettingRange)
{
  uint8_t wheelPos;
  uint8_t wheelStarts[COLOR_RANGE_TEST_COUNT] = {0, 86, 170};
  uint8_t wheelEnd = 255;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  for(uint8_t i = 0; i < COLOR_RANGE_TEST_COUNT; ++i)
  {
    expectedRed = 0;
    expectedGrn = 0;
    expectedBlu = 0;

    colorMngrUpdateRange(wheelStarts[i], wheelEnd, true, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    if(wheelStarts[i] < 85)
    {
      expectedRed = 255 - wheelStarts[i] * 3;
      expectedBlu = wheelStarts[i] * 3;
    }
    else if(wheelStarts[i] >= 85 && wheelStarts[i] < 170)
    {
      wheelPos = wheelStarts[i] - 85;
      expectedBlu = 255 - wheelPos * 3;
      expectedGrn = wheelPos * 3;
    }
    else
    {
      wheelPos = wheelStarts[i] - 170;
      expectedGrn = 255 - wheelPos * 3;
      expectedRed = wheelPos * 3;
    }

    for(uint8_t j = 0; j < TEST_MAX_PIXEL_COUNT; ++j)
    {
      zassert_equal(expectedRed, fixture->pixels[j].r,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      zassert_equal(expectedGrn, fixture->pixels[j].g,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      zassert_equal(expectedBlu, fixture->pixels[j].b,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
    }
  }
}

#define TEST_UPDATE_COUNT                     260
/**
 * @test  colorMngrUpdateRange must increment color wheel position when the
 *        reset flag is clear, wrap it arround when wheel end is reach and
 *        update the pixel color to the wheel start.
*/
ZTEST_F(colorMngr_suite, test_colorMngrUpdateRange_UpdateAndWrappingRange)
{
  uint8_t wheelPos;
  uint8_t wheelStarts[COLOR_RANGE_TEST_COUNT] = {0, 2, 200};
  uint8_t wheelEnds[COLOR_RANGE_TEST_COUNT] = {255, 255, 26};
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  for(uint8_t i = 0; i < COLOR_RANGE_TEST_COUNT; ++i)
  {
    wheelPos = wheelStarts[i];

    for(uint16_t j = 0; j < TEST_UPDATE_COUNT; ++j)
    {
      expectedRed = 0;
      expectedGrn = 0;
      expectedBlu = 0;

      colorMngrUpdateRange(wheelStarts[i], wheelEnds[i], j == 0,
        fixture->pixels, TEST_MAX_PIXEL_COUNT);

      if(wheelPos < 85)
      {
        expectedRed = 255 - wheelPos * 3;
        expectedBlu = wheelPos * 3;
      }
      else if(wheelPos >= 85 && wheelPos < 170)
      {
        expectedBlu = 255 - (wheelPos - 85) * 3;
        expectedGrn = (wheelPos - 85) * 3;
      }
      else
      {
        expectedGrn = 255 - (wheelPos - 170) * 3;
        expectedRed = (wheelPos - 170) * 3;
      }

      for(uint8_t k = 0; k < TEST_MAX_PIXEL_COUNT; ++k)
      {
        zassert_equal(expectedRed, fixture->pixels[k].r,
          "colorMngrUpdateRange failed to set the pixels to the sequence color.");
        zassert_equal(expectedGrn, fixture->pixels[k].g,
          "colorMngrUpdateRange failed to set the pixels to the sequence color.");
        zassert_equal(expectedBlu, fixture->pixels[k].b,
          "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      }

      ++wheelPos;
      if(wheelPos > wheelEnds[i] && wheelPos < wheelStarts[i])
        wheelPos = wheelStarts[i];
    }
  }
}

/**
 * @test  colorMngrApplyRangeTrail must apply the color range given as an
 *        ascending trail from the starting position.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyRangeTrail_ApplyAscendingTrail)
{
  uint8_t wheelPos;
  uint8_t step;
  uint8_t wheelStarts[COLOR_RANGE_TEST_COUNT] = {0, 2, 200};
  uint8_t wheelEnds[COLOR_RANGE_TEST_COUNT] = {255, 255, 26};
  uint32_t trailStarts[COLOR_RANGE_TEST_COUNT] = {0, 5, TEST_MAX_PIXEL_COUNT - 1};
  uint8_t pixelCntr = 0;
  ZephyrRgbPixel_t *pixelPntr;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  for(uint8_t i = 0; i < COLOR_RANGE_TEST_COUNT; ++i)
  {
    pixelPntr = fixture->pixels + trailStarts[i];
    expectedRed = 0;
    expectedGrn = 0;
    expectedBlu = 0;

    colorMngrApplyRangeTrail(trailStarts[i], wheelStarts[i], wheelEnds[i],
      true, fixture->pixels, TEST_MAX_PIXEL_COUNT);

    wheelPos = wheelStarts[i];
    step = (wheelEnds[i] - wheelStarts[i]) / TEST_MAX_PIXEL_COUNT;
    pixelPntr = fixture->pixels + trailStarts[i];
    while(pixelCntr < TEST_MAX_PIXEL_COUNT)
    {
      if(wheelPos < 85)
      {
        expectedRed = 255 - wheelPos * 3;
        expectedBlu = wheelPos * 3;
      }
      else if(wheelPos >= 85 && wheelPos < 170)
      {
        expectedBlu = 255 - (wheelPos - 85) * 3;
        expectedGrn = (wheelPos - 85) * 3;
      }
      else
      {
        expectedGrn = 255 - (wheelPos - 170) * 3;
        expectedRed = (wheelPos - 170) * 3;
      }

      zassert_equal(expectedRed, pixelPntr->r,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      zassert_equal(expectedGrn, pixelPntr->g,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      zassert_equal(expectedBlu, pixelPntr->b,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");

      ++pixelCntr;
      ++pixelPntr;
      if(pixelPntr == fixture->pixels + TEST_MAX_PIXEL_COUNT)
        pixelPntr = fixture->pixels;

      wheelPos += step;
      if(wheelPos > wheelEnds[i] && wheelPos < wheelStarts[i])
        wheelPos = wheelStarts[i];
    }
  }
}

/**
 * @test  colorMngrApplyRangeTrail must apply the color range given as an
 *        descending trail from the starting position.
*/
ZTEST_F(colorMngr_suite, test_colorMngrApplyRangeTrail_ApplyDescendingTrail)
{
  uint8_t wheelPos;
  uint8_t step;
  uint8_t wheelStarts[COLOR_RANGE_TEST_COUNT] = {0, 2, 200};
  uint8_t wheelEnds[COLOR_RANGE_TEST_COUNT] = {255, 255, 26};
  uint32_t trailStarts[COLOR_RANGE_TEST_COUNT] = {0, 5, TEST_MAX_PIXEL_COUNT - 1};
  uint8_t pixelCntr = 0;
  ZephyrRgbPixel_t *pixelPntr;
  uint8_t expectedRed;
  uint8_t expectedGrn;
  uint8_t expectedBlu;

  for(uint8_t i = 0; i < COLOR_RANGE_TEST_COUNT; ++i)
  {
    pixelPntr = fixture->pixels + trailStarts[i];
    expectedRed = 0;
    expectedGrn = 0;
    expectedBlu = 0;

    colorMngrApplyRangeTrail(trailStarts[i], wheelStarts[i], wheelEnds[i],
      false, fixture->pixels, TEST_MAX_PIXEL_COUNT);

    wheelPos = wheelStarts[i];
    step = (wheelEnds[i] - wheelStarts[i]) / TEST_MAX_PIXEL_COUNT;
    pixelPntr = fixture->pixels + trailStarts[i];
    while(pixelCntr < TEST_MAX_PIXEL_COUNT)
    {
      if(wheelPos < 85)
      {
        expectedRed = 255 - wheelPos * 3;
        expectedBlu = wheelPos * 3;
      }
      else if(wheelPos >= 85 && wheelPos < 170)
      {
        expectedBlu = 255 - (wheelPos - 85) * 3;
        expectedGrn = (wheelPos - 85) * 3;
      }
      else
      {
        expectedGrn = 255 - (wheelPos - 170) * 3;
        expectedRed = (wheelPos - 170) * 3;
      }

      zassert_equal(expectedRed, pixelPntr->r,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      zassert_equal(expectedGrn, pixelPntr->g,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");
      zassert_equal(expectedBlu, pixelPntr->b,
        "colorMngrUpdateRange failed to set the pixels to the sequence color.");

      ++pixelCntr;
      --pixelPntr;
      if(pixelPntr < fixture->pixels)
        pixelPntr = fixture->pixels + TEST_MAX_PIXEL_COUNT - 1;

      wheelPos += step;
      if(wheelPos > wheelEnds[i] && wheelPos < wheelStarts[i])
        wheelPos = wheelStarts[i];
    }
  }
}

#define COLOR_CONVERTION_TEST_CNT                   6
/**
 * @test  colorMngrConvertColor must return the wheel position by converting the
 *        RGB HEX color. The following algorithm is used to do the convertion:
 *        - Assuming only 2 color components that are complementatry weighted.
 *        - If red: the color wheel position is 0.
 *        - If red and blue: the color wheel position is 84 * blue ratio.
 *        - If blue: the color wheel position is 85
 *        - If blue and green: the color wheel position is 84 * green ratio + 85.
 *        - If green: the color wheel position is 170.
 *        - if green and red: the color wheel position is 84 * red ratio + 170
*/
ZTEST(colorMngr_suite, test_colorMngrConvertColor_Convertion)
{
  Color_t colors[COLOR_CONVERTION_TEST_CNT] = {{.hexColor = 0xff0000},
                                               {.hexColor = 0x5400ab},
                                               {.hexColor = 0x0000ff},
                                               {.hexColor = 0x0051ae},
                                               {.hexColor = 0x00ff00},
                                               {.hexColor = 0x44bb00}};
  uint8_t expectWheelPos[COLOR_CONVERTION_TEST_CNT] = {0, 56, 85, 111, 170, 192};

  for(uint8_t i = 0; i < COLOR_CONVERTION_TEST_CNT; ++i)
  {
    zassert_equal(expectWheelPos[i], colorMngrConvertColor(colors + i),
      "colorMngrConvertColor failed to convert the color into a wheel position.");
  }
}

/** @} */
