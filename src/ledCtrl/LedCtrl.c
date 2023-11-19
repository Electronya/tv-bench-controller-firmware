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
static const ZephyrRgbLed baseColors[] = {
  RGB(0x0f, 0x00, 0x00),                /**< The red base color. */
  RGB(0x00, 0x0f, 0x00),                /**< The green base color. */
  RGB(0x00, 0x00, 0x0f),                /**< The blue base color. */
};

int ledCtrlInit(void)
{
  int rc;

  rc = zephyrLedStripInit(&ledStrip, ledStrip.pixelCount);
  if(rc < 0)
    LOG_ERR("unable to initialize the led strip: ERR %d", rc);
  return rc;
}

int ledCtrlSetColor(LedCtrlBaseColor color)
{
  int rc = 0;
  ZephyrRgbLed colors[8];

  for( uint8_t i = 0; i < 8; ++i)
    colors[i] = baseColors[color];

  LOG_DBG("Setting strip color to %d color index.", color);

  rc = zephyrLedStripSetPixels(&ledStrip, 0, ledStrip.pixelCount, colors);
  if(rc < 0)
    return rc;

  rc = zephyrLedStripUpdate(&ledStrip);

  return rc;
}

/** @} */
