/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledManager.c
 * @author    jbacon
 * @date      2023-11-20
 * @brief     Led Manager Module
 *
 *            This file is the implementation of the LED manager module.
 *
 * @ingroup  ledManager
 *
 * @{
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "appMsg.h"
#include "sequenceManager.h"
#include "zephyrLedStrip.h"
#include "zephyrThread.h"

#define LED_MNGR_MODULE_NAME led_mngr_module

/* Setting module logging */
LOG_MODULE_REGISTER(LED_MNGR_MODULE_NAME);

/**
 * @brief The thread name.
*/
#define LED_MNGR_THREAD_NAME                        "ledMngr"

/**
 * @brief The thread stack size.
*/
#define LED_MNGR_STACK_SIZE                         256

/**
 * @brief The thread priority.
*/
#define LED_MNGR_PRIORITY                           1

/**
 * @brief The thread default sleep time.
*/
#define LED_MNGR_DEFAULT_SLEEP                      100

K_THREAD_STACK_DEFINE(ledMngr_stack, LED_MNGR_STACK_SIZE);

#ifndef CONFIG_ZTEST
static ZephyrLedStrip_t ledStrip = {
  .dev = DEVICE_DT_GET(DT_ALIAS(led_strip)),
  .pixelCount = DT_PROP(DT_ALIAS(led_strip), chain_length),
};
#else
static ZephyrLedStrip_t ledStrip;
#endif

typedef struct
{
  uint32_t firstLed;            /**< The strip ID of the section first LED. */
  uint32_t lastLed;             /**< The strip ID of the section last LED. */
} LedSection_t;

/**
 * @brief The Thread data structure.
*/
ZephyrThread_t thread = {
  .stack = ledMngr_stack,
  .stackSize = K_THREAD_STACK_SIZEOF(ledMngr_stack),
  .priority = LED_MNGR_PRIORITY,
};

LedSection_t section;

/**
 * @brief   The LED manager thread.
 *
 * @param p1          First user parameter.
 * @param p2          Second user parameter.
 * @param p3          Third user parameter.
 */
static void ledMngrThread(void *p1, void *p2, void *p3)
{
  int rc;
  bool reset = true;
  LedSequence_t seq = {
    .seqType = SEQ_SOLID,
    .timeBase = ZEPHYR_TIME_FOREVER,
    .startColor.hexColor = 0xffffff,
  };

  while(true)
  {
    rc = appMsgPopLedSequence(&seq);
    if(rc == 0)
      reset = true;
    else
      reset = false;

    switch(seq.seqType)
    {
      case SEQ_SOLID:
        seqMngrUpdateSolidFrame(&seq.startColor, ledStrip.rgbPixels,
          ledStrip.pixelCount);
      break;
      case SEQ_SOLID_BREATHER:
        /* TODO calculate the steps base on the sequence time base and the starting color */
        seqMngrUpdateSingleBreatherFrame(&seq.startColor, 10, reset,
          ledStrip.rgbPixels, ledStrip.pixelCount);
      break;
      case SEQ_FADE_CHASER:
        seqMngrUpdateFadeChaserFrame(&seq.startColor, false, reset,
          ledStrip.rgbPixels, ledStrip.pixelCount);
      break;
      case SEQ_INVERT_FADE_CHASER:
        seqMngrUpdateFadeChaserFrame(&seq.startColor, true, reset,
          ledStrip.rgbPixels, ledStrip.pixelCount);
      break;
      case SEQ_COLOR_RANGE:
        seqMngrUpdateColorRangeFrame(&seq.startColor, &seq.endColor, reset,
          ledStrip.rgbPixels, ledStrip.pixelCount);
      break;
      case SEQ_RANGE_CHASER:
        seqMngrUpdateColorRangeChaserFrame(&seq.startColor, &seq.endColor, false,
          reset, ledStrip.rgbPixels, ledStrip.pixelCount);
      break;
      case SEQ_INVERT_RANGE_CHASER:
        seqMngrUpdateColorRangeChaserFrame(&seq.startColor, &seq.endColor, true,
          reset, ledStrip.rgbPixels, ledStrip.pixelCount);
      break;;
      default:
        LOG_ERR("unsupported sequence type");
        return;
      break;
    }

    /* TODO calculate the sleep from the sequence time base */
    if(seq.timeBase != ZEPHYR_TIME_FOREVER)
    {
      rc = zephyrThreadSleep(seq.timeBase, seq.timeUnit);
      if(rc < 0)
        LOG_ERR("unable to sleep sequence time base");
    }
    else
    {
      rc = zephyrThreadSleep(LED_MNGR_DEFAULT_SLEEP, MILLI_SEC);
      if(rc < 0)
        LOG_ERR("unable to sleep default time");
    }

    if(rc < 0)
      return;
  }
}

int ledMngrInit(void)
{
  int rc;

  rc = zephyrLedStripInit(&ledStrip, ledStrip.pixelCount);
  if(rc < 0)
    return rc;

  thread.entry = ledMngrThread;
  zephyrThreadCreate(&thread, LED_MNGR_THREAD_NAME, ZEPHYR_TIME_NO_WAIT,
    MILLI_SEC);

  return rc;
}

/** @} */
