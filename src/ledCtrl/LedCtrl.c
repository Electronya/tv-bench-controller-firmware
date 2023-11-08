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
  .timingCntr = {
    .dev = DEVICE_DT_GET(DT_ALIAS(strip_counter)),
  },
  .dataLine = {
    .label = DT_PROP(DT_ALIAS(strip_dataline), label),
    .dev = GPIO_DT_SPEC_GET_OR(DT_ALIAS(strip_dataline), gpios, {0}),
  },
  .pixelCount = 5,
  .t0h = 300,
  .t0l = 800,
  .t1h = 750,
  .t1l = 200,
  .rst = 200000,
};
#else
static ZephyrLedStrip ledStrip;
#endif

/**
 * @brief   The base colors pixel values.
*/
static const ZephyrGrbPixel colors[] = {
  RGB(0xff, 0x00, 0x00),                /**< The red base color. */
  RGB(0x00, 0xff, 0x00),                /**< The green base color. */
  RGB(0x00, 0x00, 0xff),                /**< The blue base color. */
  RGB(0x00, 0xff, 0x00),
};

int ledCtrlInit(void)
{
  int rc;

  rc = zephyrLedStripInit(&ledStrip, LED_STRIP_COLOR_GRB, ledStrip.pixelCount);
  if(rc < 0)
    LOG_ERR("unable to initialize the led strip: ERR %d", rc);
  return rc;
}

int ledCtrlSetColor(LedCtrlBaseColor color)
{
  int rc = 0;
  ZephyrGrbPixel colors[5] = {RGB(0xaa, 0xaa, 0xaa),
                              RGB(0xaa, 0xaa, 0xaa),
                              RGB(0xaa, 0xaa, 0xaa),
                              RGB(0xaa, 0xaa, 0xaa),
                              RGB(0xaa, 0xaa, 0xaa)};

  LOG_DBG("Setting strip color to %d color index.", color);

  rc = zephyrLedStripSetGrbPixels(&ledStrip, 0, ledStrip.pixelCount, colors);
  if(rc < 0)
    return rc;

  rc = zephyrLedStripUpdate(&ledStrip);

  return rc;
}

/** @} */
