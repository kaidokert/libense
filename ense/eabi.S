.eabi_attribute Tag_DIV_use, 1

.section .text.__aeabi_memclr, "ax"
.global __aeabi_memclr
.type __aeabi_memclr, %function
.thumb_func
__aeabi_memclr:
	movs r2, #0
	b __aeabi_memset
