/*
 * Copyright (c) 2024 Nuvoton Technology Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys/slist.h>
#include <arch/arm/aarch32/mpu/arm_mpu.h>

#include "arm_mpu_mem_cfg.h"

extern char _npcm4xx_sram_vector_start[];

/* Some helper defines for nuvton regions, the same attr with ramfunc */
#define NUVOTON_REGION_RAM_RX_ATTR(size)                                           \
	{                                                                          \
		(NORMAL_OUTER_INNER_WRITE_BACK_WRITE_READ_ALLOCATE_NON_SHAREABLE | \
		 size | P_RO_U_RO_Msk)                                             \
	}

static const struct arm_mpu_region mpu_regions[] = {
	/* Region 0 */
	MPU_REGION_ENTRY("FLASH_0",
			 CONFIG_FLASH_BASE_ADDRESS,
#if defined(CONFIG_ARMV8_M_BASELINE) || defined(CONFIG_ARMV8_M_MAINLINE)
			 REGION_FLASH_ATTR(CONFIG_FLASH_BASE_ADDRESS, \
				 CONFIG_FLASH_SIZE * 1024)),
#else
			 REGION_FLASH_ATTR(REGION_FLASH_SIZE)),
#endif
	/* Region 1 */
	MPU_REGION_ENTRY("SRAM_0",
			 CONFIG_SRAM_BASE_ADDRESS,
#if defined(CONFIG_ARMV8_M_BASELINE) || defined(CONFIG_ARMV8_M_MAINLINE)
			 REGION_RAM_ATTR(CONFIG_SRAM_BASE_ADDRESS, \
				 CONFIG_SRAM_SIZE * 1024)),
#else
			 REGION_RAM_ATTR(REGION_SRAM_SIZE)),
#endif
	/* Region 2 ,use for sram vector table, size 512 bytes */
#if defined(CONFIG_XIP)
	MPU_REGION_ENTRY("SRAM_VECTOR_0",
			(uint32_t)_npcm4xx_sram_vector_start,
			NUVOTON_REGION_RAM_RX_ATTR(REGION_512B)),
#endif
};

const struct arm_mpu_config mpu_config = {
	.num_regions = ARRAY_SIZE(mpu_regions),
	.mpu_regions = mpu_regions,
};