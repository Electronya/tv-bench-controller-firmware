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
 * @brief The breather sequence command usage.
*/
#define SEQ_BREATHER_USAGE  "Set a breather sequence: sequence breather <section> <HEX color> <sequence length (sec)>."

/**
 * @brief The solid color sequence argment count
*/
#define SOLID_SEQ_ARG_CNT                   2

/**
 * @brief The breather sequence argument count
*/
#define BREATHER_SEQ_ARG_CNT                3

/**
 * @brief   Convert and check validity of the section.
 *
 * @param arg       The section string argument value.
 * @param section   The converted section.
 *
 * @return  true if the section is valid, false otherwise.
 */
static bool isSectionValid(char *arg, uint32_t *section)
{
  int rc = 0;

  *section = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  // TODO: validate with settings.
  return true;
}

/**
 * @brief   Convert and check the validity of the color.
 *
 * @param arg     The color string argument.
 * @param color   The converted color.
 *
 * @return  true if the color is valid, false otherwise.
 */
static bool isColorValid(char *arg, Color_t *color)
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

/**
 * @brief   Convert and check the validity of the sequence length.
 *
 * @param arg       The sequence length string argument.
 * @param length    The converted sequence length.
 *
 * @return  true if the sequence length is valid, false otherwise.
 */
static bool isLengthValid(char *arg, uint32_t *length)
{
  int rc = 0;

  *length = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  return true;
}

/**
 * @brief   Push a solid color sequence in the sequence queue.
 *
 * @param section   The LED strip section.
 * @param color     The solid color.
 *
 * @return  0 if successful, the error code othewise.
 */
static int pushSolidColorSequence(uint32_t section, Color_t *color)
{
  LedSequence_t sequence = {.sectionId = section,
                            .seqType = SEQ_SOLID,
                            .timeBase = ZEPHYR_TIME_FOREVER,
                            .timeUnit = SECONDS};

  sequence.startColor.hexColor = color->hexColor;

  return appMsgPushLedSequence(&sequence);
}

static int pushBreatherSequence(uint32_t section, Color_t *color,
                                uint32_t length)
{
  LedSequence_t sequence = {.sectionId = section,
                            .seqType = SEQ_SOLID_BREATHER,
                            .timeBase = length,
                            .timeUnit = SECONDS};

  sequence.startColor.hexColor = color->hexColor;

  return appMsgPushLedSequence(&sequence);
}

/**
 * @brief   Execute the solid color sequence command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execSolidSeq(const struct shell *shell, size_t argc, char **argv)
{
  int rc;
  uint32_t section;
  Color_t color;

  if(isSectionValid(argv[0], &section) && isColorValid(argv[1], &color))
  {
    rc = pushSolidColorSequence(section, &color);
    if(rc < 0)
    {
      LOG_ERR("unable to push the solid sequence");
      return rc;
    }

    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid arguments. section: %s, color: %s",
    argv[0], argv[1]);

  return -EINVAL;
}

/**
 * @brief   Execute the breather sequence command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execBreatherSeq(const struct shell *shell, size_t argc, char **argv)
{
  int rc;
  uint32_t section;
  Color_t color;
  uint32_t length;

  if(isSectionValid(argv[0], &section) && isColorValid(argv[1], &color) &&
    isLengthValid(argv[2], &length))
  {
    rc = pushBreatherSequence(section, &color, length);
    if(rc < 0)
    {
      LOG_ERR("unable to push the breather sequence");
      return rc;
    }
    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid arguments. section: %s, color: %s, length %s",
    argv[0], argv[1], argv[2]);

  return -EINVAL;
}

SHELL_STATIC_SUBCMD_SET_CREATE(seq_sub,
	SHELL_CMD_ARG(solid, NULL, SEQ_SOLID_USAGE, execSolidSeq,
                SOLID_SEQ_ARG_CNT, 0),
  SHELL_CMD_ARG(breather, NULL, SEQ_BREATHER_USAGE, execBreatherSeq,
                BREATHER_SEQ_ARG_CNT, 0),
	SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(sequence, &seq_sub, SEQ_USAGE,	NULL);

/** @} */
