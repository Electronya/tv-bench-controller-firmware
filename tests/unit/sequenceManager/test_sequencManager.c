/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_sequenceManager.h
 * @author    jbacon
 * @date      2023-12-08
 * @brief     Color Manager Module Test Cases
 *
 *            This file is the test cases of the sequence manager module.
 *
 * @ingroup  sequenceManager
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "sequenceManager.h"
#include "sequenceManager.c"

#include "appMsg.h"
#include "colorManager.h"
#include "zephyrLedStrip.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(colorMngrSetSingle, Color_t*, ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(colorMngrApplyFade, uint8_t, ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(colorMngrApplyUnfade, uint8_t, ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(colorMngrApplyFadeTrail, uint8_t, uint32_t, bool,
               ZephyrRgbPixel_t*, size_t);
FAKE_VOID_FUNC(colorMngrUpdateRange, uint8_t, uint8_t, bool,
               ZephyrRgbPixel_t*, size_t);
FAKE_VALUE_FUNC(uint8_t, colorMngrConvertColor, Color_t*);

/**
 * @brief The test max pixel count.
*/
#define TEST_MAX_PIXEL_COUNT            10

struct seqMngr_suite_fixture
{
  ZephyrRgbPixel_t pixels[TEST_MAX_PIXEL_COUNT];
};

static void *seqMngrSuiteSetup(void)
{
  struct seqMngr_suite_fixture *fixture =
    k_malloc(sizeof(struct seqMngr_suite_fixture));
  zassume_not_null(fixture, NULL);

  return (void *)fixture;
}

static void seqMngrSuiteTeardown(void *f)
{
  k_free(f);
}

static void seqMngrCaseSetup(void *f)
{
  memset(f, 0x00, TEST_MAX_PIXEL_COUNT);

  RESET_FAKE(colorMngrSetSingle);
  RESET_FAKE(colorMngrApplyFade);
  RESET_FAKE(colorMngrApplyUnfade);
  RESET_FAKE(colorMngrApplyFadeTrail);
  RESET_FAKE(colorMngrUpdateRange);
  RESET_FAKE(colorMngrConvertColor);
}

ZTEST_SUITE(seqMngr_suite, NULL, seqMngrSuiteSetup, seqMngrCaseSetup,
  NULL, seqMngrSuiteTeardown);

/**
 * @test  seqMngrUpdateSolidFrame must set the pixel buffer to the desired solid
 *        color.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateSolidFrame_SetSolidColor)
{
  Color_t color;

  seqMngrUpdateSolidFrame(&color, fixture->pixels, TEST_MAX_PIXEL_COUNT);

  zassert_equal(1, colorMngrSetSingle_fake.call_count,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
  zassert_equal(&color, colorMngrSetSingle_fake.arg0_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
  zassert_equal(fixture->pixels, colorMngrSetSingle_fake.arg1_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrSetSingle_fake.arg2_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
}

#define BREATHER_TEST_COUNT               3
/**
 * @test  seqMngrUpdateSingleBreatherFrame must set the pixels to the desired
 *        color when resetting the sequence.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateSingleBreatherFrame_Reset)
{
  Color_t color;
  uint8_t steps[BREATHER_TEST_COUNT] = {1, 10, 200};

  for(uint8_t i = 0; i < BREATHER_TEST_COUNT; ++i)
  {
    seqMngrUpdateSingleBreatherFrame(&color, steps[i], true, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    zassert_equal(1, colorMngrSetSingle_fake.call_count,
      "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
    zassert_equal(&color, colorMngrSetSingle_fake.arg0_val,
      "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
    zassert_equal(fixture->pixels, colorMngrSetSingle_fake.arg1_val,
      "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");
    zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrSetSingle_fake.arg2_val,
      "seqMngrUpdateSolidFrame failed to set the pixel buffer to the desired color.");

    RESET_FAKE(colorMngrSetSingle);
  }
}

/**
 * @test  seqMngrUpdateSingleBreatherFrame must fade the pixel by the breather
 *        step when on the exhale frames.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateSingleBreatherFrame_ExhaleFade)
{
  Color_t color;
  uint8_t steps[BREATHER_TEST_COUNT] = {1, 10, 200};

  for(uint8_t i = 0; i < BREATHER_TEST_COUNT; ++i)
  {
    seqMngrUpdateSingleBreatherFrame(&color, steps[i], false, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    zassert_equal(1, colorMngrApplyFade_fake.call_count,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(steps[i], colorMngrApplyFade_fake.arg0_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(fixture->pixels, colorMngrApplyFade_fake.arg1_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrApplyFade_fake.arg2_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");

    RESET_FAKE(colorMngrApplyFade);
  }
}

/**
 * @test  seqMngrUpdateSingleBreatherFrame must unfade the pixel by the breather
 *        step when on the inhale frames.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateSingleBreatherFrame_InhaleUnfade)
{
  Color_t color;
  uint8_t steps[BREATHER_TEST_COUNT] = {1, 10, 200};

  for(uint8_t i = 0; i < BREATHER_TEST_COUNT; ++i)
  {
    /* this reset the sequence and do the last exhale step */
    seqMngrUpdateSingleBreatherFrame(&color, 255 - steps[i] + 1, true,
      fixture->pixels, TEST_MAX_PIXEL_COUNT);
    seqMngrUpdateSingleBreatherFrame(&color, steps[i], false, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    seqMngrUpdateSingleBreatherFrame(&color, steps[i], false, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    zassert_equal(1, colorMngrApplyUnfade_fake.call_count,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(steps[i], colorMngrApplyUnfade_fake.arg0_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(fixture->pixels, colorMngrApplyUnfade_fake.arg1_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrApplyUnfade_fake.arg2_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");

    RESET_FAKE(colorMngrApplyUnfade);
  }
}

/**
 * @test  seqMngrUpdateFadeChaserFrame must set the color and set the first
 *        fade trail from the first pixel in the buffer when the reset
 *        flag is set and the invertion flag is clear.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateFadeChaserFrame_ResetNotInvertedFrame)
{
  Color_t color;
  uint8_t step;

  color.hexColor = 0x00ffffff;
  step = color.r / TEST_MAX_PIXEL_COUNT;

  seqMngrUpdateFadeChaserFrame(&color, false, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  zassert_equal(1, colorMngrSetSingle_fake.call_count,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(&color, colorMngrSetSingle_fake.arg0_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(fixture->pixels, colorMngrSetSingle_fake.arg1_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrSetSingle_fake.arg2_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(1, colorMngrApplyFadeTrail_fake.call_count,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(step, colorMngrApplyFadeTrail_fake.arg0_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(0, colorMngrApplyFadeTrail_fake.arg1_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(true, colorMngrApplyFadeTrail_fake.arg2_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(fixture->pixels, colorMngrApplyFadeTrail_fake.arg3_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrApplyFadeTrail_fake.arg4_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
}

/**
 * @test  seqMngrUpdateFadeChaserFrame must set the color and set the first
 *        fade trail from the last pixel in the buffer when both the reset
 *        and invertion flags are set.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateFadeChaserFrame_ResetInvertedFrame)
{
  Color_t color;
  uint8_t step;

  color.hexColor = 0x00ffffff;
  step = color.r / TEST_MAX_PIXEL_COUNT;

  seqMngrUpdateFadeChaserFrame(&color, true, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  zassert_equal(1, colorMngrSetSingle_fake.call_count,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(&color, colorMngrSetSingle_fake.arg0_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(fixture->pixels, colorMngrSetSingle_fake.arg1_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrSetSingle_fake.arg2_val,
    "seqMngrUpdateSolidFrame failed to set the pixel buffer to the initial color.");
  zassert_equal(1, colorMngrApplyFadeTrail_fake.call_count,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(step, colorMngrApplyFadeTrail_fake.arg0_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(TEST_MAX_PIXEL_COUNT - 1, colorMngrApplyFadeTrail_fake.arg1_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(false, colorMngrApplyFadeTrail_fake.arg2_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(fixture->pixels, colorMngrApplyFadeTrail_fake.arg3_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrApplyFadeTrail_fake.arg4_val,
    "seqMngrUpdateSolidFrame failed to fade the pixels.");
}

/**
 * @test  seqMngrUpdateFadeChaserFrame must move the fade trail of one pixel
 *        at each call and wrap back to the first pixel when not inverted.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateFadeChaserFrame_NotInvertedWrapFrame)
{
  Color_t color;
  uint8_t step;
  uint32_t chaserPoint = 1;

  color.hexColor = 0x00ffffff;
  step = color.r / TEST_MAX_PIXEL_COUNT;

  seqMngrUpdateFadeChaserFrame(&color, false, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  RESET_FAKE(colorMngrSetSingle);
  RESET_FAKE(colorMngrApplyFadeTrail);

  for(uint8_t i = 0; i < TEST_MAX_PIXEL_COUNT; ++i)
  {
    seqMngrUpdateFadeChaserFrame(&color, false, false, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    zassert_equal(1, colorMngrApplyFadeTrail_fake.call_count,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(step, colorMngrApplyFadeTrail_fake.arg0_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(chaserPoint, colorMngrApplyFadeTrail_fake.arg1_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(true, colorMngrApplyFadeTrail_fake.arg2_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(fixture->pixels, colorMngrApplyFadeTrail_fake.arg3_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrApplyFadeTrail_fake.arg4_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");

    ++chaserPoint;
    if(chaserPoint == TEST_MAX_PIXEL_COUNT)
      chaserPoint = 0;

    RESET_FAKE(colorMngrSetSingle);
    RESET_FAKE(colorMngrApplyFadeTrail);
  }
}

/**
 * @test  seqMngrUpdateFadeChaserFrame must move the fade trail of one pixel
 *        at each call and wrap back to the last pixel when inverted.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateFadeChaserFrame_InvertedWrapFrame)
{
  Color_t color;
  uint8_t step;
  int32_t chaserPoint = 8;

  color.hexColor = 0x00ffffff;
  step = color.r / TEST_MAX_PIXEL_COUNT;

  seqMngrUpdateFadeChaserFrame(&color, true, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  RESET_FAKE(colorMngrSetSingle);
  RESET_FAKE(colorMngrApplyFadeTrail);

  for(uint8_t i = 0; i < TEST_MAX_PIXEL_COUNT; ++i)
  {
    seqMngrUpdateFadeChaserFrame(&color, true, false, fixture->pixels,
      TEST_MAX_PIXEL_COUNT);

    zassert_equal(1, colorMngrApplyFadeTrail_fake.call_count,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(step, colorMngrApplyFadeTrail_fake.arg0_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(chaserPoint, colorMngrApplyFadeTrail_fake.arg1_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(false, colorMngrApplyFadeTrail_fake.arg2_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(fixture->pixels, colorMngrApplyFadeTrail_fake.arg3_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");
    zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrApplyFadeTrail_fake.arg4_val,
      "seqMngrUpdateSolidFrame failed to fade the pixels.");

    --chaserPoint;
    if(chaserPoint < 0)
      chaserPoint = TEST_MAX_PIXEL_COUNT - 1;

    RESET_FAKE(colorMngrSetSingle);
    RESET_FAKE(colorMngrApplyFadeTrail);
  }
}

#define COLOR_CONVERT_CALL_CNT      2
/**
 * @test  seqMngrUpdateColorRangeFrame must convert the start and end colors
 *        and update the color range by resetting it if the sequence is resetted.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateColorRangeFrame_Reset)
{
  Color_t startColor = {.hexColor = 0xff0000};
  Color_t endColor = {.hexColor = 0x00ff00};
  uint8_t wheelPos[COLOR_CONVERT_CALL_CNT] = {0, 170};

  SET_RETURN_SEQ(colorMngrConvertColor, wheelPos, COLOR_CONVERT_CALL_CNT);

  seqMngrUpdateColorRangeFrame(&startColor, &endColor, true, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  zassert_equal(COLOR_CONVERT_CALL_CNT, colorMngrConvertColor_fake.call_count,
    "seqMngrUpdateColorRangeFrame failed to convert the start and end colors.");
  zassert_equal(&startColor, colorMngrConvertColor_fake.arg0_history[0],
    "seqMngrUpdateColorRangeFrame failed to convert the start and end colors.");
  zassert_equal(&endColor, colorMngrConvertColor_fake.arg0_history[1],
    "seqMngrUpdateColorRangeFrame failed to convert the start and end colors.");
  zassert_equal(1, colorMngrUpdateRange_fake.call_count,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(wheelPos[0], colorMngrUpdateRange_fake.arg0_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(wheelPos[1], colorMngrUpdateRange_fake.arg1_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_true(colorMngrUpdateRange_fake.arg2_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(fixture->pixels, colorMngrUpdateRange_fake.arg3_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrUpdateRange_fake.arg4_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
}

/**
 * @test  seqMngrUpdateColorRangeFrame must convert the start and end colors
 *        and update the color range if the sequence is not resetted.
*/
ZTEST_F(seqMngr_suite, test_seqMngrUpdateColorRangeFrame_NotResetted)
{
  Color_t startColor = {.hexColor = 0xff0000};
  Color_t endColor = {.hexColor = 0x00ff00};
  uint8_t wheelPos[COLOR_CONVERT_CALL_CNT] = {0, 170};

  SET_RETURN_SEQ(colorMngrConvertColor, wheelPos, COLOR_CONVERT_CALL_CNT);

  seqMngrUpdateColorRangeFrame(&startColor, &endColor, false, fixture->pixels,
    TEST_MAX_PIXEL_COUNT);

  zassert_equal(COLOR_CONVERT_CALL_CNT, colorMngrConvertColor_fake.call_count,
    "seqMngrUpdateColorRangeFrame failed to convert the start and end colors.");
  zassert_equal(&startColor, colorMngrConvertColor_fake.arg0_history[0],
    "seqMngrUpdateColorRangeFrame failed to convert the start and end colors.");
  zassert_equal(&endColor, colorMngrConvertColor_fake.arg0_history[1],
    "seqMngrUpdateColorRangeFrame failed to convert the start and end colors.");
  zassert_equal(1, colorMngrUpdateRange_fake.call_count,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(wheelPos[0], colorMngrUpdateRange_fake.arg0_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(wheelPos[1], colorMngrUpdateRange_fake.arg1_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_false(colorMngrUpdateRange_fake.arg2_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(fixture->pixels, colorMngrUpdateRange_fake.arg3_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
  zassert_equal(TEST_MAX_PIXEL_COUNT, colorMngrUpdateRange_fake.arg4_val,
    "seqMngrUpdateColorRangeFrame failed to update the color range by reseting it.");
}

/** @} */
