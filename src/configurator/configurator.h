/**
 * Copyright (C) 2024 by Electronya
 *
 * @file      configurator.h
 * @author    jbacon
 * @date      2024-02-16
 * @brief     Configurator Module
 *
 *            This file is the declaration of the configurator module.
 *
 * @defgroup  configurator configurator
 *
 * @{
 */

#ifndef CONFIGURATOR
#define CONFIGURATOR

/**
 * @brief   Check if the configuration was uploaded by the Pi and
 *          is ready to use.
 *
 * @return  true if the configuration is ready, false otherwise.
 */
bool configuratorIsReady(void);

#endif    /* CONFIGURATOR */

/** @} */
