/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledManager.h
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
#include "ledCtrl.h"
#include "zephyrLedStrip.h"
#include "zephyrThread.h"

#define LED_Mngr_MODULE_NAME led_mngr_module

/* Setting module logging */
LOG_MODULE_REGISTER(LED_Mngr_MODULE_NAME);

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
  LedSequence_t seq = {
    .seqType = SEQ_SOLID,
    .timeBase = ZEPHYR_TIME_FOREVER,
    .colorType = COLOR_SINGLE,
    .startColor.hexColor = 0x00000000,
  };

  while(true)
  {
    rc = appMsgPopLedSequence(&seq);

    switch(seq.seqType)
    {
      case SEQ_SOLID:
      break;
      case SEQ_CHASER:
      break;
      case SEQ_INVERT_CHASER:
      break;
      default:
        LOG_ERR("unsupported sequence type");
        return;
      break;
    }

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

void ledMngrInit(void)
{
  thread.entry = ledMngrThread;

  zephyrThreadCreate(&thread, LED_MNGR_THREAD_NAME, ZEPHYR_TIME_NO_WAIT,
    MILLI_SEC);
}

/** @} */
