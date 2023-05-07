/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledCtrlHw.h
 * @author    jbacon
 * @date      2023-05-06
 * @brief     Led Control Hardware
 *
 *            This file is the definition of the LED control hardware.
 *
 * @ingroup  ledCtrl
 *
 * @{
 */

#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include "ledCtrlHw.h"

#ifndef CONFIG_ZTEST
#define STRIP_NODE		    DT_ALIAS(led_strip)
#define STRIP_NUM_PIXELS  DT_PROP(DT_ALIAS(led_strip), chain_length)
#else
#define STRIP_NODE		    0x10
#define STRIP_NUM_PIXELS	5
#endif

#define RGB(_r, _g, _b)   { .r = (_r), .g = (_g), .b = (_b) }

// static const struct led_rgb colors[] = {
// 	RGB(0x0f, 0x00, 0x00), /* red */
// 	RGB(0x00, 0x0f, 0x00), /* green */
// 	RGB(0x00, 0x00, 0x0f), /* blue */
// };

// struct led_rgb pixels[STRIP_NUM_PIXELS];

#ifndef CONFIG_ZTEST
static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);
#else
// const struct device stripDev;
// static const struct device *const strip = &stripDev;
#endif

LOG_MODULE_DECLARE(ledCtrl);

bool ledCtrlIsDeviceReady(void)
{
  return true;
}

/** @} */
