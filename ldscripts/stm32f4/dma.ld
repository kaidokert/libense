SECTIONS
{
	.dma 0x40026000 : {
		PROVIDE(.DMA_DMA1 = . + 0x0);
		PROVIDE(.DMA_DMA2 = . + 0x400);
	}
}
