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

static void seqCommandCaseSetup(void *f)
{

}

ZTEST_SUITE(seqCommand_suite, NULL, NULL, seqCommandCaseSetup, NULL, NULL);

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

/** @} */
