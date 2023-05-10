/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledCtrlHw.h
 * @author    jbacon
 * @date      2023-05-06
 * @brief     Led Control Hardware
 *
 *            This file is the implementation of the LED control module.
 *
 * @ingroup  ledCtrl
 *
 * @{
 */

#include <zephyr/kernel.h>

#include "LedCtrl.h"
#include "zephyrCommon.h"
#include "zephyrLedStrip.h"

#ifndef CONFIG_ZTEST
static ZephyrLedStrip ledStrip = {
  .dev = DEVICE_DT_GET(DT_ALIAS(led_strip)),
  .pixelCount = DT_PROP(DT_ALIAS(led_strip), chain_length),
};
#else
static ZephyrLedStrip ledStrip;
#endif

int ledCtrlInit(void)
{
  int rc;

  rc = zephyrLedStripInit(&ledStrip, LED_STRIP_COLOR_RGB, ledStrip.pixelCount);
  return 0;
}

/** @} */
