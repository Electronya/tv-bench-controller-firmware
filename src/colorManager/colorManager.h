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
 * @brief   Set the given LED to a single color.
 *
 * @param color       The LED color.
 * @param firstLed    The strip ID of the first LED.
 * @param lastLed     The strip ID of the last LED.
 *
 * @return  0 if successful, the error code otherwise.
 */
int colorMngrSetSingle(Color_t *color, uint32_t firstLed, uint32_t lasLed);

