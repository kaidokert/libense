SECTIONS
{
	.usart 0x40000000 : {
		PROVIDE(.USART_USART1 = . + 0x11000);
		PROVIDE(.USART_USART2 = . + 0x04400);
		PROVIDE(.USART_USART3 = . + 0x04800);
		PROVIDE(.USART_UART4 = . + 0x04C00);
		PROVIDE(.USART_UART5 = . + 0x05000);
		PROVIDE(.USART_USART6 = . + 0x11400);
		PROVIDE(.USART_UART7 = . + 0x07800);
		PROVIDE(.USART_UART8 = . + 0x07C00);
	}
}
