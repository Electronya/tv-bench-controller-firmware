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

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, appMsgPushLedSequence, LedSequence_t*);

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

#define SECTION_CONVERT_TEST_COUNT                  3
/**
 * @test  isSectionValid must return false if the convertion fails.
*/
ZTEST(seqCommand_suite, test_isSectionValid_convertFail)
{
  uint32_t section;
  char *args[SECTION_CONVERT_TEST_COUNT] = {"a", "1a", "a1"};

  for(uint8_t i = 0; i < SECTION_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isSectionValid(args[i], &section),
      "isSectionValid failed to flag the invalidity of the secion.");
  }
}

/**
 * @test  isSectionValid must return true if the convertion succeeds and
 *        the converted value is valid.
*/
ZTEST(seqCommand_suite, test_isSectionValid_success)
{
  uint32_t section;
  char *args[SECTION_CONVERT_TEST_COUNT] = {"1", "54", "100"};
  uint32_t expectedVals[SECTION_CONVERT_TEST_COUNT] = {1, 54, 100};

  for(uint8_t i = 0; i < SECTION_CONVERT_TEST_COUNT; ++i)
  {
    zassert_true(isSectionValid(args[i], &section),
      "isSectionValid failed to flag the validity of the secion.");
    zassert_equal(expectedVals[i], section,
      "isSectionValid failed to convert the section argument.");
  }
}

#define COLOR_CONVERT_TEST_COUNT                    3
/**
 * @test  isColorValid must return false if the convertion fails.
*/
ZTEST(seqCommand_suite, test_isColorValid_convertFail)
{
  Color_t color;
  char *args[COLOR_CONVERT_TEST_COUNT] = {"oiuj", "1kj", "lk1"};

  for(uint8_t i = 0; i < COLOR_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isColorValid(args[i], &color),
      "isColorValid failed to flag the invalidity of the color.");
  }
}

/**
 * @test  isColorValid must return false if the converted color value
 *        is out of range.
*/
ZTEST(seqCommand_suite, test_isColorValid_colorOutOfRange)
{
  Color_t color;
  char *args[COLOR_CONVERT_TEST_COUNT] = {"01000000", "55deff90", "ffffffff"};

  for(uint8_t i = 0; i < COLOR_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isColorValid(args[i], &color),
      "isColorValid failed to flag the invalidity of the color.");
  }
}

/**
 * @test  isColorValid must return true if the converted color value.
*/
ZTEST(seqCommand_suite, test_isColorValid_colorValid)
{
  Color_t color;
  Color_t expectedColor[COLOR_CONVERT_TEST_COUNT] = {{.hexColor = 0xffffff},
                                                     {.hexColor = 0xdeff90},
                                                     {.hexColor = 0x000000}};
  char *args[COLOR_CONVERT_TEST_COUNT] = {"ffffff", "deff90", "000000"};

  for(uint8_t i = 0; i < COLOR_CONVERT_TEST_COUNT; ++i)
  {
    zassert_true(isColorValid(args[i], &color),
      "isColorValid failed to flag the validity of the color.");
    zassert_equal(expectedColor[i].hexColor, color.hexColor,
      "isColorValid failed to set the color to the argument value.");
  }
}

#define LENGTH_CONVERT_TEST_COUNT                  3
/**
 * @test  isLengthValid must return false if the convertion fails.
*/
ZTEST(seqCommand_suite, test_isLengthValid_convertFail)
{
  uint32_t length;
  char *args[LENGTH_CONVERT_TEST_COUNT] = {"a", "1a", "a1"};

  for(uint8_t i = 0; i < LENGTH_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isLengthValid(args[i], &length),
      "isLengthValid failed to flag the invalidity of the sequence length.");
  }
}

/**
 * @test  isLengthValid must return true if the convertion succeeds and
 *        the converted value.
*/
ZTEST(seqCommand_suite, test_isLengthValid_success)
{
  uint32_t length;
  char *args[LENGTH_CONVERT_TEST_COUNT] = {"1", "54", "100"};
  uint32_t expectedVals[LENGTH_CONVERT_TEST_COUNT] = {1, 54, 100};

  for(uint8_t i = 0; i < LENGTH_CONVERT_TEST_COUNT; ++i)
  {
    zassert_true(isLengthValid(args[i], &length),
      "isLengthValid failed to flag the validity of the sequence length.");
    zassert_equal(expectedVals[i], length,
      "isLengthValid failed to convert the sequence length argument.");
  }
}

#define DIRECTION_CONVERT_TEST_COUNT                  2
/**
 * @test  isDirectionValid must return false if the convertion fails.
*/
ZTEST(seqCommand_suite, test_isDirectionValid_convertFail)
{
  bool isInverted;
  char *args[DIRECTION_CONVERT_TEST_COUNT] = {"norma", "inverte"};

  for(uint8_t i = 0; i < DIRECTION_CONVERT_TEST_COUNT; ++i)
  {
    zassert_false(isDirectionValid(args[i], &isInverted),
      "isDirectionValid failed to flag the invalidity of the sequence directoin.");
  }
}

/**
 * @test  isDirectionValid must return true if the convertion succeeds and
 *        the converted value.
*/
ZTEST(seqCommand_suite, test_isDirectionValid_success)
{
  bool isInverted;
  char *args[DIRECTION_CONVERT_TEST_COUNT] = {"normal", "inverted"};
  bool expectedVals[DIRECTION_CONVERT_TEST_COUNT] = {false, true};

  for(uint8_t i = 0; i < DIRECTION_CONVERT_TEST_COUNT; ++i)
  {
    zassert_true(isDirectionValid(args[i], &isInverted),
      "isDirectionValid failed to flag the validity of the sequence direction.");
    zassert_equal(expectedVals[i], isInverted,
      "isDirectionValid failed to convert the sequence direction argument.");
  }
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

/** @} */
