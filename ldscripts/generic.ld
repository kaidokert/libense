SECTIONS
{
	.text : {
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
	} >R_TEXT AT>R_TEXT

	.rodata : {
		_ctors_array = .;
		KEEP(*(SORT(.ctors)));
		KEEP(*(SORT(.init_array)));
		_ctors_end = .;
		LONG(0);

		PROVIDE(_rodata_begin = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
	} >R_TEXT

	.data : ALIGN(4) {
		PROVIDE(_data_begin = .);
		*(.data .data.*)
		PROVIDE(_data_end = .);
	} >R_DATA AT>R_TEXT
	_data_content_begin = LOADADDR(.data);
	_data_content_end = _data_content_begin + SIZEOF(.data);

	.bss : {
		PROVIDE(_bss_begin = .);
		*(.bss .bss.*)
		PROVIDE(_bss_end = .);
	} >R_DATA
}
INCLUDE "ldscripts/debug.ld"
INCLUDE "ldscripts/scs.ld"
