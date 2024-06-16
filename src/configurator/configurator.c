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
   .dynamicConfig.activeLedCount = 0,
   .dynamicConfig.sectionCount = 0,
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

size_t configuratorGetMaxLedCount(void)
{
  return config.maxLedCount;
}

size_t configuratorGetActiveLedCount(void)
{
  return config.dynamicConfig.activeLedCount;
}

int configuratorSetActiveLedCount(size_t activeLedCount)
{
  if(activeLedCount == 0 || activeLedCount > config.maxLedCount)
    return -EINVAL;

  config.dynamicConfig.activeLedCount = activeLedCount;

  return 0;
}

size_t configuratorGetMaxSectionCount(void)
{
  return MAX_SECTION_COUNT;
}

size_t configuratorGetSectionCount(void)
{
  return config.dynamicConfig.sectionCount;
}

int configuratorSetSectionCount(size_t seqCount)
{
  if(seqCount == 0 || seqCount > MAX_SECTION_COUNT)
    return -EINVAL;

  config.dynamicConfig.sectionCount = seqCount;

  return 0;
}

int configuratorGetSectionLeds(size_t sectionId, size_t *firstLed,
                               size_t *ledCount)
{
  if(!config.isReady)
    return -EPERM;

  if(sectionId >= config.dynamicConfig.sectionCount)
    return -ERANGE;

  *firstLed = config.dynamicConfig.sections[sectionId].firstLed;
  *ledCount = config.dynamicConfig.sections[sectionId].ledCount;

  return 0;
}

int configuratorSetSectionLeds(size_t sectionId, size_t firstLed,
                               size_t ledCount)
{
  if(sectionId >= config.dynamicConfig.sectionCount)
    return -ERANGE;

  config.dynamicConfig.sections[sectionId].firstLed = firstLed;
  config.dynamicConfig.sections[sectionId].ledCount = ledCount;

  return 0;
}

int configuratorGetSectionSwitches(size_t sectionId, size_t *switchId,
                                   Color_t *color)
{
  if(!config.isReady)
    return -EPERM;

  if(sectionId >= config.dynamicConfig.sectionCount)
    return -ERANGE;

  *switchId = config.dynamicConfig.sections[sectionId].switchId;
  color->hexColor = config.dynamicConfig.sections[sectionId].switchSeq.startColor.hexColor;

  return 0;
}

int configuratorGetSection(size_t index, LedSection_t **section)
{
  if(!config.isReady)
    return -EPERM;

  if(index >= config.dynamicConfig.sectionCount)
    return -EINVAL;

  *section = config.dynamicConfig.sections + index;

  return 0;
}

int configuratorSetSectionConfig(size_t index, LedSection_t *section)
{
  if(config.dynamicConfig.sectionCount == 0)
    return -EPERM;

  if(index >= config.dynamicConfig.sectionCount ||
     section->switchId >= MAX_SWITCH_COUNT)
    return -EINVAL;

  memcpy(config.dynamicConfig.sections + index, section, sizeof(LedSection_t));

  return 0;
}

/** @} */
