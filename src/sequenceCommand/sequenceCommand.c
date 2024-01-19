/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      sequenceCommand.c
 * @author    jbacon
 * @date      2024-01-17
 * @brief     Sequence Command Module
 *
 *            This file is the implementation of the sequence command module.
 *
 * @defgroup  sequenceCommand sequenceCommand
 *
 * @{
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include "appMsg.h"

#define SEQUENCEL_COMMAND_MODULE_NAME sequence_command_module

/* Setting module logging */
LOG_MODULE_REGISTER(SEQUENCEL_COMMAND_MODULE_NAME);

/**
 * @brief The sequence command usage.
*/
#define SEQ_USAGE           "Set a specified sequence."

/**
 * @brief The solid color sequence command usage.
*/
#define SEQ_SOLID_USAGE     "Set a solid color sequence: sequence solid <section> <HEX color>."

/**
 * @brief The solid color sequence argment count
*/
#define SOLID_SEQ_ARG_CNT                   2

/**
 * @brief   Convert and check validity of the section.
 *
 * @param arg
 * @param section
 * @return true
 * @return false
 */
bool isSectionValid(char *arg, uint32_t *section)
{
  int rc = 0;

  *section = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  // TODO: validate with settings.
  return true;
}


/** @} */
