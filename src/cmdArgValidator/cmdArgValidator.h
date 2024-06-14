/**
 * Copyright (C) 2024 by Electronya
 *
 * @file      cmdArgValidator.h
 * @author    jbacon
 * @date      2024-06-14
 * @brief     Command Argument Validator Module
 *
 *            This file is the declaration of the command argument
 *            validator module.
 *
 * @defgroup  cmdArgValidator cmdArgValidator
 *
 * @{
 */

#ifndef CMD_ARG_VALIDATOR
#define CMD_ARG_VALIDATOR

#include <zephyr/kernel.h>

#include "appMsg.h"

/**
 * @brief   Convert and check validity of the section ID.
 *
 * @param arg         The section string argument value.
 * @param section     The converted section.
 *
 * @return  true if the section is valid, false otherwise.
 */
bool isSectionIdValid(char *arg, uint32_t *section);

/**
 * @brief   Convert and check the validity of the color.
 *
 * @param arg       The color string argument.
 * @param color     The converted color.
 *
 * @return  true if the color is valid, false otherwise.
 */
bool isColorValid(char *arg, Color_t *color);

/**
 * @brief   Convert and check the validity of the sequence length.
 *
 * @param arg         The sequence length string argument.
 * @param length      The converted sequence length.
 *
 * @return  true if the sequence length is valid, false otherwise.
 */
bool isSeqLengthValid(char *arg, uint32_t *length);

/**
 * @brief   Convert and check the validity of the sequence direction.
 *
 * @param arg         The sequence direction string argument.
 * @param isInverted  The converted direction flag.
 *
 * @return  true if the sequence direction is valid, false otherwise.
 */
bool isSeqDirectionValid(char *arg, bool *isInverted);

#endif    /* CMD_ARG_VALIDATOR */

/** @} */
