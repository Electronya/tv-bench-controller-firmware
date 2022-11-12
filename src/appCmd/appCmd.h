/**
 * Copyright (C) 2020 by Electronya
 *
 * @file      appCmd.h
 * @author    jbacon
 * @date      2020-10-07
 * @brief     app Command Inplementation
 *
 * This file is the emplementation of the app command.
 *
 * @defgroup  appCmd appCmd
 * @brief     app Command Module
 *
 * This module is the implementation of the app command
 */
#ifndef APP_CMD_H_
#define APP_CMD_H_

#include <zephyr/shell/shell.h>

/**
 * Execute the app name command
 *
 * @param shell     Handle to the shell
 * @param argc      Command argument count
 * @param argv      Pointer to the array of arguments
 *
 * @return 0 if successful, -1 otherwise
 */
static int execName(const struct shell *shell, size_t argc, char **argv);

/**
 * Execute the app version command
 *
 * @param shell     Handle to the shell
 * @param argc      Command argument count
 * @param argv      Pointer to the array of arguments
 *
 * @return 0 if successful, -1 otherwise
 */
static int execVersion(const struct shell *shell, size_t argc, char **argv);

/**
 * Execute the app info command
 *
 * @param shell     Handle to the shell
 * @param argc      Command argument count
 * @param argv      Pointer to the array of arguments
 *
 * @return 0 if successful, -1 otherwise
 */
static int execInfo(const struct shell *shell, size_t argc, char **argv);

#endif      /* APP_CMD_H_ */
