/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      appMsg.h
 * @author    jbacon
 * @date      2023-11-22
 * @brief     Application Messages Module
 *
 *            This file is the declaration of the application messages module.
 *
 * @defgroup  appMsg appMsg
 *
 * @{
 */

#ifndef APP_MESSAGES
#define APP_MESSAGES

#include "zephyrCommon.h"
#include "zephyrLedStrip.h"

/**
 * @brief The sequence type.
*/
typedef enum
{
  SEQ_SOLID,                            /**< The solid sequence. */
  SEQ_SOLID_BREATHER,                   /**< The solid breather sequence. */
  SEQ_FADE_CHASER,                      /**< The fade chaser sequence. */
  SEQ_INVERT_FADE_CHASER,               /**< The inverted fade chaser sequence. */
  SEQ_COLOR_RANGE,                      /**< The color range sequence. */
  SEQ_RANGE_CHASER,                     /**< The color range chaser sequence. */
  SEQ_INVERT_RANGE_CHASER,              /**< The inverted color range chaser sequence.*/
  SEQ_COUNT,                            /**< The sequence type count. */
} SequenceType_t;

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
} Color_t;

/**
 * @brief The LED management message.
*/
typedef struct
{
  SequenceType_t seqType;               /**< The sequence type. */
  uint32_t timeBase;                    /**< The sequence time base. */
  ZephyrTimeUnit_t timeUnit;            /**< The sequence time unit. */
  Color_t startColor;                   /**< The solid color or the range start color. */
  Color_t endColor;                     /**< The range end color. */
  uint8_t sectionId;                    /**< The section ID to apply the sequence to. */
} LedSequence_t;

/**
 * @brief   Intialize the message queues.
 *
 * @return  o if successful, the error code otherwise.
 */
int appMsgInit(void);

/**
 * @brief   Push a LED management message in the queue.
 *
 * @param msg     The input buffer of the message.
 *
 * @return  0 if successful, the error code otherwise.
 */
int appMsgPushLedSequence(LedSequence_t *msg);

/**
 * @brief   Pop a LED management message from the queue.
 *
 * @param msg     The output buffer of the message.
 *
 * @return  0 if successful, the error code otherwise.
 */
int appMsgPopLedSequence(LedSequence_t *msg);

#endif    /* APP_MESSAGES */

/** @} */
