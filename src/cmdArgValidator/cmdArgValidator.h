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
 * @brief   Validate the active LED count argument.
 *
 * @param arg       The active LED count argument.
 * @param ledCount  The converted active LED count.
 *
 * @return  true if the active LED count is valid, false otherwise.
 */
bool isActiveLedCountValid(char *arg, size_t *ledCount);

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
 * @brief   Validate the section count argument.
 *
 * @param arg           The section count argument.
 * @param sectionCount  The converted section count.
 *
 * @return  true if the section count is valid, false otherwise.
 */
bool isSectionCountValid(char *arg, size_t *sectionCount);

/**
 * @brief   Validate the section LEDs arguments.
 *
 * @param argv          The section LEDs arguments (first LED, LED count).
 * @param firstLed      The converted section first LED.
 * @param ledCount      The converted section LED count.
 *
 * @return  true if the section LEDS is valid, false otherwise.
 */
bool isSectionLedsValid(char **argv, size_t *firstLed, size_t *ledCount);

/**
 * @brief   Validate the section switch argument.
 *
 * @param arg           The section switch argument.
 * @param switchId      The converted switch ID.
 *
 * @return  true if the section switch is valid, false otherwise.
 */
bool isSectionSwitchValid(char *arg, size_t *switchId);

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
