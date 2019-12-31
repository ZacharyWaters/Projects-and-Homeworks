//////////////////////////////////////////////////////////////////////
//
//  INDEX
// 
//////////////////////////////////////////////////////////////////////
/* 
        General
        Bitmaps
        Video
        Interrupts
        DMA
        Input

====================================================================*/

// *** General =======================================================

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char s8;
typedef short s16;
typedef int s32;

#define TRUE 1
#define FALSE 0



// *** Video =======================================================

/* Mode 3 */
extern volatile unsigned short *videoBuffer;               // 0x6000000
// Need in one of your .c files:
//     volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
#define REG_DISPCNT  *(volatile unsigned short *) 0x4000000
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define COLOR(r,g,b) ((r) + ((g) << 5) + ((b) << 10))


// Mode 4
#define BUFFER0 (volatile unsigned short *)0x6000000
#define BUFFER1 (volatile unsigned short *)0x600A000
#define PALETTE ((volatile unsigned short *)0x5000000)
#define BACKBUFFER (1<<4)





typedef struct { u16 tileimg[8192]; } charblock;
typedef struct { u16 tilemap[1024]; } screenblock;

extern charblock *charbase;
extern screenblock *screenbase;





//Background Enables
#define BG0_ENABLE  (1<<8)
#define BG1_ENABLE  (1<<9)
#define BG2_ENABLE  (1<<10)
#define BG3_ENABLE  (1<<11)

//background control registers
#define REG_BG0CNT *(volatile unsigned short*)0x4000008
#define REG_BG1CNT *(volatile unsigned short*)0x400000A
#define REG_BG2CNT *(volatile unsigned short*)0x400000C
#define REG_BG3CNT *(volatile unsigned short*)0x400000E

#define BLACK   COLOR(0,0,0)
#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31,0,0)
#define GREEN   COLOR(0, 31, 0)
#define BLUE    COLOR(0,0,31)
#define YELLOW  COLOR(31,31,0)
#define CYAN    COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define GRAY    COLOR(25, 25, 25)

#define MODE0 0
#define MODE1 1
#define MODE2 2
#define MODE3 3
#define MODE4 4
#define MODE5 5
#define MODE6 6

/* Interrupts -------------------------------------------------------------------- */


#define REG_IME *(volatile u16*)0x4000208
#define REG_IE *(volatile u16*)0x4000200
#define REG_IF *(volatile u16*)0x4000202
#define REG_INTERRUPT *(volatile u32*)0x3007FFC
// or #define REG_INTERRUPT *((void (**) (void))0x3007FFC)
#define REG_DISPSTAT *(volatile u16*)0x4000004

//interrupt constants for turning them on

#define VBOCCUR  (1<<0)  // VB – vertical blank is occurring
#define HBOCCUR  (1<<1)  // HB – horizontal blank is occurring
#define VCREACH  (1<<2)  // VC – vertical count reached
#define VBENABLE (1<<3)  //	VBE – enables vblank interrupt
#define HBENABLE (1<<4)  // HBE – enables hblank interrupt
#define VCENABLE (1<<5)  //	VCE – enables vcount interrupt
// Note: Bits 6-15			VCOUNT – vertical count value (0-159)





// Interrupt constants for checking which type of interrupt happened
#define INT_VB     (1 <<  0)	// VB – vertical blank interrupt
#define INT_HB     (1 <<  1)    // HB – horizontal blank interrupt
#define INT_VC     (1 <<  2)	// VC – vertical scanline count interrupt
#define INT_T0     (1 <<  3)	// T0 – timer 0 interrupt
#define INT_T1     (1 <<  4)	// T1 – timer 1 interrupt
#define INT_T2     (1 <<  5)	// T2 – timer 2 interrupt
#define INT_T3     (1 <<  6)	// T3 – timer 3 interrupt
#define INT_COM    (1 <<  7)	// COM – serial communication interrupt
#define INT_DMA0   (1 <<  8)	// DMA0 – DMA0 finished interrupt
#define INT_DMA1   (1 <<  9)	// DMA1 – DMA1 finished interrupt
#define INT_DMA2   (1 << 10) 	// DMA2 – DMA2 finished interrupt
#define INT_DMA3   (1 << 11)	// DMA3 – DMA3 finished interrupt
#define INT_BUTTON (1 << 12) 	// BUTTON – button interrupt
#define INT_CART   (1 << 13)	// CART – game cartridge interrupt


/* DMA */

#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)


// *** Input =========================================================

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define BUTTONS (*(volatile unsigned int *)0x4000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)





#define BUTTON_INDEX_A      0
#define BUTTON_INDEX_B      1
#define BUTTON_INDEX_SELECT 2
#define BUTTON_INDEX_START  3
#define BUTTON_INDEX_RIGHT  4
#define BUTTON_INDEX_LEFT   5
#define BUTTON_INDEX_UP     6
#define BUTTON_INDEX_DOWN   7
#define BUTTON_INDEX_R      8
#define BUTTON_INDEX_L      9

// Prototypes
void waitForVblank();
