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

ZTEST(seqCommand_suite, test_tbd)
{
  zassert_true(true);
}

/** @} */
