/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      colorManager.h
 * @author    jbacon
 * @date      2023-12-08
 * @brief     Color Manager Module
 *
 *            This file is the declaration of the color manager module.
 *
 * @defgroup  colorManager colorManger
 *
 * @{
 */

#ifndef COLOR_MANAGER
#define COLOR_MANAGER

#include "appMsg.h"
#include "zephyrLedStrip.h"

/**
 * @brief The color wheel value from red to blue.
*/
#define COLOR_WHEEL_RED_TO_BLU                0

/**
 * @brief The color wheel value from blue to green.
*/
#define COLOR_WHEEL_BLU_TO_GRN                85

/**
 * @brief The color wheel value from green to red.
*/
#define COLOR_WHEEL_GRN_TO_RED                170

/**
 * @brief   Set the given pixels to a single color.
 *
 * @param color       The pixel color.
 * @param pixels      The pixel buffer.
 * @param pixelCnt    The count of pixel to manage.
 */
void colorMngrSetSingle(Color_t *color, ZephyrRgbPixel_t *pixels,
                        size_t pixelCnt);

/**
 * @brief   Apply a constant fade on a set of pixel.
 *
 * @param fadeLvl     The amount of fade to use,
 * @param pixels      The pixel buffer.
 * @param pixelCnt    The count of pixel to manage.
 */
void colorMngrApplyFade(uint8_t fadeLvl, ZephyrRgbPixel_t *pixels,
                        size_t pixelCnt);

/**
 * @brief   Apply a fade trail on a set of pixels.
 *
 * @param fadeLvl     The amount of fade to use.
 * @param fadeStart   The strip ID marking the fade starting point.
 * @param isAscending The ascending fade flag.
 * @param pixels      The pixel buffer.
 * @param pixelCnt    The count pixel to manage.
 */
void colorMngrApplyFadeTrail(uint8_t fadeLvl, uint32_t fadeStart,
                             bool isAscending, ZephyrRgbPixel_t *pixels,
                             size_t pixelCnt);

/**
 * @brief   Update the color of a set of pixel in the given color range by the
 *          given step. The range is given by the color wheel start and end.
 *
 * @param wheelStart  The starting color of the color range.
 * @param wheelEnd    The ending color of the range.
 * @param reset       The reset flag.
 * @param pixels      The pixel buffer.
 * @param pixelCnt    The count of pixel to manage.
 */
void colorMngrUpdateRange(uint8_t wheelStart, uint8_t wheelEnd, bool reset,
                          ZephyrRgbPixel_t *pixels, size_t pixelCnt);

#endif    /* COLOR_MANAGER */

/** @} */
