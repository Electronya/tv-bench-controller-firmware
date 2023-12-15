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

#include "colorManager.h"
#include "ledCtrl.h"
#include "zephyrLedStrip.h"

#define COLOR_MNGR_MODULE_NAME color_mngr_module

/* Setting module logging */
LOG_MODULE_REGISTER(COLOR_MNGR_MODULE_NAME);

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

void colorMngrApplyFade(uint32_t fadeLvl, ZephyrRgbPixel_t *pixels,
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

void colorMngrApplyFadeTrail(uint32_t fadeLvl, uint32_t fadeStart,
                             bool isAscending, ZephyrRgbPixel_t *pixels,
                             size_t pixelCnt)
{
  ZephyrRgbPixel_t *pixelPntr = pixels + fadeStart;
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

/** @} */
