#ifdef CONFIG_GPIO_NPCM4XX
GPIO_SIG_DEFINE(GPIO04, E1, SIG_DESC_CLEAR(0x4D, 0), SIG_DESC_CLEAR(0x19, 1), SIG_DESC_CLEAR(0x1B, 1), SIG_DESC_CLEAR(0x14, 3))
GPIO_SIG_DEFINE(GPIO05, D2, SIG_DESC_CLEAR(0x4D, 0), SIG_DESC_CLEAR(0x19, 2), SIG_DESC_CLEAR(0x1A, 6), SIG_DESC_CLEAR(0x13, 1))
GPIO_SIG_DEFINE(GPIO14, H4, SIG_DESC_CLEAR(0x1B, 2), SIG_DESC_CLEAR(0x1C, 6))
GPIO_SIG_DEFINE(GPIO15, H3, SIG_DESC_CLEAR(0x1A, 7), SIG_DESC_CLEAR(0x1B, 3), SIG_DESC_CLEAR(0x1C, 7))
GPIO_SIG_DEFINE(GPIO25, L5, SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x1A, 5))
GPIO_SIG_DEFINE(GPIO26, M5, SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x1A, 5))
GPIO_SIG_DEFINE(GPIO34, L6, SIG_DESC_CLEAR(0x31, 0), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x1A, 3))
GPIO_SIG_DEFINE(GPIO35, M6, SIG_DESC_CLEAR(0x31, 1), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x1A, 3))
GPIO_SIG_DEFINE(GPIO36, L7, SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x17, 5), SIG_DESC_CLEAR(0x14, 4), SIG_DESC_CLEAR(0x1A, 2))
GPIO_SIG_DEFINE(GPIO37, M7, SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x13, 2), SIG_DESC_CLEAR(0x1A, 2))
GPIO_SIG_DEFINE(GPIO60, K10, SIG_DESC_SET(0x5D, 2), SIG_DESC_CLEAR(0x1A, 0))
GPIO_SIG_DEFINE(GPIO61, K11, SIG_DESC_SET(0x5D, 1), SIG_DESC_CLEAR(0x1A, 0))
GPIO_SIG_DEFINE(GPIO96, A5, SIG_DESC_SET(0x17, 3))
GPIO_SIG_DEFINE(GPIO97, B5, SIG_DESC_SET(0x17, 4))
GPIO_SIG_DEFINE(GPIOA2, B3, SIG_DESC_CLEAR(0x31, 2), SIG_DESC_CLEAR(0x1A, 1))
GPIO_SIG_DEFINE(GPIOA7, B2, SIG_DESC_CLEAR(0x31, 3), SIG_DESC_CLEAR(0x1A, 1))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(uart0), okay) && CONFIG_UART_NPCM4XX
/* DEVALTB, DEVALTC */
SIG_DEFINE(UARTA_CR_SIN, H4, SIG_DESC_CLEAR(0x1B, 2), SIG_DESC_SET(0x1C, 6))
/* DEVALTA, DEVLATB */
SIG_DEFINE(UARTA_CR_SOUT, H3, SIG_DESC_SET(0x1A, 7), SIG_DESC_CLEAR(0x1B, 3))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(uart1), okay) && CONFIG_UART_NPCM4XX
/* EMAC_CTL, DEVALT9, DEVALTB */
SIG_DEFINE(UARTB_CR_SIN, E1, SIG_DESC_CLEAR(0x4D, 0), SIG_DESC_CLEAR(0x19, 1), SIG_DESC_SET(0x1B, 1))
/* EMAC_CTL, DEVALT9, DEVALTA */
SIG_DEFINE(UARTB_CR_SOUT, D2, SIG_DESC_CLEAR(0x4D, 0), SIG_DESC_CLEAR(0x19, 2), SIG_DESC_SET(0x1A, 6))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c0), okay) && CONFIG_I2C_NPCM4XX
/* DEVALT5D, DEVALTA */
SIG_DEFINE(SCL1A, K10, SIG_DESC_SET(0x5D, 2), SIG_DESC_SET(0x1A, 0))
/* DEVALT5D, DEVALTA */
SIG_DEFINE(SDA1A, K11, SIG_DESC_SET(0x5D, 1), SIG_DESC_SET(0x1A, 0))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c1), okay) && CONFIG_I2C_NPCM4XX
/* DEVALT6, DEVALTA */
SIG_DEFINE(SCL1B, M5, SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_SET(0x1A, 5))
/* DEVALT6, DEVALTA */
SIG_DEFINE(SDA1B, L5, SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_SET(0x1A, 5))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c2), okay) && CONFIG_I2C_NPCM4XX
/* DEVALT31, DEVALTA */
SIG_DEFINE(SCL3A, B2, SIG_DESC_CLEAR(0x31, 3), SIG_DESC_SET(0x1A, 1))
/* DEVALT31, DEVALTA */
SIG_DEFINE(SDA3A, B3, SIG_DESC_CLEAR(0x31, 2), SIG_DESC_SET(0x1A, 1))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c3), okay) && CONFIG_I2C_NPCM4XX
/* DEVALT6, DEVALT7, DEVALT4, DEVALTA */
SIG_DEFINE(SCL4A, L7, SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x17, 5), SIG_DESC_CLEAR(0x14, 4), SIG_DESC_SET(0x1A, 2))
/* DEVALT6, DEVALT3, DEVALTA */
SIG_DEFINE(SDA4A, M7, SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x16, 3), SIG_DESC_CLEAR(0x13, 2), SIG_DESC_SET(0x1A, 2))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c4), okay) && CONFIG_I2C_NPCM4XX
/* DEVALT31, DEVALT6 */
SIG_DEFINE(SCL5A, L6, SIG_DESC_CLEAR(0x31, 0), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_SET(0x1A, 3))
/* DEVALT31, DEVALT6, DEVALTA */
SIG_DEFINE(SDA5A, M6, SIG_DESC_CLEAR(0x31, 1), SIG_DESC_CLEAR(0x16, 2), SIG_DESC_CLEAR(0x16, 3), SIG_DESC_SET(0x1A, 3))
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(i2c5), okay) && CONFIG_I2C_NPCM4XX
/* DEVALT7 */
SIG_DEFINE(SCL6A, A5, SIG_DESC_CLEAR(0x17, 2), SIG_DESC_CLEAR(0x17, 3))
/* DEVALT7, DEVALT5 */
SIG_DEFINE(SDA6A, B5, SIG_DESC_CLEAR(0x17, 2), SIG_DESC_CLEAR(0x17, 4), SIG_DESC_CLEAR(0x15, 4))
#endif
