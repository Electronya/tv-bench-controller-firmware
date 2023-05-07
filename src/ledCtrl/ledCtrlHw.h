/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledCtrlHw.h
 * @author    jbacon
 * @date      2023-05-06
 * @brief     Led Control Hardware
 *
 *            This file is the declaration of the LED control hardware.
 *
 * @ingroup  ledCtrl
 *
 * @{
 */

#ifndef LED_CTRL_HW
#define LED_CTRL_HW

#include <zephyr/kernel.h>

/**
 * @brief   Validate that the LED strip device is ready.
 *
 * @return  True if the LED strip device is ready, false otherwise.
 */
bool ledCtrlIsDeviceReady(void);

#endif    /* LED_CTRL_HW */

/** @} */
