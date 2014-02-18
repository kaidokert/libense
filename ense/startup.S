.eabi_attribute Tag_DIV_use, 2

.section ..init.head, "ax"

.global _start
.type _start, %function
.thumb_func
_start:

.global _init_head
.type _init_head, %function
.thumb_func
_init_head:
	bl _pre_init


.section ..init.tail, "ax"

.global _init_tail
.type _init_tail, %function
.thumb_func
_init_tail:
	bl _post_init


.section ..fini.tail, "ax"

.global _fini_tail
.type _fini_tail, %function
.thumb_func
_fini_tail:
	bl _post_fini
..loop:
	b ..loop
