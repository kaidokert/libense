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

	.data : AT(ALIGN(_rodata_end, 4)) {
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
INCLUDE "ldscripts/debug.ld"
INCLUDE "ldscripts/scs.ld"
