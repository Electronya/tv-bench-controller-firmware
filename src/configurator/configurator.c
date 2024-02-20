/**
 * Copyright (C) 2024 by Electronya
 *
 * @file      configurator.h
 * @author    jbacon
 * @date      2024-02-16
 * @brief     Configurator Module
 *
 *            This file is the implementation of the configurator module.
 *
 * @ingroup  configurator
 *
 * @{
 */

#include <zephyr/logging/log.h>

#include "configurator.h"
#include "zephyrLedStrip.h"

#define CONFIGURATOR_MODULE_NAME configurator_module

/* Setting module logging */
LOG_MODULE_REGISTER(CONFIGURATOR_MODULE_NAME);

/**
 * @brief The configuration
*/
static Configuration_t config =
  {.isReady = false,
#ifndef CONFIG_ZTEST
   .maxLedCount = DT_PROP(DT_ALIAS(led_strip), chain_length),
#else
   .maxLedCount = 18,
#endif
  };

bool configuratorIsReady(void)
{
  return config.isReady;
}

void configuratorSetAsReady(void)
{
  config.isReady = true;
}

int configuratorSetActiveLedCount(uint8_t activeLedCount)
{
  if(activeLedCount == 0 || activeLedCount > config.maxLedCount)
    return -EINVAL;

  config.dynamicConfig.activeLedCount = activeLedCount;

  return 0;
}

/** @} */
