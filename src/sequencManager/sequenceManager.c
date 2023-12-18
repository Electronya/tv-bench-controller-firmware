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

/** @} */
