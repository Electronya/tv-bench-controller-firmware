/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      colorManager.h
 * @author    jbacon
 * @date      2023-11-20
 * @brief     Color Manager Module
 *
 *            This file is the implementation of the color manager module.
 *
 * @ingroup  colorManager
 *
 * @{
 */

#include <zephyr/logging/log.h>

#include "ledCtrl.h"
#include "zephyrLedStrip.h"

#define COLOR_MNGR_MODULE_NAME color_mngr_module

/* Setting module logging */
LOG_MODULE_REGISTER(COLOR_MNGR_MODULE_NAME);

int colorMngrSetSingle(Color_t *color, size_t firstLed, size_t lastLed)
{
  ZephyrRgbLed_t *pixels;
  size_t pixelCount = lastLed - firstLed;

  pixels = k_malloc(pixelCount * sizeof(ZephyrRgbLed_t));
  if(!pixels)
    return -ENOSPC;

  for(uint8_t i = 0; i < pixelCount; i++)
  {
    pixels[i].r = color->r;
    pixels[i].g = color->g;
    pixels[i].b = color->b;
  }

  return ledCtrlUpdatePixels(pixels, firstLed, lastLed);
}

int colorMngrSetFade(Color_t *color, uint32_t fadeLvl, uint32_t fadeStart,
                     uint32_t firstLed, uint32_t lastLed, bool isAscending)
{
  ZephyrRgbLed_t *pixels;
  size_t pixelCount = lastLed - firstLed;
  size_t pixelCntr = 0;

  pixels = k_malloc(pixelCount * sizeof(ZephyrRgbLed_t));
  if(!pixels)
    return -ENOSPC;

  while(pixelCntr < lastLed - firstLed)
  {
    pixels[fadeStart].r = (int32_t)(color->r - pixelCntr * fadeLvl) <= 0 ? 0 :
      color->r - pixelCntr * fadeLvl;
    pixels[fadeStart].g = (int32_t)(color->g - pixelCntr * fadeLvl) <= 0 ? 0 :
      color->g - pixelCntr * fadeLvl;
    pixels[fadeStart].b = (int32_t)(color->b - pixelCntr * fadeLvl) <= 0 ? 0 :
      color->b - pixelCntr * fadeLvl;

    pixelCntr++;
    if(isAscending)
    {
      fadeStart++;
      if(fadeStart == lastLed + 1)
        fadeStart = firstLed;
    }
    else
    {
      fadeStart--;
      if(fadeStart == 0xffffffff)
        fadeStart = lastLed;
    }
  }

  return ledCtrlUpdatePixels(pixels, firstLed, lastLed);
}

/** @} */
