/**
 * Copyright (C) 2024 by Electronya
 *
 * @file      cmdArgValidator.h
 * @author    jbacon
 * @date      2024-06-14
 * @brief     Command Argument Validator Module
 *
 *            This file is the implementation of the command argument
 *            validator module.
 *
 * @ingroup  cmdArgValidator
 *
 * @{
 */

#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include "cmdArgValidator.h"
#include "configurator.h"

#define CMD_ARG_VALIDATOR_MODULE_NAME cmd_arg_validator_module

/* Setting module logging */
LOG_MODULE_REGISTER(CMD_ARG_VALIDATOR_MODULE_NAME);

/**
 * @brief The normal direction argument value.
*/
#define NORMAL_DIRECTION                    "normal"

/**
 * @brief The inverted direction argument value.
*/
#define INVERTED_DIRECTION                  "inverted"

bool isActiveLedCountValid(char *arg, size_t *ledCount)
{
  int rc = 0;

  *ledCount = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  if(*ledCount > configuratorGetMaxLedCount())
    return false;

  return true;
}

bool isSectionIdValid(char *arg, uint32_t *section)
{
  int rc = 0;

  *section = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  if(*section >= configuratorGetSectionCount())
    return false;

  return true;
}

bool isSectionCountValid(char *arg, size_t *sectionCount)
{
  int rc = 0;

  *sectionCount = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  if(*sectionCount > configuratorGetMaxSectionCount())
    return false;

  return true;
}

bool isSectionLedsValid(char **argv, size_t *firstLed, size_t *ledCount)
{
  int rc = 0;
  size_t activeLedCount;
  size_t lastLed;
  size_t sectionCount;
  size_t sectionLastLed;
  LedSection_t *section;

  *firstLed = shell_strtoul(argv[0], 10, &rc);
  if(rc < 0)
    return false;

  *ledCount = shell_strtoul(argv[1], 10, &rc);
  if(rc < 0)
    return false;

  activeLedCount = configuratorGetActiveLedCount();
  lastLed = *firstLed + *ledCount - 1;
  if(*firstLed >= activeLedCount || lastLed >= activeLedCount)
    return false;

  sectionCount = configuratorGetSectionCount();
  for(uint32_t i = 0; i < sectionCount; ++i)
  {
    rc = configuratorGetSection(i, &section);
    if(rc < 0)
    {
      LOG_ERR("unable to get section");
      return false;
    }

    sectionLastLed = section->firstLed + section->ledCount - 1;
    if((*firstLed >= section->firstLed && *firstLed <= sectionLastLed) ||
       (lastLed >= section->firstLed && lastLed <= sectionLastLed))
      return false;
  }

  return true;
}

bool isSectionSwitchValid(char *arg, size_t *switchId)
{
  int rc = 0;

  *switchId = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  if(*switchId >= MAX_SWITCH_COUNT)
    return false;

  return true;
}

bool isColorValid(char *arg, Color_t *color)
{
  int rc = 0;
  uint32_t convertColor;

  convertColor = shell_strtoul(arg, 16, &rc);
  if(rc < 0)
    return false;

  if(convertColor > 0xffffff)
    return false;

  color->hexColor = convertColor;

  return true;
}

bool isSeqLengthValid(char *arg, uint32_t *length)
{
  int rc = 0;

  *length = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  return true;
}

bool isSeqDirectionValid(char *arg, bool *isInverted)
{
  if(strcmp(arg, NORMAL_DIRECTION) == 0)
  {
    *isInverted = false;
    return true;
  }

  if(strcmp(arg, INVERTED_DIRECTION) == 0)
  {
    *isInverted = true;
    return true;
  }

  return false;
}

/** @} */
