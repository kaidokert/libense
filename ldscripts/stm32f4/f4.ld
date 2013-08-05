OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)


MEMORY
{
	INIT  (r)   : ORIGIN = 0x00000000, LENGTH = 0x08000000
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 0x00100000
	CCM   (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00010000
	RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00020000
}

INCLUDE "ldscripts/generic.ld"
SECTIONS
{
	..stack = ORIGIN(CCM) + LENGTH(CCM);

	.ccmdata : AT(ALIGN(_data_content_end, 4)+4) {
		PROVIDE(_ccmdata_begin = .);
		*(.ccmdata .ccmdata.*)
		PROVIDE(_ccmdata_end = .);
	} >CCM
	_ccmdata_content_begin = LOADADDR(.ccmdata);
	_ccmdata_content_end = _ccmdata_content_begin + SIZEOF(.ccmdata);

}
INCLUDE "ldscripts/stm32f4/rcc.ld"
INCLUDE "ldscripts/stm32f4/gpio.ld"
INCLUDE "ldscripts/stm32f4/usart.ld"
