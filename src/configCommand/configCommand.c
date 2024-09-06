/**
 * Copyright (C) 2024 by Electronya
 *
 * @file      configCommand.c
 * @author    jbacon
 * @date      2024-06-12
 * @brief     Configuration Command Module
 *
 *            This file is the implementation of the configuration
 *            command module.
 *
 * @defgroup  configCommand configCommand
 *
 * @{
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include <string.h>

#include "cmdArgValidator.h"
#include "configurator.h"

#define CONFIG_CMD_MODULE_NAME config_COMMAND_module

/* Setting module logging */
LOG_MODULE_REGISTER(CONFIG_CMD_MODULE_NAME);

/**
 * @brief The config command usage.
 */
#define CONFIG_USAGE                  "Get or set the dynamic configuration."

/**
 * @brief The max LED count command usage.
 */
#define MAX_LED_USAGE                 "Get the max LED count."

/**
 * @brief The active LED command usage.
 */
#define ACTIVE_LED_USAGE              "Get or set the active LED count."
/**
 * @brief The active LED get command usage.
 */
#define ACTIVE_LED_GET_USAGE          "Get the active LED count: config activeLed get."

/**
 * @brief The active LED set command usage.
 */
#define ACTIVE_LED_SET_USAGE          "Set the active LED count: config activeLed set <active LED count>."

/**
 * @brief The section command usage.
 */
#define SECTION_USAGE                 "Get or set a section configuration."

/**
 * @brief The section count command usage.
 */
#define SECTION_COUNT_USAGE           "Get or set the section count."

/**
 * @brief The get section max count command usage.
 */
#define SECTION_MAX_COUNT_USAGE       "Get the maximum section count."

/**
 * @brief The get section count command usage.
 */
#define SECTION_GET_COUNT_USAGE       "Get the current section count."

/**
 * @brief The set section count command usage.
 */
#define SECTION_SET_COUNT_USAGE       "Set the section count: section count set <section count>."

/**
 * @brief The section configuration command usage.
 */
#define SECTION_CONF_USAGE            "Get or set a section configuration."

/**
 * @brief The section LED configuration command usage.
 */
#define SECTION_CONF_LED_USAGE        "Get or set a section LED configuration."

/**
 * @brief The get section LED configuration command usage.
 */
#define SECTION_GET_CONF_LED_USAGE    "Get a section LED configuration: section conf leds get <section ID>."

/**
 * @brief The set section LED configuration command usage.
 */
#define SECTION_SET_CONF_LED_USAGE    "Set a section LED configuration: section conf leds set <section ID> <first LED ID> <led count>."

/**
 * @brief The section switches configuration command usage.
 */
#define SECTION_CONF_SWITCHES_USAGE   "Get or set a section switches configuration."

/**
 * @brief The get section switches configuration command usage.
 */
#define SECTION_GET_CONF_SW_USAGE     "Get a section switches configuration: section conf switches get <section ID>."

/**
 * @brief The set section switches configuration command usage.
 */
#define SECTION_SET_CONF_SW_USAGE     "Set a section switches configuration: section conf switches <section ID> <switch ID> <HEX color>."

/**
 * @brief The set section to no switch configuration command usage.
 */
#define SECTION_SET_CONF_NO_SW_USAGE  "Set a section to no switch configuration: section conf switches no."

/**
 * @brief The active LED set command argument count.
 */
#define ACTIVE_LED_SET_ARG_CNT        2

/**
 * @brief The active LED set command argument count.
 */
#define SECTION_SET_COUNT_ARG_CNT     2

/**
 * @brief The section LED configuration get command argument count.
 */
#define SECTION_GET_CONF_LED_ARG_CNT  2

/**
 * @brief The section LED configuration set command argument count.
 */
#define SECTION_SET_CONF_LED_ARG_CNT  4

/**
 * @brief The section switch configuration get command argument count.
 */
#define SECTION_GET_CONF_SW_ARG_CNT   2

/**
 * @brief The section switch configuration set command argument count.
 */
#define SECTION_SET_CONF_SW_ARG_CNT   4

/**
 * @brief   Execute the max LED get command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetMaxLedCount(const struct shell *shell,
                              size_t argc, char **argv)
{
  size_t maxLedCount;

  maxLedCount = configuratorGetMaxLedCount();

  shell_print(shell, "OK: max LED count: %u", maxLedCount);

  return 0;
}

/**
 * @brief   Execute the active LED get command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetActiveLedCount(const struct shell *shell,
                                 size_t argc, char **argv)
{
  size_t ledCount;

  ledCount = configuratorGetActiveLedCount();

  shell_print(shell, "OK: active LED count: %u", ledCount);

  return 0;
}

/**
 * @brief   Execute the active LED set command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execSetActiveLedCount(const struct shell *shell,
                                 size_t argc, char **argv)
{
  int rc;
  size_t ledCount;

  if(isActiveLedCountValid(argv[1], &ledCount))
  {
    rc = configuratorSetActiveLedCount(ledCount);
    if(rc < 0)
    {
      LOG_ERR("unable to set the active LED count");
      return rc;
    }

    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid argument. LED count: %s", argv[1]);

  return -EINVAL;
}

SHELL_STATIC_SUBCMD_SET_CREATE(activeLed_sub,
  SHELL_CMD(max, NULL, MAX_LED_USAGE, execGetMaxLedCount),
	SHELL_CMD(get, NULL, ACTIVE_LED_GET_USAGE, execGetActiveLedCount),
  SHELL_CMD_ARG(set, NULL, ACTIVE_LED_SET_USAGE, execSetActiveLedCount,
                ACTIVE_LED_SET_ARG_CNT, 0),
  SHELL_SUBCMD_SET_END);

/**
 * @brief   Execute the get max section count command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetMaxSectionCount(const struct shell *shell,
                                  size_t argc, char **argv)
{
  size_t maxSectionCount;

  maxSectionCount = configuratorGetMaxSectionCount();

  shell_print(shell, "OK: max section count: %u", maxSectionCount);

  return 0;
}

/**
 * @brief   Execute the get section count command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetSectionCount(const struct shell *shell,
                               size_t argc, char **argv)
{
  size_t sectionCount;

  sectionCount = configuratorGetSectionCount();

  shell_print(shell, "OK: section count: %u", sectionCount);

  return 0;
}

/**
 * @brief   Execute the set section count command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execSetSectionCount(const struct shell *shell,
                               size_t argc, char **argv)
{
  int rc = 0;
  size_t sectionCount;

  if(isSectionCountValid(argv[1], &sectionCount))
  {
    rc = configuratorSetSectionCount(sectionCount);
    if(rc < 0)
    {
      LOG_ERR("unable to set the section count");
      return rc;
    }

    shell_print(shell, "OK");
    return 0;
  }

  shell_print(shell, "FAILED: Invalid argument. Section count: %s", argv[1]);
  return -EINVAL;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sectionCount_sub,
  SHELL_CMD(max, NULL, SECTION_MAX_COUNT_USAGE, execGetMaxSectionCount),
	SHELL_CMD(get, NULL, SECTION_GET_COUNT_USAGE, execGetSectionCount),
  SHELL_CMD_ARG(set, NULL, SECTION_SET_COUNT_USAGE, execSetSectionCount,
                SECTION_SET_COUNT_ARG_CNT, 0),
  SHELL_SUBCMD_SET_END);

/**
 * @brief   Execute the get section LEDs command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetSectionLeds(const struct shell *shell,
                              size_t *argc, char **argv)
{
  int rc;
  size_t sectionId;
  size_t firstLed;
  size_t ledCount;

  if(isSectionIdValid(argv[1], &sectionId))
  {
    rc = configuratorGetSectionLeds(sectionId, &firstLed, &ledCount);
    if(rc < 0)
    {
      LOG_ERR("unable to get section %u LEDs", sectionId);
      shell_print(shell, "FAILED: Unable to get LEDs info for section: %u",
        sectionId);
      return rc;
    }

    shell_print(shell, "OK: section %u LED info: first LED: %u, LED count: %u",
      sectionId, firstLed, ledCount);
    return 0;
  }

  shell_print(shell, "FAILED: Invalid section ID: %s", argv[1]);
  return -EINVAL;
}

/**
 * @brief   Execute the set section LEDs command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execSetSectionLeds(const struct shell *shell,
                              size_t *argc, char **argv)
{
  int rc;
  size_t sectionId;
  size_t firstLed;
  size_t ledCount;

  if(isSectionIdValid(argv[1], &sectionId) &&
     isSectionLedsValid(argv + 2, &firstLed, &ledCount))
    {
      rc = configuratorSetSectionLeds(sectionId, firstLed, ledCount);
      if(rc < 0)
      {
        LOG_ERR("unable to set section %u LED info of first LED: %u, LED count: %u",
          sectionId, firstLed, ledCount);
        shell_print(shell,
          "FAILED: Unable to set section %u LED info: first LED: %u, LED count: %u",
          sectionId, firstLed, ledCount);
        return rc;
      }

      shell_print(shell, "OK");
      return 0;
    }

  shell_print(shell,
    "FAILED: Invalid arguments: section ID: %s, first LED: %s, LED count: %s",
    argv[1], argv[2], argv[3]);
  return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sectionLeds_sub,
  SHELL_CMD_ARG(get, NULL, SECTION_GET_CONF_LED_USAGE, execGetSectionLeds,
    SECTION_GET_CONF_LED_ARG_CNT, 0),
	SHELL_CMD_ARG(set, NULL, SECTION_SET_CONF_LED_USAGE, execSetSectionLeds,
    SECTION_SET_CONF_LED_ARG_CNT, 0),
  SHELL_SUBCMD_SET_END);

/**
 * @brief   Execute the get section switches command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetSectionSwitches(const struct shell *shell,
                                  size_t *argc, char **argv)
{
  int rc;
  size_t sectionId;
  size_t switchId;
  Color_t color;

  if(isSectionIdValid(argv[1], &sectionId))
  {
    rc = configuratorGetSectionSwitches(sectionId, &switchId, &color);
    if(rc < 0)
    {
      LOG_ERR("unable to get switch configuration of section %u", sectionId);
      shell_print(shell,
        "FAILED: unable to get switch configuration of section %u", sectionId);
      return rc;
    }

    shell_print(shell,
      "OK: section %u switch config: switch ID: %u, color: 0x%06x", sectionId,
      switchId, color.hexColor);
    return 0;
  }

  shell_print(shell, "FAILED: Invalid section ID: %s", argv[1]);
  return -EINVAL;
}

/**
 * @brief   Execute the set section switches command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execSetSectionSwitches(const struct shell *shell,
                                  size_t *argc, char **argv)
{
  int rc;
  size_t sectionId;
  size_t switchId;
  Color_t color;

  if(isSectionIdValid(argv[1], &sectionId) &&
     isSectionSwitchValid(argv[2], &switchId) && isColorValid(argv[3], &color))
    {
      rc = configuratorSetSectionSwitches(sectionId, switchId, color);
      if(rc < 0)
      {
        LOG_ERR("unable to set section %u switch configuration: switch ID: %d, color: 0x%06x",
          sectionId, switchId, color.hexColor);
        shell_print(shell,
          "FAILED: Unable to set section %u switch configuration: switch ID: %d, color: 0x%06x",
          sectionId, switchId, color.hexColor);
        return rc;
      }

      shell_print(shell, "OK");
      return 0;
    }

  shell_print(shell,
    "FAILED: Invalid arguments: section ID: %s, switch ID: %s, color: %s",
    argv[1], argv[2], argv[3]);
  return -EINVAL;
}

/**
 * @brief   Execute the set section no switches command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execSetSectionNoSwitches(const struct shell *shell,
                                    size_t *argc, char **argv)
{
  int rc;
  size_t sectionId;
  size_t switchId = MAX_SWITCH_COUNT;
  Color_t color = {.hexColor = 0xffffff};

  if(isSectionIdValid(argv[1], &sectionId))
  {
    rc = configuratorSetSectionSwitches(sectionId, switchId, color);
      if(rc < 0)
      {
        LOG_ERR("unable to set section %u switch configuration: no switch",
          sectionId);
        shell_print(shell,
          "FAILED: Unable to set section %u switch configuration: no switch",
          sectionId);
        return rc;
      }

      shell_print(shell, "OK");
      return 0;
  }

  shell_print(shell,
    "FAILED: Invalid arguments: section ID: %s",argv[1]);
  return -EINVAL;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sectionSwitches_sub,
  SHELL_CMD_ARG(get, NULL, SECTION_GET_CONF_SW_USAGE, execGetSectionSwitches,
    SECTION_GET_CONF_LED_ARG_CNT, 0),
	SHELL_CMD_ARG(set, NULL, SECTION_SET_CONF_SW_USAGE, execSetSectionSwitches,
    SECTION_SET_CONF_LED_ARG_CNT, 0),
  SHELL_CMD(no, NULL, SECTION_SET_CONF_NO_SW_USAGE, execSetSectionNoSwitches),
  SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(sectionConf_sub,
  SHELL_CMD(leds, &sectionLeds_sub, SECTION_CONF_LED_USAGE, NULL),
	SHELL_CMD(switches, &sectionSwitches_sub, SECTION_CONF_SWITCHES_USAGE, NULL),
  SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(section_sub,
  SHELL_CMD(count, &sectionCount_sub, SECTION_COUNT_USAGE, NULL),
  SHELL_CMD(conf, &sectionConf_sub, SECTION_CONF_USAGE, NULL),
  SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(config_sub,
	SHELL_CMD(activeLed, &activeLed_sub, CONFIG_USAGE, NULL),
  SHELL_CMD(section, &section_sub, SECTION_USAGE, NULL),
	SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(config, &config_sub, CONFIG_USAGE,	NULL);

/** @} */
