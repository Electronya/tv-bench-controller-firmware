/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      appMsg.h
 * @author    jbacon
 * @date      2023-11-22
 * @brief     Application Messages Module
 *
 *            This file is the implementation of the application messages module.
 *
 * @ingroup  appMsg
 *
 * @{
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "appMsg.h"
#include "zephyrCommon.h"
#include "zephyrMsgQueue.h"

#define APP_MSG_MODULE_NAME app_msg_module

/* Setting module logging */
LOG_MODULE_REGISTER(APP_MSG_MODULE_NAME);

/**
 * @brief The maximum LED management message count.
*/
#define MAX_LED_MNGMT_MSG_COUNT             5

/**
 * @brief The queue IDs.
*/
enum
{
  LED_MNGMT_QUEUE = 0,                  /**< The LED management queue ID. */
  MSG_QUEUE_COUNT,
};

/**
 * @brief The message queues.
*/
ZephyrMsgQueue_t queues[MSG_QUEUE_COUNT];

int msgInit(void)
{
  int rc = 0;
  size_t msgSizes[MSG_QUEUE_COUNT] = {sizeof(LedSequence_t)};
  size_t maxDepths[MSG_QUEUE_COUNT] = {MAX_LED_MNGMT_MSG_COUNT};

  for(uint8_t i = 0; i < MSG_QUEUE_COUNT && rc == 0; ++i)
  {
    rc = zephyrMsgQueueInit(queues + i, msgSizes[i], maxDepths[i]);
    if(rc < 0)
      LOG_ERR("unable to initilize the message queue ID: %d", i);
  }

  return rc;
}

int msgPushLedSequence(LedSequence_t *msg)
{
  return zephyrMsgQueuePush(queues + LED_MNGMT_QUEUE, (void*)msg,
    ZEPHYR_TIME_FOREVER, MILLI_SEC);
}

int msgPopLedSequence(LedSequence_t *msg)
{
  return zephyrMsgQueuePop(queues + LED_MNGMT_QUEUE, (void*)msg,
    ZEPHYR_TIME_NO_WAIT, MILLI_SEC);
}

/** @} */
