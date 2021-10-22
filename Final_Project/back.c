/*
 * =====================================================================
 * NAME         : Main.c
 *
 * Descriptions : Main routine for S3C2450
 *
 * IDE          : GCC-4.1.0
 *
 * Modification
 *	  
 * =====================================================================
 */
#include "2450addr.h"
#include "libc.h"
#include "option.h"
#include "kingw.h"
#include "kingb.h"
#include "queenb.h"
#include "queenw.h"
#include "bishopb.h"
#include "bishopw.h"
#include "knightb.h"
#include "knightw.h"
#include "rookb.h"
#include "rookw.h"
#include "pawnb.h"
#include "pawnw.h"
#include "initimg.h"
#include "check.h"
#define EXAMPLE 440
#define BLACK	0x0000
#define WHITE	0xFFFF
#define BLUE	0x001F
#define GREEN	0x03E0
#define RED		0x7C00
#define BROWN	0x7CA0
/*	
 * 420 : SW_DMA (½Ç½À 8-1 : SW DMA Trigger Test )
 *
 * 421 : UART_DMA (½Ç½À 8-2 : UART1¿¡ ÀÇÇÑ DMA trigger Test)
 *	
 * 422 : Timer_DMA (½Ç½À 8-3 : Timer¿¡ ÀÇÇÑ DMA trigger Test)
 */
 
 /***************************************
 * 
 * Title: SW_DMA
 * 
 ***************************************/
extern void DMA0_SW_Init(void);
extern void DMA0_Timer_Init(void);
extern void DMA0_UART_Init(void);
extern void DMA0_SW_Start(void);
extern void DMA0_HW_Start(void);
extern void Lcd_Port_Init(void);
extern void NonPal_Lcd_Init(void);
extern void NonPal_Put_Pixel(int x, int y, int color);
extern void Timer_Delay(int msec);


#if EXAMPLE == 440

//** ISR Function declaration 
static void DMA0_ISR(void) __attribute__ ((interrupt ("IRQ")));;
//**ISR Declaration
static void Touch_ISR(void) __attribute__ ((interrupt ("IRQ")));
// ISR Declaration
static void Timer0_ISR(void) __attribute__ ((interrupt ("IRQ")));


// Global Variables Declaration
static	unsigned long src=0x33000000;
static	unsigned long dst=0x33100000;
static	unsigned int  size = 12; /* word size */
static	unsigned long pattern;
static	int timer_old1=0,timer_old2=0;

// Prevent_Cache_Data
volatile  int ADC_x, ADC_y;
volatile  int Touch_Pressed=0, Touch_division=1;
volatile  int time_count=900;
const int lcdXc=240, lcdYc=136;
int boardArr[8][8] ={
	{20,60,100,140,180,220,260,300},
	{16,46,76,106,136,166,196,226}
};

#define LCD_XSIZE 		(480)	
#define LCD_YSIZE 		(272)
// Calibration 정보 저장 값
volatile int Cal_x1=880;
volatile int Cal_y1=656;
volatile int Cal_x2=145;
volatile int Cal_y2=320; 

// 좌표 변환 값
volatile int Touch_x, Touch_y;
// Calibration 완료 값
volatile unsigned int Touch_config=1;

int mapArr[8][8] = {
   {-4,-2,-3,-5,-6,-3,-2,-4},
   {-1,-1,-1,-1,-1,-1,-1,-1},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0},
   { 1, 1, 1, 1, 1, 1, 1, 1},
   { 4, 2, 3, 5, 6, 3, 2, 4}
};

int checked=0;

int X_Y[8][8][2]={
	{{20,16} ,{60,16} ,{100,16} ,{140,16} ,{180,16} ,{220,16} ,{260,16} ,{300,16} },
	{{20,46} ,{60,46} ,{100,46} ,{140,46} ,{180,46} ,{220,46} ,{260,46} ,{300,46} },
	{{20,76} ,{60,76} ,{100,76} ,{140,76} ,{180,76} ,{220,76} ,{260,76} ,{300,76} },
	{{20,106},{60,106},{100,106},{140,106},{180,106},{220,106},{260,106},{300,106}},
	{{20,136},{60,136},{100,136},{140,136},{180,136},{220,136},{260,136},{300,136}},
	{{20,166},{60,166},{100,166},{140,166},{180,166},{220,166},{260,166},{300,166}},
	{{20,196},{60,196},{100,196},{140,196},{180,196},{220,196},{260,196},{300,196}},
	{{20,226},{60,226},{100,226},{140,226},{180,226},{220,226},{260,226},{300,226}}
};

static void Timer0_ISR()
{
	/* ÇØ´ç ÀÎÅÍ·´Æ® Masking */ 

	rINTMSK1 |= (1<<10);	
	time_count -=1;
	/* TODO : Pending Register Clear */
	rSRCPND1 |= (0xffffffff);
	rINTPND1 |= (0xffffffff);
	//rINTOFFSET1 = 0x400; This register has been already setup in C function by asm
						//So, you don't care about this

	Uart_Send_String("Timer ISR\n");
	
	/* ÇØ´ç ÀÎÅÍ·´Æ® UnMasking */
	rINTMSK1 &= ~(1<<10);			//prevent ReAsk same INTERRUPT
}

static void DMA0_ISR(void)
{
	/* ÀÎÅÍ·´Æ® Çã¿ëÇÏÁö ¾ÊÀ½ on DMA0 */
	rINTSUBMSK |= (0x1<<18); 
	rINTMSK1 |= (0x1<<17);

	Uart_Printf("__irq ISR½ÇÇà°á°ú  ");	
	MemDump(dst, 12); 	

	/*  TO DO: Stop DMA0 trigger  */
	rDMASKTRIG0 |= 0x1<<2;
		

	/* Masking Disable on DMA0 */
	rINTSUBMSK &= ~(0x1<<18);
	rINTMSK1 &= ~(0x1<<17);

}

static void Touch_ISR()
{
	/* ÀÎÅÍ·´Æ® Çã¿ëÇÏÁö ¾ÊÀ½ on Touch */
	rINTSUBMSK |= (0x1<<9);
	rINTMSK1 |= (0x1<<31);	
	
	/* TO DO: Pendng Clear on Touch */	
	rSUBSRCPND |= (0x1<<9);
	rSRCPND1 |= (0x1<<31);
	rINTPND1 |= (0x1<<31);

	if(rADCTSC & 0x100)
	{
		rADCTSC &= (0xff); 
		Touch_Pressed = 0;
		Touch_division = 1;
		Uart_Send_String("Detect Stylus Up Interrupt Signal \n");
	}
	
	else
	{
		Uart_Send_String(" ISR starting...  \n");
		
		rADCTSC &=~(0x1<<8);				//detect stylus down
		rADCTSC |= (0x1<<7);				//set bit for x position measurement
		rADCTSC |= (0x1<<6);				//YM=Hi-z, YP=Hi-z
		rADCTSC &=~(0x1<<5);		
		rADCTSC |= (0x1<<4);				//XM=VSS,XP=VDD
		rADCTSC |= (0x1<<3);				//pull-up EN
		rADCTSC |= (0x1<<2);				//Normal-ADC


		/* TO DO : ENABLE_START */		
		rADCCON |= (0x1);
		while(rADCCON & 0x1);

		/* wait until End of A/D Conversion */
		while(!(rADCCON & (1<<15)));

		while(!(0x8000&rADCCON));
		ADC_x=(int)(0x3ff&rADCDAT0);
		ADC_y=(int)(0x3ff&rADCDAT1);
		// Touch calibration complete
		if(Touch_config)// default 1
		{
			Touch_y=(ADC_y-Cal_y1)*(LCD_YSIZE-10)/(Cal_y2-Cal_y1)+5;
			Touch_x=(ADC_x-Cal_x2)*(LCD_XSIZE-10)/(Cal_x1-Cal_x2)+5;
			Touch_x=LCD_XSIZE-Touch_x;
			if(Touch_x<0) Touch_x=0;
			if(Touch_x>=LCD_XSIZE) Touch_x=LCD_XSIZE-1;
			if(Touch_y<0) Touch_y=0;
			if(Touch_y>=LCD_YSIZE) Touch_y=LCD_YSIZE-1;
			if((Touch_x<346)&&(Touch_y>256)) Touch_y=255;
		}
		// before calibration		
		else
		{
			Touch_x = ADC_x;
			Touch_y = ADC_y;
		}

		
		Touch_Pressed = 1;
		Touch_division = 0;
		
		/* TO DO : change to Waiting for interrupt mode 
		 *		   Stylus Up, YM_out Enable, YP_out Disable, XM_out Disable, XP_out disable
		 */
		rADCTSC |= (0x1<<8);
		rADCTSC |= (0x1<<7);
		rADCTSC |= (0x1<<6);
		rADCTSC &= ~(0x1<<5);
		rADCTSC |= (0x1<<4);
		rADCTSC &= ~(0x1<<3);
		rADCTSC &= ~(0x1<<2);
		rADCTSC |= (0x1<<1);
		rADCTSC |= (0x1);
			 
	}
	
	/* ÀÎÅÍ·´Æ® ´Ù½Ã Çã¿ë  on Touch */
	rINTSUBMSK &= ~(0x1<<9);
	rINTMSK1 &= ~(0x1<<31);
}

void display_piece(int piece_num,int x, int y)/////////////////////////////////////////////////////////////////////
{
	switch(piece_num)
	{
			case -1:
				Lcd_Draw_BMP(x, y, pawnb);
				break;
			case -2:
				Lcd_Draw_BMP(x, y, knightb);
				break;
			case -3:
				Lcd_Draw_BMP(x, y, bishopb);
				break;
			case -4:
				Lcd_Draw_BMP(x, y, rookb);
				break;
			case -5:				
				Lcd_Draw_BMP(x, y, queenb);
				break;
			case -6:		
				Lcd_Draw_BMP(x, y, kingb);
				break;
			case  1:
				Lcd_Draw_BMP(x, y, pawnw);
				break;
			case  2:				
				Lcd_Draw_BMP(x, y, knightw);
				break;
			case  3:				
				Lcd_Draw_BMP(x, y, bishopw);
				break;
			case  4:				
				Lcd_Draw_BMP(x, y, pawnb);
				break;
				return pawnb;
			case  5:				
				Lcd_Draw_BMP(x, y, queenw);
				break;
			case  6:				
				Lcd_Draw_BMP(x, y, kingw);
				break;
	}
}



void Main(void)
{
	int i,j;
	int cXpos=0, cYpos=0;
	int cnt,cnt2,m;
	int z,x;
	int color=0;
	int area[8][2]={{20,16},{60,46},{20,76},{60,106},{20,136},{60,166},{20,196},{60,226}};
	int turn1 =1,turn2=1;
	int time1 =900,time2=900;
	int sort_x, sort_y;



	pISR_DMA0 = (unsigned int)DMA0_ISR;
	pISR_ADC = (unsigned int)Touch_ISR;
	pISR_TIMER0 = (unsigned int)Timer0_ISR;


	Uart_Init(115200);
	DMA0_SW_Init();
	Touch_Init();	
	Lcd_Port_Init();
	NonPal_Lcd_Init();
	Timer_Init();

	/* TODO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ DMA0_ISR ÇÔ¼ö µî·Ï */


	/*  ÀÎÅÍ·´Æ® Çã¿ë on DMA0 */
	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");

	rINTSUBMSK &= ~(0x1<<18);	
	rINTMSK1 &= ~(0x1<<17);

	DMA0_SW_Start();
	rINTSUBMSK &= ~(0x1<<9);
	rINTMSK1 &= ~(0x1<<31);	


	//Timer Unmask
	rINTMSK1 &= ~(1<<10);
	
	Timer_Delay(1000);

	Lcd_Draw_BMP(0, 0, initimg);

	while(1){
		if((((Touch_x-20)/40)>=2) && (((Touch_x-20)/40)<=6)&& (((Touch_y-17)/30) >=3 ) && (((Touch_y-17)/30)<=6)){
			break;
		}
	}

	//Draw loop for boardimg version
	//Lcd_Draw_BMP(0, 0, board); 

	//Draw loop for Pixel version
	
	for(j=0; j<272; j++)
	{
		for(i=0; i<480; i++)
		{
			NonPal_Put_Pixel(i,j,WHITE);
		}
	}
	for(j=10; j<=260; j++)
	{		
		for(i=14; i<=346; i++)
			NonPal_Put_Pixel(i,j,BROWN);
	}
	for(cnt=0;cnt<8;cnt++)
	{
		cXpos=area[cnt][0];
		for(m=0;m<4;m++)
		{
			cYpos=area[cnt][1];
			cXpos=area[cnt][0]+(80*m);
			for(j=cYpos; j<cYpos+30;j++)
			{
				for(i=cXpos; i<cXpos+40; i++)
				{
					NonPal_Put_Pixel(i,j,WHITE);			
				}
			}
		}
	}
	

///////////////////////////////PLUS///////////////////////////
Lcd_Draw_BMP(boardArr[0][0], boardArr[1][0], rookb);
Lcd_Draw_BMP(boardArr[0][1], boardArr[1][0], knightb);
Lcd_Draw_BMP(boardArr[0][2], boardArr[1][0], bishopb);
Lcd_Draw_BMP(boardArr[0][3], boardArr[1][0], queenb);
Lcd_Draw_BMP(boardArr[0][4], boardArr[1][0], kingb);
Lcd_Draw_BMP(boardArr[0][5], boardArr[1][0], bishopb);
Lcd_Draw_BMP(boardArr[0][6], boardArr[1][0], knightb);
Lcd_Draw_BMP(boardArr[0][7], boardArr[1][0], rookb);

Lcd_Draw_BMP(boardArr[0][0], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][1], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][2], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][3], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][4], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][5], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][6], boardArr[1][1], pawnb);
Lcd_Draw_BMP(boardArr[0][7], boardArr[1][1], pawnb);

Lcd_Draw_BMP(boardArr[0][0], boardArr[1][7], rookw);
Lcd_Draw_BMP(boardArr[0][1], boardArr[1][7], knightw);
Lcd_Draw_BMP(boardArr[0][2], boardArr[1][7], bishopw);
Lcd_Draw_BMP(boardArr[0][3], boardArr[1][7], queenw);
Lcd_Draw_BMP(boardArr[0][4], boardArr[1][7], kingw);
Lcd_Draw_BMP(boardArr[0][5], boardArr[1][7], bishopw);
Lcd_Draw_BMP(boardArr[0][6], boardArr[1][7], knightw);
Lcd_Draw_BMP(boardArr[0][7], boardArr[1][7], rookw);

Lcd_Draw_BMP(boardArr[0][0], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][1], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][2], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][3], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][4], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][5], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][6], boardArr[1][6], pawnw);
Lcd_Draw_BMP(boardArr[0][7], boardArr[1][6], pawnw);
/////////////////////////////////////////////////////////////
	//Loop start
	Uart_Printf("*** Touch Test *** \n");
	
	while(1)
	{

		sort_x = (Touch_x-20)/40; 
		sort_y = (Touch_y-17)/30;

		if((turn1) && (!turn2)){
			if(timer_old2== time_count+1){
				timer_old1 = time_count;
				time1 = timer_old2 - time_count;
			}
				//Uart_Printf("*** Turn1 *** turn2 : %d sec save\n",time2);
		}
		else if((!turn1) && (turn2))
		{
			if(timer_old1 == time_count+1){
				timer_old2 = time_count;
				time2 = timer_old1 - time_count;
			}
			//Uart_Printf("*** Turn1 *** turn2 : %d sec save11\n",time2);
		}
		else if(turn1 && turn2) {
				time1-=1;
				//Uart_Printf("*** Turn1 *** turn2 : %d sec save22\n",time1);
		}

		if(Touch_Pressed)
		{
			while(Touch_division==0)
			{

				Uart_Printf("X : %d, Y: %d \n", (Touch_x-20)/40, (Touch_y-17)/30);
        // Uart_Printf("X : %d, Y: %d \n", ADC_x, ADC_y);
       	NonPal_Put_Pixel(Touch_x,Touch_y,BLACK);

				int x_buf1, y_buf1, x_buf2, y_buf2;
				int selected_piece;

       	if(!checked)//piece is not selected
       	{
       		x_buf1 = (Touch_x-20)/40;// ex) 0
       		y_buf1 = (Touch_y-17)/30;// ex) 1						
       		selected_piece=mapArr[y_buf1][x_buf1];
       		mapArr[y_buf1][x_buf1]=0;		 //there is nothing
      		if(selected_piece==0) continue;//if touched empty board
      		Lcd_Draw_BMP(X_Y[y_buf1][x_buf1][0], X_Y[y_buf1][x_buf1][1], check);
      		Timer_Delay(500);
       		checked=1;
       		//   ex)							20				,					46	
       	}
       	else//The piece is already selected
       	{
       		x_buf2 = (Touch_x-20)/40;// ex) 3
       		y_buf2 = (Touch_y-17)/30;// ex) 3	
       		for(j = X_Y[y_buf1][x_buf1][1]; j < X_Y[y_buf1][x_buf1][1]+30; j++)
					{
						for(i = X_Y[y_buf1][x_buf1][0]; i < X_Y[y_buf1][x_buf1][0]+40; i++)
							{
								if((y_buf1*1+x_buf1)%2){
									NonPal_Put_Pixel(i,j,BROWN);
								}
								else
									NonPal_Put_Pixel(i,j,WHITE);		
							}
					}
					mapArr[y_buf2][x_buf2] = selected_piece;
					display_piece(selected_piece,X_Y[y_buf2][x_buf2][0],X_Y[y_buf2][x_buf2][1]);
					Timer_Delay(500);
					checked=0; //The piece is moved
       	}
				Uart_Printf("Draw!");		
			}
		}
	}
	// if you want x,y position change (ADC_x*480/600)-200  /600,-200 change here












}

#endif

























#if EXAMPLE == 444

//** TO DO: ISR Function declaration 

// Global Variables Declaration
// ¸ñÀûÁö´Â CACHE ¿µ¿ªÀÌ ¾Æ´Ò°Í
static	unsigned long src=0x33000000;
static	unsigned long dst=0x33100000;
static	unsigned int  size = 12; /* word size */
static	unsigned long pattern;

static void DMA0_ISR(void)
{
	/* ÀÎÅÍ·´Æ® Çã¿ëÇÏÁö ¾ÊÀ½ on DMA0 */
	rINTSUBMSK |= (0x1<<18); 
	rINTMSK1 |= (0x1<<17);
	
	/* TO DO: Pendng Clear on DMA0 */
	rSUBSRCPND |= 0x1<<18;
	rSRCPND1 |= 0x1<<17;
	rINTPND1 |= 0x1<<17;
		
	Uart_Printf("__irq ISR½ÇÇà°á°ú  ");	
	MemDump(dst, 12); 	
	
	/*  TO DO: Stop DMA0 trigger  */
	rDMASKTRIG0 |= 0x1<<2;
		
	/* Masking Disable on DMA0 */
	rINTSUBMSK &= ~(0x1<<18);
	rINTMSK1 &= ~(0x1<<17);


}

void Main(void)
{	
	char value;

	Uart_Init(115200);	
	DMA0_SW_Init();

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* TODO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ DMA0_ISR ÇÔ¼ö µî·Ï */
	pISR_DMA0 = (unsigned int)DMA0_ISR;
			
	/*  ÀÎÅÍ·´Æ® Çã¿ë on DMA0 */
	rINTSUBMSK &= ~(0x1<<18);	
	rINTMSK1 &= ~(0x1<<17);
	
	DMA0_SW_Start();


	while(1)
	{
		if(Touch_Pressed)
		{
			Uart_Printf("X : %d, Y: %d \n", ADC_x, ADC_y);	
		}

		cXpos=lcdXc-5;
		cYpos=lcdYc-5;

		for(j=cYpos; j<cYpos+5;j++)
		{
			for(i=cXpos; i<cXpos+5; i++)
			{
				NonPal_Put_Pixel(i,j,RED);			
			}
		}
	}



}
# endif
 

/***************************************
 * 
 * Title: UART_DMA
 * 
 ***************************************/
#if EXAMPLE == 444

//** ISR Function declaration 
static void DMA0_ISR(void) __attribute__ ((interrupt ("IRQ")));;
// Global Variables Declaration
// ¸ñÀûÁö´Â CACHE ¿µ¿ªÀÌ ¾Æ´Ò°Í
static	unsigned long src=0x33000000;
static	unsigned long dst=0x33100000;
static	unsigned int  size = 12; /* word size */
static	unsigned long pattern;

static void DMA0_ISR(void)
{
	/* ÀÎÅÍ·´Æ® Çã¿ëÇÏÁö ¾ÊÀ½ on DMA0 */
	rINTSUBMSK |= (0x1<<18); 
	rINTMSK1 |= (0x1<<17);
	
	/* TO DO: Pendng Clear on DMA0 */
	rSUBSRCPND |= 0x1<<18;
	rSRCPND1 |= 0x1<<17;
	rINTPND1 |= 0x1<<17;
		
	Uart_Printf("__irq ISR½ÇÇà°á°ú  ");	
	MemDump(dst, 12); 	
	
	/*  TO DO: Stop DMA0 trigger  */
//	rDMASKTRIG0 |= 0x1<<2;
	
	/* Masking Disable on DMA0 */
	rINTSUBMSK &= ~(0x1<<18);
	rINTMSK1 &= ~(0x1<<17);
}

void Main(void)
{	
	char value;

	Uart_DMA_Init(115200);	
	DMA0_UART_Init();

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* TO DO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ DMA0_ISR ÇÔ¼ö µî·Ï */
	pISR_DMA0 = (unsigned int)DMA0_ISR;
		
	/* TO DO : ÀÎÅÍ·´Æ® Çã¿ë on DMA0 */	
	rINTSUBMSK &= ~(0x1<<18);
	rINTMSK1 &= ~(0x1<<17);
		
	DMA0_HW_Start();
	
	while(1){
		Uart_Printf("Input Value = : ");	
		value = Uart_Get_Char();
		Uart_Printf("value=%c \n", value);								
	}
}
# endif

 /***************************************
 * 
 * Title: Timer_DMA
 * 
 ***************************************/

#if EXAMPLE == 444
//** ISR Function declaration 
static void DMA0_ISR(void) __attribute__ ((interrupt ("IRQ")));;
// Global Variables Declaration
// ¸ñÀûÁö´Â CACHE ¿µ¿ªÀÌ ¾Æ´Ò°Í
static	unsigned long src=0x33000000;
static	unsigned long dst=0x33100000;
static	unsigned int  size = 12; /* word size */
static	unsigned long pattern;

static void DMA0_ISR(void)
{
	/* ÀÎÅÍ·´Æ® Çã¿ëÇÏÁö ¾ÊÀ½ on DMA0 */
	rINTSUBMSK |= (0x1<<18); 
	rINTMSK1 |= (0x1<<17);
	
	/* TO DO: Pendng Clear on DMA0 */
	rSUBSRCPND |= 0x1<<18;
	rSRCPND1 |= 0x1<<17;
	rINTPND1 |= 0x1<<17;
	
        Uart_Printf("__irq ISR½ÇÇà°á°ú  ");	
	MemDump(dst, 12); 	
	
	/*  TO DO: Stop DMA0 trigger  */
	//rDMASKTRIG0 |= 0x1<<2;
	
	/* Masking Disable on DMA0 */
	rINTSUBMSK &= ~(0x1<<18);
	rINTMSK1 &= ~(0x1<<17);
	
}

void Main(void)
{	
	char value;

	Uart_Init(115200);	
	DMA0_Timer_Init();
	
	Timer_DMA_Init();

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Test...!!!\n");
	
	/* TODO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ DMA0_ISR ÇÔ¼ö µî·Ï */
	pISR_DMA0 = (unsigned int)DMA0_ISR;
		
	/* TO DO : ÀÎÅÍ·´Æ® Çã¿ë on DMA0 */	
	rINTSUBMSK &= ~(0x1<<18);	
	rINTMSK1 &= ~(0x1<<17);
	
	
	DMA0_HW_Start();	
	Timer_Delay(1000);	

}



/* Touch_Main.c */
//**ISR Declaration
void Touch_ISR(void) __attribute__ ((interrupt ("IRQ")));

volatile  int ADC_x, ADC_y;
volatile  int Touch_Pressed=0;

void Touch_ISR()
{
	/* ÀÎÅÍ·´Æ® Çã¿ëÇÏÁö ¾ÊÀ½ on Touch */
	rINTSUBMSK |= (0x1<<9);
	rINTMSK1 |= (0x1<<31);	
	
	/* TO DO: Pendng Clear on Touch */	
	rSUBSRCPND |= (0x1<<9);
	rSRCPND1 |= (0x1<<31);
	rINTPND1 |= (0x1<<31);

	if(rADCTSC & 0x100)
	{
		rADCTSC &= (0xff); 
		Touch_Pressed = 0;
		Uart_Send_String("Detect Stylus Up Interrupt Signal \n");
	}
	
	else
	{
		Uart_Send_String(" ISR ³»ºÎ  \n");
		
		rADCTSC &=~(0x1<<8);				//detect stylus down
		rADCTSC &= ~(0x1<<7);				//set bit for x position measurement
		rADCTSC |= (0x1<<6);				//YM=Hi-z, YP=Hi-z
		rADCTSC |= (0x1<<5);		
		rADCTSC &= ~(0x1<<4);				//XM=VSS,XP=VDD
		rADCTSC &= ~(0x1<<3);				//pull-up EN
		rADCTSC &= ~(0x1<<2);				//Normal-ADC
		rADCTSC &= ~(0x3);
		rADCTSC |= (0x1);					//X-position = 1


		/* TO DO : ENABLE_START */		
		rADCCON |= (0x1);
		
		/* wait until End of A/D Conversion */
		while(!(rADCCON & (1<<15)));
		rADCCON &= ~(0x1);					//Stop Conversion

		/*Set ADCTSC reg for Y Conversion*/ 
		ADC_x = (rADCDAT0 & 0x3ff);			//Store X value
		
		rADCTSC |= (0x1<<7);				//YM=VSS, YP=VDD
		rADCTSC &= ~(0x1<<6);
		rADCTSC &= ~(0x1<<5);				//XM=Hi-z, XP=Hi-z
		rADCTSC |= (0x1<<4);
		/* clear and then set  ADCTSC [1:0] for Y Conversion*/
		rADCTSC &= ~(0x3);
		rADCTSC |= (0x2);
	
		rADCCON |= (0x1);				//StartConversion
		while(!(rADCCON & (1<<15)));	//wait untill End of A/D Conversion

		ADC_y = (rADCDAT1 & 0x3ff);		//Store y value
		
		Touch_Pressed = 1;
		
		/* TO DO : change to Waiting for interrupt mode 
		 *		   Stylus Up, YM_out Enable, YP_out Disable, XM_out Disable, XP_out disable
		 */
		rADCTSC |= (0x1<<8);
		rADCTSC |= (0x1<<7);
		rADCTSC |= (0x1<<6);
		rADCTSC &= ~(0x1<<5);
		rADCTSC |= (0x1<<4);
		rADCTSC &= ~(0x1<<3);
		rADCTSC &= ~(0x1<<2);
		rADCTSC |= (0x1<<1);
		rADCTSC |= (0x1);
			 
	}
	
	/* ÀÎÅÍ·´Æ® ´Ù½Ã Çã¿ë  on Touch */
	rINTSUBMSK &= ~(0x1<<9);
	rINTMSK1 &= ~(0x1<<31);
	
}

void Main(void)
{	
	Uart_Init(115200);
	Touch_Init();	
	
	Uart_Printf("*** Touch Test *** \n");
	
	/* TO DO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ Touch_ISR ÇÔ¼ö µî·Ï */
	pISR_ADC = (unsigned int)Touch_ISR;
	
	/* TO DO :  ÀÎÅÍ·´Æ® Çã¿ë on Touch */
	rINTSUBMSK &= ~(0x1<<9);
	rINTMSK1 &= ~(0x1<<31);	
	
	while(1)
	{
		if(Touch_Pressed)
		{
			Uart_Printf("X : %d, Y: %d \n", ADC_x, ADC_y);	
		}
	}
}


#endif





/***************************************
 * 
 * Title: Timer_Interrupt
 * 
 ***************************************/

#if EXAMPLE == 444

// ISR Declaration
void Timer0_ISR(void) __attribute__ ((interrupt ("IRQ")));

void Timer0_ISR()
{
	/* ÇØ´ç ÀÎÅÍ·´Æ® Masking */ 

	rINTMSK1 |= (1<<10);	
	/* TODO : Pending Register Clear */
	rSRCPND1 |= (0xffffffff);
	rINTPND1 |= (0xffffffff);
	//rINTOFFSET1 = 0x400; This register has been already setup in C function by asm
						//So, you don't care about this

	Uart_Send_String("Timer ISR\n");
	
	/* ÇØ´ç ÀÎÅÍ·´Æ® UnMasking */
	rINTMSK1 &= ~(1<<10);			//prevent ReAsk same INTERRUPT
}

void Main()
{
	Uart_Init(115200);	

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Timer Interrupt Test...!!!\n");
	
	Timer_Init();
	
	/* TODO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ Timer0_ISR ÇÔ¼ö µî·Ï */
	pISR_TIMER0 = (unsigned int)Timer0_ISR;
	/* Interrupt Unmasking */
	rINTMSK1 &= ~(1<<10);
	
	Timer_Delay(1000);
		
}

#endif


/***************************************
 * 
 * Title: UART_Interrupt
 * 
 ***************************************/

#if EXAMPLE == 444

static char Input_Char;

//** ISR Function declaration 
static void Uart_RX1_ISR(void) __attribute__ ((interrupt ("IRQ")));;

int Uart_Check_Overrun_Error(){
	return (rUERSTAT1&0x01);
}

static void Uart_RX1_ISR(void)
{
	/* TO DO : UART1_RX °¡ ´Ù½Ã ¹ß»ýÇÏÁö ¾Êµµ·Ï Masking  */
	rINTSUBMSK |=(0x1<<3);
	rINTMSK1 |= (0x1<<23);
	
	
	/* TO DO: Pendng Clear on UART1_RX,UART1_ERR */
	rSUBSRCPND |= (0x1<<3);
	rSRCPND1 |= (0x1<<23);
	rINTPND1 |= (0x1<<23);
	
	
	/* ISRoutine Execution*/
	Uart_Send_Byte('\n');
	Uart_Send_String("Uart Receive data is OK..!!\n");
	
	#if 1 
	if(Uart_Check_Overrun_Error()) 	Uart_Send_Byte('*'); Uart_Send_Byte('\n');
	#endif
	
	/* UnMasking on UART1_RX */  
	rINTSUBMSK &=~(0x1<<3);
	rINTMSK1 &=~(0x1<<23);
	rUCON1 |= (0x1<<6);
}


void Main(void)
{	

	Uart_Init(115200);	

	Uart_Send_Byte('\n');
	Uart_Send_Byte('A');	
	Uart_Send_String("Hello Uart Interrupt Test...!!!\n");
	
	/* TODO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ Uart_RX1_ISR ÇÔ¼ö µî·Ï */
	pISR_UART1 = (unsigned int)Uart_RX1_ISR;
		
	/* TO DO : ÀÎÅÍ·´Æ® Çã¿ë on UART1_RX,ERR  */
	rINTSUBMSK &=~(0x1<<3);
	rINTSUBMSK &=~(0x1<<5);
	rINTMSK1 &= ~(0x1<<23);
		
	for(;;)
	{
		Input_Char=Uart_Get_Char();
		Uart_Printf("Input charter is : %d \n ", Input_Char);
	}
			
			
}

#endif

/***************************************
 * 
 * Title: Key_Interrupt (External Interrupt)
 * 
 ***************************************/

#if EXAMPLE == 444

// Global Variables Declaration
volatile int Key_Value = 0;

// ISR Declaration
void Key_ISR(void) __attribute__ ((interrupt ("IRQ")));;

void Key_ISR()
{
	unsigned int temp;

	/* ÇØ´ç ÀÎÅÍ·´Æ® Masking */ 
	rINTMSK1 |= (0x7<<2);
	rEINTMASK |= (0x7<<4);

	/* Å°¸¦ ÀÐ±â À§ÇÏ¿© Polling ÇÔ¼ö »ç¿ë */
	temp = rGPFCON;
	
	/* TO DO :  1) GPFCON[6:2] change to Input mode 
	 *          2) ÀÔ·ÂµÈ Å°°ªÀ» È®ÀÎÇÏ¿© º¯¼ö¿¡ ÀúÀå 
     */		 
	//rGPFDAT |= (0x1f << 2);
	rGPFCON &= ~(0x3ff<<4);
	Key_Value = Key_Get_Pressed();
    /* ¹Ýµå½Ã port¸¦ º¹¿øÇÏ±â À§ÇÏ¿© ÀúÀåÇÑ °ªÀ¸·Î º¹¿ø */
	rGPFCON = temp;

	/* TO DO : KeyoutÀ» 0·Î ÇÏ¿© Å° ÀÔ·Â ´ë±â»óÅÂ·Î ¹Ù²Þ */
	rGPGDAT &= ~0x1;
	rGPFDAT &= ~(0x1<<7);	
	
	/* TO DO: Pendng Clear on EINT2,3,4,5,6 */
	rEINTPEND |= (0x7<<4);
	rSRCPND1 |= (0x7<<2);
 	rINTPND1 |= (0x7<<2);	
	/* ÇØ´ç ÀÎÅÍ·´Æ® UnMasking */ 
	rINTMSK1 &= ~(0x7<<2);
	rEINTMASK &= ~(0x7<<4);	
}

void Main(void)
{	
	int a,i;
	
	Led_Init();
	Uart_Init(115200);
	Key_IRQ_Port_Init();
	
	/* TODO : ÀÎÅÍ·´Æ® º¤ÅÍ¿¡ Key_ISR ÇÔ¼ö µî·Ï */
	//pISR_EINT3 = (unsigned int)Key_EINT3_ISR;	
	pISR_EINT2 = (unsigned int)Key_ISR;
	pISR_EINT3 = (unsigned int)Key_ISR;
	pISR_EINT4_7 = (unsigned int)Key_ISR;
	
	
	/* TO DO : ÀÎÅÍ·´Æ® Çã¿ë on EINT2,3,4,5,6*/	
	//rINTMSK &= ~(1<<3);
	rINTMSK1&=~(7<<2);
	rEINTMASK&=~(7<<4);

	Uart_Send_String("Key ISR Test \n");
	
	for(;;)
	{
		if(Key_Value)
		Uart_Printf("Key Value =%d\n",  Key_Value); 
		Key_Value = 0;			
	}
			
}





































#endif 




