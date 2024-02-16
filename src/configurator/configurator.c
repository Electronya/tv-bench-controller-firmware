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

#define CONFIGURATOR_MODULE_NAME configurator_module

/* Setting module logging */
LOG_MODULE_REGISTER(CONFIGURATOR_MODULE_NAME);

/**
 * @brief The configuration data structure.
*/
typedef struct
{
  bool isReady;
} Configuration_t;

static Configuration_t config = {.isReady = false};

bool configuratorIsReady(void)
{
  return config.isReady;
}

/** @} */
