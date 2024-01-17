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

#define SEQUENCEL_COMMAND_MODULE_NAME sequence_command_module

/* Setting module logging */
LOG_MODULE_REGISTER(SEQUENCEL_COMMAND_MODULE_NAME);

/** @} */
