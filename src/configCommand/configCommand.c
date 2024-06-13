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

#include "configurator.h"

#define CONFIG_CMD_MODULE_NAME config_COMMAND_module

/* Setting module logging */
LOG_MODULE_REGISTER(CONFIG_CMD_MODULE_NAME);

/**
 * @brief The config command usage.
 */
#define CONFIG_USAGE                "Set the dynamic configuration."

/**
 * @brief The max LED count command usage.
 */
#define MAX_LED_USAGE               "Get the max LED count."

/**
 * @brief The active LED command usage.
 */
#define ACTIVE_LED_USAGE            "Get or set the active LED count."
/**
 * @brief The active LED get command usage.
 */
#define ACTIVE_LED_GET_USAGE        "Get the active LED count: config activeLed get."

/**
 * @brief The active LED set command usage.
 */
#define ACTIVE_LED_SET_USAGE        "Set the active LED count: config activeLed set <active LED count>."

/**
 * @brief The active LED set command argument count.
 */
#define ACTIVE_LED_SET_ARG_CNT      2

/**
 * @brief   Validate the active LED count argument.
 *
 * @param arg       The active LED count argument.
 * @param ledCount  The converted active LED count.
 *
 * @return  true if the active LED count is valid, false otherwise.
 */
static bool isActiveLedCountValid(char *arg, size_t *ledCount)
{
  int rc = 0;

  *ledCount = shell_strtoul(arg, 10, &rc);
  if(rc < 0)
    return false;

  if(*ledCount > configuratorGetMaxLedCount())
    return false;

  return true;
}

/**
 * @brief   Execute the max LED get command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetMaxLedCount(const struct shell *shell, size_t argc, char **argv)
{
  size_t maxLedCount;

  maxLedCount = configuratorGetMaxLedCount();

  shell_print(shell, "OK: max LED count: %u", maxLedCount);

  return 0;
}
SHELL_STATIC_SUBCMD_SET_CREATE(maxLed_sub,
	SHELL_CMD(get, NULL, MAX_LED_USAGE, execGetMaxLedCount),
  SHELL_SUBCMD_SET_END);

/**
 * @brief   Execute the active LED get command.
 *
 * @param shell     The shell instance.
 * @param argc      The command argument count.
 * @param argv      The command argument vector.
 *
 * @return  0 if successful, the error code otherwise.
 */
static int execGetActiveLedCount(const struct shell *shell, size_t argc, char **argv)
{
  int rc;
  size_t ledCount;

  rc = configuratorGetActiveLedCount(&ledCount);
  if(rc < 0)
  {
    shell_print(shell, "FAILED: Unable to retreive the active LED count. error: %d", rc);
    return rc;
  }

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
static int execSetActiveLedCount(const struct shell *shell, size_t argc, char **argv)
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

  shell_print(shell, "FAILED: Invalid argument. LED count: %s.", argv[1]);

  return -EINVAL;
}

SHELL_STATIC_SUBCMD_SET_CREATE(activeLed_sub,
	SHELL_CMD(get, NULL, ACTIVE_LED_GET_USAGE, execGetActiveLedCount),
  SHELL_CMD_ARG(set, NULL, ACTIVE_LED_SET_USAGE, execSetActiveLedCount,
                ACTIVE_LED_SET_ARG_CNT, 0),
  SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(config_sub,
  SHELL_CMD(maxLed, &maxLed_sub, MAX_LED_USAGE, NULL),
	SHELL_CMD(activeLed, &activeLed_sub, CONFIG_USAGE, NULL),
	SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(config, &config_sub, CONFIG_USAGE,	NULL);

/** @} */
