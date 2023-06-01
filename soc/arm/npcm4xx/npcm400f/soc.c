/*
 * Copyright (c) 2021 Nuvoton Technology Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <device.h>
#include <init.h>
#include <soc.h>
#include <logging/log.h>
#include "NPCM4XX.h"
#include "syscfg_drv.h"

LOG_MODULE_REGISTER(soc, CONFIG_SOC_LOG_LEVEL);

void z_platform_init(void)
{
	/* uartA init */
	PinSelect(Pin_H3_CR_SOUT);
	PinSelect(Pin_H4_CR_SIN);
}

static int soc_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	return 0;
}

SYS_INIT(soc_init, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
