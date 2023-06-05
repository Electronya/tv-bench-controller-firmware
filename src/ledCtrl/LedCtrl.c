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
#include <zephyr/logging/log.h>

#include "LedCtrl.h"
#include "zephyrCommon.h"
#include "zephyrLedStrip.h"

#define LED_CTRL_MODULE_NAME led_ctrl_module

/* Setting module logging */
LOG_MODULE_REGISTER(LED_CTRL_MODULE_NAME);

#ifndef CONFIG_ZTEST
static ZephyrLedStrip ledStrip = {
  .dev = DEVICE_DT_GET(DT_ALIAS(led_strip)),
  .pixelCount = DT_PROP(DT_ALIAS(led_strip), chain_length),
};
#else
static ZephyrLedStrip ledStrip;
#endif

/**
 * @brief   The base colors pixel values.
*/
static const ZephyrRgbLed colors[] = {
  RGB(0xff, 0x00, 0x00),                /**< The red base color. */
  RGB(0x00, 0xff, 0x00),                /**< The green base color. */
  RGB(0x00, 0x00, 0xff),                /**< The blue base color. */
};

int ledCtrlInit(void)
{
  int rc;

  rc = zephyrLedStripInit(&ledStrip, LED_STRIP_COLOR_RGB, ledStrip.pixelCount);
  return rc;
  return 0;
}

/** @} */
