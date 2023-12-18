/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      sequenceManager.h
 * @author    jbacon
 * @date      2023-12-18
 * @brief     Sequence Manager Module
 *
 *            This file is the declaration of the sequence manager module.
 *
 * @defgroup  sequenceManager sequenceManger
 *
 * @{
 */

#ifndef SEQUENCE_MANAGER
#define SEQUENCE_MANAGER

#include "appMsg.h"
#include "zephyrLedStrip.h"

/**
 * @brief   Update the pixels for the next solid color frame.
 *
 * @param color       The next solid color.
 * @param pixels      The pixel buffer.
 * @param pixelCnt    The pixel count.
 */
void seqMngrUpdateSolidFrame(Color_t *color, ZephyrRgbPixel_t *pixels,
                             size_t pixelCnt);

#endif    /* SEQUENCE_MANAGER */

/** @} */
