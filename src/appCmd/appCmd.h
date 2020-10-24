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

#include <shell/shell.h>

/** Application info title */
#define APP_INFO_TITLE    "Application Information"

/** Application name */
#define APP_NAME          "Name: TV Bench Controller LED Coprocessor"

/** Application version */
#define APP_VERSION       "Version: 1.0.0"

/** app command usage */
#define APP_CMD_USAGE     "Application related commands."

/** app info command usage */
#define APP_INFO_USAGE    "Display application information.\nUsage: app info"

/** app name command usage */
#define APP_NAME_USAGE    "Display application name.\nUsage: app name"

/** app version command usage */
#define APP_VER_UAGE      "Display application version.\nUsage: app version"

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
