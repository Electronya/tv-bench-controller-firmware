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

#include "zephyrCommon.h"
#include "zephyrMsgQueue.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, zephyrMsgQueueInit, ZephyrMsgQueue*, size_t, size_t);
FAKE_VALUE_FUNC(int, zephyrMsgQueuePush, ZephyrMsgQueue*, const void*, uint32_t,
  ZephyrTimeUnit);
FAKE_VALUE_FUNC(int, zephyrMsgQueuePop, ZephyrMsgQueue*, void*, uint32_t,
  ZephyrTimeUnit);

static void messagesCaseSetup(void *f)
{
  RESET_FAKE(zephyrMsgQueueInit);
  RESET_FAKE(zephyrMsgQueuePush);
  RESET_FAKE(zephyrMsgQueuePop);
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
 * @test  msgInit must return the success code when all the queue intialization
 *        succeed.
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

/**
 * @test  msgPushLedSequence must return any error code raised by pushing
 *        the message to the LED management queue.
*/
ZTEST(messages_suite, test_msgPushLedSequence_PushFail)
{
  int failRet = -ENOSPC;
  LedMngmtMsg msg;

  zephyrMsgQueuePush_fake.return_val = failRet;

  zassert_equal(failRet, msgPushLedSequence(&msg),
    "msgPushLedSequence failed to return the error code.");
  zassert_equal(1, zephyrMsgQueuePush_fake.call_count,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal(queues + LED_MNGMT_QUEUE, zephyrMsgQueuePush_fake.arg0_val,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal((void*)(&msg), zephyrMsgQueuePush_fake.arg1_val,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal(ZEPHYR_TIME_FOREVER, zephyrMsgQueuePush_fake.arg2_val,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal(MILLI_SEC, zephyrMsgQueuePush_fake.arg3_val,
    "msgPushLedSequence failed to push the LED management message.");
}

/**
 * @test  msgPushLedSequence must return the success code when the push
 *        push operation succeeds.
*/
ZTEST(messages_suite, test_msgPushLedSequence_Success)
{
  int successRet = 0;
  LedMngmtMsg msg;

  zephyrMsgQueuePush_fake.return_val = successRet;

  zassert_equal(successRet, msgPushLedSequence(&msg),
    "msgPushLedSequence failed to return the success code.");
  zassert_equal(1, zephyrMsgQueuePush_fake.call_count,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal(queues + LED_MNGMT_QUEUE, zephyrMsgQueuePush_fake.arg0_val,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal((void*)(&msg), zephyrMsgQueuePush_fake.arg1_val,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal(ZEPHYR_TIME_FOREVER, zephyrMsgQueuePush_fake.arg2_val,
    "msgPushLedSequence failed to push the LED management message.");
  zassert_equal(MILLI_SEC, zephyrMsgQueuePush_fake.arg3_val,
    "msgPushLedSequence failed to push the LED management message.");
}

/**
 * @test  msgPopLedSequence must return any error code raise by the message pop
 *        operation.
*/
ZTEST(messages_suite, test_msgPopLedSequence_PopFail)
{
  int failRet = -ENOMSG;
  LedMngmtMsg msg;

  zephyrMsgQueuePop_fake.return_val = failRet;

  zassert_equal(failRet, msgPopLedSequence(&msg),
    "msgPopLedSequence failed to return the error code.");
  zassert_equal(1, zephyrMsgQueuePop_fake.call_count,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal(queues + LED_MNGMT_QUEUE, zephyrMsgQueuePop_fake.arg0_val,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal((void*)(&msg), zephyrMsgQueuePop_fake.arg1_val,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal(ZEPHYR_TIME_NO_WAIT, zephyrMsgQueuePop_fake.arg2_val,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal(MILLI_SEC, zephyrMsgQueuePop_fake.arg3_val,
    "msgPopLedSequence failed to pop the LED management message.");
}

/**
 * @test  msgPopLedSequence must return the success code when the pop operation
 *        succeeds.
*/
ZTEST(messages_suite, test_msgPopLedSequence_Success)
{
  int successRet = 0;
  LedMngmtMsg msg;

  zephyrMsgQueuePop_fake.return_val = successRet;

  zassert_equal(successRet, msgPopLedSequence(&msg),
    "msgPopLedSequence failed to return the error code.");
  zassert_equal(1, zephyrMsgQueuePop_fake.call_count,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal(queues + LED_MNGMT_QUEUE, zephyrMsgQueuePop_fake.arg0_val,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal((void*)(&msg), zephyrMsgQueuePop_fake.arg1_val,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal(ZEPHYR_TIME_NO_WAIT, zephyrMsgQueuePop_fake.arg2_val,
    "msgPopLedSequence failed to pop the LED management message.");
  zassert_equal(MILLI_SEC, zephyrMsgQueuePop_fake.arg3_val,
    "msgPopLedSequence failed to pop the LED management message.");
}

/** @} */
