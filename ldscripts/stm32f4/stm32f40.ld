OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_init_head)


MEMORY
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 0x00100000
	RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00020000
	CCM   (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00010000
}

#define R_TEXT RAM
#define R_DATA RAM

#include <generic.ld>
SECTIONS
{
	..stack = ORIGIN(CCM) + LENGTH(CCM);
	..stack_end = ..stack - 0x200;

	.rodata : {
		. = ALIGN(4);
		_memalloc_regions = .;
		..region.RAM.begin = ABSOLUTE(ALIGN(ADDR(.bss) + SIZEOF(.bss), 4));
		..region.RAM.end = ABSOLUTE(ORIGIN(RAM) + LENGTH(RAM));
			LONG(ABSOLUTE(..region.RAM.begin));
			LONG(ABSOLUTE(..region.RAM.end - ..region.RAM.begin));
		..region.CCM.begin = ABSOLUTE(ALIGN(ADDR(.ccmdata) + SIZEOF(.ccmdata), 4));
		..region.CCM.end = ABSOLUTE(..stack_end);
			LONG(ABSOLUTE(..region.CCM.begin));
			LONG(ABSOLUTE(..region.CCM.end - ..region.CCM.begin));
	} >R_TEXT

	.ccmdata : {
		PROVIDE(_ccmdata_begin = .);
		*(.ccmdata .ccmdata.*)
	} >CCM
	_ccmdata_content_begin = LOADADDR(.ccmdata);
	_ccmdata_content_end = _ccmdata_content_begin + SIZEOF(.ccmdata);

}
#include "rcc.ld"
#include "gpio.ld"
#include "syscfg.ld"
#include "usart.ld"
#include "dma.ld"
#include "exti.ld"
