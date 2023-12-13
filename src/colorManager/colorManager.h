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

/**
 * @brief   Allocate an array of colors
 *
 * @param pixelCount
 * @param colors
 * @return int
 */
int colorMngrAllocateColors(size_t pixelCount, Color_t *colors);

/**
 * @brief   Set the given LED to a single color.
 *
 * @param color       The LED color.
 * @param firstLed    The strip ID of the first LED.
 * @param lastLed     The strip ID of the last LED.
 *
 * @return  0 if successful, the error code otherwise.
 */
int colorMngrSetSingle(Color_t *color, uint32_t firstLed, uint32_t lasLed);

/**
 * @brief   Set the given LEDs to a fading color.
 *
 * @param color       The LED base color.
 * @param fadeLvl     The amount of fade to use.
 * @param fadeStart   The strip ID marking the fade starting point.
 * @param firstLed    The strip ID of the section first LED.
 * @param lastLed     The strip ID of the section last LED.
 * @param isAscending The ascending fade flag.
 *
 * @return  0 if successful, the error code otherwise.
 */
int colorMngrSetFade(Color_t *color, uint32_t fadeLvl, uint32_t fadeStart,
                     uint32_t firstLed, uint32_t lastLed, bool isAscending);

#endif    /* COLOR_MANAGER */

/** @} */
