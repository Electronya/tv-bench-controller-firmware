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

/** @} */
