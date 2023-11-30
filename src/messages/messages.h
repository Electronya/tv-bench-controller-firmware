/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      messages.h
 * @author    jbacon
 * @date      2023-11-22
 * @brief     Led Manager Module
 *
 *            This file is the declaration of the messages module.
 *
 * @defgroup  messages messages
 *
 * @{
 */

#ifndef MESSAGES
#define MESSAGES

#include "zephyrCommon.h"
#include "zephyrLedStrip.h"

/**
 * @brief The sequence type.
*/
typedef enum
{
  SEQ_SOLID,                            /**< The solid sequence. */
  SEQ_CHASER,                           /**< The chaser sequence. */
  SEQ_INVERT_CHASER,                    /**< The inverted chaser sequence. */
  SEQ_COUNT,                            /**< The sequence type count. */
} SequenceType;

/**
 * @brief The color type.
*/
typedef enum
{
  COLOR_SINGLE,                         /**< The single color type. */
  COLOR_RANGE,                          /**< The range color type (rainbow). */
  COLOR_COUNT,                          /**< The color type count. */
} ColorType;

/**
 * @brief The color union.
*/
typedef union
{
  uint32_t hexColor;                    /**< The color HEX value. */
  struct
  {
    uint8_t b;                          /**< The blue value. */
    uint8_t g;                          /**< The green value. */
    uint8_t r;                          /**< The red vaue. */
    uint8_t unused;                     /**< The unused byte */
  };
} Color;

/**
 * @brief The LED management message.
*/
typedef struct
{
  SequenceType seqType;                 /**< The sequence type. */
  uint32_t timeBase;                    /**< The sequence time base. */
  ZephyrTimeUnit timeUnit;              /**< The sequence time unit. */
  ColorType colorType;                  /**< The color type. */
  Color startColor;                     /**< The solid color or the range start color. */
  Color endColor;                       /**< The range end color. */
} LedMngmtMsg;

/**
 * @brief   Intialize the message queues.
 *
 * @return  o if successful, the error code otherwise.
 */
int msgInit(void);

/**
 * @brief   Push a LED management message in the queue.
 *
 * @param msg     The input buffer of the message.
 *
 * @return  0 if successful, the error code otherwise.
 */
int msgPushLedSequence(LedMngmtMsg *msg);

/**
 * @brief   Pop a LED management message from the queue.
 *
 * @param msg     The output buffer of the message.
 *
 * @return  0 if successful, the error code otherwise.
 */
int msgPopLedSequence(LedMngmtMsg *msg);

#endif    /* MESSAGES */

/** @} */
