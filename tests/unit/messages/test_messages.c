/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_messages.h
 * @author    jbacon
 * @date      2023-11-20
 * @brief     Messages Module Test Cases
 *
 *            This file is the test cases of the messages module.
 *
 * @ingroup  messages
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "messages.h"
#include "messages.c"

#include "zephyrMsgQueue.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, zephyrMsgQueueInit, ZephyrMsgQueue*, size_t, size_t);

static void messagesCaseSetup(void *f)
{
  RESET_FAKE(zephyrMsgQueueInit);
}

ZTEST_SUITE(messages_suite, NULL, NULL, messagesCaseSetup, NULL, NULL);

/**
 * @test  msgInit must return the error code as soon as the initialization
 *        of a message queue fails.
*/
ZTEST(messages_suite, test_msgInit_QueueInitFail)
{
  int failRet = -ENOSPC;
  int successRet = 0;
  size_t expectedMsgSizes[MSG_QUEUE_COUNT] = {sizeof(LedMngmtMsg)};
  size_t expectedDepths[MSG_QUEUE_COUNT] = {5};
  int retVals[MSG_QUEUE_COUNT] = {successRet};

  for(uint8_t i = 0; i < MSG_QUEUE_COUNT; ++i)
  {
    if(i > 0)
      retVals[i - 1] = successRet;
    retVals[i] = failRet;

    SET_RETURN_SEQ(zephyrMsgQueueInit, retVals, MSG_QUEUE_COUNT);

    zassert_equal(failRet, msgInit(), "msgInit failed to return the error code");
    zassert_equal(i + 1, zephyrMsgQueueInit_fake.call_count,
      "msgInit failed to initalize all the message queues.");

    for(uint8_t j = 0; j < MSG_QUEUE_COUNT; ++j)
    {
      zassert_equal(queues + j, zephyrMsgQueueInit_fake.arg0_history[j],
        "msgInit failed to initalize all the message queues.");
      zassert_equal(expectedMsgSizes[j], zephyrMsgQueueInit_fake.arg1_history[j],
        "msgInit failed to initalize all the message queues.");
      zassert_equal(expectedDepths[j], zephyrMsgQueueInit_fake.arg2_history[j],
        "msgInit failed to initalize all the message queues.");
    }
  }
}

/**
 * @test  msgInit must initalize all the message queues and return the success
 *        code.
*/
ZTEST(messages_suite, test_msgInit_Success)
{
  int successRet = 0;
  size_t expectedMsgSizes[MSG_QUEUE_COUNT] = {sizeof(LedMngmtMsg)};
  size_t expectedDepths[MSG_QUEUE_COUNT] = {5};
  int retVals[MSG_QUEUE_COUNT] = {successRet};

  SET_RETURN_SEQ(zephyrMsgQueueInit, retVals, MSG_QUEUE_COUNT);

  zassert_equal(successRet, msgInit(),
    "msgInit failed to return the success code");
  zassert_equal(MSG_QUEUE_COUNT, zephyrMsgQueueInit_fake.call_count,
    "msgInit failed to initalize all the message queues.");

  for(uint8_t i = 0; i < MSG_QUEUE_COUNT; ++i)
  {
    zassert_equal(queues + i, zephyrMsgQueueInit_fake.arg0_history[i],
      "msgInit failed to initalize all the message queues.");
    zassert_equal(expectedMsgSizes[i], zephyrMsgQueueInit_fake.arg1_history[i],
      "msgInit failed to initalize all the message queues.");
    zassert_equal(expectedDepths[i], zephyrMsgQueueInit_fake.arg2_history[i],
      "msgInit failed to initalize all the message queues.");
  }
}

/** @} */
