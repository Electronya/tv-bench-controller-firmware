/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledCtrl.h
 * @author    jbacon
 * @date      2023-05-06
 * @brief     Led Control Module
 *
 *            This file is the implementation of the LED control module.
 *
 * @ingroup  ledCtrl
 *
 * @{
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "ledCtrl.h"
#include "zephyrCommon.h"

#define LED_CTRL_MODULE_NAME led_ctrl_module

/* Setting module logging */
LOG_MODULE_REGISTER(LED_CTRL_MODULE_NAME);

#ifndef CONFIG_ZTEST
static ZephyrLedStrip_t ledStrip = {
  .dev = DEVICE_DT_GET(DT_ALIAS(led_strip)),
  .pixelCount = DT_PROP(DT_ALIAS(led_strip), chain_length),
};
#else
static ZephyrLedStrip_t ledStrip;
#endif

/**
 * @brief   The base colors pixel values.
*/
static const ZephyrRgbLed_t baseColors[] = {
  RGB(0xff, 0x00, 0x00),                /**< The red base color. */
  RGB(0x00, 0xff, 0x00),                /**< The green base color. */
  RGB(0x00, 0x00, 0xff),                /**< The blue base color. */
  RGB(0xff, 0xff, 0x00),                /**< The yellow base color. */
  RGB(0xff, 0x00, 0xff),                /**< The magenta base color. */
  RGB(0x00, 0xff, 0xff),                /**< The cyan base color. */
  RGB(0xff, 0xff, 0xff),                /**< The white base color. */
};

int ledCtrlInit(void)
{
  int rc;

  rc = zephyrLedStripInit(&ledStrip, ledStrip.pixelCount);
  if(rc < 0)
    LOG_ERR("unable to initialize the led strip: ERR %d", rc);
  return rc;
}

size_t ledCtrlGetMaxPixelCount(void)
{
  return zephyrLedStripGetPixelCnt(&ledStrip);
}

int ledCtrlSetToBaseColor(ZephyrRgbLed_t *pixels, size_t start, size_t end,
                          LedCtrlBaseColor_t color)
{
  size_t maxPixelCount = zephyrLedStripGetPixelCnt(&ledStrip);

  if(start > end || start >= maxPixelCount || end > maxPixelCount)
    return -EINVAL;

  for(uint8_t i = start; i < end; ++i)
  {
    pixels[i].r = baseColors[color].r;
    pixels[i].g = baseColors[color].g;
    pixels[i].b = baseColors[color].b;
  }

  return 0;
}

int ledCtrlUpdatePixels(ZephyrRgbLed_t *pixels, size_t start, size_t end)
{
  int rc = 0;
  size_t maxPixelCount = zephyrLedStripGetPixelCnt(&ledStrip);

  if(start > end || start >= maxPixelCount || end > maxPixelCount)
    return -EINVAL;

  rc = zephyrLedStripSetPixels(&ledStrip, start, end, pixels);
  if(rc < 0)
    return rc;

  rc = zephyrLedStripUpdate(&ledStrip);
  return rc;
}

/** @} */
