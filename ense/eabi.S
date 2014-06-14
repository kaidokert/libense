.eabi_attribute Tag_DIV_use, 1

.section .text.__aeabi_memset, "ax"
.global __aeabi_memset
.type __aeabi_memset, %function
.thumb_func
__aeabi_memset:
	mov r3, r1
	mov r1, r2
	mov r2, r3
	b memset

.section .text.__aeabi_memclr, "ax"
.global __aeabi_memclr
.type __aeabi_memclr, %function
.thumb_func
__aeabi_memclr:
	movs r2, #0
	b __aeabi_memset
