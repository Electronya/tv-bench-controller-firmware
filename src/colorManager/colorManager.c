/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      colorManager.h
 * @author    jbacon
 * @date      2023-12-08
 * @brief     Color Manager Module
 *
 *            This file is the implementation of the color manager module.
 *
 * @ingroup  colorManager
 *
 * @{
 */

#include <zephyr/logging/log.h>

#include "colorManager.h"
#include "zephyrLedStrip.h"

#define COLOR_MNGR_MODULE_NAME color_mngr_module

/* Setting module logging */
LOG_MODULE_REGISTER(COLOR_MNGR_MODULE_NAME);

/**
 * @brief   Calcultate the new color based on the color wheel position.
 *
 * @param wheelPos  The color wheel position.
 * @param red       The calculated red value.
 * @param green     The calculated green value.
 * @param blue      The calculated blue value.
 */
static void calculateNewColor(uint8_t wheelPos, uint8_t *red,
                              uint8_t *green, uint8_t *blue)
{
  if(wheelPos < COLOR_WHEEL_BLU_TO_GRN)
  {
    *red = 255 - wheelPos * 3;
    *blue = wheelPos * 3;
  }
  else if(wheelPos >= COLOR_WHEEL_BLU_TO_GRN &&
    wheelPos < COLOR_WHEEL_GRN_TO_RED)
  {
    *blue = 255 - (wheelPos - COLOR_WHEEL_BLU_TO_GRN) * 3;
    *green = (wheelPos - COLOR_WHEEL_BLU_TO_GRN) * 3;
  }
  else
  {
    *green = 255 - (wheelPos - COLOR_WHEEL_GRN_TO_RED) * 3;
    *red = (wheelPos - COLOR_WHEEL_GRN_TO_RED) * 3;
  }
}

void colorMngrSetSingle(Color_t *color, ZephyrRgbPixel_t *pixels,
                        size_t pixelCnt)
{
  for(uint8_t i = 0; i < pixelCnt; i++)
  {
    pixels[i].r = color->r;
    pixels[i].g = color->g;
    pixels[i].b = color->b;
  }
}

void colorMngrApplyFade(uint8_t fadeLvl, ZephyrRgbPixel_t *pixels,
                        size_t pixelCnt)
{
  for(uint8_t i = 0; i < pixelCnt; ++i)
  {
    pixels[i].r = (int32_t)(pixels[i].r - fadeLvl) <= 0 ? 0 :
      pixels[i].r - fadeLvl;
    pixels[i].g = (int32_t)(pixels[i].g - fadeLvl) <= 0 ? 0 :
      pixels[i].g - fadeLvl;
    pixels[i].b = (int32_t)(pixels[i].b - fadeLvl) <= 0 ? 0 :
      pixels[i].b - fadeLvl;
  }
}

void colorMngrApplyFadeTrail(uint8_t fadeLvl, uint32_t trailStart,
                             bool isAscending, ZephyrRgbPixel_t *pixels,
                             size_t pixelCnt)
{
  ZephyrRgbPixel_t *pixelPntr = pixels + trailStart;
  size_t pixelCntr = 0;

  while(pixelCntr < pixelCnt)
  {
    pixelPntr->r = (int32_t)(pixelPntr->r - pixelCntr * fadeLvl) <= 0 ? 0 :
      pixelPntr->r - pixelCntr * fadeLvl;
    pixelPntr->g = (int32_t)(pixelPntr->g - pixelCntr * fadeLvl) <= 0 ? 0 :
      pixelPntr->g - pixelCntr * fadeLvl;
    pixelPntr->b = (int32_t)(pixelPntr->b - pixelCntr * fadeLvl) <= 0 ? 0 :
      pixelPntr->b - pixelCntr * fadeLvl;

    pixelCntr++;
    if(isAscending)
    {
      pixelPntr++;
      if(pixelPntr == pixels + pixelCnt)
        pixelPntr = pixels;
    }
    else
    {
      pixelPntr--;
      if(pixelPntr < pixels)
        pixelPntr = pixels + pixelCnt - 1;
    }
  }
}

void colorMngrUpdateRange(uint8_t wheelStart, uint8_t wheelEnd, bool reset,
                          ZephyrRgbPixel_t *pixels, size_t pixelCnt)
{
  static uint8_t wheelPos = 0;
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;

  if(reset)
    wheelPos = wheelStart;

  calculateNewColor(wheelPos, &red, &green, &blue);

  for(uint8_t i = 0; i < pixelCnt; ++i)
  {
    pixels[i].r = red;
    pixels[i].g = green;
    pixels[i].b = blue;
  }

  ++wheelPos;
  if(wheelPos > wheelEnd && wheelPos < wheelStart)
    wheelPos = wheelStart;
}

void colorMngrApplyRangeTrail(uint32_t trailStart, uint8_t wheelStart,
                              uint8_t wheelEnd, bool isAscending,
                              ZephyrRgbPixel_t *pixels, size_t pixelCnt)
{
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
  uint8_t step = (wheelEnd - wheelStart) / pixelCnt;
  uint8_t wheelPos = wheelStart;
  size_t pixelCntr = 0;
  ZephyrRgbPixel_t *pixelPntr = pixels + trailStart;

  while(pixelCntr < pixelCnt)
  {
    calculateNewColor(wheelPos, &red, &green, &blue);

    pixelPntr->r = red;
    pixelPntr->g = green;
    pixelPntr->b = blue;

    ++pixelCntr;
    if(isAscending)
    {
      pixelPntr++;
      if(pixelPntr == pixels + pixelCnt)
        pixelPntr = pixels;
    }
    else
    {
      pixelPntr--;
      if(pixelPntr < pixels)
        pixelPntr = pixels + pixelCnt - 1;
    }

    wheelPos += step;
    if(wheelPos > wheelEnd && wheelPos < wheelStart)
      wheelPos = wheelStart;
  }
}

/** @} */
