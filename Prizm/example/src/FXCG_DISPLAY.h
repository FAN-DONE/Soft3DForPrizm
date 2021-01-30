#ifndef FXCG_DISPLAY_H
#define FXCG_DISPLAY_H

#include <fxcg/display.h>

unsigned short* VRAM_base;//от╢Ф

#define LCD_GRAM 0x202
#define LCD_BASE 0xB4000000
#define SYNCO() __asm__ volatile("SYNCO\n\t":::"memory");
#define MSTPCR0_2	(volatile unsigned*)0xA4150030
#define DMA0_DMAOR	(volatile unsigned short*)0xFE008060
#define DMA0_SAR_0	(volatile unsigned*)0xFE008020
#define DMA0_DAR_0	(volatile unsigned*)0xFE008024
#define DMA0_TCR_0	(volatile unsigned*)0xFE008028
#define DMA0_CHCR_0	(volatile unsigned*)0xFE00802C

static inline void PutDisplayWait(void);
static inline void PutDisplay(void);
static inline void PutDisplayEX(unsigned y0, unsigned y1);

static inline void PutDisplayWait(void) {
	while (1) {
		if ((*DMA0_DMAOR) & 4)
			break;
		if ((*DMA0_CHCR_0) & 2)
			break;
	}
	SYNCO();
	*DMA0_CHCR_0 &= ~1;
	*DMA0_DMAOR = 0;
}

static inline void PutDisplay(void) {
	Bdisp_WriteDDRegister3_bit7(1);
	Bdisp_DefineDMARange(6, 389, 0, 215);
	Bdisp_DDRegisterSelect(LCD_GRAM);
	*MSTPCR0_2 &= ~(1 << 21);
	*DMA0_CHCR_0 &= ~1;
	*DMA0_DMAOR = 0;
	*DMA0_SAR_0 = ((unsigned)VRAM_base) & 0x1FFFFFFF;
	*DMA0_DAR_0 = LCD_BASE & 0x1FFFFFFF;
	*DMA0_TCR_0 = 5184;
	*DMA0_CHCR_0 = 0x00101401;
	*DMA0_DMAOR |= 1;
	*DMA0_DMAOR &= ~6;
}

static inline void PutDisplayEX(unsigned y0, unsigned y1) {
	Bdisp_WriteDDRegister3_bit7(1);
	Bdisp_DefineDMARange(6, 389, y0, y1);
	Bdisp_DDRegisterSelect(LCD_GRAM);
	*MSTPCR0_2 &= ~(1 << 21);
	*DMA0_CHCR_0 &= ~1;
	*DMA0_DMAOR = 0;
	*DMA0_SAR_0 = ((unsigned)VRAM_base + (y0 * 384 * 2)) & 0x1FFFFFFF;
	*DMA0_DAR_0 = LCD_BASE & 0x1FFFFFFF;
	*DMA0_TCR_0 = 384 * (y1 - y0 + 1) / 16;
	*DMA0_CHCR_0 = 0x00101401;
	*DMA0_DMAOR |= 1;
	*DMA0_DMAOR &= ~6;
}

#endif
