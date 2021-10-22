#include "2450addr.h"
#include "libc.h"

// Global Variables Declaration
// 목적지는 CACHE 영역이 아닐것
static	unsigned long src=0x33000000;
static	unsigned long dst=0x33100000;
static	unsigned int size = 12; /* word size */
static	unsigned long pattern;

//Function Declatation
void DMA0_SW_Init(void);
void DMA0_Timer_Init(void);
void DMA0_UART_Init(void);
void DMA0_SW_Start(void);
void DMA0_HW_Start(void);


void DMA0_SW_Init(void)
{


	rDMASKTRIG0 = 0x0;
	rDISRC0 =src;
	rDIDST0 =dst;
	rDISRCC0 &= ~0x3;
	rDIDSTC0 &=~(0x7);
	rDIDSTC0 |= 0x6;
	rDCON0 =(unsigned int)(0<<31)+(1<<30)+(1<<29)+(0<<28)+(1<<27)+(1<<24)+(0<<23)+(0<<22)+(0<<20)+size*4;
	rDMAREQSEL0	= (0x0<<0);

}
void DMA0_Timer_Init(void)
{
	rDMASKTRIG0 = 0x0;
	rDISRC0 =src;
	rDIDST0 =dst;
	rDISRCC0 &= ~0x3;
	rDIDSTC0 &=~(0x7);
	rDCON0 =(unsigned int)(0<<31)+(1<<30)+(1<<29)+(0<<28)+(1<<27)+(3<<24)+
			(1<<23)+(0<<22)+(0<<20)+size*4;
	rDMAREQSEL0	= (9<<1)+(0x1<<0);
	rDMASKTRIG0 |= (0x1<<1);
}
void DMA0_UART_Init(void)
{
	rDMASKTRIG0 = 0x0;
	rDISRC0 =src;
	rDIDST0 =dst;
	rDISRCC0 &= ~0x3;
	rDIDSTC0 &=~(0x7);
	rDCON0 =(unsigned int)(0<<31)+(1<<30)+(1<<29)+(0<<28)+(1<<27)+(1<<24)+(1<<23)+(0<<22)+(0<<20)+size*4;
	rDMAREQSEL0	= (21<<1)+(0x1<<0);
	rDMASKTRIG0 |= (0x1<<1);
}
void DMA0_SW_Start(void)
{
	MemFill(src, 0x00000000, size);
	MemFill(dst, 0x00000000, size);	
	Uart_Printf("\nStart Memory copy test With DMA\n");	

	pattern = 0x5555aaaa;
	Uart_Printf("Fill pattern [0x%08x] to [0x%08x]\n",pattern, src);
	MemFill(src, pattern, size);
	MemDump(src, size);
	Uart_Printf("\nCopy from [0x%08x] to [0x%08x] by DMA\n",src, dst);

	rDMASKTRIG0 |= (0x1<<1);	

	rDMASKTRIG0 |= (0x1);
}

void DMA0_HW_Start(void)
{	
	MemFill(src, 0x00000000, size);
	MemFill(dst, 0x00000000, size);	
	Uart_Printf("\nStart Memory copy test With DMA\n");	
	
	pattern = 0x5555aaaa;
	Uart_Printf("Fill pattern [0x%08x] to [0x%08x]\n",pattern, src);
	MemFill(src, pattern, size);
	MemDump(src, size);
	Uart_Printf("\nCopy from [0x%08x] to [0x%08x] by DMA\n",src, dst);
}