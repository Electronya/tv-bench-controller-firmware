/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      ledCtrlHw.h
 * @author    jbacon
 * @date      2023-05-09
 * @brief     Led Control Module
 *
 *            This file is the declaration of the LED control module.
 *
 * @defgroup  ledCtrl
 *
 * @{
 */

#ifndef LED_CTRL
#define LED_CTRL

/**
 * @brief   Base colors.
*/
typedef enum
{
  LED_CTRL_RED = 0,             /**< The RED base color. */
  LED_CTRL_GRN,                 /**< The GREEN base color. */
  LED_CTRL_BLU,                 /**< The BLUE base color. */
  LED_CTRL_CLR_CNT,             /**< The count of supported base color. */
} LedCtrlBaseColor;

/**
 * @brief   Initialize the module.
 *
 * @return  0 if successful, the error code otherwise.
*/
int ledCtrlInit(void);

#endif    /* LED_CTRL */

/** @} */
