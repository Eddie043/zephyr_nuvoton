#include <stdint.h>
#include <kernel.h>
/**
 * cache area control: each bit controls 16MB cache area
 *	1: cacheable
 *	0: no-cache
 *
 *	bit[0]: 1st 16MB from 0x0000_0000 to 0x00ff_ffff
 *	bit[1]: 2nd 16MB from 0x0100_0000 to 0x01ff_ffff
 *	...
 *	bit[30]: 31th 16MB from 0x1e00_0000 to 0x1eff_ffff
 *	bit[31]: 32th 16MB from 0x1f00_0000 to 0x1fff_ffff
 */
#define CACHE_AREA_CTRL_REG     0xa40
#define CACHE_AREA_SIZE_LOG2    24
#define CACHE_AREA_SIZE         (1 << CACHE_AREA_SIZE_LOG2)

#define CACHE_INVALID_REG       0xa44
#define DCACHE_INVALID(addr)    (BIT(31) | ((addr & GENMASK(10, 0)) << 16))
#define ICACHE_INVALID(addr)    (BIT(15) | ((addr & GENMASK(10, 0)) << 0))

#define CACHE_FUNC_CTRL_REG     0xa48
#define ICACHE_CLEAN            BIT(2)
#define DCACHE_CLEAN            BIT(1)
#define CACHE_EANABLE           BIT(0)

/* cache size = 32B * 128 = 4KB */
#define CACHE_LINE_SIZE_LOG2    5
#define CACHE_LINE_SIZE         (1 << CACHE_LINE_SIZE_LOG2)
#define N_CACHE_LINE            128
#define CACHE_ALIGNED_ADDR(addr) \
	((addr >> CACHE_LINE_SIZE_LOG2) << CACHE_LINE_SIZE_LOG2)

/* prefetch buffer */
#define PREFETCH_BUF_SIZE       CACHE_LINE_SIZE

#define CACHED_SRAM_ADDR        DT_REG_ADDR_BY_IDX(DT_NODELABEL(sdram0), 0)
#define CACHED_SRAM_SIZE        DT_REG_SIZE_BY_IDX(DT_NODELABEL(sdram0), 0)
#define CACHED_SRAM_END         (CACHED_SRAM_ADDR + CACHED_SRAM_SIZE - 1)

static void aspeed_cache_init(void)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));
	uint32_t start_bit, end_bit;

	/* set all cache areas to no-cache by default */
	sys_write32(0, base + CACHE_FUNC_CTRL_REG);

	/* calculate how many areas need to be set */
	start_bit = CACHED_SRAM_ADDR >> CACHE_AREA_SIZE_LOG2;
	end_bit = CACHED_SRAM_END >> CACHE_AREA_SIZE_LOG2;
	sys_write32(GENMASK(end_bit, start_bit), base + CACHE_AREA_CTRL_REG);

	/* enable cache */
	sys_write32(CACHE_EANABLE, base + CACHE_FUNC_CTRL_REG);
}

void cache_data_enable(void)
{
	aspeed_cache_init();
}

void cache_data_disable(void)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));

	sys_write32(0, base + CACHE_FUNC_CTRL_REG);
}

void cache_instr_enable(void)
{
	aspeed_cache_init();
}

void cache_instr_disable(void)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));

	sys_write32(0, base + CACHE_FUNC_CTRL_REG);
}

int cache_data_all(int op)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));
	uint32_t ctrl = sys_read32(base + CACHE_FUNC_CTRL_REG);

	ARG_UNUSED(op);

	ctrl &= ~DCACHE_CLEAN;
	sys_write32(ctrl, base + CACHE_FUNC_CTRL_REG);
	ctrl |= DCACHE_CLEAN;
	sys_write32(ctrl, base + CACHE_FUNC_CTRL_REG);

	return 0;
}

int cache_data_range(void *addr, size_t size, int op)
{
	ARG_UNUSED(op);
	return 0;
}

int cache_instr_all(int op)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));
	uint32_t ctrl = sys_read32(base + CACHE_FUNC_CTRL_REG);

	ARG_UNUSED(op);

	ctrl &= ~ICACHE_CLEAN;
	sys_write32(ctrl, base + CACHE_FUNC_CTRL_REG);
	ctrl |= ICACHE_CLEAN;
	sys_write32(ctrl, base + CACHE_FUNC_CTRL_REG);

	return 0;
}

int cache_instr_range(void *addr, size_t size, int op)
{
	ARG_UNUSED(op);
	return 0;
}

#ifdef CONFIG_DCACHE_LINE_SIZE_DETECT
size_t cache_data_line_size_get(void)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));
	uint32_t ctrl = sys_read32(base + CACHE_FUNC_CTRL_REG);

	return (ctrl & CACHE_EANABLE) ? CACHE_LINE_SIZE : 0;
}
#endif /* CONFIG_DCACHE_LINE_SIZE_DETECT */

#ifdef CONFIG_ICACHE_LINE_SIZE_DETECT
size_t cache_instr_line_size_get(void)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));
	uint32_t ctrl = sys_read32(base + CACHE_FUNC_CTRL_REG);

	return (ctrl & CACHE_EANABLE) ? CACHE_LINE_SIZE : 0;
}
#endif /* CONFIG_ICACHE_LINE_SIZE_DETECT */
