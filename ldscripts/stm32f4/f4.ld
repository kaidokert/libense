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

	.ccmdata : AT(ALIGN(_rodata_end, 4)) {
		PROVIDE(_ccmdata_begin = .);
		*(.ccmdata .ccmdata.*)
		PROVIDE(_ccmdata_end = .);

		_stack = ORIGIN(CCM) + LENGTH(CCM);
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

	/* Stabs debugging sections.  */
	.stab            0 : { *(.stab) }
	.stabstr         0 : { *(.stabstr) }
	.stab.excl       0 : { *(.stab.excl) }
	.stab.exclstr    0 : { *(.stab.exclstr) }
	.stab.index      0 : { *(.stab.index) }
	.stab.indexstr   0 : { *(.stab.indexstr) }
	.comment         0 : { *(.comment) }
	/* DWARF debug sections.
	Symbols in the DWARF debugging sections are relative to the beginning
	of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug           0 : { *(.debug) }
	.line            0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo   0 : { *(.debug_srcinfo) }
	.debug_sfnames   0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges   0 : { *(.debug_aranges) }
	.debug_pubnames  0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info      0 : { *(.debug_info) }
	.debug_abbrev    0 : { *(.debug_abbrev) }
	.debug_line      0 : { *(.debug_line) }
	.debug_frame     0 : { *(.debug_frame) }
	.debug_str       0 : { *(.debug_str) }
	.debug_loc       0 : { *(.debug_loc) }
	.debug_macinfo   0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	/* DWARF 3 */
	.debug_pubtypes  0 : { *(.debug_pubtypes) }
	.debug_ranges    0 : { *(.debug_ranges) }
	/* DWARF Extension.  */
	.debug_macro     0 : { *(.debug_macro) }
}
