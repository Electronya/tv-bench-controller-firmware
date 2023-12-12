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

