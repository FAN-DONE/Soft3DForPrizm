#ifndef FXCG_HARDWARE_H
#define FXCG_HARDWARE_H

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/heap.h>
#include <fxcg/file.h>
#include <fxcg/rtc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/misc.h>
#include <fxcg/system.h>

volatile static unsigned short* __VRAM;

volatile static unsigned int __VRAM_CLEAR_COLOR;

#define LCD_GRAM 0x202
#define LCD_BASE 0xB4000000
#define SYNCO() __asm__ volatile("SYNCO\n\t":::"memory");
#define MSTPCR0_2	(volatile unsigned*)0xA4150030
#define DMA_DMAOR	(volatile unsigned short*)0xFE008060
#define DMA_SAR_0	(volatile unsigned*)0xFE008020
#define DMA_DAR_0	(volatile unsigned*)0xFE008024
#define DMA_TCR_0	(volatile unsigned*)0xFE008028
#define DMA_CHCR_0	(volatile unsigned*)0xFE00802C
#define DMA_SAR_1	(volatile unsigned*)0xFE008030
#define DMA_DAR_1	(volatile unsigned*)0xFE008034
#define DMA_TCR_1	(volatile unsigned*)0xFE008038
#define DMA_CHCR_1	(volatile unsigned*)0xFE80003C
#define DMA_SAR_2	(volatile unsigned*)0xFE008040
#define DMA_DAR_2	(volatile unsigned*)0xFE008044
#define DMA_TCR_2	(volatile unsigned*)0xFE008048
#define DMA_CHCR_2	(volatile unsigned*)0xFE80004C
#define DMA_SAR_3	(volatile unsigned*)0xFE008050
#define DMA_DAR_3	(volatile unsigned*)0xFE008054
#define DMA_TCR_3	(volatile unsigned*)0xFE008058
#define DMA_CHCR_3	(volatile unsigned*)0xFE80005C



void HardwareInit(unsigned short VRAM_CLEAR_COLOR_FIX){
	 __VRAM=(unsigned short*)GetVRAMAddress();
	Bdisp_EnableColor(1);
	EnableStatusArea(3);
	__VRAM_CLEAR_COLOR = (unsigned)((VRAM_CLEAR_COLOR_FIX << 16) | VRAM_CLEAR_COLOR_FIX);
	*MSTPCR0_2 &= ~(1 << 21);
	*((volatile unsigned*)0xE5200FE0)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FE4)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FE8)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FEC)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FF0)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FF4)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FF8)=__VRAM_CLEAR_COLOR;
	*((volatile unsigned*)0xE5200FFC)=__VRAM_CLEAR_COLOR;
	//TODO
}

//====================================================
//======================显示==========================
//====================================================
static inline void PutDisplayWait(void) {
	while (1) {
		if ((*DMA_DMAOR) & 4)//发生地址错误
			break;
		if ((*DMA_CHCR_0) & 2)//发生NMI中断
			break;
	}
	SYNCO();
	*DMA_CHCR_0 &= ~1;//清除传输结束flag
	*DMA_DMAOR = 0;//清除flag并禁用DMA所有通道
}

static inline void PutDisplay(void) {
	Bdisp_WriteDDRegister3_bit7(1);
	Bdisp_DefineDMARange(6, 389, 0, 215);
	Bdisp_DDRegisterSelect(LCD_GRAM);
	*DMA_CHCR_0 &= ~1;
	*DMA_DMAOR = 0;
	*DMA_SAR_0 = ((unsigned) __VRAM) & 0x1FFFFFFF;
	*DMA_DAR_0 = LCD_BASE & 0x1FFFFFFF;
	*DMA_TCR_0 = 5184;
	*DMA_CHCR_0 = 0x00101401;
	*DMA_DMAOR |= 1;
	*DMA_DMAOR &= ~6;
}

static inline void PutDisplayEX(unsigned y0, unsigned y1) {
	Bdisp_WriteDDRegister3_bit7(1);
	Bdisp_DefineDMARange(6, 389, y0, y1);
	Bdisp_DDRegisterSelect(LCD_GRAM);
	*DMA_CHCR_0 &= ~1;//清除传输完成flag
	*DMA_DMAOR = 0;//清除flag并禁用DMA所有通道
	*DMA_SAR_0 = ((unsigned) __VRAM + (y0 * 384 * 2)) & 0x1FFFFFFF;
	*DMA_DAR_0 = LCD_BASE & 0x1FFFFFFF;
	*DMA_TCR_0 = 384 * (y1 - y0 + 1) / 16;
	*DMA_CHCR_0 = 0x00101401;
	*DMA_DMAOR |= 1;//开启所有通道
	*DMA_DMAOR &= ~6;//清除错误标志
}

static inline void VRAMClear_HW_FIX(void) {
	*DMA_CHCR_0&=~1;
	*DMA_DMAOR=0;
	*DMA_SAR_0=0xE5200FE0;
	*DMA_DAR_0= ((unsigned) __VRAM) & 0x1FFFFFFF;
	*DMA_TCR_0=5184;
	*DMA_CHCR_0=0x00104401;//32字节单位传输
	*DMA_DMAOR |= 1;
	*DMA_DMAOR &= ~6;
		while (1) {
		if ((*DMA_DMAOR) & 4)
			break;
		if ((*DMA_CHCR_0) & 2)
			break;
	}
	SYNCO();
	*DMA_CHCR_0 &= ~1;
	*DMA_DMAOR = 0;
}

static inline void VRAMClear_HW(unsigned short color) {
	unsigned colorr = (unsigned)((color << 16) | color);
	*((volatile unsigned*)0xE5200FC0)=colorr;
	*((volatile unsigned*)0xE5200FC4)=colorr;
	*((volatile unsigned*)0xE5200FC8)=colorr;
	*((volatile unsigned*)0xE5200FDC)=colorr;
	*((volatile unsigned*)0xE5200FD0)=colorr;
	*((volatile unsigned*)0xE5200FD4)=colorr;
	*((volatile unsigned*)0xE5200FD8)=colorr;
	*((volatile unsigned*)0xE5200FDC)=colorr;
	*DMA_CHCR_0&=~1;
	*DMA_DMAOR=0;
	*DMA_SAR_0=0xE5200FC0;
	*DMA_DAR_0= ((unsigned) __VRAM) & 0x1FFFFFFF;
	*DMA_TCR_0=5184;
	*DMA_CHCR_0=0x00104401;//32字节单位传输
	*DMA_DMAOR |= 1;
	*DMA_DMAOR &= ~6;
		while (1) {
		if ((*DMA_DMAOR) & 4)
			break;
		if ((*DMA_CHCR_0) & 2)
			break;
	}
	SYNCO();
	*DMA_CHCR_0 &= ~1;
	*DMA_DMAOR = 0;
}

static inline void Memset_HW_32(void*dest,int c,unsigned n) {
	*((volatile unsigned*)0xE5200FC0)=c;
	*((volatile unsigned*)0xE5200FC4)=c;
	*((volatile unsigned*)0xE5200FC8)=c;
	*((volatile unsigned*)0xE5200FDC)=c;
	*((volatile unsigned*)0xE5200FD0)=c;
	*((volatile unsigned*)0xE5200FD4)=c;
	*((volatile unsigned*)0xE5200FD8)=c;
	*((volatile unsigned*)0xE5200FDC)=c;
	*DMA_CHCR_0&=~1;
	*DMA_DMAOR=0;
	*DMA_SAR_0=0xE5200FC0;
	*DMA_DAR_0= ((unsigned)dest) & 0x1FFFFFFF;
	*DMA_TCR_0=n;
	*DMA_CHCR_0=0x00104401;
	*DMA_DMAOR |= 1;
	*DMA_DMAOR &= ~6;
		while (1) {
		if ((*DMA_DMAOR) & 4)
			break;
		if ((*DMA_CHCR_0) & 2)
			break;
	}
	SYNCO();
	*DMA_CHCR_0 &= ~1;
	*DMA_DMAOR = 0;
}

static inline void VRAMClear(unsigned short color) {
	unsigned int* VRAM32 = (unsigned int*) __VRAM;
	unsigned colorr = (unsigned)((color << 16) | color);
	for (unsigned int i = 0; i <= LCD_WIDTH_PX * LCD_HEIGHT_PX / 2; i++)
		*(VRAM32++) = colorr;
}


//====================================================
//======================按键==========================
//====================================================
static inline int KeyDown(int basic_keycode) {
	const volatile unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
	int row, col, word, bit;
	row = basic_keycode % 10;
	col = basic_keycode / 10 - 1;
	word = row >> 1;
	bit = col + ((row & 1) << 3);
	return (0 != (keyboard_register[word] & 1 << bit));
}

#endif
