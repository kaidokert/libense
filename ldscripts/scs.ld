SECTIONS
{
	.scs 0xE000E000 : {
		PROVIDE(.SCS_ICTR = . + 0x004);
		PROVIDE(.SCS_ACTLR = . + 0x008);
		PROVIDE(.SCS_STIR = . + 0xF00);

		PROVIDE(.SCS_CPUID = . + 0xD00);
		PROVIDE(.SCS_ICSR = . + 0xD04);
		PROVIDE(.SCS_VTOR = . + 0xD08);
		PROVIDE(.SCS_AIRCR = . + 0xD0C);
		PROVIDE(.SCS_SCR = . + 0xD10);
		PROVIDE(.SCS_CCR = . + 0xD14);
		PROVIDE(.SCS_SHP = . + 0xD18);
		PROVIDE(.SCS_SHCSR = . + 0xD24);
		PROVIDE(.SCS_CFSR = . + 0xD28);
		PROVIDE(.SCS_MMFSR = . + 0xD28);
		PROVIDE(.SCS_BFSR = . + 0xD29);
		PROVIDE(.SCS_UFSR = . + 0xD2A);
		PROVIDE(.SCS_HFSR = . + 0xD2C);
		/* TODO: DFSR */
		PROVIDE(.SCS_MMFAR = . + 0xD34);
		PROVIDE(.SCS_BFAR = . + 0xD38);
		PROVIDE(.SCS_AFSR = . + 0xD3C);
		PROVIDE(.SCS_CPACR = . + 0xD88);
		PROVIDE(.SCS_FPCCR = . + 0xF34);
		PROVIDE(.SCS_FPCAR = . + 0xF38);
		PROVIDE(.SCS_FPDSCR = . + 0xF3C);
		/* instance needs location */
		PROVIDE(.SCS_FPSCR = . + 0xF3C);
		/* TODO: MVFR0 */
		/* TODO: MVFR1 */
		/* TODO: PID0-7 */
		/* TODO: CID0-3 */

		PROVIDE(.SCS_Systick = . + 0x010);

		PROVIDE(.SCS_NVIC = . + 0x100);

		/* TODO: MPU */
	}
}
