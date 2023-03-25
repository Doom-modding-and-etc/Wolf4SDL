#include <stdlib.h>
#include <gpstdlib.h>
#include "SDL_dmaaudio.h"
#include <x_gp32.h>

#define L3C                             0x200   /* bit 9  */
#define L3M                             0x400   /* bit 10 */
#define L3D                             0x800   /* bit 11 */
#define L3_MASK                         0xe00

#define L3DELAY                             8   /* delay for bus-transfer */

#define PRESCALER_11_ENTRIES (32)
#define PRESCALER_22_ENTRIES (16)
#define PRESCALER_44_ENTRIES (8)

/* Prototype for the interrupt service routine (implemented in SDL_dmaaudioasm.s) */
void sdl_gp32_dma_audio_ISR (void);

static const long prescaler11_256[PRESCALER_11_ENTRIES] = {
	2822400,		5644800,		8467200,		11289600,
	14112000,	16934400,	19756800,	22579200,
	25401600,	28224000,	31046400,	33868800,
	36691200,	39513600,	42336000,	45158400,
	47980800,	50803200,	53625600,	56448000,
	59270400,	62092800,	64915200,	67737600,
	70560000,	73382400,	76204800,	79027200,
	81849600,	84672000,	87494400,	90316800
};

static const long prescaler11_384[PRESCALER_11_ENTRIES] = {
	4233600,		8467200,		12700800,	16934400,
	21168000,	25401600,	29635200,	33868800,
	38102400,	42336000,	46569600,	50803200,
	55036800,	59270400,	63504000,	67737600,
	71971200,	76204800,	80438400,	84672000,
	88905600,	93139200,	97372800,	101606400,
	105840000,	110073600,	114307200,	118540800,
	122774400,	127008000,	131241600,	135475200
};

static const long prescaler22_256[PRESCALER_22_ENTRIES] = {
	5644800,		11289600,	16934400,	22579200,
	28224000,	33868800,	39513600,	45158400,
	50803200,	56448000,	62092800,	67737600,
	73382400,	79027200,	84672000,	90316800
};

static const long prescaler22_384[PRESCALER_22_ENTRIES] = {
	8467200,		16934400,	25401600,	33868800,
	42336000,	50803200,	59270400,	67737600,
	76204800,	84672000,	93139200,	101606400,
	110073600,	118540800,	127008000,	135475200
};

static const long prescaler44_256[PRESCALER_44_ENTRIES] = {
	11289600,	22579200,	33868800,	45158400,
	56448000,	67737600,	79027200,	90316800
};

static const long prescaler44_384[PRESCALER_44_ENTRIES] = {
	16934400,	33868800,	50803200,	67737600,
	84672000,	101606400,	118540800,	135475200
};

/* Pointer to the memory block which contains the output buffer */
static char *pOutBufferMemory;
short *pOutBuffer[3];
static short nStarted;

/*
   This routine sends an address plus type of transfer to the L3 bus
*/
static void _WrL3Addr(unsigned char data)
{
   signed long i,j;

   rPEDAT = rPEDAT & ~L3_MASK;         //L3D=L/L3M=L(in address mode)/L3C=L
   rPEDAT |= L3C;                      //L3C=H
   for(j=0;j<L3DELAY;j++);             //tsu(L3) > 190ns

   //PD[8:6]=L3D:L3C:L3M
   for(i=0;i<8;i++)  //LSB first
   {
      if(data&0x1)   //if data's LSB is 'H'
      {
         rPEDAT &= ~L3C;           //L3C=L
         rPEDAT |= L3D;            //L3D=H
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
         rPEDAT |= L3C;            //L3C=H
         rPEDAT |= L3D;            //L3D=H
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
      }
      else     //if data's LSB is 'L'
      {
         rPEDAT &= ~L3C;           //L3C=L
         rPEDAT &= ~L3D;           //L3D=L
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
         rPEDAT |= L3C;            //L3C=H
         rPEDAT &= ~L3D;           //L3D=L
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
      }
      data >>=1;
   }
   //L3M=H,L3C=H
   rPEDAT = (rPEDAT & ~L3_MASK) | (L3C|L3M);
}

/*
   This routine sends a data word to the L3 bus
*/
static void _WrL3Data(unsigned char data,int halt)
{
   signed long i,j;

   if(halt)
   {
      rPEDAT |= L3C;                //L3C=H(while tstp, L3 interface halt condition)
      for(j=0;j<L3DELAY;j++);       //tstp(L3) > 190ns
      rPEDAT &= (~L3M);
      for (j=0;j<L3DELAY;j++);
      rPEDAT |= L3M;
   }

   rPEDAT = (rPEDAT & ~L3_MASK) | (L3C|L3M);        //L3M=H(in data transfer mode)
   for(j=0;j<L3DELAY;j++);                          //tsu(L3)D > 190ns

   //PD[8:6]=L3D:L3C:L3M
   for(i=0;i<8;i++)
   {
      if(data&0x1)   //if data's LSB is 'H'
      {
         rPEDAT &= ~L3C;           //L3C=L
         rPEDAT |= L3D;            //L3D=H
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
         rPEDAT |= (L3C|L3D);      //L3C=H,L3D=H
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
      }
      else     //if data's LSB is 'L'
      {
         rPEDAT &= ~L3C;           //L3C=L
         rPEDAT &= ~L3D;           //L3D=L
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
         rPEDAT |= L3C;            //L3C=H
         rPEDAT &= ~L3D;           //L3D=L
         for(j=0;j<L3DELAY;j++);   //tcy(L3) > 500ns
      }
      data>>=1;   //for check next bit
   }

   rPEDAT = (rPEDAT & ~L3_MASK) | L3C;
   for (j=0;j<L3DELAY;j++);
   rPEDAT |= L3M;
}

/*
        This routine initializes the i/o port used for L3-bus and
        then configures the UDA1330
*/
static void Init1330( int SC384 )
{
   /****** I/O Port E Initialize ******/
   /* port reconfiguration :
      PORTE 9,10,11 --> output
      pull-up disable
      L3_MODE, L3_CLK High
   */
   if (SC384)
   	SC384=1;

   rPEDAT = (rPEDAT & ~0xe00) | (L3M|L3C);
   rPEUP |= 0xe00;
   rPECON = (rPECON & (~(0x3f << 18))) | (0x15<<18);

   /****** send commands via L3 Interface
   Data bits 7 to 2 represent a 6-bit device address where bit 7 is the MSB.
   The address of the UDA1330ATS is 000101 (bit 7 to bit 2).
   ******/

   /* status type transfer , data value - clock=512fs, MSB format */
   _WrL3Addr(0x14+2); //STATUS (0) (000101xx+10)
   _WrL3Data(  ((2-SC384)<<4)+  //  00,  : 512,384,256fs         (SC : System Clock Freq)
               (0<<1)   // 000,  : iis,lsb16,lsb18,lsb20,msb
               ,0);

   /* data type transfer , data value - full volume */
   _WrL3Addr(0x14 + 0);       /* DATA0 (000101xx+00) */
   _WrL3Data(0x0              /* volume */
               ,0);

   /* data type transfer , data value - de-emphasis, no muting */
   _WrL3Addr(0x14 + 0);       /* DATA0 (000101xx+00) */
   _WrL3Data(0x80+            /* select de-emhasis/mute */
               (2<<3)         /* de-emp 44khz */
               ,1);
}

static void InitIIS(int nReverse, int nPrescaler, int SC384)
{
	int nRev;

	if (nReverse)
		nRev=0;
	else
		nRev=1;

   if (SC384)
   	SC384=1;
    /****** IIS Initialize ******/

    /* prescaler for 11khz */
    rIISPSR=(nPrescaler<<5)+nPrescaler;

    rIISCON=/* 8,7,6 readonly */
            (1<<5)+  /* dma transmit request enable */
            (0<<4)+  /* dma receive request disable */
            (0<<3)+  /* Transmit LR-Clock idle state */
            (1<<2)+  /* Receive LR-Clock idle state */
            (1<<1)+  /* prescaler enabled */
            (0<<0);  /* iis disabled */

    rIISMOD=
            (0<<8)+             /* master mode */
            (2<<6)+             /* transmit mode */
            (nRev<<5)+          /* lowbyte=left channel (1) or right channel (0) */
            (0<<4)+             /* iis compatible MSB samples */
            (1<<3)+             /* 16 bit per channel */
            (SC384<<2)+             /* CDCLK=256fs (0) or 384fs (1) */
            (1<<0);             /* serial bitclock 48fs */

    rIISFIFCON=
               (1<<11)+   /* transmit fifo mode = dma */
               (0<<10)+   /* recieve fifo mode = normal */
               (1<<9)+    /* transmit fifo enable */
               (0<<8);    /* receive fifo disable */
                /* 7-4 Transmit Fifo Data Count (readonly) */
                /* 3-0 Recieve Fifo Data Count (readonly) */

    /****** IIS Tx Start ******/
    rIISCON |=0x1;  /* iis enable */

}

static void DmaIsrInstall(unsigned long nr,void *ptr)
{
	unsigned int mask;

	mask = 1<<nr;

	/* Clear any pending interrupts */
	rSRCPND = mask;
	rINTPND = mask;
	/* Set this interrupt to IRQ rather than FIQ. Shouldn't be
		necessary because INTMOD should be zero, but just in case...
	 */
	rINTMOD &= ~(mask);

	/* set vector for selected interrupt interrupt. This system call copies the
		vector to the table used by the interrupt handler in the bios. It also clears
		the mask bit for the interrupt
	*/

	x_gp32_IsrInstall(nr,ptr);
}

static void DmaIsrUninstall(unsigned long nr)
{
	unsigned int mask;

	x_gp32_IsrUninstall(nr);

	mask = 1<<nr;

	/* Clear any pending interrupts */
	rSRCPND = mask;
	rINTPND = mask;
}

int sdl_gp32_dma_audio_start (int nRate, int nBufferSamples, int nReverse)
{
	int x;
	int nPrescaler=0;
	int nLeastDiff;
	int SC384=0;
	long pclk;
	int diff;
	const long *p256;
	const long *p384;
	int nEntries;
	unsigned int nOutBuffer;

	if (nStarted)
		return GP32_DMA_AUD_ERR_STARTED;

	/* You only get one go at this. If the memory allocation fails, you
		have to call sdl_gp32_dma_audio_stop before trying again.
	 */
	nStarted=1;

	x_gp32_ArmEnableCPSRInterrupt();

	/* Get space for 3 buffers, of stereo 16-bit samples. This is going to be set to
		non-cached, so make it a 4k chunk, which is the resolution of the MMU set up
		by the GP32 bios.

		There are two buffers for the buffer-flipping sound, and another buffer which
		is always silence for when the SDL sound is paused/locked.

		Add 8k to the buffer size, to allow 4k for alignment with the MMU pages, and
		another 4k for the pages that must be allocated to the output buffer
		For example: 	you want an output buffer of 4104 bytes (1026 samples).
							malloc returns a pointer of 0x0C100004.
							The first MMU page that can be used is 0x0C101000
							-> 4092 bytes lost there.
							The end of the buffer is 0x0C101000 + 4104, which is 0x0C102008
							The end of the MMU page to cover the buffer is 0x0C102fff
							-> 4087 bytes lost there.
	 */
	pOutBufferMemory = (char *)malloc ((nBufferSamples * 12)+8192);
	/* Deal with the pointer as an unsigned int, which is a little bit naughty but
		the compiler will complain otherwise
	 */
	if (!pOutBufferMemory)
		return GP32_DMA_AUD_ERR_MEMORY;

	nOutBuffer = (unsigned int)pOutBufferMemory;
	/* Shift the pointer along to the next 4k boundary */
	nOutBuffer += 0xfff;
	nOutBuffer &= 0xfffff000;
	pOutBuffer[0] = (short *)nOutBuffer;
	/* OK, this is the pointer. Now need the address of the end of the buffer. */
	nOutBuffer += (nBufferSamples * 12) + 0xfff;
	nOutBuffer &= 0xfffff000;
	nOutBuffer--;
	/* This is the end of the buffer. Set the buffer area to non-cached */
	x_gp32_dma_MMUChange ( pOutBuffer[0], (void *)nOutBuffer, 0xffa );

	/* Clear the output buffer */
	for (x=0;x<(nBufferSamples*6);x++)
		pOutBuffer[0][x]=0;

	/* Second output buffer is contiguous with the first one */
	pOutBuffer[1] = pOutBuffer[0] + (nBufferSamples<<1);
	/* Third output buffer is contiguous with the second one */
	pOutBuffer[2] = pOutBuffer[1] + (nBufferSamples<<1);

   /* stop dma2 */
   rDMASKTRIG2=(1<<2)+(0<<1)+0;  /* stop, DMA2 channel off, no-sw trigger */

	/* Find the closest prescaler for the current PCLK */
	nLeastDiff = 1000000000;
	pclk = x_gp32_GetPCLK();

	/* Assume the sample rate is 11025. */
	p256 = prescaler11_256;
	p384 = prescaler11_384;
	nEntries = PRESCALER_11_ENTRIES;

	if (nRate==22050)
	{
		p256 = prescaler22_256;
		p384 = prescaler22_384;
		nEntries = PRESCALER_22_ENTRIES;
	}
	if (nRate==44100)
	{
		p256 = prescaler44_256;
		p384 = prescaler44_384;
		nEntries = PRESCALER_44_ENTRIES;
	}

	/* Find the prescaler and fclk that best match pclk */
	for (x=0;x<nEntries;x++)
	{
		diff = pclk - p256[x];
		if (diff<0)
			diff=-diff;
		if (diff<nLeastDiff)
		{
			SC384=0;
			nPrescaler=x;
			nLeastDiff=diff;
		}
		diff = pclk - p384[x];
		if (diff<0)
			diff=-diff;
		if (diff<nLeastDiff)
		{
			SC384=1;
			nPrescaler=x;
			nLeastDiff=diff;
		}
	}

	for (x=0;x<1000000;x++);

   /* initialize uda1330a by sending appropriate commands over L3 bus */
   Init1330(SC384);
   /* setup IIS bus */
   InitIIS(nReverse,nPrescaler,SC384);
   /* install dma2-irq */
   DmaIsrInstall(nINT_DMA2,(void*)sdl_gp32_dma_audio_ISR);
   /* start playing */
   rDIDST2=(1<<30)+     	  	/* destination on peripheral bus */
         (1<<29)+       		/* fixed adress */
         ((int)IISFIF); 		/* IIS FIFO txd */
   rDISRC2=(0<<30)+       		/* source from system bus */
         (0<<29)+       		/* auto-increment */
         (int)pOutBuffer[0];	/* buffer adress */
   rDCON2 =(1<<30)+       		/* handshake mode */
         (0<<29)+       		/* DREQ and DACK are synchronized to PCLK (APB clock) */
         (1<<28)+       		/* generate irq when transfer is done */
         (0<<27)+       		/* unit transfer */
         (0<<26)+       		/* single service */
         (0<<24)+       		/* dma-req.source=I2SSDO */
         (1<<23)+       		/* (H/W request mode) */
         (1<<22)+     			/* auto reload on/off */
         (1<<20)+       		/* data size, byte,hword,word */
         (nBufferSamples<<1);	/* transfer size (halfwords) */

   rDMASKTRIG2=(0<<2)+(1<<1)+0;  /* no-stop, DMA2 channel on, no-sw trigger */

   return 0;
}

void sdl_gp32_dma_audio_stop (void)
{
	if (!nStarted)
		return;

	/* stop dma2 */
   rDMASKTRIG2=(1<<2)+(0<<1)+0;  /* stop, DMA2 channel off, no-sw trigger */

   DmaIsrUninstall(nINT_DMA2);

	nStarted=0;

	if (pOutBufferMemory)
	{
		free(pOutBufferMemory);
		pOutBufferMemory = 0;
	}
}

