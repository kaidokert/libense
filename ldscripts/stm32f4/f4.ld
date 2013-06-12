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

SECTIONS
{
	.text : AT(ORIGIN(FLASH)) {
		LONG(ABSOLUTE(..stack));
		KEEP(*(..isr_vectors))

		KEEP(*(..init.head))
		KEEP(*(.init));
		KEEP(*(..init.tail))

		KEEP(*(.fini))
		KEEP(*(..fini.tail))

		PROVIDE(_text_begin = .);
		*(.text .text.*)
		PROVIDE(_text_end = .);
	} >FLASH

	..stack = ORIGIN(CCM) + LENGTH(CCM);

	.rodata ALIGN(4) : AT(ALIGN(_text_end, 4)) {
		_ctors_array = .;
		KEEP(*(SORT(.ctors)));
		KEEP(*(SORT(.init_array)));
		_ctors_end = .;
		LONG(0);

		PROVIDE(_rodata_begin = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
	} >FLASH

	.ccmdata : AT(ALIGN(_rodata_end, 4)) {
		PROVIDE(_ccmdata_begin = .);
		*(.ccmdata .ccmdata.*)
		PROVIDE(_ccmdata_end = .);
	} >CCM
	_ccmdata_content_begin = LOADADDR(.ccmdata);
	_ccmdata_content_end = _ccmdata_content_begin + SIZEOF(.ccmdata);

	.data : AT(ALIGN(_ccmdata_content_end, 4)) {
		PROVIDE(_data_begin = .);
		*(.data .data.*)
		PROVIDE(_data_end = .);
	} >RAM
	_data_content_begin = LOADADDR(.data);
	_data_content_end = _data_content_begin + SIZEOF(.data);

	.bss : {
		PROVIDE(_bss_begin = .);
		*(.bss .bss.*)
		PROVIDE(_bss_end = .);
	} >RAM

}
INCLUDE "ldscripts/stm32f4/rcc.ld"
INCLUDE "ldscripts/debug.ld"
