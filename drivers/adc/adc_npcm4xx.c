/*
 * Copyright (c) 2020 Nuvoton Technology Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nuvoton_npcm4xx_adc

#include <assert.h>
#include <drivers/adc.h>
#include <common/reg/reg_def.h>
#include <common/reg/reg_access.h>
#include <drivers/clock_control.h>
#include <kernel.h>
#include <soc.h>

#define ADC_CONTEXT_USES_KERNEL_TIMER
#include "adc_context.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(adc_npcm4xx, CONFIG_ADC_LOG_LEVEL);

/* ADC channel number */
#define NPCM4XX_ADC_CH_COUNT 24

/* ADC internal reference voltage (Unit:mV) */
#define NPCM4XX_ADC_VREF_VOL 2048

#define ADC_NPCM4XX_REG_BASE    ((struct adc_reg *)(DT_INST_REG_ADDR(0)))

enum adc_channel_en {
	CHEN0_AVSB = 0,
	CHEN1_VSB,
	CHEN2_VCC,
	CHEN3_VHIF,
	CHEN4_VIN7,
	CHEN5_VIN5,
	CHEN6_VIN16,
	CHEN7_THR16,
	CHEN8_VIN15,
	CHEN9_THR15,
	CHEN10_VIN14,
	CHEN11_THR14,
	CHEN12_VIN1,
	CHEN13_THR1,
	CHEN14_VIN2,
	CHEN15_THR2,
	CHEN16_VIN3,
	CHEN17_VTT,
	CHEN18_VBAT,
	CHEN19_TD2P,
	CHEN20_TD1P,
	CHEN21_TD0P,
	CHEN22_TD3P,
	CHEN23_TD4P
};

/* Device config */
struct adc_npcm4xx_config {
	/* adc controller base address */
	uintptr_t base;
};

/* Driver data */
struct adc_npcm4xx_data {
	/* mutex of ADC channels */
	struct adc_context ctx;
	uint8_t CurChannel_Idx;
	/*
	 * Bit-mask indicating the channels to be included in each sampling
	 * of this sequence.
	 */
	uint32_t channels;
	/* ADC Device pointer used in api functions */
	const struct device *adc_dev;
	uint16_t *buffer;
	uint16_t *repeat_buffer;
	/* end pointer of buffer to ensure enough space for storing ADC data. */
	uint16_t *buf_end;
};

/* Driver convenience defines */
#define DRV_CONFIG(dev) ((const struct adc_npcm4xx_config *)(dev)->config)

#define DRV_DATA(dev) ((struct adc_npcm4xx_data *)(dev)->data)

static int adc_npcm4xx_validate_buffer_size(const struct device *dev,
					    const struct adc_sequence *sequence)
{
	uint8_t channels = 0;
	uint32_t mask;
	size_t needed;

	for (mask = BIT(NPCM4XX_ADC_CH_COUNT - 1); mask != 0; mask >>= 1) {
		if (mask & sequence->channels) {
			channels++;
		}
	}

	needed = channels * sizeof(uint16_t);
	if (sequence->options) {
		needed *= (1 + sequence->options->extra_samplings);
	}

	if (sequence->buffer_size < needed) {
		return -ENOSPC;
	}

	return 0;
}

static void npcm4xx_adc_start_convert(const struct device *dev)
{
	struct adc_reg *const adc_regs = ADC_NPCM4XX_REG_BASE;
	struct adc_npcm4xx_data *const data = DRV_DATA(dev);
	uint8_t IsDiode;

	IsDiode = false;

	data->CurChannel_Idx = find_lsb_set(data->channels) - 1;

	/* Set voltage large than 2.048V */
	adc_regs->ADCACTRL1 &= ~MaskBit(NPCM4XX_ACTRL1_PWCTRL);

	/* Start ADC scan conversion */
	adc_regs->DSADCCTRL0 &= ~0x1F; /* CH_SEL Mask */

	if ((data->CurChannel_Idx >= CHEN19_TD2P) && (data->CurChannel_Idx <= CHEN23_TD4P)) {
		IsDiode = true;
		/* Set voltage smaller than 2.048V */
		adc_regs->ADCACTRL1 |= MaskBit(NPCM4XX_ACTRL1_PWCTRL);
	}
	if (IsDiode) {
		adc_regs->DSADCCTRL0 &= ~MaskBit(NPCM4XX_CTRL0_VNT);
		switch (data->CurChannel_Idx) {
		case CHEN19_TD2P:
			adc_regs->DSADCCTRL0 |= 0x00;
			adc_regs->ADCTM &= ~(0x03 << NPCM4XX_TM_T_MODE1);
			break;
		case CHEN20_TD1P:
			adc_regs->DSADCCTRL0 |= 0x01;
			adc_regs->ADCTM &= ~(0x03 << NPCM4XX_TM_T_MODE2);
			break;
		case CHEN21_TD0P:
			adc_regs->DSADCCTRL0 |= 0x02;
			adc_regs->ADCTM &= ~(0x03 << NPCM4XX_TM_T_MODE3);
			break;
		case CHEN22_TD3P:
			adc_regs->DSADCCTRL0 |= 0x03;
			adc_regs->ADCTM &= ~(0x03 << NPCM4XX_TM_T_MODE4);
			break;
		case CHEN23_TD4P:
			adc_regs->DSADCCTRL0 |= 0x04;
			adc_regs->ADCTM &= ~(0x03 << NPCM4XX_TM_T_MODE5);
			break;
		default:
			break;
		}
	} else   {
		adc_regs->DSADCCTRL0 |= MaskBit(NPCM4XX_CTRL0_VNT);
		if ((data->CurChannel_Idx >= CHEN4_VIN7) && (data->CurChannel_Idx <= CHEN18_VBAT)) {
			switch (data->CurChannel_Idx) {
			case CHEN4_VIN7:
				adc_regs->DSADCCTRL0 |= 0x06;
				break;
			case CHEN5_VIN5:
				adc_regs->DSADCCTRL0 |= 0x07;
				break;
			case CHEN6_VIN16:
				adc_regs->DSADCCTRL0 |= 0x08;
				adc_regs->DSADCCTRL6 |= MaskBit(0);
				break;
			case CHEN7_THR16:
				adc_regs->DSADCCTRL0 |= 0x08;
				adc_regs->DSADCCTRL6 &= ~MaskBit(0);
				/* Set voltage smaller than 2.048V */
				adc_regs->ADCACTRL1 |= MaskBit(NPCM4XX_ACTRL1_PWCTRL);
				break;
			case CHEN8_VIN15:
				adc_regs->DSADCCTRL0 |= 0x09;
				adc_regs->DSADCCTRL6 |= MaskBit(1);
				break;
			case CHEN9_THR15:
				adc_regs->DSADCCTRL0 |= 0x09;
				adc_regs->DSADCCTRL6 &= ~MaskBit(1);
				/* Set voltage smaller than 2.048V */
				adc_regs->ADCACTRL1 |= MaskBit(NPCM4XX_ACTRL1_PWCTRL);
				break;
			case CHEN10_VIN14:
				adc_regs->DSADCCTRL0 |= 0x0A;
				adc_regs->DSADCCTRL6 |= MaskBit(2);
				break;
			case CHEN11_THR14:
				adc_regs->DSADCCTRL0 |= 0x0A;
				adc_regs->DSADCCTRL6 &= ~MaskBit(2);
				/* Set voltage smaller than 2.048V */
				adc_regs->ADCACTRL1 |= MaskBit(NPCM4XX_ACTRL1_PWCTRL);
				break;
			case CHEN12_VIN1:
				adc_regs->DSADCCTRL0 |= 0x0B;
				adc_regs->DSADCCTRL6 |= MaskBit(3);
				break;
			case CHEN13_THR1:
				adc_regs->DSADCCTRL0 |= 0x0B;
				adc_regs->DSADCCTRL6 &= ~MaskBit(3);
				/* Set voltage smaller than 2.048V */
				adc_regs->ADCACTRL1 |= MaskBit(NPCM4XX_ACTRL1_PWCTRL);
				break;
			case CHEN14_VIN2:
				adc_regs->DSADCCTRL0 |= 0x0C;
				adc_regs->DSADCCTRL6 |= MaskBit(4);
				break;
			case CHEN15_THR2:
				adc_regs->DSADCCTRL0 |= 0x0C;
				adc_regs->DSADCCTRL6 &= ~MaskBit(4);
				/* Set voltage smaller than 2.048V */
				adc_regs->ADCACTRL1 |= MaskBit(NPCM4XX_ACTRL1_PWCTRL);
				break;
			case CHEN16_VIN3:
				adc_regs->DSADCCTRL0 |= 0x0D;
				break;
			case CHEN17_VTT:
				adc_regs->DSADCCTRL0 |= 0x0E;
				break;
			case CHEN18_VBAT:
				adc_regs->DSADCCTRL0 |= 0x0F;
				break;

			}
		} else   {
			adc_regs->DSADCCTRL0 |= data->CurChannel_Idx;
		}

	}

	/* clear sts */
	adc_regs->DSADCSTS = adc_regs->DSADCSTS;
	/* enable interrupt(ADC_ICEN) and start */
	adc_regs->DSADCCFG |= (MaskBit(NPCM4XX_CFG_ICEN) | MaskBit(NPCM4XX_CFG_START));
}

/* ADC local functions */
static void adc_npcm4xx_isr(const struct device *dev)
{
	struct adc_reg *const adc_regs = ADC_NPCM4XX_REG_BASE;
	struct adc_npcm4xx_data *const data = dev->data;
	uint16_t result, channel;

	/* clear sts */
	adc_regs->DSADCSTS = adc_regs->DSADCSTS;

	/* get data */
	channel = data->channels;
	result = adc_regs->TCHNDAT;
	result &= ~MaskBit(NPCM4XX_TCHNDATA_NEW);
	if ((data->CurChannel_Idx >= CHEN0_AVSB) && (data->CurChannel_Idx <= CHEN18_VBAT)) {
		if ((data->CurChannel_Idx == CHEN7_THR16) ||
		    (data->CurChannel_Idx == CHEN9_THR15) ||
		    (data->CurChannel_Idx == CHEN11_THR14) ||
		    (data->CurChannel_Idx == CHEN13_THR1) ||
		    (data->CurChannel_Idx == CHEN15_THR2)) {

			result = (result << 5);
		} else   {
			result <<= 1; /* X2 for large than 2.048V by Spec.*/
		}
	} else   {
		/* TDxP */
		result = (result << 5);
	}

	if (data->buffer < data->buf_end) {
		*data->buffer++ = result;
	}
	data->channels &= ~BIT(data->CurChannel_Idx);

	if (data->channels) {
		npcm4xx_adc_start_convert(dev);
	} else   {
		/* Inform sampling is done */
		adc_context_on_sampling_done(&data->ctx, data->adc_dev);
	}

}

static int adc_npcm4xx_start_read(const struct device *dev,
				  const struct adc_sequence *sequence)
{
	struct adc_npcm4xx_data *const data = DRV_DATA(dev);
	int error = 0;

	if (!sequence->channels ||
	    (sequence->channels & ~BIT_MASK(NPCM4XX_ADC_CH_COUNT))) {
		LOG_ERR("Invalid ADC channels");
		return -EINVAL;
	}
	/* Fixed 10 bit resolution of npcm4xx ADC */
	if (sequence->resolution != 10) {
		LOG_ERR("Unfixed 10 bit ADC resolution");
		return -ENOTSUP;
	}

	error = adc_npcm4xx_validate_buffer_size(dev, sequence);
	if (error) {
		LOG_ERR("ADC buffer size too small");
		return error;
	}

	/* Save ADC sequence sampling buffer and its end pointer address */
	data->buffer = sequence->buffer;
	data->buf_end = data->buffer + sequence->buffer_size / sizeof(uint16_t);

	/* Start ADC conversion */
	adc_context_start_read(&data->ctx, sequence);
	error = adc_context_wait_for_completion(&data->ctx);

	return error;
}

/* ADC api functions */
static void adc_context_start_sampling(struct adc_context *ctx)
{
	struct adc_npcm4xx_data *const data =
		CONTAINER_OF(ctx, struct adc_npcm4xx_data, ctx);

	data->repeat_buffer = data->buffer;
	data->channels = ctx->sequence.channels;

	if (!data->channels) {
		LOG_ERR("No ADC channel can start sampling!!");
	} else   {
		npcm4xx_adc_start_convert(data->adc_dev);
	}
}

static void adc_context_update_buffer_pointer(struct adc_context *ctx,
					      bool repeat_sampling)
{
	struct adc_npcm4xx_data *const data =
		CONTAINER_OF(ctx, struct adc_npcm4xx_data, ctx);

	if (repeat_sampling) {
		data->buffer = data->repeat_buffer;
	}
}

static int adc_npcm4xx_channel_setup(const struct device *dev,
				     const struct adc_channel_cfg *channel_cfg)
{
	uint8_t channel_id = channel_cfg->channel_id;

	if (channel_id >= NPCM4XX_ADC_CH_COUNT) {
		LOG_ERR("Invalid channel %d", channel_id);
		return -EINVAL;
	}

	if (channel_cfg->acquisition_time != ADC_ACQ_TIME_DEFAULT) {
		LOG_ERR("Unsupported channel acquisition time");
		return -ENOTSUP;
	}

	if (channel_cfg->differential) {
		LOG_ERR("Differential channels are not supported");
		return -ENOTSUP;
	}

	if (channel_cfg->gain != ADC_GAIN_1) {
		LOG_ERR("Unsupported channel gain %d", channel_cfg->gain);
		return -ENOTSUP;
	}

	if (channel_cfg->reference != ADC_REF_INTERNAL) {
		LOG_ERR("Unsupported channel reference");
		return -ENOTSUP;
	}

	return 0;
}

static int adc_npcm4xx_read(const struct device *dev,
			    const struct adc_sequence *sequence)
{
	struct adc_npcm4xx_data *const data = DRV_DATA(dev);
	int error;

	adc_context_lock(&data->ctx, false, NULL);
	error = adc_npcm4xx_start_read(dev, sequence);
	adc_context_release(&data->ctx, error);

	return error;
}

#if defined(CONFIG_ADC_ASYNC)
static int adc_npcm4xx_read_async(const struct device *dev,
				  const struct adc_sequence *sequence,
				  struct k_poll_signal *async)
{
	struct adc_npcm4xx_data *const data = DRV_DATA(dev);
	int error;

	adc_context_lock(&data->ctx, true, async);
	error = adc_npcm4xx_start_read(dev, sequence);
	adc_context_release(&data->ctx, error);

	return error;
}
#endif /* CONFIG_ADC_ASYNC */

/* ADC driver registration */
static const struct adc_driver_api adc_npcm4xx_driver_api = {
	.channel_setup = adc_npcm4xx_channel_setup,
	.read = adc_npcm4xx_read,
#if defined(CONFIG_ADC_ASYNC)
	.read_async = adc_npcm4xx_read_async,
#endif
	.ref_internal = NPCM4XX_ADC_VREF_VOL,
};

static int adc_npcm4xx_init(const struct device *dev);

static const struct adc_npcm4xx_config adc_npcm4xx_cfg = {
	.base = DT_INST_REG_ADDR(0),
};

static struct adc_npcm4xx_data adc_npcm4xx_data = {
	ADC_CONTEXT_INIT_TIMER(adc_npcm4xx_data, ctx),
	ADC_CONTEXT_INIT_LOCK(adc_npcm4xx_data, ctx),
	ADC_CONTEXT_INIT_SYNC(adc_npcm4xx_data, ctx),
};

DEVICE_DT_INST_DEFINE(0,
		      adc_npcm4xx_init, NULL,
		      &adc_npcm4xx_data, &adc_npcm4xx_cfg,
		      POST_KERNEL,
		      CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
		      &adc_npcm4xx_driver_api);

static int adc_npcm4xx_init(const struct device *dev)
{
	struct adc_npcm4xx_data *const data = ((struct adc_npcm4xx_data *)(dev)->data);

	/* Save ADC device in data */
	data->adc_dev = dev;

	IRQ_CONNECT(DT_INST_IRQN(0), DT_INST_IRQ(0, priority),
		    adc_npcm4xx_isr, DEVICE_DT_INST_GET(0), 0);
	irq_enable(DT_INST_IRQN(0));

	adc_context_unlock_unconditionally(&data->ctx);
	return 0;
}
