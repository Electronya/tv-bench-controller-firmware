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

K_THREAD_STACK_DEFINE(ledMngr_stack, LED_MNGR_STACK_SIZE);

/**
 * @brief The Thread data structure.
*/
ZephyrThread_t thread = {
  .stack = ledMngr_stack,
  .stackSize = K_THREAD_STACK_SIZEOF(ledMngr_stack),
  .priority = LED_MNGR_PRIORITY,
};

/**
 * @brief   The LED manager thread.
 *
 * @param p1    First user parameter.
 * @param p2    Second user parameter.
 * @param p3    Third user parameter.
 */
void ledMngrThread(void *p1, void *p2, void *p3)
{
  // LedSequence_t seq;

  while(true)
  {

  }
}

void ledMngrInit(void)
{
  thread.entry = ledMngrThread;

  zephyrThreadCreate(&thread, LED_MNGR_THREAD_NAME, ZEPHYR_TIME_NO_WAIT,
    MILLI_SEC);
}

/** @} */
