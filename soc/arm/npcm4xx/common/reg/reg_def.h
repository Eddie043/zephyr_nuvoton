/*
 * Copyright (c) 2023 Nuvoton Technology Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _NUVOTON_NPCM4XX_REG_DEF_H
#define _NUVOTON_NPCM4XX_REG_DEF_H

/*
 * NPCM4XX register structure size/offset checking macro function to mitigate
 * the risk of unexpected compiling results. All addresses of NPCM4XX registers
 * must meet the alignment requirement of cortex-m4.
 * DO NOT use 'packed' attribute if module contains different length ie.
 * 8/16/32 bits registers.
 */
#define NPCM4XX_REG_SIZE_CHECK(reg_def, size) \
	BUILD_ASSERT(sizeof(struct reg_def) == size, \
		"Failed in size check of register structure!")
#define NPCM4XX_REG_OFFSET_CHECK(reg_def, member, offset) \
	BUILD_ASSERT(offsetof(struct reg_def, member) == offset, \
		"Failed in offset check of register structure member!")

/*
 * NPCM4XX register access checking via structure macro function to mitigate the
 * risk of unexpected compiling results if module contains different length
 * registers. For example, a word register access might break into two byte
 * register accesses by adding 'packed' attribute.
 *
 * For example, add this macro for word register 'PRSC' of PWM module in its
 * device init function for checking violation. Once it occurred, core will be
 * stalled forever and easy to find out what happens.
 */
#define NPCM4XX_REG_WORD_ACCESS_CHECK(reg, val) { \
		uint16_t placeholder = reg; \
		reg = val; \
		__ASSERT(reg == val, "16-bit reg access failed!"); \
		reg = placeholder; \
	}
#define NPCM4XX_REG_DWORD_ACCESS_CHECK(reg, val) { \
		uint32_t placeholder = reg; \
		reg = val; \
		__ASSERT(reg == val, "32-bit reg access failed!"); \
		reg = placeholder; \
	}
/*
 * Core Domain Clock Generator (CDCG) device registers
 */
struct cdcg_reg {
	/* High Frequency Clock Generator (HFCG) registers */
	/* 0x000: HFCG Control */
	volatile uint8_t HFCGCTRL;
	volatile uint8_t reserved1;
	/* 0x002: HFCG M Low Byte Value */
	volatile uint8_t HFCGML;
	volatile uint8_t reserved2;
	/* 0x004: HFCG M High Byte Value */
	volatile uint8_t HFCGMH;
	volatile uint8_t reserved3;
	/* 0x006: HFCG N Value */
	volatile uint8_t HFCGN;
	volatile uint8_t reserved4;
	/* 0x008: HFCG Prescaler */
	volatile uint8_t HFCGP;
	volatile uint8_t reserved5[7];
	/* 0x010: HFCG Bus Clock Dividers */
	volatile uint8_t HFCBCD;
	volatile uint8_t reserved6;
	/* 0x012: HFCG Bus Clock Dividers */
	volatile uint8_t HFCBCD1;
	volatile uint8_t reserved7;
	/* 0x014: HFCG Bus Clock Dividers */
	volatile uint8_t HFCBCD2;
	volatile uint8_t reserved8[235];

	/* Low Frequency Clock Generator (LFCG) registers */
	/* 0x100: LFCG Control */
	volatile uint8_t  LFCGCTL;
	volatile uint8_t reserved9;
	/* 0x102: High-Frequency Reference Divisor I */
	volatile uint16_t HFRDI;
	/* 0x104: High-Frequency Reference Divisor F */
	volatile uint16_t HFRDF;
	/* 0x106: FRCLK Clock Divisor */
	volatile uint16_t FRCDIV;
	/* 0x108: Divisor Correction Value 1 */
	volatile uint16_t DIVCOR1;
	/* 0x10A: Divisor Correction Value 2 */
	volatile uint16_t DIVCOR2;
	volatile uint8_t reserved10[8];
	/* 0x114: LFCG Control 2 */
	volatile uint8_t  LFCGCTL2;
	volatile uint8_t  reserved11;
};

/* CDCG register fields */
#define NPCM4XX_HFCGCTRL_LOAD                    0
#define NPCM4XX_HFCGCTRL_LOCK                    2
#define NPCM4XX_HFCGCTRL_CLK_CHNG                7

/*
 * Power Management Controller (PMC) device registers
 */
struct pmc_reg {
	/* 0x000: Power Management Controller */
	volatile uint8_t PMCSR;
	volatile uint8_t reserved1[2];
	/* 0x003: Enable in Sleep Control */
	volatile uint8_t ENIDL_CTL;
	/* 0x004: Disable in Idle Control */
	volatile uint8_t DISIDL_CTL;
	/* 0x005: Disable in Idle Control 1 */
	volatile uint8_t DISIDL_CTL1;
	volatile uint8_t reserved2[2];
	/* 0x008 - 0D: Power-Down Control 1 - 6 */
	volatile uint8_t PWDWN_CTL1[6];
	volatile uint8_t reserved3[18];
	/* 0x020 - 21: Power-Down Control 1 - 2 */
	volatile uint8_t RAM_PD[2];
	volatile uint8_t reserved4[2];
	/* 0x024: Power-Down Control 7 */
	volatile uint8_t PWDWN_CTL7[1];
};

/* PMC multi-registers */
#define NPCM4XX_PWDWN_CTL_OFFSET(n) (((n) < 7) ? (0x007 + n) : (0x024 + (n - 7)))
#define NPCM4XX_PWDWN_CTL(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_PWDWN_CTL_OFFSET(n)))

/* PMC register fields */
#define NPCM4XX_PMCSR_DI_INSTW                   0
#define NPCM4XX_PMCSR_DHF                        1
#define NPCM4XX_PMCSR_IDLE                       2
#define NPCM4XX_PMCSR_NWBI                       3
#define NPCM4XX_PMCSR_OHFC                       6
#define NPCM4XX_PMCSR_OLFC                       7
#define NPCM4XX_DISIDL_CTL_RAM_DID               5
#define NPCM4XX_ENIDL_CTL_LP_WK_CTL              6
#define NPCM4XX_ENIDL_CTL_PECI_ENI               2

/*
 * System Configuration (SCFG) device registers
 */
struct scfg_reg {
	/* 0x000: Device Control */
	volatile uint8_t DEVCNT;
	/* 0x001: Straps Status */
	volatile uint8_t STRPST;
	/* 0x002: Reset Control and Status */
	volatile uint8_t RSTCTL;
	volatile uint8_t reserved1[3];
	/* 0x006: Device Control 4 */
	volatile uint8_t DEV_CTL4;
	volatile uint8_t reserved2[9];
	/* 0x010 - 1F: Device Alternate Function 0 - F */
	volatile uint8_t DEVALT0[16];
	volatile uint8_t reserved3[4];
	/* 0x024: DEVALTCX */
	volatile uint8_t DEVALTCX;
	volatile uint8_t reserved4[3];
	/* 0x028: Device Pull-Up Enable 0 */
	volatile uint8_t DEVPU0;
	/* 0x029: Device Pull-Down Enable 1 */
	volatile uint8_t DEVPD1;
	volatile uint8_t reserved5;
	/* 0x02B: Low-Voltage Pins Control 1 */
	volatile uint8_t LV_CTL1;
};

/* SCFG multi-registers */
#define NPCM4XX_DEVALT_OFFSET(n) (0x010 + (n))
#define NPCM4XX_DEVALT(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_DEVALT_OFFSET(n)))

/* SCFG register fields */
#define NPCM4XX_DEVCNT_F_SPI_TRIS                6
#define NPCM4XX_DEVCNT_HIF_TYP_SEL_FIELD         FIELD(2, 2)
#define NPCM4XX_DEVCNT_JEN1_HEN                  5
#define NPCM4XX_DEVCNT_JEN0_HEN                  4
#define NPCM4XX_STRPST_TRIST                     1
#define NPCM4XX_STRPST_TEST                      2
#define NPCM4XX_STRPST_JEN1                      4
#define NPCM4XX_STRPST_JEN0                      5
#define NPCM4XX_STRPST_SPI_COMP                  7
#define NPCM4XX_RSTCTL_VCC1_RST_STS              0
#define NPCM4XX_RSTCTL_DBGRST_STS                1
#define NPCM4XX_RSTCTL_VCC1_RST_SCRATCH          3
#define NPCM4XX_RSTCTL_LRESET_PLTRST_MODE        5
#define NPCM4XX_RSTCTL_HIPRST_MODE               6
#define NPCM4XX_DEV_CTL4_F_SPI_SLLK              2
#define NPCM4XX_DEV_CTL4_SPI_SP_SEL              4
#define NPCM4XX_DEV_CTL4_WP_IF                   5
#define NPCM4XX_DEV_CTL4_VCC1_RST_LK             6
#define NPCM4XX_DEVPU0_I2C0_0_PUE                0
#define NPCM4XX_DEVPU0_I2C0_1_PUE                1
#define NPCM4XX_DEVPU0_I2C1_0_PUE                2
#define NPCM4XX_DEVPU0_I2C2_0_PUE                4
#define NPCM4XX_DEVPU0_I2C3_0_PUE                6
#define NPCM4XX_DEVPU1_F_SPI_PUD_EN              7
#define NPCM4XX_DEVALTCX_GPIO_PULL_EN            7

/*
 * System Glue (GLUE) device registers
 */
struct glue_reg {
	volatile uint8_t reserved1[2];
	/* 0x002: SMBus Start Bit Detection */
	volatile uint8_t SMB_SBD;
	/* 0x003: SMBus Event Enable */
	volatile uint8_t SMB_EEN;
	volatile uint8_t reserved2[12];
	/* 0x010: Simple Debug Port Data 0 */
	volatile uint8_t SDPD0;
	volatile uint8_t reserved3;
	/* 0x012: Simple Debug Port Data 1 */
	volatile uint8_t SDPD1;
	volatile uint8_t reserved4;
	/* 0x014: Simple Debug Port Control and Status */
	volatile uint8_t SDP_CTS;
	volatile uint8_t reserved5[12];
	/* 0x021: SMBus Bus Select */
	volatile uint8_t SMB_SEL;
	volatile uint8_t reserved6[5];
	/* 0x027: PSL Control and Status */
	volatile uint8_t PSL_CTS;
};

/*
 * Universal Asynchronous Receiver-Transmitter (UART) device registers
 */
struct uart_reg {
	/* 0x000: Transmit Data Buffer */
	volatile uint8_t UTBUF;
	volatile uint8_t reserved1;
	/* 0x002: Receive Data Buffer */
	volatile uint8_t URBUF;
	volatile uint8_t reserved2;
	/* 0x004: Interrupt Control */
	volatile uint8_t UICTRL;
	volatile uint8_t reserved3;
	/* 0x006: Status */
	volatile uint8_t USTAT;
	volatile uint8_t reserved4;
	/* 0x008: Frame Select */
	volatile uint8_t UFRS;
	volatile uint8_t reserved5;
	/* 0x00A: Mode Select */
	volatile uint8_t UMDSL;
	volatile uint8_t reserved6;
	/* 0x00C: Baud Rate Divisor */
	volatile uint8_t UBAUD;
	volatile uint8_t reserved7;
	/* 0x00E: Baud Rate Prescaler */
	volatile uint8_t UPSR;
	volatile uint8_t reserved8[7];
	/* 0x016: FIFO Control */
	volatile uint8_t UFCTRL;
	volatile uint8_t reserved9;
	/* 0x018: TX FIFO Current Level */
	volatile uint8_t UTXFLV;
	volatile uint8_t reserved10;
	/* 0x01A: RX FIFO Current Level */
	volatile uint8_t URXFLV;
	volatile uint8_t reserved11;
};

/* UART register fields */
#define NPCM4XX_UICTRL_TBE                       0
#define NPCM4XX_UICTRL_RBF                       1
#define NPCM4XX_UICTRL_ETI                       5
#define NPCM4XX_UICTRL_ERI                       6
#define NPCM4XX_UICTRL_EEI                       7
#define NPCM4XX_USTAT_PE                         0
#define NPCM4XX_USTAT_FE                         1
#define NPCM4XX_USTAT_DOE                        2
#define NPCM4XX_USTAT_ERR                        3
#define NPCM4XX_USTAT_BKD                        4
#define NPCM4XX_USTAT_RB9                        5
#define NPCM4XX_USTAT_XMIP                       6
#define NPCM4XX_UFRS_CHAR_FIELD                  FIELD(0, 2)
#define NPCM4XX_UFRS_STP                         2
#define NPCM4XX_UFRS_XB9                         3
#define NPCM4XX_UFRS_PSEL_FIELD                  FIELD(4, 2)
#define NPCM4XX_UFRS_PEN                         6
#define NPCM4XX_UFCTRL_FIFOEN                    0
//#define NPCM4XX_UMDSL_FIFO_MD                    0
#define NPCM4XX_UTXFLV_TFL                       FIELD(0, 5)
#define NPCM4XX_URXFLV_RFL                       FIELD(0, 5)
//#define NPCM4XX_UFTSTS_TEMPTY_LVL_STS            5
//#define NPCM4XX_UFTSTS_TFIFO_EMPTY_STS           6
//#define NPCM4XX_UFTSTS_NXMIP                     7
//#define NPCM4XX_UFRSTS_RFULL_LVL_STS             5
//#define NPCM4XX_UFRSTS_RFIFO_NEMPTY_STS          6
//#define NPCM4XX_UFRSTS_ERR                       7
//#define NPCM4XX_UFTCTL_TEMPTY_LVL_SEL            FIELD(0, 5)
//#define NPCM4XX_UFTCTL_TEMPTY_LVL_EN             5
//#define NPCM4XX_UFTCTL_TEMPTY_EN                 6
//#define NPCM4XX_UFTCTL_NXMIPEN                   7
//#define NPCM4XX_UFRCTL_RFULL_LVL_SEL             FIELD(0, 5)
//#define NPCM4XX_UFRCTL_RFULL_LVL_EN              5
//#define NPCM4XX_UFRCTL_RNEMPTY_EN                6
//#define NPCM4XX_UFRCTL_ERR_EN                    7

/*
 * Multi-Input Wake-Up Unit (MIWU) device registers
 */

/* MIWU multi-registers */
#define NPCM4XX_WKEDG_OFFSET(n)    (0x000 + ((n) * 2L) + ((n) < 5 ? 0 : 0x1E))
#define NPCM4XX_WKAEDG_OFFSET(n)   (0x001 + ((n) * 2L) + ((n) < 5 ? 0 : 0x1E))
#define NPCM4XX_WKPND_OFFSET(n)    (0x00A + ((n) * 4L) + ((n) < 5 ? 0 : 0x10))
#define NPCM4XX_WKPCL_OFFSET(n)    (0x00C + ((n) * 4L) + ((n) < 5 ? 0 : 0x10))
#define NPCM4XX_WKEN_OFFSET(n)     (0x01E + ((n) * 2L) + ((n) < 5 ? 0 : 0x12))
#define NPCM4XX_WKINEN_OFFSET(n)   (0x01F + ((n) * 2L) + ((n) < 5 ? 0 : 0x12))
#define NPCM4XX_WKMOD_OFFSET(n)    (0x070 + (n))

#define NPCM4XX_WKEDG(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKEDG_OFFSET(n)))
#define NPCM4XX_WKAEDG(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKAEDG_OFFSET(n)))
#define NPCM4XX_WKPND(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKPND_OFFSET(n)))
#define NPCM4XX_WKPCL(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKPCL_OFFSET(n)))
#define NPCM4XX_WKEN(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKEN_OFFSET(n)))
#define NPCM4XX_WKINEN(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKINEN_OFFSET(n)))
#define NPCM4XX_WKMOD(base, n) (*(volatile uint8_t *)(base + \
						NPCM4XX_WKMOD_OFFSET(n)))

/*
 * General-Purpose I/O (GPIO) device registers
 */
struct gpio_reg {
	/* 0x000: Port GPIOx Data Out */
	volatile uint8_t PDOUT;
	/* 0x001: Port GPIOx Data In */
	volatile uint8_t PDIN;
	/* 0x002: Port GPIOx Direction */
	volatile uint8_t PDIR;
	/* 0x003: Port GPIOx Pull-Up or Pull-Down Enable */
	volatile uint8_t PPULL;
	/* 0x004: Port GPIOx Pull-Up/Down Selection */
	volatile uint8_t PPUD;
	/* 0x005: Port GPIOx Drive Enable by VDD Present */
	volatile uint8_t PENVDD;
	/* 0x006: Port GPIOx Output Type */
	volatile uint8_t PTYPE;
};

/*
 * Pulse Width Modulator (PWM) device registers
 */
struct pwm_reg {
	/* 0x000: Clock Prescaler */
	volatile uint16_t PRSC;
	/* 0x002: Cycle Time */
	volatile uint16_t CTR;
	/* 0x004: PWM Control */
	volatile uint8_t PWMCTL;
	volatile uint8_t reserved1;
	/* 0x006: Duty Cycle */
	volatile uint16_t DCR;
	volatile uint8_t reserved2[4];
	/* 0x00C: PWM Control Extended */
	volatile uint8_t PWMCTLEX;
	volatile uint8_t reserved3;
};

/* PWM register fields */
#define NPCM4XX_PWMCTL_INVP                      0
#define NPCM4XX_PWMCTL_CKSEL                     1
#define NPCM4XX_PWMCTL_HB_DC_CTL_FIELD           FIELD(2, 2)
#define NPCM4XX_PWMCTL_PWR                       7
#define NPCM4XX_PWMCTLEX_FCK_SEL_FIELD           FIELD(4, 2)
#define NPCM4XX_PWMCTLEX_OD_OUT                  7

/*
 * Analog-To-Digital Converter (ADC) device registers
 */
struct adc_reg {
	/* 0x000: ADC Status */
	volatile uint16_t ADCSTS;
	/* 0x002: ADC Configuration */
	volatile uint16_t ADCCNF;
	/* 0x004: ADC Timing Control */
	volatile uint16_t ATCTL;
	/* 0x006: ADC Single Channel Address */
	volatile uint16_t ASCADD;
	/* 0x008: ADC Scan Channels Select */
	volatile uint16_t ADCCS;
	volatile uint8_t reserved1[10];
	/* 0x014: Threshold Control 1 */
	volatile uint16_t THRCTL1;
	/* 0x016: Threshold Control 2 */
	volatile uint16_t THRCTL2;
	/* 0x018: Threshold Control 3 */
	volatile uint16_t THRCTL3;
	/* 0x01A:  Threshold Status */
	volatile uint16_t THRCTS;
	volatile uint8_t reserved2[4];
	/* 0x020: Internal register 1 for ADC Speed */
	volatile uint16_t ADCCNF2;
	/* 0x022: Internal register 2 for ADC Speed */
	volatile uint16_t GENDLY;
	volatile uint8_t reserved3[2];
	/* 0x026: Internal register 3 for ADC Speed */
	volatile uint16_t MEAST;
	volatile uint8_t reserved4[18];
	/* 0x03A: Deassertion Threshold Control 1 Word */
	volatile uint16_t THR_DCTL1;
	/* 0x03C: Deassertion Threshold Control 2 Word */
	volatile uint16_t THR_DCTL2;
	/* 0x03E: Deassertion Threshold Control 3 Word */
	volatile uint16_t THR_DCTL3;
	/* 0x040 - 52: Data Buffer of Channel 0 - 9 */
	volatile uint16_t CHNDAT[10];
};

/* ADC register fields */
#define NPCM4XX_ATCTL_SCLKDIV_FIELD              FIELD(0, 6)
#define NPCM4XX_ATCTL_DLY_FIELD                  FIELD(8, 3)
#define NPCM4XX_ASCADD_SADDR_FIELD               FIELD(0, 5)
#define NPCM4XX_ADCSTS_EOCEV                     0
#define NPCM4XX_ADCSTS_EOCCEV                    1
#define NPCM4XX_ADCCNF_ADCMD_FIELD               FIELD(1, 2)
#define NPCM4XX_ADCCNF_ADCRPTC                   3
#define NPCM4XX_ADCCNF_INTECEN                   6
#define NPCM4XX_ADCCNF_START                     4
#define NPCM4XX_ADCCNF_ADCEN                     0
#define NPCM4XX_ADCCNF_STOP                      11
#define NPCM4XX_CHNDAT_CHDAT_FIELD               FIELD(0, 10)
#define NPCM4XX_CHNDAT_NEW                       15
#define NPCM4XX_THRCTL_THEN                      15
#define NPCM4XX_THRCTL_L_H                       14
#define NPCM4XX_THRCTL_CHNSEL                    FIELD(10, 4)
#define NPCM4XX_THRCTL_THRVAL                    FIELD(0, 10)
#define NPCM4XX_THRCTS_ADC_WKEN                  15
#define NPCM4XX_THRCTS_THR3_IEN                  10
#define NPCM4XX_THRCTS_THR2_IEN                  9
#define NPCM4XX_THRCTS_THR1_IEN                  8
#define NPCM4XX_THRCTS_ADC_EVENT                 7
#define NPCM4XX_THRCTS_THR3_STS                  2
#define NPCM4XX_THRCTS_THR2_STS                  1
#define NPCM4XX_THRCTS_THR1_STS                  0
#define NPCM4XX_THR_DCTL_THRD_EN                 15
#define NPCM4XX_THR_DCTL_THR_DVAL                FIELD(0, 10)

/*
 * Timer Watchdog (TWD) device registers
 */
struct twd_reg {
	/* [0x00] Timer and Watchdog Configuration */
	volatile uint8_t TWCFG;
	volatile uint8_t reserved1[1];
	/* [0x02] Timer and Watchdog Clock Prescaler */
	volatile uint8_t TWCP;
	volatile uint8_t reserved2[1];
	/* [0x04] TWD Timer 0 Counter Preset */
	volatile uint16_t TWDT0;
	/* [0x06] TWDT0 Control and Status */
	volatile uint8_t T0CSR;
	volatile uint8_t reserved3[1];
	/* [0x08] Watchdog Count */
	volatile uint8_t WDCNT;
	volatile uint8_t reserved4[1];
	/* [0x0A] Watchdog Service Data Match */
	volatile uint8_t WDSDM;
	volatile uint8_t reserved5[1];
	/* [0x0C] TWD Timer 0 Counter */
	volatile uint16_t TWMT0;
	/* [0x0E] Watchdog Counter */
	volatile uint8_t TWMWD;
	volatile uint8_t reserved6[1];
	/* [0x10] Watchdog Clock Prescaler */
	volatile uint8_t WDCP;
};

/* TWD register fields */
#define TWD_TWCFG_LTWD_CFG              (0)
#define TWD_TWCFG_LTWCP                 (1)
#define TWD_TWCFG_LTWDT0                (2)
#define TWD_TWCFG_LWDCNT                (3)
#define TWD_TWCFG_WDCT0I                (4)
#define TWD_TWCFG_WDSDME                (5)
#define TWD_TWCP_MDIV                   (0)
#define TWD_T0CSR_RST                   (0)
#define TWD_T0CSR_TC                    (1)
#define TWD_T0CSR_WDLTD                 (3)
#define TWD_T0CSR_WDRST_STS             (4)
#define TWD_T0CSR_WD_RUN                (5)
#define TWD_T0CSR_T0EN                  (6)
#define TWD_T0CSR_TESDIS                (7)
#define TWD_WDCP_WDIV                   (0)

/* Flash Interface Unit (FIU) device registers */
struct fiu_reg {
	volatile uint8_t reserved1;
	/* 0x001: Burst Configuration */
	volatile uint8_t BURST_CFG;
	/* 0x002: FIU Response Configuration */
	volatile uint8_t RESP_CFG;
	volatile uint8_t reserved2[17];
	/* 0x014: SPI Flash Configuration */
	volatile uint8_t SPI_FL_CFG;
	volatile uint8_t reserved3;
	/* 0x016: UMA Code Byte */
	volatile uint8_t UMA_CODE;
	/* 0x017: UMA Address Byte 0 */
	volatile uint8_t UMA_AB0;
	/* 0x018: UMA Address Byte 1 */
	volatile uint8_t UMA_AB1;
	/* 0x019: UMA Address Byte 2 */
	volatile uint8_t UMA_AB2;
	/* 0x01A: UMA Data Byte 0 */
	volatile uint8_t UMA_DB0;
	/* 0x01B: UMA Data Byte 1 */
	volatile uint8_t UMA_DB1;
	/* 0x01C: UMA Data Byte 2 */
	volatile uint8_t UMA_DB2;
	/* 0x01D: UMA Data Byte 3 */
	volatile uint8_t UMA_DB3;
	/* 0x01E: UMA Control and Status */
	volatile uint8_t UMA_CTS;
	/* 0x01F: UMA Extended Control and Status */
	volatile uint8_t UMA_ECTS;
	/* 0x020: UMA Data Bytes 0-3 */
	volatile uint32_t UMA_DB0_3;
	volatile uint8_t reserved4[2];
	/* 0x026: CRC Control Register */
	volatile uint8_t CRCCON;
	/* 0x027: CRC Entry Register */
	volatile uint8_t CRCENT;
	/* 0x028: CRC Initialization and Result Register */
	volatile uint32_t CRCRSLT;
	volatile uint8_t reserved5[2];
	/* 0x02E: FIU Read Command for Back-up flash */
	volatile uint8_t RD_CMD_BACK;
	volatile uint8_t reserved6;
	/* 0x030: FIU Read Command for private flash */
	volatile uint8_t RD_CMD_PVT;
	/* 0x031: FIU Read Command for shared flash */
	volatile uint8_t RD_CMD_SHD;
	volatile uint8_t reserved7;
	/* 0x033: FIU Extended Configuration */
	volatile uint8_t FIU_EXT_CFG;
	/* 0x034: UMA AB0~3 */
	volatile uint32_t UMA_AB0_3;
	volatile uint8_t reserved8[4];
	/* 0x03C: Set command enable in 4 Byte address mode */
	volatile uint8_t SET_CMD_EN;
	/* 0x03D: 4 Byte address mode Enable */
	volatile uint8_t ADDR_4B_EN;
	volatile uint8_t reserved9[3];
	/* 0x041: Master Inactive Counter Threshold */
	volatile uint8_t MI_CNT_THRSH;
	/* 0x042: FIU Matser Status */
	volatile uint8_t FIU_MSR_STS;
	/* 0x043: FIU Master Interrupt Enable and Configuration */
	volatile uint8_t FIU_MSR_IE_CFG;
	/* 0x044: Quad Program Enable */
	volatile uint8_t Q_P_EN;
	volatile uint8_t reserved10[3];
	/* 0x048: Extended Data Byte Configuration */
	volatile uint8_t EXT_DB_CFG;
	/* 0x049: Direct Write Configuration */
	volatile uint8_t DIRECT_WR_CFG;
	volatile uint8_t reserved11[6];
	/* 0x050 ~ 0x060: Extended Data Byte F to 0 */
	volatile uint8_t EXT_DB_F_0[16];
};

/* FIU register fields */
/* 0x001: BURST CFG */
#define NPCM4XX_BURST_CFG_R_BURST               FIELD(0, 1)
#define NPCM4XX_BURST_CFG_SLAVE                 2
#define NPCM4XX_BURST_CFG_UNLIM_BURST           3
#define NPCM4XX_BURST_CFG_SPI_WR_EN             7

/* 0x002: RESP CFG */
#define NCPM4XX_RESP_CFG_QUAD_EN                3

/* 0x014: SPI FL CFG */
#define NPCM4XX_SPI_FL_CFG_RD_MODE              FIELD(6, 7)
#define NPCM4XX_SPI_FL_CFG_RD_MODE_NORMAL       0
#define NPCM4XX_SPI_FL_CFG_RD_MODE_FAST         1
#define NPCM4XX_SPI_FL_CFG_RD_MODE_FAST_DUAL    3

/* 0x01E: UMA CTS */
#define NPCM4XX_UMA_CTS_D_SIZE                  FIELD(0, 2)
#define NPCM4XX_UMA_CTS_A_SIZE                  3
#define NPCM4XX_UMA_CTS_C_SIZE                  4
#define NPCM4XX_UMA_CTS_RD_WR                   5
#define NPCM4XX_UMA_CTS_DEV_NUM                 6
#define NPCM4XX_UMA_CTS_EXEC_DONE               7

/* 0x01F: UMA ECTS */
#define NPCM4XX_UMA_ECTS_SW_CS0                 0
#define NPCM4XX_UMA_ECTS_SW_CS1                 1
#define NPCM4XX_UMA_ECTS_SW_CS2                 2
#define NPCM4XX_UMA_ECTS_DEV_NUM_BACK           3
#define NPCM4XX_UMA_ECTS_UMA_ADDR_SIZE          FIELD(4, 6)

/* 0x026: CRC Control Register */
#define NPCM4XX_CRCCON_CALCEN                   0
#define NPCM4XX_CRCCON_CKSMCRC                  1
#define NCPM4XX_CRCCON_UMAENT                   2

/* 0x033: FIU Extended Configuration Register */
#define NPCM4XX_FIU_EXT_CFG_FOUR_BADDR          0

/* 0x03C: Set command enable 4 bytes address mode */
#define NPCM4XX_SET_CMD_EN_PVT_CMD_EN           4
#define NCPM4XX_SET_CMD_EN_SHD_CMD_EN           5
#define NCPM4XX_SET_CMD_EN_BACK_CMD_EN          6

/* 0x03D: 4 bytes address mode enable */
#define NPCM4XX_ADDR_4B_EN_PVT_4B               4
#define NCPM4XX_ADDR_4B_EN_SHD_4B               5
#define NCPM4XX_ADDR_4B_EN_BACK_4B              6

/* 0x043: FIU master interrupt enable and configuration register */
#define NPCM4XX_FIU_MSR_IE_CFG_RD_PEND_UMA_IE   0
#define NPCM4XX_FIU_MSR_IE_CFG_RD_PEND_FIU_IE   1
#define NPCM4XX_FIU_MSR_IE_CFG_MSTR_INACT_IE    2
#define NPCM4XX_FIU_MSR_IE_CFG_UMA_BLOCK        3

/* 0x044: Quad program enable register */
#define NPCM4XX_Q_P_EN_QUAD_P_EN                0

/* 0x048: Extended data byte configurartion */
#define NPCM4XX_EXT_DB_CFG_D_SIZE_DB            FIELD(0, 4)
#define NPCM4XX_EXT_DB_CFG_EXT_DB_EN            5

/* 0x049: Direct write configuration */
#define NPCM4XX_DIRECT_WR_CFG_DIRECT_WR_BLOCK   1
#define NPCM4XX_DIRECT_WR_CFG_DW_CS2            5
#define NPCM4XX_DIRECT_WR_CFG_DW_CS1            6
#define NPCM4XX_DIRECT_WR_CFG_DW_CS0            7

/* BURST CFG R_BURST selections */
#define NPCM4XX_BURST_CFG_R_BURST_1B            0
#define NPCM4XX_BURST_CFG_R_BURST_16B           3

/* FIU read write command init flags */
#define NPCM4XX_FIU_SPI_NOR_READ_INIT           0
#define NPCM4XX_FIU_SPI_NOR_WRITE_INIT          1
#define NPCM4XX_FIU_SPI_NOR_READ_INIT_OK        BIT(NPCM4XX_FIU_SPI_NOR_READ_INIT)
#define NPCM4XX_FIU_SPI_NOR_WRITE_INIT_OK       BIT(NPCM4XX_FIU_SPI_NOR_WRITE_INIT)

#define NPCM4XX_FIU_SINGLE_DUMMY_BYTE           0x8
#define NPCM4XX_FIU_ADDR_3B_LENGTH              0x3
#define NPCM4XX_FIU_ADDR_4B_LENGTH              0x4
#define NPCM4XX_FIU_EXT_DB_SIZE                 0x10

/* UMA fields selections */
#define UMA_FLD_ADDR     BIT(NPCM4XX_UMA_CTS_A_SIZE)  /* 3-bytes ADR field */
#define UMA_FLD_NO_CMD   BIT(NPCM4XX_UMA_CTS_C_SIZE)  /* No 1-Byte CMD field */
#define UMA_FLD_WRITE    BIT(NPCM4XX_UMA_CTS_RD_WR)   /* Write transaction */
#define UMA_FLD_SHD_SL   BIT(NPCM4XX_UMA_CTS_DEV_NUM) /* Shared flash selected */
#define UMA_FLD_EXEC     BIT(NPCM4XX_UMA_CTS_EXEC_DONE)

#define UMA_FIELD_ADDR_0 0x00
#define UMA_FIELD_ADDR_1 0x01
#define UMA_FIELD_ADDR_2 0x02
#define UMA_FIELD_ADDR_3 0x03
#define UMA_FIELD_ADDR_4 0x04

#define UMA_FIELD_DATA_0 0x00
#define UMA_FIELD_DATA_1 0x01
#define UMA_FIELD_DATA_2 0x02
#define UMA_FIELD_DATA_3 0x03
#define UMA_FIELD_DATA_4 0x04

/* UMA code for transaction */
#define UMA_CODE_ONLY_WRITE           (UMA_FLD_EXEC | UMA_FLD_WRITE)
#define UMA_CODE_ONLY_READ_BYTE(n)    (UMA_FLD_EXEC | UMA_FLD_NO_CMD | UMA_FIELD_DATA_##n)

/*
 * Enhanced Serial Peripheral Interface (eSPI) device registers
 */
struct espi_reg {
	/* 0x000: eSPI Identification */
	volatile uint32_t ESPIID;
	/* 0x004: eSPI Configuration */
	volatile uint32_t ESPICFG;
	/* 0x008: eSPI Status */
	volatile uint32_t ESPISTS;
	/* 0x00C: eSPI Interrupt Enable */
	volatile uint32_t ESPIIE;
	/* 0x010: eSPI Wake-Up Enable */
	volatile uint32_t ESPIWE;
	/* 0x014: Virtual Wire Register Index */
	volatile uint32_t VWREGIDX;
	/* 0x018: Virtual Wire Register Data */
	volatile uint32_t VWREGDATA;
	/* 0x01C: OOB Receive Buffer Read Head */
	volatile uint32_t OOBRXRDHEAD;
	/* 0x020: OOB Transmit Buffer Write Head */
	volatile uint32_t OOBTXWRHEAD;
	/* 0x024: OOB Channel Control */
	volatile uint32_t OOBCTL;
	/* 0x028: Flash Receive Buffer Read Head */
	volatile uint32_t FLASHRXRDHEAD;
	/* 0x02C: Flash Transmit Buffer Write Head */
	volatile uint32_t FLASHTXWRHEAD;
	volatile uint32_t reserved1;
	/* 0x034: Flash Channel Configuration */
	volatile uint32_t FLASHCFG;
	/* 0x038: Flash Channel Control */
	volatile uint32_t FLASHCTL;
	/* 0x03C: eSPI Error Status */
	volatile uint32_t ESPIERR;
	/* 0x040: Peripheral Bus Master Receive Buffer Read Head */
	volatile uint32_t PBMRXRDHEAD;
	/* 0x044: Peripheral Bus Master Transmit Buffer Write Head */
	volatile uint32_t PBMTXWRHEAD;
	/* 0x048: Peripheral Channel Configuration */
	volatile uint32_t PERCFG;
	/* 0x04C: Peripheral Channel Control */
	volatile uint32_t PERCTL;
	volatile uint32_t reserved2[44];
	/* 0x100 - 127: Virtual Wire Event Slave-to-Master 0 - 9 */
	volatile uint32_t VWEVSM[10];
	volatile uint32_t reserved3[6];
	/* 0x140 - 16F: Virtual Wire Event Master-to-Slave 0 - 11 */
	volatile uint32_t VWEVMS[12];
	volatile uint32_t reserved4[99];
	/* 0x2FC: Virtual Wire Channel Control */
	volatile uint32_t VWCTL;
	/* 0x300 - 34F: OOB Receive Buffer 0 - 19 */
	volatile uint32_t OOBRXBUF[20];
	volatile uint32_t reserved5[12];
	/* 0x380 - 3CF: OOB Transmit Buffer 0-19 */
	volatile uint32_t OOBTXBUF[20];
	volatile uint32_t reserved6[11];
	/* 0x3FC: OOB Channel Control used in 'direct' mode */
	volatile uint32_t OOBCTL_DIRECT;
	/* 0x400 - 443: Flash Receive Buffer 0-16 */
	volatile uint32_t FLASHRXBUF[17];
	volatile uint32_t reserved7[15];
	/* 0x480 - 497: Flash Transmit Buffer 0-5 */
	volatile uint32_t FLASHTXBUF[6];
	volatile uint32_t reserved8[25];
	/* 0x4FC: Flash Channel Control used in 'direct' mode */
	volatile uint32_t FLASHCTL_DIRECT;
};

/* eSPI register fields */
#define NPCM4XX_ESPICFG_PCHANEN             0
#define NPCM4XX_ESPICFG_VWCHANEN            1
#define NPCM4XX_ESPICFG_OOBCHANEN           2
#define NPCM4XX_ESPICFG_FLASHCHANEN         3
#define NPCM4XX_ESPICFG_HPCHANEN            4
#define NPCM4XX_ESPICFG_HVWCHANEN           5
#define NPCM4XX_ESPICFG_HOOBCHANEN          6
#define NPCM4XX_ESPICFG_HFLASHCHANEN        7
#define NPCM4XX_ESPICFG_CHANS_FIELD         FIELD(0, 4)
#define NPCM4XX_ESPICFG_HCHANS_FIELD        FIELD(4, 4)
#define NPCM4XX_ESPICFG_IOMODE_FIELD        FIELD(8, 9)
#define NPCM4XX_ESPICFG_MAXFREQ_FIELD       FIELD(10, 12)
#define NPCM4XX_ESPICFG_PCCHN_SUPP          24
#define NPCM4XX_ESPICFG_VWCHN_SUPP          25
#define NPCM4XX_ESPICFG_OOBCHN_SUPP         26
#define NPCM4XX_ESPICFG_FLASHCHN_SUPP       27
#define NPCM4XX_ESPIIE_IBRSTIE              0
#define NPCM4XX_ESPIIE_CFGUPDIE             1
#define NPCM4XX_ESPIIE_BERRIE               2
#define NPCM4XX_ESPIIE_OOBRXIE              3
#define NPCM4XX_ESPIIE_FLASHRXIE            4
#define NPCM4XX_ESPIIE_SFLASHRDIE           5
#define NPCM4XX_ESPIIE_PERACCIE             6
#define NPCM4XX_ESPIIE_DFRDIE               7
#define NPCM4XX_ESPIIE_VWUPDIE              8
#define NPCM4XX_ESPIIE_ESPIRSTIE            9
#define NPCM4XX_ESPIIE_PLTRSTIE             10
#define NPCM4XX_ESPIIE_AMERRIE              15
#define NPCM4XX_ESPIIE_AMDONEIE             16
#define NPCM4XX_ESPIIE_BMTXDONEIE           19
#define NPCM4XX_ESPIIE_PBMRXIE              20
#define NPCM4XX_ESPIIE_PMSGRXIE             21
#define NPCM4XX_ESPIIE_BMBURSTERRIE         22
#define NPCM4XX_ESPIIE_BMBURSTDONEIE        23
#define NPCM4XX_ESPIWE_IBRSTWE              0
#define NPCM4XX_ESPIWE_CFGUPDWE             1
#define NPCM4XX_ESPIWE_BERRWE               2
#define NPCM4XX_ESPIWE_OOBRXWE              3
#define NPCM4XX_ESPIWE_FLASHRXWE            4
#define NPCM4XX_ESPIWE_PERACCWE             6
#define NPCM4XX_ESPIWE_DFRDWE               7
#define NPCM4XX_ESPIWE_VWUPDWE              8
#define NPCM4XX_ESPIWE_ESPIRSTWE            9
#define NPCM4XX_ESPIWE_PBMRXWE              20
#define NPCM4XX_ESPIWE_PMSGRXWE             21
#define NPCM4XX_ESPISTS_IBRST               0
#define NPCM4XX_ESPISTS_CFGUPD              1
#define NPCM4XX_ESPISTS_BERR                2
#define NPCM4XX_ESPISTS_OOBRX               3
#define NPCM4XX_ESPISTS_FLASHRX             4
#define NPCM4XX_ESPISTS_PERACC              6
#define NPCM4XX_ESPISTS_DFRD                7
#define NPCM4XX_ESPISTS_VWUPD               8
#define NPCM4XX_ESPISTS_ESPIRST             9
#define NPCM4XX_ESPISTS_PLTRST              10
#define NPCM4XX_ESPISTS_AMERR               15
#define NPCM4XX_ESPISTS_AMDONE              16
#define NPCM4XX_ESPISTS_VWUPDW              17
#define NPCM4XX_ESPISTS_BMTXDONE            19
#define NPCM4XX_ESPISTS_PBMRX               20
#define NPCM4XX_ESPISTS_PMSGRX              21
#define NPCM4XX_ESPISTS_BMBURSTERR          22
#define NPCM4XX_ESPISTS_BMBURSTDONE         23
#define NPCM4XX_ESPISTS_ESPIRST_LVL         24
#define NPCM4XX_VWEVMS_WIRE                 FIELD(0, 4)
#define NPCM4XX_VWEVMS_VALID                FIELD(4, 4)
#define NPCM4XX_VWEVMS_IE                   18
#define NPCM4XX_VWEVMS_WE                   20
#define NPCM4XX_VWEVSM_WIRE                 FIELD(0, 4)
#define NPCM4XX_VWEVSM_VALID                FIELD(4, 4)
#define NPCM4XX_VWEVSM_BIT_VALID(n)         (4+n)
#define NPCM4XX_VWEVSM_HW_WIRE              FIELD(24, 4)
#define NPCM4XX_OOBCTL_OOB_FREE             0
#define NPCM4XX_OOBCTL_OOB_AVAIL            1
#define NPCM4XX_OOBCTL_RSTBUFHEADS          2
#define NPCM4XX_OOBCTL_OOBPLSIZE            FIELD(10, 3)
#define NPCM4XX_FLASHCFG_FLASHBLERSSIZE     FIELD(7, 3)
#define NPCM4XX_FLASHCFG_FLASHPLSIZE        FIELD(10, 3)
#define NPCM4XX_FLASHCFG_FLASHREQSIZE       FIELD(13, 3)
#define NPCM4XX_FLASHCTL_FLASH_NP_FREE      0
#define NPCM4XX_FLASHCTL_FLASH_TX_AVAIL     1
#define NPCM4XX_FLASHCTL_STRPHDR            2
#define NPCM4XX_FLASHCTL_DMATHRESH          FIELD(3, 2)
#define NPCM4XX_FLASHCTL_AMTSIZE            FIELD(5, 8)
#define NPCM4XX_FLASHCTL_RSTBUFHEADS        13
#define NPCM4XX_FLASHCTL_CRCEN              14
#define NPCM4XX_FLASHCTL_CHKSUMSEL          15
#define NPCM4XX_FLASHCTL_AMTEN              16

/*
 * Mobile System Wake-Up Control (MSWC) device registers
 */
struct mswc_reg {
	/* 0x000: MSWC Control Status 1 */
	volatile uint8_t MSWCTL1;
	volatile uint8_t reserved1;
	/* 0x002: MSWC Control Status 2 */
	volatile uint8_t MSWCTL2;
	volatile uint8_t reserved2[5];
	/* 0x008: Host Configuration Base Address Low */
	volatile uint8_t HCBAL;
	volatile uint8_t reserved3;
	/* 0x00A: Host Configuration Base Address High */
	volatile uint8_t HCBAH;
	volatile uint8_t reserved4;
	/* 0X00C: MSWC INTERRUPT ENABLE 2 */
	volatile uint8_t MSIEN2;
	volatile uint8_t reserved5;
	/* 0x00E: MSWC Host Event Status 0 */
	volatile uint8_t MSHES0;
	volatile uint8_t reserved6;
	/* 0x010: MSWC Host Event Interrupt Enable */
	volatile uint8_t MSHEIE0;
	volatile uint8_t reserved7;
	/* 0x012: Host Control */
	volatile uint8_t HOST_CTL;
	volatile uint8_t reserved8;
	/* 0x014: SMI Pulse Length */
	volatile uint8_t SMIP_LEN;
	volatile uint8_t reserved9;
	/* 0x016: SCI Pulse Length */
	volatile uint8_t SCIP_LEN;
	volatile uint8_t reserved10[5];
	/* 0x01C: SRID Core Access */
	volatile uint8_t SRID_CR;
	volatile uint8_t reserved11[3];
	/* 0x020: SID Core Access */
	volatile uint8_t SID_CR;
	volatile uint8_t reserved12;
	/* 0x022: DEVICE_ID Core Access */
	volatile uint8_t DEVICE_ID_CR;
	volatile uint8_t reserved13[5];
	/* 0x028: Chip Revision Core Access */
	volatile uint8_t CHPREV_CR;
	volatile uint8_t reserved14[5];
	/* 0x02E: Virtual Wire Sleep States */
	volatile uint8_t VW_SLPST1;
	volatile uint8_t reserved15;
};

/* MSWC register fields */
#define NPCM4XX_MSWCTL1_HRSTOB              0
#define NPCS_MSWCTL1_HWPRON              1
#define NPCM4XX_MSWCTL1_PLTRST_ACT          2
#define NPCM4XX_MSWCTL1_VHCFGA              3
#define NPCM4XX_MSWCTL1_HCFGLK              4
#define NPCM4XX_MSWCTL1_PWROFFB             6
#define NPCM4XX_MSWCTL1_A20MB               7

/*
 * Shared Memory (SHM) device registers
 */
struct shm_reg {
	/* 0x000: Shared Memory Core Status */
	volatile uint8_t SMC_STS;
	/* 0x001: Shared Memory Core Control */
	volatile uint8_t SMC_CTL;
	/* 0x002: Shared Memory Host Control */
	volatile uint8_t SHM_CTL;
	volatile uint8_t reserved1[2];
	/* 0x005: Indirect Memory Access Window Size */
	volatile uint8_t IMA_WIN_SIZE;
	volatile uint8_t reserved2;
	/* 0x007: Shared Access Windows Size */
	volatile uint8_t WIN_SIZE;
	/* 0x008: Shared Access Window 1, Semaphore */
	volatile uint8_t SHAW1_SEM;
	/* 0x009: Shared Access Window 2, Semaphore */
	volatile uint8_t SHAW2_SEM;
	volatile uint8_t reserved3;
	/* 0x00B: Indirect Memory Access, Semaphore */
	volatile uint8_t IMA_SEM;
	volatile uint8_t reserved4[2];
	/* 0x00E: Shared Memory Configuration */
	volatile uint16_t SHCFG;
	/* 0x010: Shared Access Window 1 Write Protect */
	volatile uint8_t WIN1_WR_PROT;
	/* 0x011: Shared Access Window 1 Read Protect */
	volatile uint8_t WIN1_RD_PROT;
	/* 0x012: Shared Access Window 2 Write Protect */
	volatile uint8_t WIN2_WR_PROT;
	/* 0x013: Shared Access Window 2 Read Protect */
	volatile uint8_t WIN2_RD_PROT;
	volatile uint8_t reserved5[2];
	/* 0x016: Indirect Memory Access Write Protect */
	volatile uint8_t IMA_WR_PROT;
	/* 0x017: Indirect Memory Access Read Protect */
	volatile uint8_t IMA_RD_PROT;
	volatile uint8_t reserved6[8];
	/* 0x020: Shared Access Window 1 Base */
	volatile uint32_t WIN_BASE1;
	/* 0x024: Shared Access Window 2 Base */
	volatile uint32_t WIN_BASE2;
	volatile uint32_t reserved7;
	/* 0x02C: Indirect Memory Access Base */
	volatile uint32_t IMA_BASE;
	volatile uint8_t reserved8[10];
	/* 0x03A: Reset Configuration */
	volatile uint8_t RST_CFG;
	volatile uint8_t reserved9[5];
	/* 0x040: Debug Port 80 Buffered Data */
	volatile uint16_t DP80BUF;
	/* 0x042: Debug Port 80 Status */
	volatile uint8_t DP80STS;
	volatile uint8_t reserved10;
	/* 0x044: Debug Port 80 Control */
	volatile uint8_t DP80CTL;
	volatile uint8_t reserved11[3];
	/* 0x048: Host_Offset in Windows 1, 2 Status */
	volatile uint8_t HOFS_STS;
	/* 0x049: Host_Offset in Windows 1, 2 Control */
	volatile uint8_t HOFS_CTL;
	/* 0x04A: Core_Offset in Window 2 Address */
	volatile uint16_t COFS2;
	/* 0x04C: Core_Offset in Window 1 Address */
	volatile uint16_t COFS1;
	volatile uint16_t reserved12;
};

/* SHM register fields */
#define NPCM4XX_SMC_STS_HRERR               0
#define NPCM4XX_SMC_STS_HWERR               1
#define NPCM4XX_SMC_STS_HSEM1W              4
#define NPCM4XX_SMC_STS_HSEM2W              5
#define NPCM4XX_SMC_STS_SHM_ACC             6
#define NPCM4XX_SMC_CTL_HERR_IE             2
#define NPCM4XX_SMC_CTL_HSEM1_IE            3
#define NPCM4XX_SMC_CTL_HSEM2_IE            4
#define NPCM4XX_SMC_CTL_ACC_IE              5
#define NPCM4XX_SMC_CTL_PREF_EN             6
#define NPCM4XX_SMC_CTL_HOSTWAIT            7
#define NPCM4XX_FLASH_SIZE_STALL_HOST       6
#define NPCM4XX_FLASH_SIZE_RD_BURST         7
#define NPCM4XX_WIN_SIZE_RWIN1_SIZE_FIELD   FIELD(0, 4)
#define NPCM4XX_WIN_SIZE_RWIN2_SIZE_FIELD   FIELD(4, 4)
#define NPCM4XX_WIN_PROT_RW1L_RP            0
#define NPCM4XX_WIN_PROT_RW1L_WP            1
#define NPCM4XX_WIN_PROT_RW1H_RP            2
#define NPCM4XX_WIN_PROT_RW1H_WP            3
#define NPCM4XX_WIN_PROT_RW2L_RP            4
#define NPCM4XX_WIN_PROT_RW2L_WP            5
#define NPCM4XX_WIN_PROT_RW2H_RP            6
#define NPCM4XX_WIN_PROT_RW2H_WP            7
#define NPCM4XX_PWIN_SIZEI_RPROT            13
#define NPCM4XX_PWIN_SIZEI_WPROT            14
#define NPCM4XX_CSEM2                       6
#define NPCM4XX_CSEM3                       7
#define NPCM4XX_DP80STS_FWR                 5
#define NPCM4XX_DP80STS_FNE                 6
#define NPCM4XX_DP80STS_FOR                 7
#define NPCM4XX_DP80CTL_DP80EN              0
#define NPCM4XX_DP80CTL_SYNCEN              1
#define NPCM4XX_DP80CTL_ADV                 2
#define NPCM4XX_DP80CTL_RAA                 3
#define NPCM4XX_DP80CTL_RFIFO               4
#define NPCM4XX_DP80CTL_CIEN                5
#define NPCM4XX_DP80CTL_DP80_HF_CFG         7

/*
 * Keyboard and Mouse Controller (KBC) device registers
 */
struct kbc_reg {
	/* 0x000h: Host Interface Control */
	volatile uint8_t HICTRL;
	volatile uint8_t reserved1;
	/* 0x002h: Host Interface IRQ Control */
	volatile uint8_t HIIRQC;
	volatile uint8_t reserved2;
	/* 0x004h: Host Interface Keyboard/Mouse Status */
	volatile uint8_t HIKMST;
	volatile uint8_t reserved3;
	/* 0x006h: Host Interface Keyboard Data Out Buffer */
	volatile uint8_t HIKDO;
	volatile uint8_t reserved4;
	/* 0x008h: Host Interface Mouse Data Out Buffer */
	volatile uint8_t HIMDO;
	volatile uint8_t reserved5;
	/* 0x00Ah: Host Interface Keyboard/Mouse Data In Buffer */
	volatile uint8_t HIKMDI;
	/* 0x00Bh: Host Interface Keyboard/Mouse Shadow Data In Buffer */
	volatile uint8_t SHIKMDI;
};

/* KBC register field */
#define NPCM4XX_HICTRL_OBFKIE               0
#define NPCM4XX_HICTRL_OBFMIE               1
#define NPCM4XX_HICTRL_OBECIE               2
#define NPCM4XX_HICTRL_IBFCIE               3
#define NPCM4XX_HICTRL_PMIHIE               4
#define NPCM4XX_HICTRL_PMIOCIE              5
#define NPCM4XX_HICTRL_PMICIE               6
#define NPCM4XX_HICTRL_FW_OBF               7
#define NPCM4XX_HIKMST_OBF                  0
#define NPCM4XX_HIKMST_IBF                  1
#define NPCM4XX_HIKMST_F0                   2
#define NPCM4XX_HIKMST_A2                   3
#define NPCM4XX_HIKMST_ST0                  4
#define NPCM4XX_HIKMST_ST1                  5
#define NPCM4XX_HIKMST_ST2                  6
#define NPCM4XX_HIKMST_ST3                  7

/*
 * Power Management Channel (PMCH) device registers
 */

struct pmch_reg {
	/* 0x000: Host Interface PM Status */
	volatile uint8_t HIPMST;
	volatile uint8_t reserved1;
	/* 0x002: Host Interface PM Data Out Buffer */
	volatile uint8_t HIPMDO;
	volatile uint8_t reserved2;
	/* 0x004: Host Interface PM Data In Buffer */
	volatile uint8_t HIPMDI;
	/* 0x005: Host Interface PM Shadow Data In Buffer */
	volatile uint8_t SHIPMDI;
	/* 0x006: Host Interface PM Data Out Buffer with SCI */
	volatile uint8_t HIPMDOC;
	volatile uint8_t reserved3;
	/* 0x008: Host Interface PM Data Out Buffer with SMI */
	volatile uint8_t HIPMDOM;
	volatile uint8_t reserved4;
	/* 0x00A: Host Interface PM Data In Buffer with SCI */
	volatile uint8_t HIPMDIC;
	volatile uint8_t reserved5;
	/* 0x00C: Host Interface PM Control */
	volatile uint8_t HIPMCTL;
	/* 0x00D: Host Interface PM Control 2 */
	volatile uint8_t HIPMCTL2;
	/* 0x00E: Host Interface PM Interrupt Control */
	volatile uint8_t HIPMIC;
	volatile uint8_t reserved6;
	/* 0x010: Host Interface PM Interrupt Enable */
	volatile uint8_t HIPMIE;
	volatile uint8_t reserved7;
};

/* PMCH register field */
#define NPCM4XX_HIPMIE_SCIE                 1
#define NPCM4XX_HIPMIE_SMIE                 2
#define NPCM4XX_HIPMCTL_IBFIE               0
#define NPCM4XX_HIPMCTL_OBEIE               1
#define NPCM4XX_HIPMCTL_SCIPOL              6
#define NPCM4XX_HIPMST_OBF                  0
#define NPCM4XX_HIPMST_IBF                  1
#define NPCM4XX_HIPMST_F0                   2
#define NPCM4XX_HIPMST_CMD                  3
#define NPCM4XX_HIPMST_ST0                  4
#define NPCM4XX_HIPMST_ST1                  5
#define NPCM4XX_HIPMST_ST2                  6
#define NPCM4XX_HIPMIC_SMIB                 1
#define NPCM4XX_HIPMIC_SCIB                 2
#define NPCM4XX_HIPMIC_SMIPOL               6

/*
 * Core Access to Host (C2H) device registers
 */
struct c2h_reg {
	/* 0x000: Indirect Host I/O Address */
	volatile uint16_t IHIOA;
	/* 0x002: Indirect Host Data */
	volatile uint8_t IHD;
	volatile uint8_t reserved1;
	/* 0x004: Lock Host Access */
	volatile uint16_t LKSIOHA;
	/* 0x006: Access Lock Violation */
	volatile uint16_t SIOLV;
	/* 0x008: Core-to-Host Modules Access Enable */
	volatile uint16_t CRSMAE;
	/* 0x00A: Module Control */
	volatile uint8_t SIBCTRL;
	volatile uint8_t reserved3;
};

/* C2H register fields */
#define NPCM4XX_LKSIOHA_LKCFG               0
#define NPCM4XX_LKSIOHA_LKSPHA              2
#define NPCM4XX_LKSIOHA_LKHIKBD             11
#define NPCM4XX_CRSMAE_CFGAE                0
#define NPCM4XX_CRSMAE_HIKBDAE              11
#define NPCM4XX_SIOLV_SPLV                  2
#define NPCM4XX_SIBCTRL_CSAE                0
#define NPCM4XX_SIBCTRL_CSRD                1
#define NPCM4XX_SIBCTRL_CSWR                2

/*
 * i2c device registers
 */
struct i2c_reg {
	/* [0x00] I2C Serial Data */
	volatile uint8_t SMBnSDA;
	volatile uint8_t RESERVE0[1];
	/* [0x02] I2C Status */
	volatile uint8_t SMBnST;
	volatile uint8_t RESERVE1[1];
	/* [0x04] I2C Control Status */
	volatile uint8_t SMBnCST;
	volatile uint8_t RESERVE2[1];
    /* [0x06] I2C Control 1 */
	volatile uint8_t SMBnCTL1;
	volatile uint8_t RESERVE3[1];
    /* [0x08] I2C Own Address 1 */
	volatile uint8_t SMBnADDR1;
    /* [0x09] Timeout Status */
	volatile uint8_t TIMEOUT_ST;
    /* [0x0A] I2C Control 2 */
	volatile uint8_t SMBnCTL2;
    /* [0x0B] Timeout Enable */
	volatile uint8_t TIMEOUT_EN;
    /* [0x0C] I2C Own Address 2 */
	volatile uint8_t SMBnADDR2;
	volatile uint8_t RESERVE4[1];
    /* [0x0E] I2C Control 3 */
	volatile uint8_t SMBnCTL3;
    /* [0x0F] DMA Control */
	volatile uint8_t DMA_CTRL;
    /* [0x10] I2C Own Address 3 */
	volatile uint8_t SMBnADDR3;
    /* [0x11] I2C Own Address 7 */
	volatile uint8_t SMBnADDR7;
    /* [0x12] I2C Own Address 4 */
	volatile uint8_t SMBnADDR4;
    /* [0x13] I2C Own Address 8 */
	volatile uint8_t SMBnADDR8;
    /* [0x14] I2C Own Address 5 */
	volatile uint8_t SMBnADDR5;
    /* [0x15] I2C Own Address 9 */
	volatile uint8_t SMBnADDR9;
    /* [0x16] I2C Own Address 6 */
	volatile uint8_t SMBnADDR6;
    /* [0x17] I2C Own Address 10 */
	volatile uint8_t SMBnADDR10;
    /* [0x18] I2C Control Status 2 */
	volatile uint8_t SMBnCST2;
    /* [0x19] I2C Control Status 3 */
	volatile uint8_t SMBnCST3;
    /* [0x1A] I2C Control 4 */
	volatile uint8_t SMBnCTL4;
    /* [0x1B] I2C Control 5 */
	volatile uint8_t SMBnCTL5;
    /* [0x1C] I2C SCL Low Time (Fast Mode) */
	volatile uint8_t SMBnSCLLT;
    /* [0x1D] I2C Address Match Status */
	volatile uint8_t ADDMTCH_ST;
    /* [0x1E] I2C SCL High Time (Fast Mode) */
	volatile uint8_t SMBnSCLHT;
    /* [0x1F] I2C Version */
	volatile uint8_t SMBn_VER;
    /* [0x20] DMA Address Byte 1 */
	volatile uint8_t DMA_ADDR1;
    /* [0x21] DMA Address Byte 2 */
	volatile uint8_t DMA_ADDR2;
    /* [0x22] DMA Address Byte 3 */
	volatile uint8_t DMA_ADDR3;
    /* [0x23] DMA Address Byte 4 */
	volatile uint8_t DMA_ADDR4;
    /* [0x24] Data Length Byte 1 */
	volatile uint8_t DATA_LEN1;
    /* [0x25] Data Length Byte 2 */
	volatile uint8_t DATA_LEN2;
    /* [0x26] Data Counter Byte 1 */
	volatile uint8_t DATA_CNT1;
    /* [0x27] Data Counter Byte 2 */
	volatile uint8_t DATA_CNT2;
	volatile uint8_t RESERVE7[1];
    /* [0x29] Timeout Control 1 */
	volatile uint8_t TIMEOUT_CTL1;
    /* [0x2A] Timeout Control 2 */
	volatile uint8_t TIMEOUT_CTL2;
    /* [0x2B] I2C PEC Data */
	volatile uint8_t SMBnPEC;
};

/* I2C register fields */
/*--------------------------*/
/*     SMBnST fields        */
/*--------------------------*/
#define NPCM4XX_SMBnST_XMIT                  0
#define NPCM4XX_SMBnST_MASTER                1
#define NPCM4XX_SMBnST_NMATCH                2
#define NPCM4XX_SMBnST_STASTR                3
#define NPCM4XX_SMBnST_NEGACK                4
#define NPCM4XX_SMBnST_BER                   5
#define NPCM4XX_SMBnST_SDAST                 6
#define NPCM4XX_SMBnST_SLVSTP                7

/*--------------------------*/
/*     SMBnCST fields       */
/*--------------------------*/
#define NPCM4XX_SMBnCST_BUSY                 0
#define NPCM4XX_SMBnCST_BB                   1
#define NPCM4XX_SMBnCST_MATCH                2
#define NPCM4XX_SMBnCST_GCMATCH              3
#define NPCM4XX_SMBnCST_TSDA                 4
#define NPCM4XX_SMBnCST_TGSCL                5
#define NPCM4XX_SMBnCST_MATCHAF              6
#define NPCM4XX_SMBnCST_ARPMATCH             7

/*--------------------------*/
/*     SMBnCTL1 fields      */
/*--------------------------*/
#define NPCM4XX_SMBnCTL1_START               0
#define NPCM4XX_SMBnCTL1_STOP                1
#define NPCM4XX_SMBnCTL1_INTEN               2
#define NPCM4XX_SMBnCTL1_EOBINTE             3
#define NPCM4XX_SMBnCTL1_ACK                 4
#define NPCM4XX_SMBnCTL1_GCMEN               5
#define NPCM4XX_SMBnCTL1_NMINTE              6
#define NPCM4XX_SMBnCTL1_STASTRE             7

/*--------------------------*/
/*     SMBnADDR1-10 fields  */
/*--------------------------*/
#define NPCM4XX_SMBnADDR_ADDR                0
#define NPCM4XX_SMBnADDR_SAEN                7

/*--------------------------*/
/*    TIMEOUT_ST fields     */
/*--------------------------*/
#define NPCM4XX_TIMEOUT_ST_T_OUTST1          0
#define NPCM4XX_TIMEOUT_ST_T_OUTST2          1
#define NPCM4XX_TIMEOUT_ST_T_OUTST1_EN       6
#define NPCM4XX_TIMEOUT_ST_T_OUTST2_EN       7

/*--------------------------*/
/*     SMBnCTL2 fields      */
/*--------------------------*/
#define NPCM4XX_SMBnCTL2_ENABLE              0
#define NPCM4XX_SMBnCTL2_SCLFRQ60_FIELD      FIELD(1, 7)

/*--------------------------*/
/*     TIMEOUT_EN fields    */
/*--------------------------*/
#define NPCM4XX_TIMEOUT_EN_TIMEOUT_EN        0
#define NPCM4XX_TIMEOUT_EN_TO_CKDIV          2

/*--------------------------*/
/*     SMBnCTL3 fields      */
/*--------------------------*/
#define NPCM4XX_SMBnCTL3_SCLFRQ87_FIELD      FIELD(0, 2)
#define NPCM4XX_SMBnCTL3_ARPMEN              2
#define NPCM4XX_SMBnCTL3_SLP_START           3
#define NPCM4XX_SMBnCTL3_400K_MODE           4
#define NPCM4XX_SMBnCTL3_SDA_LVL             6
#define NPCM4XX_SMBnCTL3_SCL_LVL             7

/*--------------------------*/
/*      DMA_CTRL fields     */
/*--------------------------*/
#define NPCM4XX_DMA_CTRL_DMA_INT_CLR         0
#define NPCM4XX_DMA_CTRL_DMA_EN              1
#define NPCM4XX_DMA_CTRL_LAST_PEC            2
#define NPCM4XX_DMA_CTRL_DMA_STALL           3
#define NPCM4XX_DMA_CTRL_DMA_IRQ             7

/*--------------------------*/
/*     SMBnCST2 fields      */
/*--------------------------*/
#define NPCM4XX_SMBnCST2_MATCHA1F            0
#define NPCM4XX_SMBnCST2_MATCHA2F            1
#define NPCM4XX_SMBnCST2_MATCHA3F            2
#define NPCM4XX_SMBnCST2_MATCHA4F            3
#define NPCM4XX_SMBnCST2_MATCHA5F            4
#define NPCM4XX_SMBnCST2_MATCHA6F            5
#define NPCM4XX_SMBnCST2_MATCHA7F            6
#define NPCM4XX_SMBnCST2_INTSTS              7

/*--------------------------*/
/*     SMBnCST3 fields      */
/*--------------------------*/
#define NPCM4XX_SMBnCST3_MATCHA8F            0
#define NPCM4XX_SMBnCST3_MATCHA9F            1
#define NPCM4XX_SMBnCST3_MATCHA10F           2
#define NPCM4XX_SMBnCST3_EO_BUSY             7

/*
 * INTERNAL 8-BIT/16-BIT/32-BIT TIMER (ITIM32) device registers
 */

struct itim32_reg {
	/* [0x00] Internal 8-Bit Timer Counter */
	volatile uint8_t CNT;
	/* [0x01] Internal Timer Prescaler */
	volatile uint8_t PRE;
	/* [0x02] Internal 16-Bit Timer Counter */
	volatile uint16_t CNT16;
	/* [0x04] Internal Timer Control and Status */
	volatile uint8_t CTS;
	volatile uint8_t RESERVED1[3];
	/* [0x08] Internal 32-Bit Timer Counter */
	volatile uint32_t CNT32;
};

/* ITIM32 register fields */
#define ITIM32_CTS_ITEN                         (7)
#define ITIM32_CTS_CKSEL                        (4)
#define ITIM32_CTS_TO_WUE                       (3)
#define ITIM32_CTS_TO_IE                        (2)
#define ITIM32_CTS_TO_STS                       (0)

/*
 * Tachometer (TACH) Sensor device registers
 */
struct tach_reg {
	/* 0x000: Timer/Counter 1 */
	volatile uint16_t TCNT1;
	/* 0x002: Reload/Capture A */
	volatile uint16_t TCRA;
	/* 0x004: Reload/Capture B */
	volatile uint16_t TCRB;
	/* 0x006: Timer/Counter 2 */
	volatile uint16_t TCNT2;
	/* 0x008: Clock Prescaler */
	volatile uint8_t TPRSC;
	volatile uint8_t reserved1;
	/* 0x00A: Clock Unit Control */
	volatile uint8_t TCKC;
	volatile uint8_t reserved2;
	/* 0x00C: Timer Mode Control */
	volatile uint8_t TMCTRL;
	volatile uint8_t reserved3;
	/* 0x00E: Timer Event Control */
	volatile uint8_t TECTRL;
	volatile uint8_t reserved4;
	/* 0x010: Timer Event Clear */
	volatile uint8_t TECLR;
	volatile uint8_t reserved5;
	/* 0x012: Timer Interrupt Enable */
	volatile uint8_t TIEN;
	volatile uint8_t reserved6;
	/* 0x014: Compare A */
	volatile uint16_t TCPA;
	/* 0x016: Compare B */
	volatile uint16_t TCPB;
	/* 0x018: Compare Configuration */
	volatile uint8_t TCPCFG;
	volatile uint8_t reserved7;
	/* 0x01A: Timer Wake-Up Enablen */
	volatile uint8_t TWUEN;
	volatile uint8_t reserved8;
	/* 0x01C: Timer Configuration */
	volatile uint8_t TCFG;
	volatile uint8_t reserved9;
};

/* TACH register fields */
#define NPCM4XX_TCKC_LOW_PWR                7
#define NPCM4XX_TCKC_PLS_ACC_CLK            6
#define NPCM4XX_TCKC_C1CSEL_FIELD           FIELD(0, 3)
#define NPCM4XX_TCKC_C2CSEL_FIELD           FIELD(3, 3)
#define NPCM4XX_TMCTRL_MDSEL_FIELD          FIELD(0, 3)
#define NPCM4XX_TMCTRL_TAEN                 5
#define NPCM4XX_TMCTRL_TBEN                 6
#define NPCM4XX_TMCTRL_TAEDG                3
#define NPCM4XX_TMCTRL_TBEDG                4
#define NPCM4XX_TCFG_TADBEN                 6
#define NPCM4XX_TCFG_TBDBEN                 7
#define NPCM4XX_TECTRL_TAPND                0
#define NPCM4XX_TECTRL_TBPND                1
#define NPCM4XX_TECTRL_TCPND                2
#define NPCM4XX_TECTRL_TDPND                3
#define NPCM4XX_TECLR_TACLR                 0
#define NPCM4XX_TECLR_TBCLR                 1
#define NPCM4XX_TECLR_TCCLR                 2
#define NPCM4XX_TECLR_TDCLR                 3
#define NPCM4XX_TIEN_TAIEN                  0
#define NPCM4XX_TIEN_TBIEN                  1
#define NPCM4XX_TIEN_TCIEN                  2
#define NPCM4XX_TIEN_TDIEN                  3
#define NPCM4XX_TWUEN_TAWEN                 0
#define NPCM4XX_TWUEN_TBWEN                 1
#define NPCM4XX_TWUEN_TCWEN                 2
#define NPCM4XX_TWUEN_TDWEN                 3

/* Debug Interface registers */
struct dbg_reg {
	/* 0x000: Debug Control */
	volatile uint8_t DBGCTRL;
	volatile uint8_t reserved1;
	/* 0x002: Debug Freeze Enable 1 */
	volatile uint8_t DBGFRZEN1;
	/* 0x003: Debug Freeze Enable 2 */
	volatile uint8_t DBGFRZEN2;
	/* 0x004: Debug Freeze Enable 3 */
	volatile uint8_t DBGFRZEN3;
	/* 0x005: Debug Freeze Enable 4 */
	volatile uint8_t DBGFRZEN4;
};
/* Debug Interface registers fields */
#define NPCM4XX_DBGFRZEN3_GLBL_FRZ_DIS      7

/* Platform Environment Control Interface (PECI) device registers */
struct peci_reg {
	/* 0x000: PECI Control Status */
	volatile uint8_t PECI_CTL_STS;
	/* 0x001: PECI Read Length */
	volatile uint8_t PECI_RD_LENGTH;
	/* 0x002: PECI Address */
	volatile uint8_t PECI_ADDR;
	/* 0x003: PECI Command */
	volatile uint8_t PECI_CMD;
	/* 0x004: PECI Control 2 */
	volatile uint8_t PECI_CTL2;
	/* 0x005: PECI Index */
	volatile uint8_t PECI_INDEX;
	/* 0x006: PECI Index Data */
	volatile uint8_t PECI_IDATA;
	/* 0x007: PECI Write Length */
	volatile uint8_t PECI_WR_LENGTH;
	volatile uint8_t reserved1;
	/* 0x009: PECI Configuration */
	volatile uint8_t PECI_CFG;
	volatile uint8_t reserved2;
	/* 0x00B: PECI Write FCS */
	volatile uint8_t PECI_WR_FCS;
	/* 0x00C: PECI Read FCS */
	volatile uint8_t PECI_RD_FCS;
	/* 0x00D: PECI Assured Write FCS */
	volatile uint8_t PECI_AW_FCS;
	/* 0X00E: PECI Version */
	volatile uint8_t PECI_VER;
	/* 0x00F: PECI Transfer Rate */
	volatile uint8_t PECI_RATE;
	/* 0x010 - 0x02A: PECI Data In/Out */
	union {
		volatile uint8_t PECI_DATA_IN[27];
		volatile uint8_t PECI_DATA_OUT[27];
	};
};

/* PECI register fields */
#define NPCM4XX_PECI_CTL_STS_START_BUSY     0
#define NPCM4XX_PECI_CTL_STS_DONE           1
#define NPCM4XX_PECI_CTL_STS_CRC_ERR        3
#define NPCM4XX_PECI_CTL_STS_ABRT_ERR       4
#define NPCM4XX_PECI_CTL_STS_AWFCS_EB       5
#define NPCM4XX_PECI_CTL_STS_DONE_EN        6
#define NPCM4XX_PECI_RATE_MAX_BIT_RATE      FIELD(0, 5)
#define NPCM4XX_PECI_RATE_MAX_BIT_RATE_MASK 0x1F
/* The minimal valid value of NPCM4XX_PECI_RATE_MAX_BIT_RATE field */
#define PECI_MAX_BIT_RATE_VALID_MIN      0x05
#define PECI_HIGH_SPEED_MIN_VAL          0x07

#define NPCM4XX_PECI_RATE_EHSP              6
#define NPCM4XX_PECI_RATE_HLOAD             7

#endif /* _NUVOTON_NPCM4XX_REG_DEF_H */
