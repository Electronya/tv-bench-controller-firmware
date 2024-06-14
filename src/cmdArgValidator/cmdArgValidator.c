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
