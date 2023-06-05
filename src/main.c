/**
 * Copyright (C) 2020 by Electronya
 *
 * @file      main.c
 * @author    jbacon
 * @date      2020-10-07
 * @brief     TV Bench Controller LED Coprocessor Firmware
 *
 * This file is the entry point for the Electronya TV Bench Controller LED driving coprocessor.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#include "LedCtrl.h"

#define MAIN_MODULE_NAME main_module

/* Setting module logging */
LOG_MODULE_REGISTER(MAIN_MODULE_NAME);

void main(void)
{
  int rc = 0;
  LedCtrlBaseColor color = LED_CTRL_RED;

  LOG_INF("booting tv bench controller");

  rc = ledCtrlInit();

  while(!(rc < 0))
  {
    rc = ledCtrlSetColor(color);
    if(rc < 0)
      LOG_ERR("Unable to set the led strip color.");
    ++color;
    if(color == LED_CTRL_CLR_CNT)
      color = LED_CTRL_RED;
    k_sleep(K_SECONDS(1));
  }
}
