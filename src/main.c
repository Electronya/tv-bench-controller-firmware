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

#include "appMsg.h"
#include "ledManager.h"
#include "zephyrLedStrip.h"

#define MAIN_MODULE_NAME main_module

/* Setting module logging */
LOG_MODULE_REGISTER(MAIN_MODULE_NAME);

int main(void)
{
  int rc;

  LOG_INF("booting tv bench controller");

  rc = appMsgInit();
  if(rc < 0)
  {
    LOG_ERR("unable to initialize the application messages.");
    return rc;
  }

  rc = ledMngrInit();
  if(rc < 0)
  {
    LOG_ERR("unable to initialize the LED manager.");
    return rc;
  }

  return 0;
}
