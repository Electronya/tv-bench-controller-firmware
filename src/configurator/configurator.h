/**
 * Copyright (C) 2024 by Electronya
 *
 * @file      configurator.h
 * @author    jbacon
 * @date      2024-02-16
 * @brief     Configurator Module
 *
 *            This file is the declaration of the configurator module.
 *
 * @defgroup  configurator configurator
 *
 * @{
 */

#ifndef CONFIGURATOR
#define CONFIGURATOR

#include "appMsg.h"

/**
 * @brief The maximum number of section
*/
#define MAX_SECTION_COUNT                         10

/**
 * @brief The LED section data structure.
*/
typedef struct
{
  uint8_t firstLed;
  uint8_t lastLed;
  uint8_t switchId;
  LedSequence_t switchSeq;
} LedSection_t;

/**
 * @brief The dynamic configuration data structure.
*/
typedef struct
{
  uint8_t activeLedCount;
  uint8_t sectionCount;
  LedSection_t sections[MAX_SECTION_COUNT];
} DynamicConfig_t;

/**
 * @brief The configuration data structure.
*/
typedef struct
{
  bool isReady;
  uint8_t maxLedCount;
  DynamicConfig_t dynamicConfig;
} Configuration_t;

/**
 * @brief   Check if the configuration was uploaded by the Pi and
 *          is ready to use.
 *
 * @return  true if the configuration is ready, false otherwise.
 */
bool configuratorIsReady(void);

/**
 * @brief   Set the configuration as ready after it was uploaded by the Pi.
 */
void configuratorSetAsReady(void);

#endif    /* CONFIGURATOR */

/** @} */
