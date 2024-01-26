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

#include <string.h>

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
 * @brief The fade chaser sequence command usage.
*/
#define SEQ_FADE_CHASER_USAGE  "Set a fade chaser sequence: sequence fade_chaser <section> <HEX color> <sequence length (sec)> <direction>."

/**
 * @brief The color range sequence command usage.
*/
#define SEQ_COLOR_RANGE_USAGE  "Set a color range sequence: sequence range <section> <HEX start color> <HEX end color> <sequence length (sec)>."

/**
 * @brief The color range sequence command usage.
*/
#define SEQ_RANGE_CHASER_USAGE  "Set a color range sequence: sequence range <section> <HEX start color> <HEX end color> <sequence length (sec)> <direction>."

/**
 * @brief The solid color sequence argment count
*/
#define SOLID_SEQ_ARG_CNT                   2

/**
 * @brief The breather sequence argument count
*/
#define BREATHER_SEQ_ARG_CNT                3

/**
 * @brief The fade chaser sequence argument count
*/
#define FADE_CHASER_SEQ_ARG_CNT             4

/**
 * @brief The color range sequence argument count
*/
#define COLOR_RANGE_SEQ_ARG_CNT             4

/**
 * @brief The color range chser sequence argument count
*/
#define RANGE_CHASER_SEQ_ARG_CNT             5

/**
 * @brief The normal direction argument value.
*/
#define NORMAL_DIRECTION                    "normal"

/**
 * @brief The inverted direction argument value.
*/
#define INVERTED_DIRECTION                  "inverted"

/**
 * @brief   Convert and check validity of the section.
 *
 * @param arg         The section string argument value.
 * @param section     The converted section.
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
 * @param arg       The color string argument.
 * @param color     The converted color.
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
 * @param arg         The sequence length string argument.
 * @param length      The converted sequence length.
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
 * @brief   Convert and check the validity of the sequence direction.
 *
 * @param arg         The sequence direction string argument.
 * @param isInverted  The converted direction flag.
 *
 * @return  true if the sequence direction is valid, false otherwise.
 */
static bool isDirectionValid(char *arg, bool *isInverted)
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

/**
 * @brief   Push a solid color sequence in the sequence queue.
 *
 * @param section     The LED strip section.
 * @param color       The solid color.
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

/**
 * @brief   Push a breather sequence in the sequence queue.
 *
 * @param section     The LED strip section.
 * @param color       The breather color.
 * @param length      The length of the breath.
 *
 * @return  0 if successful, the error code otherwise.
 */
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
 * @brief   Push a fade chaser sequence in the sequence queue.
 *
 * @param section       The LED strip section.
 * @param color         The chaser color.
 * @param length        The chaser full travel time length.
 * @param isinverted    The inversion flag.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int pushFadeChaserSequence(uint32_t section, Color_t *color,
                                  uint32_t length, bool isinverted)
{
  LedSequence_t sequence = {.sectionId = section,
                            .timeBase = length,
                            .timeUnit = SECONDS};

  sequence.seqType = isinverted ? SEQ_INVERT_FADE_CHASER : SEQ_FADE_CHASER;

  sequence.startColor.hexColor = color->hexColor;

  return appMsgPushLedSequence(&sequence);
}

/**
 * @brief   Push a color range sequence in the sequence queue.
 *
 * @param section       The LED strip section.
 * @param startClr      The range start color.
 * @param endClr        The range end color.
 * @param length        The full range time length.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int pushColorRangeSequence(uint32_t section, Color_t *startClr,
                                  Color_t *endClr, uint32_t length)
{
  LedSequence_t sequence = {.sectionId = section,
                            .seqType = SEQ_COLOR_RANGE,
                            .timeBase = length,
                            .timeUnit = SECONDS};

  sequence.startColor.hexColor = startClr->hexColor;
  sequence.endColor.hexColor = endClr->hexColor;

  return appMsgPushLedSequence(&sequence);
}

/**
 * @brief   Push a color range chaser sequence in the sequence queue.
 *
 * @param section       The LED strip section.
 * @param startClr      The start color of the range.
 * @param endClr        The end color of the range.
 * @param length        The full chaser travel time length.
 * @param isinverted    The inversion flag.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int pushRangeChaserSequence(uint32_t section, Color_t *startClr,
                                   Color_t *endClr, uint32_t length,
                                   bool isinverted)
{
  LedSequence_t sequence = {.sectionId = section,
                            .timeBase = length,
                            .timeUnit = SECONDS};

  sequence.seqType = isinverted ? SEQ_INVERT_RANGE_CHASER : SEQ_RANGE_CHASER;

  sequence.startColor.hexColor = startClr->hexColor;
  sequence.endColor.hexColor = endClr->hexColor;

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

/**
 * @brief   Execute the fade chaser sequence command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execFadeChaserSeq(const struct shell *shell, size_t argc, char **argv)
{
  int rc;
  uint32_t section;
  Color_t color;
  uint32_t length;
  bool isInverted;

  if(isSectionValid(argv[0], &section) && isColorValid(argv[1], &color) &&
    isLengthValid(argv[2], &length) && isDirectionValid(argv[3], &isInverted))
  {
    rc = pushFadeChaserSequence(section, &color, length, isInverted);
    if(rc < 0)
    {
      LOG_ERR("unable to push the breather sequence");
      return rc;
    }
    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid arguments. section: %s, color: %s, length %s, direction %s",
    argv[0], argv[1], argv[2], argv[3]);

  return -EINVAL;
}

/**
 * @brief   Execute the color range sequence command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execRangeSeq(const struct shell *shell, size_t argc, char **argv)
{
  int rc;
  uint32_t section;
  Color_t startClr;
  Color_t endClr;
  uint32_t length;

  if(isSectionValid(argv[0], &section) && isColorValid(argv[1], &startClr) &&
    isColorValid(argv[2], &endClr) && isLengthValid(argv[3], &length))
  {
    rc = pushColorRangeSequence(section, &startClr, &endClr, length);
    if(rc < 0)
    {
      LOG_ERR("unable to push the breather sequence");
      return rc;
    }
    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid arguments. section: %s, start color: %s, end color: %s, length %s",
    argv[0], argv[1], argv[2], argv[3]);

  return -EINVAL;
}

/**
 * @brief   Execute the color range chaser sequence command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execRangeChaserSeq(const struct shell *shell, size_t argc, char **argv)
{
  int rc;
  uint32_t section;
  Color_t startClr;
  Color_t endClr;
  uint32_t length;
  bool isInverted;

  if(isSectionValid(argv[0], &section) && isColorValid(argv[1], &startClr) &&
    isColorValid(argv[2], &endClr) && isLengthValid(argv[3], &length) &&
    isDirectionValid(argv[4], &isInverted))
  {
    rc = pushRangeChaserSequence(section, &startClr, &endClr, length, isInverted);
    if(rc < 0)
    {
      LOG_ERR("unable to push the breather sequence");
      return rc;
    }
    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid arguments. section: %s, start color: %s, end color: %s, length %s, direction: %s",
    argv[0], argv[1], argv[2], argv[3], argv[4]);

  return -EINVAL;
}

SHELL_STATIC_SUBCMD_SET_CREATE(seq_sub,
	SHELL_CMD_ARG(solid, NULL, SEQ_SOLID_USAGE, execSolidSeq,
                SOLID_SEQ_ARG_CNT, 0),
  SHELL_CMD_ARG(breather, NULL, SEQ_BREATHER_USAGE, execBreatherSeq,
                BREATHER_SEQ_ARG_CNT, 0),
  SHELL_CMD_ARG(fade_chaser, NULL, SEQ_FADE_CHASER_USAGE, execFadeChaserSeq,
                FADE_CHASER_SEQ_ARG_CNT, 0),
  SHELL_CMD_ARG(range, NULL, SEQ_COLOR_RANGE_USAGE, execRangeSeq,
                COLOR_RANGE_SEQ_ARG_CNT, 0),
  SHELL_CMD_ARG(range, NULL, SEQ_RANGE_CHASER_USAGE, execRangeChaserSeq,
                RANGE_CHASER_SEQ_ARG_CNT, 0),
	SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(sequence, &seq_sub, SEQ_USAGE,	NULL);

/** @} */
