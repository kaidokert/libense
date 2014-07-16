OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)


MEMORY
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 0x00100000
	RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00020000
	CCM   (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00010000
}

REGION_ALIAS("R_TEXT", RAM)
REGION_ALIAS("R_DATA", RAM)

INCLUDE "ldscripts/generic.ld"
SECTIONS
{
	..stack = ORIGIN(CCM) + LENGTH(CCM);
	..stack_end = ..stack - 0x200;

	.text : {
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
	} >R_TEXT AT>R_TEXT

	.ccmdata : {
		PROVIDE(_ccmdata_begin = .);
		*(.ccmdata .ccmdata.*)
	} >CCM
	_ccmdata_content_begin = LOADADDR(.ccmdata);
	_ccmdata_content_end = _ccmdata_content_begin + SIZEOF(.ccmdata);

}
INCLUDE "ldscripts/stm32f4/rcc.ld"
INCLUDE "ldscripts/stm32f4/gpio.ld"
INCLUDE "ldscripts/stm32f4/syscfg.ld"
INCLUDE "ldscripts/stm32f4/usart.ld"
INCLUDE "ldscripts/stm32f4/dma.ld"
INCLUDE "ldscripts/stm32f4/exti.ld"
