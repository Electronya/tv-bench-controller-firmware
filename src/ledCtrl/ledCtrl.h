/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledCtrl.h
 * @author    jbacon
 * @date      2023-05-09
 * @brief     Led Control Module
 *
 *            This file is the declaration of the LED control module.
 *
 * @defgroup  ledCtrl ledCtrl
 *
 * @{
 */

#ifndef LED_CTRL
#define LED_CTRL

#include "zephyrLedStrip.h"

/**
 * @brief   Base colors.
*/
typedef enum
{
  LED_COLOR_RED = 0,            /**< The RED base color. */
  LED_COLOR_GRN,                /**< The GREEN base color. */
  LED_COLOR_BLU,                /**< The BLUE base color. */
  LED_COLOR_YEL,                /**< The YELLOW base color. */
  LED_COLOR_MAG,                /**< The MAGENTA base color. */
  LED_COLOR_CYAN,               /**< The CYAN base color. */
  LED_COLOR_WHT,                /**< The WHITE base color. */
  LED_COLOR_CNT,                /**< The count of supported base color. */
} LedCtrlBaseColor_t;

/**
 * @brief   Initialize the module.
 *
 * @return  0 if successful, the error code otherwise.
*/
int ledCtrlInit(void);

/**
 * @brief   Get the miximal pixel count.
 *
 * @return  The miximal pixel count.
 */
size_t ledCtrlGetMaxPixelCount(void);

/**
 * @brief   Set a group a pixel to a base color.
 *
 * @param pixels    The pixel buffer.
 * @param start     The start index of the pixel group.
 * @param end       The non inclusive end index of the pixel group.
 * @param color     The desired base color.
 *
 * @return          0 if successful, the error code otherwise.
 */
int ledCtrlSetToBaseColor(ZephyrRgbLed_t *pixels, size_t start, size_t end,
                          LedCtrlBaseColor_t color);

/**
 * @brief   Update a group of pixel to the given colors.
 *
 * @param pixels    The pixel buffer.
 * @param start     The start index of the pixel group.
 * @param end       The non inclusive end index of the pixel group.
 *
 * @return  0 if successful, the error code otherwise.
 */
int ledCtrlUpdatePixels(ZephyrRgbLed_t *pixels, size_t start, size_t end);

#endif    /* LED_CTRL */

/** @} */
