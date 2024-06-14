/**
 * Copyright (C) 2023 by Electronya
 *
 * @file      test_configCommand.c
 * @author    jbacon
 * @date      2024-02-13
 * @brief     Configuration Command Module Test Cases
 *
 *            This file is the test cases of the configuration command module.
 *
 * @ingroup  configCommand
 *
 * @{
 */

#include <zephyr/ztest.h>
#include <zephyr/fff.h>

#include "configCommand.c"

#include "cmdArgValidator.h"
#include "configurator.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(bool, isActiveLedCountValid, char*, uint32_t*);
FAKE_VALUE_FUNC(bool, isSectionCountValid, char*, uint32_t*);
FAKE_VALUE_FUNC(size_t, configuratorGetMaxLedCount);
FAKE_VALUE_FUNC(size_t, configuratorGetActiveLedCount);
FAKE_VALUE_FUNC(int, configuratorSetActiveLedCount, size_t);
FAKE_VALUE_FUNC(size_t, configuratorGetMaxSectionCount);
FAKE_VALUE_FUNC(size_t, configuratorGetSectionCount);
FAKE_VALUE_FUNC(int, configuratorSetSectionCount, size_t);

static void configCommandCaseSetup(void *f)
{
  // TODO: implement or cleanup.
}

ZTEST_SUITE(configCommand_suite, NULL, NULL,
  configCommandCaseSetup, NULL, NULL);

/** @} */
