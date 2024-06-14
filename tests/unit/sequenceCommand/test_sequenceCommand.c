/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_sequenceCommand.h
 * @author    jbacon
 * @date      2023-11-20
 * @brief     Sequence Command Module Test Cases
 *
 *            This file is the test cases of the sequence command module.
 *
 * @ingroup  sequenceCommand
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include <zephyr/sys/util.h>

#include "sequenceCommand.c"

#include "appMsg.h"
#include "cmdArgValidator.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, appMsgPushLedSequence, LedSequence_t*);
FAKE_VALUE_FUNC(bool, isSectionIdValid, char*, uint32_t*);
FAKE_VALUE_FUNC(bool, isColorValid, char*, Color_t*);
FAKE_VALUE_FUNC(bool, isSeqLengthValid, char*, uint32_t*);
FAKE_VALUE_FUNC(bool, isSeqDirectionValid, char*, bool*);

static void seqCommandCaseSetup(void *f)
{
  RESET_FAKE(appMsgPushLedSequence);
}

ZTEST_SUITE(seqCommand_suite, NULL, NULL, seqCommandCaseSetup, NULL, NULL);

/**
 * @brief The expected sequence.
*/
static LedSequence_t expectedSeq;

/**
 * @brief   The custom push solid sequence mock.
 *
 * @param sequence  The sequence to push.
 *
 * @return  since always successful, always 0.
 */
static int customPushSolidSequence(LedSequence_t *seq)
{
  zassert_equal(expectedSeq.seqType, seq->seqType, "bad sequence pushed.");
  zassert_equal(expectedSeq.sectionId, seq->sectionId, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeBase, seq->timeBase, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeUnit, seq->timeUnit, "bad sequence pushed.");
  zassert_equal(expectedSeq.startColor.hexColor, seq->startColor.hexColor,
    "bad sequence pushed.");

  return 0;
}

/**
 * @brief   The custom push breather sequence mock.
 *
 * @param sequence  The sequence to push.
 *
 * @return  since always successful, always 0.
 */
static int customPushBreatherSequence(LedSequence_t *seq)
{
  zassert_equal(expectedSeq.seqType, seq->seqType, "bad sequence pushed.");
  zassert_equal(expectedSeq.sectionId, seq->sectionId, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeBase, seq->timeBase, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeUnit, seq->timeUnit, "bad sequence pushed.");
  zassert_equal(expectedSeq.startColor.hexColor, seq->startColor.hexColor,
    "bad sequence pushed.");

  return 0;
}

/**
 * @brief   The custom push fade chase sequence mock.
 *
 * @param sequence  The sequence to push.
 *
 * @return  since always successful, always 0.
 */
static int customPushFadeChaserSequence(LedSequence_t *seq)
{
  zassert_equal(expectedSeq.seqType, seq->seqType, "bad sequence pushed.");
  zassert_equal(expectedSeq.sectionId, seq->sectionId, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeBase, seq->timeBase, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeUnit, seq->timeUnit, "bad sequence pushed.");
  zassert_equal(expectedSeq.startColor.hexColor, seq->startColor.hexColor,
    "bad sequence pushed.");

  return 0;
}

/**
 * @brief   The custom push color range sequence mock.
 *
 * @param sequence  The sequence to push.
 *
 * @return  since always successful, always 0.
 */
static int custompushColorRangeSequence(LedSequence_t *seq)
{
  zassert_equal(expectedSeq.seqType, seq->seqType, "bad sequence pushed.");
  zassert_equal(expectedSeq.sectionId, seq->sectionId, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeBase, seq->timeBase, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeUnit, seq->timeUnit, "bad sequence pushed.");
  zassert_equal(expectedSeq.startColor.hexColor, seq->startColor.hexColor,
    "bad sequence pushed.");
  zassert_equal(expectedSeq.endColor.hexColor, seq->endColor.hexColor,
    "bad sequence pushed.");

  return 0;
}

/**
 * @brief   The custom push color range chaser sequence mock.
 *
 * @param sequence  The sequence to push.
 *
 * @return  since always successful, always 0.
 */
static int custompushRangeChaserSequence(LedSequence_t *seq)
{
  zassert_equal(expectedSeq.seqType, seq->seqType, "bad sequence pushed.");
  zassert_equal(expectedSeq.sectionId, seq->sectionId, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeBase, seq->timeBase, "bad sequence pushed.");
  zassert_equal(expectedSeq.timeUnit, seq->timeUnit, "bad sequence pushed.");
  zassert_equal(expectedSeq.startColor.hexColor, seq->startColor.hexColor,
    "bad sequence pushed.");
  zassert_equal(expectedSeq.endColor.hexColor, seq->endColor.hexColor,
    "bad sequence pushed.");

  return 0;
}

/**
 * @test  pushSolidColorSequence must return the error if the pushing
 *        operation fails.
*/
ZTEST(seqCommand_suite, test_pushSolidColorSequence_pushFail)
{
  int failRet = -ENOSPC;
  uint32_t section = 10;
  Color_t color = {.hexColor = 0xffffff};

  appMsgPushLedSequence_fake.return_val = failRet;

  zassert_equal(failRet, pushSolidColorSequence(section, &color),
    "pushSolidColorSequence failed to return the error code.");
}

/**
 * @test  pushSolidColorSequence must return the success code and push
 *        the new sequence.
*/
ZTEST(seqCommand_suite, test_pushSolidColorSequence_success)
{
  int successRet = 0;
  uint32_t section = 10;
  Color_t color = {.hexColor = 0xffffff};

  appMsgPushLedSequence_fake.custom_fake = customPushSolidSequence;

  expectedSeq.seqType = SEQ_SOLID;
  expectedSeq.sectionId = section;
  expectedSeq.startColor.hexColor = color.hexColor;
  expectedSeq.timeBase = ZEPHYR_TIME_FOREVER;
  expectedSeq.timeUnit = SECONDS;

  zassert_equal(successRet, pushSolidColorSequence(section, &color),
    "pushSolidColorSequence failed to return the success code.");
}

/**
 * @test  pushBreatherSequence must return the error if the pushing
 *        operation fails.
*/
ZTEST(seqCommand_suite, test_pushBreatherSequence_pushFail)
{
  int failRet = -ENOSPC;
  uint32_t section = 10;
  Color_t color = {.hexColor = 0xffffff};
  uint32_t length = 50;

  appMsgPushLedSequence_fake.return_val = failRet;

  zassert_equal(failRet, pushBreatherSequence(section, &color, length),
    "pushBreatherSequence failed to return the error code.");
}

/**
 * @test  pushBreatherSequence must return the success code and push
 *        the new sequence.
*/
ZTEST(seqCommand_suite, test_pushBreatherSequence_success)
{
  int successRet = 0;
  uint32_t section = 10;
  Color_t color = {.hexColor = 0xffffff};
  uint32_t length = 50;

  appMsgPushLedSequence_fake.custom_fake = customPushBreatherSequence;

  expectedSeq.seqType = SEQ_SOLID_BREATHER;
  expectedSeq.sectionId = section;
  expectedSeq.startColor.hexColor = color.hexColor;
  expectedSeq.timeBase = length;
  expectedSeq.timeUnit = SECONDS;

  zassert_equal(successRet, pushBreatherSequence(section, &color, length),
    "pushBreatherSequence failed to return the success code.");
}

/**
 * @test  pushFadeChaserSequence must return the error if the pushing
 *        operation fails.
*/
ZTEST(seqCommand_suite, test_pushFadeChaserSequence_pushFail)
{
  int failRet = -ENOSPC;
  uint32_t section = 10;
  Color_t color = {.hexColor = 0xffffff};
  uint32_t length = 50;
  bool isInverted = false;

  appMsgPushLedSequence_fake.return_val = failRet;

  zassert_equal(failRet, pushFadeChaserSequence(section, &color, length,
    isInverted), "pushFadeChaserSequence failed to return the error code.");
}

#define DIRECTION_TEST_COUNT                          2
/**
 * @test  pushFadeChaserSequence must return the success code and push
 *        the new sequence.
*/
ZTEST(seqCommand_suite, test_pushFadeChaserSequence_success)
{
  int successRet = 0;
  uint32_t sections[DIRECTION_TEST_COUNT] = {10, 2};
  Color_t colors[DIRECTION_TEST_COUNT] = {{.hexColor = 0xffffff},
                                          {.hexColor = 0x00aa00}};
  uint32_t lengths[DIRECTION_TEST_COUNT] = {50, 100};
  bool isInverted[DIRECTION_TEST_COUNT] = {true, false};

  appMsgPushLedSequence_fake.custom_fake = customPushFadeChaserSequence;

  for(uint8_t i = 0; i < DIRECTION_TEST_COUNT; ++i)
  {
    expectedSeq.seqType = isInverted[i] ? SEQ_INVERT_FADE_CHASER : SEQ_FADE_CHASER;
    expectedSeq.sectionId = sections[i];
    expectedSeq.startColor.hexColor = colors[i].hexColor;
    expectedSeq.timeBase = lengths[i];
    expectedSeq.timeUnit = SECONDS;

    zassert_equal(successRet, pushFadeChaserSequence(sections[i], colors + i,
      lengths[i], isInverted[i]), "pushFadeChaserSequence failed to return the success code.");
  }
}

/**
 * @test  pushColorRangeSequence must return the error if the pushing
 *        operation fails.
*/
ZTEST(seqCommand_suite, test_pushColorRangeSequence_pushFail)
{
  int failRet = -ENOSPC;
  uint32_t section = 10;
  Color_t startClr = {.hexColor = 0xffffff};
  Color_t endClr = {.hexColor = 0x00ee00};
  uint32_t length = 50;

  appMsgPushLedSequence_fake.return_val = failRet;

  zassert_equal(failRet, pushColorRangeSequence(section, &startClr, &endClr,
    length), "pushColorRangeSequence failed to return the error code.");
}

/**
 * @test  pushColorRangeSequence must return the success code and push
 *        the new sequence.
*/
ZTEST(seqCommand_suite, test_pushColorRangeSequence_success)
{
  int successRet = 0;
  uint32_t section = 10;
  Color_t startClr = {.hexColor = 0xffffff};
  Color_t endClr = {.hexColor = 0x00ee00};
  uint32_t length = 50;

  appMsgPushLedSequence_fake.custom_fake = custompushColorRangeSequence;

  expectedSeq.seqType = SEQ_COLOR_RANGE;
  expectedSeq.sectionId = section;
  expectedSeq.startColor.hexColor = startClr.hexColor;
  expectedSeq.endColor.hexColor = endClr.hexColor;
  expectedSeq.timeBase = length;
  expectedSeq.timeUnit = SECONDS;

  zassert_equal(successRet, pushColorRangeSequence(section, &startClr, &endClr,
    length), "pushColorRangeSequence failed to return the success code.");
}

/**
 * @test  pushRangeChaserSequence must return the error if the pushing
 *        operation fails.
*/
ZTEST(seqCommand_suite, test_pushRangeChaserSequence_pushFail)
{
  int failRet = -ENOSPC;
  uint32_t section = 10;
  Color_t startClr = {.hexColor = 0xffffff};
  Color_t endClr = {.hexColor = 0x00ee00};
  uint32_t length = 50;
  bool isInverted = false;

  appMsgPushLedSequence_fake.return_val = failRet;

  zassert_equal(failRet, pushRangeChaserSequence(section, &startClr, &endClr,
    length, isInverted), "pushRangeChaserSequence failed to return the error code.");
}

#define DIRECTION_TEST_COUNT                          2
/**
 * @test  pushRangeChaserSequence must return the success code and push
 *        the new sequence.
*/
ZTEST(seqCommand_suite, test_pushRangeChaserSequence_success)
{
  int successRet = 0;
  uint32_t sections[DIRECTION_TEST_COUNT] = {10, 2};
  Color_t startClrs[DIRECTION_TEST_COUNT] = {{.hexColor = 0xffffff},
                                             {.hexColor = 0x00aa00}};
  Color_t endClrs[DIRECTION_TEST_COUNT] = {{.hexColor = 0x00bb00},
                                           {.hexColor = 0xffccff}};
  uint32_t lengths[DIRECTION_TEST_COUNT] = {50, 100};
  bool isInverted[DIRECTION_TEST_COUNT] = {true, false};

  appMsgPushLedSequence_fake.custom_fake = custompushRangeChaserSequence;

  for(uint8_t i = 0; i < DIRECTION_TEST_COUNT; ++i)
  {
    expectedSeq.seqType = isInverted[i] ? SEQ_INVERT_RANGE_CHASER : SEQ_RANGE_CHASER;
    expectedSeq.sectionId = sections[i];
    expectedSeq.startColor.hexColor = startClrs[i].hexColor;
    expectedSeq.endColor.hexColor = endClrs[i].hexColor;
    expectedSeq.timeBase = lengths[i];
    expectedSeq.timeUnit = SECONDS;

    zassert_equal(successRet, pushRangeChaserSequence(sections[i],
      startClrs + i, endClrs + i, lengths[i], isInverted[i]),
      "pushRangeChaserSequence failed to return the success code.");
  }
}

/** @} */
