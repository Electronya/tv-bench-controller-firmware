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
 * @brief The maximum switch count
*/
#define MAX_SWITCH_COUNT                          3

/**
 * @brief The LED section data structure.
*/
typedef struct
{
  size_t firstLed;
  size_t lastLed;
  size_t switchId;
  LedSequence_t switchSeq;
} LedSection_t;

/**
 * @brief The dynamic configuration data structure.
*/
typedef struct
{
  size_t activeLedCount;
  size_t sectionCount;
  LedSection_t sections[MAX_SECTION_COUNT];
} DynamicConfig_t;

/**
 * @brief The configuration data structure.
*/
typedef struct
{
  bool isReady;
  size_t maxLedCount;
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

/**
 * @brief   Set the active LED count.
 *
 * @param activeLedCount  The active LED count.
 *
 * @return  o if successful, the error code otherwise.
 */
int configuratorSetActiveLedCount(size_t activeLedCount);

/**
 * @brief   Set the section count.
 *
 * @param seqCount        The section count.
 *
 * @return  0 if successful, the error code otherwise.
 */
int configuratorSetSectionCount(size_t seqCount);

/**
 * @brief   Set the section at index configuration
 *
 * @param index           The section index.
 * @param section         The section configuration.
 *
 * @return  0 if successful, the error code otherwise.
 */
int configuratorSetSectionConfig(size_t index, LedSection_t *section);

/**
 * @brief   Get the maximum number of LED in the strip.
 *
 * @return  The maximum allowed count of LED in the strip.
 */
size_t configuratorGetMaxLedCount(void);

/**
 * @brief   Get the active LED count in the strip.
 *
 * @param activeLedCount  The active LED count in the strip.
 *
 * @return  0 if successful, the error code otherwise.
 */
int configuratorGetActiveLedCount(size_t *activeLedCount);

/**
 * @brief   Get a specific section configuration.
 *
 * @param index           The section index.
 * @param section         The section configuration.
 *
 * @return  0 if successful, the erroro code otherwise.
 */
int configuratorGetSection(size_t index, LedSection_t **section);

#endif    /* CONFIGURATOR */

/** @} */
