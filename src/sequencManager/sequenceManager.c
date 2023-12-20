/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      sequenceManager.h
 * @author    jbacon
 * @date      2023-12-18
 * @brief     Color Manager Module
 *
 *            This file is the implementation of the sequence manager module.
 *
 * @ingroup  sequenceManager
 *
 * @{
 */

#include <zephyr/logging/log.h>

#include "sequenceManager.h"
#include "colorManager.h"
#include "zephyrLedStrip.h"

#define SEQ_MNGR_MODULE_NAME  seq_mngr_module

/* Setting module logging */
LOG_MODULE_REGISTER(SEQ_MNGR_MODULE_NAME);

void seqMngrUpdateSolidFrame(Color_t *color, ZephyrRgbPixel_t *pixels,
                             size_t pixelCnt)
{
  colorMngrSetSingle(color, pixels, pixelCnt);
}

void seqMngrUpdateSingleBreatherFrame(Color_t *color, uint8_t step, bool reset,
                                      ZephyrRgbPixel_t *pixels, size_t pixelCnt)
{
  static uint16_t frameCntr = 0;
  static bool exhale = true;

  if(reset)
  {
    frameCntr = 0;
    colorMngrSetSingle(color, pixels, pixelCnt);
    frameCntr += step;
  }
  else
  {
    if(exhale)
      colorMngrApplyFade(step, pixels, pixelCnt);
    else
      colorMngrApplyUnfade(step, pixels, pixelCnt);
    frameCntr += step;
    if(frameCntr > 255)
    {
      frameCntr = 0;
      exhale = !exhale;
    }
  }
}

void seqMngrUpdateFadeChaserFrame(Color_t *color, bool isInverted, bool reset,
                                  ZephyrRgbPixel_t *pixels, size_t pixelCnt)
{
  static ZephyrRgbPixel_t *chaserPoint = NULL;
  uint8_t step;

  if(color->r < color->g && color->r < color->b)
    step = color->r / pixelCnt;
  else if(color->g < color->r && color->g < color->b)
    step = color->g / pixelCnt;
  else
    step = color->b / pixelCnt;

  if(reset)
    chaserPoint = isInverted ? pixels + pixelCnt - 1 : pixels;

  colorMngrSetSingle(color, pixels, pixelCnt);
  colorMngrApplyFadeTrail(step, chaserPoint - pixels, !isInverted, pixels,
    pixelCnt);

  if(isInverted)
  {
    --chaserPoint;
    if(chaserPoint < pixels)
      chaserPoint = pixels + pixelCnt - 1;
  }
  else
  {
    ++chaserPoint;
    if(chaserPoint == pixels + pixelCnt)
      chaserPoint = pixels;
  }

}

/** @} */
