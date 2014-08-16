SECTIONS
{
	.ivt : {
		LONG(ABSOLUTE(..stack));
		KEEP(*(..isr_vectors))
	} >R_TEXT

	.text : {
		KEEP(*(..init.head))
		KEEP(*(.init));
		KEEP(*(..init.tail))

		KEEP(*(.fini))
		KEEP(*(..fini.tail))

		PROVIDE(_text_begin = .);
		*(.text .text.*)
		PROVIDE(_text_end = .);
	} >R_TEXT

	.ARM.exidx : {
		*(.ARM.exidx .ARM.exidx.*)
	} >R_TEXT

	.rodata : {
		_ctors_array = .;
		KEEP(*(SORT(.ctors)));
		KEEP(*(SORT(.init_array)));
		LONG(0);

		*(.rodata .rodata.*)
	} >R_TEXT

	.data : ALIGN(4) {
		PROVIDE(_data_begin = .);
		*(.data .data.*)
	} >R_DATA AT>R_TEXT
	_data_content_begin = LOADADDR(.data);
	_data_content_end = _data_content_begin + SIZEOF(.data);

	.bss : {
		PROVIDE(_bss_begin = .);
		*(.bss .bss.*)
		PROVIDE(_bss_end = .);
	} >R_DATA
}
#include <debug.ld>
#include <scs.ld>
