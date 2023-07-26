/*
//
//      ID Engine
//      ID_SD.c - Sound Manager for Wolfenstein 3D
//      v1.2
//      By Jason Blochowiak
//

//
//      This module handles dealing with generating sound on the appropriate
//              hardware
//
//      Depends on: User Mgr (for parm checking)
//
//      Globals:
//              For User Mgr:
//                      SoundBlasterPresent - SoundBlaster card present?
//                      AdLibPresent - AdLib card present?
//                      SoundMode - What device is used for sound effects
//                              (Use SM_SetSoundMode() to set)
//                      MusicMode - What device is used for music
//                              (Use SM_SetMusicMode() to set)
//                      DigiMode - What device is used for digitized sound effects
//                              (Use SM_SetDigiDevice() to set)
//
//              For Cache Mgr:
//                      NeedsDigitized - load digitized sounds?
//                      NeedsMusic - load music?
//
*/

#include "wl_def.h"

#ifndef VIEASM
#ifndef SEGA_SATURN
#include <SDL_mixer.h>
#else
#include "Platform/Sega/Saturn/pcmsys.h"
#include <string.h>
#endif

#if defined(GP2X_940)
#include "gp2x/fmopl.h"
#else
#if defined(USE_DOSBOX)
#include "aud_sys/dosbox/dbopl.h"
#elif defined(USE_NUKEDOPL)
#include "aud_sys/nukedopl3/opl3.h"
#elif defined(USE_FBNEO_FMOPL)
#include "aud_sys/fbneo/fmoplneo.h"
#else
#include "aud_sys/mame/fmopl.h"
#endif
#endif

#define ORIGSAMPLERATE 7042

#ifndef SEGA_SATURN
#ifdef USE_DOSBOX
static Chip oplChip;

static int YM3812Init(int numChips, int clock, int rate)
{
    DBOPL_InitTables();
    Chip__Chip(&oplChip);
    Chip__Setup(&oplChip, rate);
    return 1;
}

static void YM3812Write(Chip *which, Bit32u reg, Bit8u val)
{
    Chip__WriteReg(which, reg, val);
}

static void YM3812UpdateOne(Chip *which, short* stream, int length)
{
#if 0
    Bit32s buffer[512 * 2];
    int i;

    /* 
    ** length is at maximum samplesPerMusicTick = param_samplerate / 700
    ** so 512 is sufficient for a sample rate of 358.4 kHz (default 44.1 kHz)
    */
    if (length > 512)
        length = 512;

    if (which.opl3Active)
    {
        Chip__GenerateBlock3(&which, length, buffer);

        /* 
        ** GenerateBlock3 generates a number of "length" 32-bit stereo samples
        ** so we only need to convert them to 16-bit samples
        */
        for (i = 0; i < length * 2; i++)  /* * 2 for left/right channel */
        {
            /* Multiply by 4 to match loudness of MAME emulator. */
            Bit32s sample = buffer[i] << 2;
            if (sample > 32767) sample = 32767;
            else if (sample < -32768) sample = -32768;
            stream[i] = sample;
        }
    }
    else
    {
        Chip__GenerateBlock2(&which, length, buffer);

        /* 
        ** GenerateBlock2 generates a number of "length" 16-bit mono samples
        ** so we need to convert them to 32-bit stereo samples
        */
        for (i = 0; i < length; i++)
        {
            /* Multiply by 4 to match loudness of MAME emulator. */
            /* Then upconvert to stereo. */
            Bit32s sample = buffer[i] << 2;
            if (sample > 32767) sample = 32767;
            else if (sample < -32768) sample = -32768;
            stream[i * 2] = stream[i * 2 + 1] = (short)sample;
        }
    }
#else
    int buffer[2048 * 2];
    int i;

    /* 
    ** length should be at least the max. samplesPerMusicTick
    ** in Catacomb 3-D and Keen 4-6, which is param_samplerate / 700.
    ** So 512 is sufficient for a sample rate of 358.4 kHz, which is
    ** significantly higher than the OPL rate anyway.
    */
    if (length > 2048)
        length = 2048;

    Chip__GenerateBlock2(which, length, buffer);

    /* 
    ** GenerateBlock2 generates a number of "length" 32-bit mono samples
    ** so we only need to convert them to 16-bit mono samples
    */
    for (i = 0; i < length; i++)
    {
        /* Scale volume */
        int sample = 2 * buffer[i];
        if (sample > 32767) sample = 32767;
        else if (sample < -32768) sample = -32768;
#ifdef MIXER_SAMPLE_FORMAT_FLOAT
        stream[i] = (float)sample / 32767.0f;
#elif defined (MIXER_SAMPLE_FORMAT_SINT16)
        stream[i] = sample;
#else
        stream[i * 2] = stream[i * 2 + 1] = (short )sample; 
#endif
    }
#endif
}

#elif defined(USE_NUKEDOPL)
static opl3_chip oplChip;

static int YM3812Init(int numChips, int clock, int rate)
{
    OPL3_Reset(&oplChip, rate);
    return 1;
}

static void YM3812Write(opl3_chip* which, unsigned int reg, unsigned char val)
{
    OPL3_WriteReg(which, (unsigned short)reg, (unsigned char)val);
}

static void YM3812UpdateOne(opl3_chip* which, short* stream, int length)
{
    short buffer[2048 * 2];
    int i;

    /* 
    ** length should be at least the max. samplesPerMusicTick
    ** in Catacomb 3-D and Keen 4-6, which is param_samplerate / 700.
    ** So 512 is sufficient for a sample rate of 358.4 kHz, which is
    ** significantly higher than the OPL rate anyway.
    */
    if (length > 2048)
        length = 2048;

     OPL3_GenerateStream(which, buffer, length);

     /*
     ** OPL3_GenerateStream generates a number of "length" 32-bit stereo samples
     ** so we only need to convert them to 16-bit samples
     */
     for (i = 0; i < length * 2; i++)  /* * 2 for left/right channel */
     {
        /* Multiply by 4 to match loudness of MAME emulator. */
        short sample = buffer[i] << 2;
        if (sample > 32767) sample = 32767;
        else if (sample < -32768) sample = -32768;
        stream[i] = sample;
     }
}
#else

static int oplChip = 0;

#endif
#ifdef USE_AUDIO_CVT
static unsigned short mix_format;
static int mix_channels;
#else
typedef struct
{
	char RIFF[4];
	unsigned int filelenminus8;
	char WAVE[4];
	char fmt_[4];
    unsigned int formatlen;
    unsigned short val0x0001;
    unsigned short channels;
    unsigned int samplerate;
    unsigned int bytespersec;
    unsigned short bytespersample;
    unsigned short bitspersample;
} headchunk;

typedef struct
{
	char chunkid[4];
    unsigned int chunklength;
} wavechunk;
#endif
#endif

#ifndef SEGA_SATURN
#ifdef USE_AUDIO_CVT
static Mix_Chunk SoundChunks[STARTMUSIC - STARTDIGISOUNDS] = { 0 };
#else
static Mix_Chunk *SoundChunks[ STARTMUSIC - STARTDIGISOUNDS];
#endif
globalsoundpos channelSoundPos[MIX_CHANNELS];
#endif
/*      Global variables     */
        boolean       
#ifndef SEGA_SATURN
                        AdLibPresent,
#endif
                        SoundBlasterPresent,
#ifndef SEGA_SATURN                        
                        SBProPresent,
#endif                        
                        SoundPositioned;
#ifndef SEGA_SATURN
        SDMode          SoundMode;
        SMMode          MusicMode;
#endif
        SDSMode         DigiMode;
#ifndef SEGA_SATURN
static  unsigned char          **SoundTable;
#endif
#ifdef SEGA_SATURN
#ifndef USE_ADX
        int             DigiMap[LASTSOUND];
#endif
#else
        int             DigiMap[LASTSOUND];
#endif
#ifndef SEGA_SATURN
        int             DigiChannel[STARTMUSIC - STARTDIGISOUNDS];

/*      Internal variables      */
static  boolean                 SD_Started;
static  boolean                 nextsoundpos;
static  soundnames              SoundNumber;
static  soundnames              DigiNumber;
static  unsigned short                    SoundPriority;
static  unsigned short                    DigiPriority;
static  int                     LeftPosition;
static  int                     RightPosition;

        unsigned short                    NumDigi;
        digiinfo                *DigiList;
static  boolean                 DigiPlaying;

/*      PC Sound variables     */
static  volatile unsigned char           pcLastSample;
static  unsigned char * volatile         pcSound;
static  unsigned int                pcLengthLeft;

/*      AdLib variables      */
static  unsigned char * volatile         alSound;
static  unsigned char                    alBlock;
static  unsigned int                alLengthLeft;
static  unsigned int                alTimeCount;
static  Instrument              alZeroInst;

/*      Sequencer variables      */
static  volatile boolean        sqActive;
static  unsigned short         *sqHack;
static  unsigned short         *sqHackPtr;
static  int                     sqHackLen;
static  int                     sqHackSeqLen;
static  unsigned int                sqHackTime;
#endif

#ifdef SEGA_SATURN
extern int SDL_OpenAudio(SDL_AudioSpec* desired, SDL_AudioSpec* obtained);
#ifndef PONY	
extern PCM m_dat[4];
static Mix_Chunk* SoundChunks[STARTMUSIC - STARTDIGISOUNDS];
uintptr_t lowsound = (uintptr_t)0x002C0000;
#endif
extern void	satPlayMusic(unsigned char track);
extern void	satStopMusic(void);
extern 	void sound_cdda(int track, int loop);
short	load_adx(char* filename);
#endif
wlinline void Delay (int wolfticks)
{
    if (wolfticks > 0)
        SDL_Delay ((unsigned int)(wolfticks * 100) / 7);
}
#ifndef SEGA_SATURN
static void SDL_SoundFinished(void)
{
	SoundNumber   = (soundnames)0;
	SoundPriority = 0;
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_PCPlaySound() - Plays the specified sound on the PC speaker
//
///////////////////////////////////////////////////////////////////////////
*/
static void
SDL_PCPlaySound(PCSound *sound)
{
        pcLastSample = (unsigned char)-1;
        pcLengthLeft = sound->common.length;
        pcSound = sound->data;
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_PCStopSound() - Stops the current sound playing on the PC Speaker
//
///////////////////////////////////////////////////////////////////////////
*/
static void
SDL_PCStopSound(void)
{
        pcSound = 0;
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_ShutPC() - Turns off the pc speaker
//
///////////////////////////////////////////////////////////////////////////
*/
static void
SDL_ShutPC(void)
{
        pcSound = 0;
}

/* Adapted from Chocolate Doom (chocolate-doom/pcsound/pcsound_sdl.c) */
#define SQUARE_WAVE_AMP 0x2000

static void SDL_PCMixCallback(int chan, void* stream, int len, void* udata)
{
    static int current_remaining = 0;
    static int current_freq = 0;
    static int phase_offset = 0;

    short *leftptr;
    short *rightptr;
    short this_value;
    
    int i;
    int nsamples;

    /* Number of samples is quadrupled, because of 16-bit and stereo */

    nsamples = len / 4;

    leftptr = (short *) stream;
    rightptr = ((short *) stream) + 1;

    /* Fill the output buffer */

    for (i=0; i<nsamples; ++i)
    {
        /* Has this sound expired? If so, retrieve the next frequency */

        while (current_remaining == 0) 
        {
            phase_offset = 0;

            /* Get the next frequency to play */

            if(pcSound)
            {
                /* The PC speaker sample rate is 140Hz (see SDL_t0SlowAsmService) */
                current_remaining = param_samplerate / 140;

                if(*pcSound!=pcLastSample)
                {
                    pcLastSample=*pcSound;
					
                    if(pcLastSample)
                        /*
                        ** The PC PIC counts down at 1.193180MHz
                        ** So pwm_freq = counter_freq / reload_value
                        ** reload_value = pcLastSample * 60 (see SDL_DoFX)
                        */
                        current_freq = 1193180 / (pcLastSample * 60);
                    else
                        current_freq = 0;
						
                }
                pcSound++;
                pcLengthLeft--;
                if(!pcLengthLeft)
                {
                    pcSound=0;
                    SoundNumber=(soundnames)0;
                    SoundPriority=0;
                }
            }
            else
            {	
                current_freq = 0;
                current_remaining = 1;
            }
        }

        /* Set the value for this sample. */

        if (current_freq == 0)
        {
            /* Silence */

            this_value = 0;
        }
        else 
        {
            int frac;

            /*
            ** Determine whether we are at a peak or trough in the current
            ** sound.  Multiply by 2 so that frac % 2 will give 0 or 1 
            ** depending on whether we are at a peak or trough.
            */
            frac = (phase_offset * current_freq * 2) / param_samplerate;

            if ((frac % 2) == 0) 
            {
                this_value = SQUARE_WAVE_AMP;
            }
            else
            {
                this_value = -SQUARE_WAVE_AMP;
            }

            ++phase_offset;
        }

        --current_remaining;

        /* Use the same value for the left and right channels. */

        *leftptr += this_value;
        *rightptr += this_value;

        leftptr += 2;
        rightptr += 2;
    }
}

void
SD_StopDigitized(void)
{
    DigiPlaying = false;
    DigiNumber = (soundnames) 0;
    DigiPriority = 0;
    SoundPositioned = false;
    if ((DigiMode == sds_PC) && (SoundMode == sdm_PC))
        SDL_SoundFinished();

    switch (DigiMode)
    {
        case sds_PC:
            SDL_PCStopSound();
            break;
        case sds_SoundBlaster:
            Mix_HaltChannel(-1);
            break;
        default:
            break;       
    }
}

int SD_GetChannelForDigi(int which)
{
	int channel;
    if(DigiChannel[which] != -1) return DigiChannel[which];

    channel = Mix_GroupAvailable(1);
    if(channel == -1) channel = Mix_GroupOldest(1);
    if(channel == -1)           /* All sounds stopped in the meantime? */
        return Mix_GroupAvailable(1);
    return channel;
}


void SD_SetPosition(int channel, int leftpos, int rightpos)
{
    if((leftpos < 0) || (leftpos > 15) || (rightpos < 0) || (rightpos > 15)
            || ((leftpos == 15) && (rightpos == 15)))
        Quit("SD_SetPosition: Illegal position");

    switch (DigiMode)
    {
        case sds_SoundBlaster:
            Mix_SetPanning(channel, 255 - (leftpos * 28), 255 - (rightpos * 28));
            break;
        default:
            break;       
    }
}

#ifndef USE_AUDIO_CVT
static short GetSample(float csample, unsigned char *samples, int size)
{
    float s0=0, s1=0, s2=0;
    int cursample = (int) csample;
    float sf = csample - (float) cursample;
	float val;
	int intval;
    if(cursample-1 >= 0) s0 = (float) (samples[cursample-1] - 128);
    s1 = (float) (samples[cursample] - 128);
    if(cursample+1 < size) s2 = (float) (samples[cursample+1] - 128);

    val = s0*sf*(sf-1)/2 - s1*(sf*sf-1) + s2*(sf+1)*sf/2;
    intval = (int) (val * 256);
    if(intval < -32768) intval = -32768;
    else if(intval > 32767) intval = 32767;
    return (short) intval;
}
#endif
#endif

void SD_PrepareSound(int which)
{
#ifdef SEGA_SATURN
    char filename[15];
    unsigned char* mem_buf;
#ifndef USE_ADX	
    sprintf(filename, "%03d.PCM", which);
#else	
    sprintf(filename, "%03d.ADX", which);
#endif


#ifdef PONY
    /*	if(fileId>0)  */
    {


        /*		
        **      if(which <23)
        **		if(fileSize>8192 && fileSize<20000)
        */
        {
#ifndef USE_ADX				
            /*			load_8bit_pcm((Sint8*)filename, ORIGSAMPLERATE);           */
#else
            load_adx((char*)filename);
#endif			
        }
    }
#else
    Sint32 fileId;
    long fileSize;

    fileId = GFS_NameToId((char*)filename);

    fileSize = GetFileSize(fileId);

    if (fileId > 0)
    {
        fileSize = GetFileSize(fileId);

        if (which < 23)
            /*		if(fileSize>8192 && fileSize<20000)       */
        {
            mem_buf = SafeMalloc(fileSize);
            /* CHECKMALLOCRESULT(mem_buf); */
        }
        else
        {
            mem_buf = lowsound;
            lowsound += (size_t)fileSize;

            if (lowsound % 4 != 0)
                lowsound = (lowsound + (4 - 1)) & -4;
        }

        GFS_Load(fileId, 0, mem_buf, fileSize);
        SoundChunks[which] = (Mix_Chunk*)malloc(sizeof(Mix_Chunk));

        SoundChunks[which]->abuf = mem_buf;
        SoundChunks[which]->alen = fileSize;

        if (fileSize < 0x900)
            SoundChunks[which]->alen = 0x900;
    }
    else
    {
        SoundChunks[which]->alen = 0;
    }
#endif	
#else
#ifdef USE_AUDIO_CVT
    SDL_AudioCVT cvt;
    unsigned int page;
    unsigned int size;
    unsigned char* origsamples;
    Mix_Chunk* chunk;

    if (DigiList == NULL)
        Quit("SD_PrepareSound(%i): DigiList not initialized!\n", which);

    origsamples = PM_GetSoundPage(page);
    if (origsamples + size >= PM_GetPageEnd())
        Quit("SD_PrepareSound(%i): Sound reaches out of page file!\n", which);

    if (SDL_BuildAudioCVT(&cvt,
        AUDIO_U8, 1, ORIGSAMPLERATE,
        mix_format, mix_channels, param_samplerate) < 0)
    {
        Quit("SDL_BuildAudioCVT: %s\n", SDL_GetError());
    }

    cvt.len = (int)size;
    cvt.buf = (Uint8*)malloc(cvt.len * cvt.len_mult);
    /* [FG] clear buffer(cvt.len * cvt.len_mult >= cvt.len_cvt) */
    memset(cvt.buf, 0, cvt.len * cvt.len_mult);
    memcpy(cvt.buf, origsamples, cvt.len);

    if (SDL_ConvertAudio(&cvt) < 0)
    {
        free(cvt.buf);
        Quit("SDL_ConvertAudio: %s\n", SDL_GetError());
    }

    chunk = &SoundChunks[which];
    chunk->allocated = 1;
    chunk->volume = MIX_MAX_VOLUME;
    chunk->abuf = cvt.buf;
    chunk->alen = cvt.len_cvt;
#else
    unsigned int i;
	unsigned int page = DigiList[which].startpage;
	unsigned int size = DigiList[which].length;
	unsigned char *origsamples;
	int destsamples = (int)((float)size * (float)param_samplerate
        / (float)ORIGSAMPLERATE);
	unsigned char *wavebuffer;
	short *newsamples;
	float cursample;
	float samplestep;
	headchunk head = {{'R','I','F','F'}, 0, {'W','A','V','E'},
        {'f','m','t',' '}, 0x10, 0x0001, 1, (unsigned int) param_samplerate, (unsigned int) (param_samplerate*2), 2, 16};
    wavechunk dhead = {{'d', 'a', 't', 'a'}, (unsigned int) (destsamples*2)};
	SDL_RWops* temp;
    if(DigiList == NULL)
        Quit("SD_PrepareSound(%i): DigiList not initialized!\n", which);

    origsamples = PM_GetSoundPage(page);
    if(origsamples + size >= PM_GetPageEnd())
        Quit("SD_PrepareSound(%i): Sound reaches out of page file!\n", which);
    
    wavebuffer = wlcast_conversion(unsigned char*, SafeMalloc(sizeof(headchunk) + sizeof(wavechunk)
        + destsamples * 2));     /* dest are 16-bit samples */

    head.filelenminus8 = sizeof(head) + destsamples*2;  /* (sizeof(dhead)-8 = 0) */
    memcpy(wavebuffer, &head, sizeof(head));
    memcpy(wavebuffer+sizeof(head), &dhead, sizeof(dhead));

    /* 
    ** alignment is correct, as wavebuffer comes from malloc
    ** and sizeof(headchunk) % 4 == 0 and sizeof(wavechunk) % 4 == 0
    */
    newsamples = (short *)(void *) (wavebuffer + sizeof(headchunk)
        + sizeof(wavechunk));
    cursample = 0.F;
    samplestep = (float) ORIGSAMPLERATE / (float) param_samplerate;
    for(i=0; i<(unsigned int)destsamples; i++, cursample+=samplestep)
    {
        newsamples[i] = GetSample((float)size * (float)i / (float)destsamples,
            origsamples, (int)size);
    }

    temp = SDL_RWFromMem(wavebuffer,
        sizeof(headchunk) + sizeof(wavechunk) + destsamples * 2);

    SoundChunks[which] = Mix_LoadWAV_RW(temp, 1);

    free(wavebuffer);
#endif
#endif
}

#ifndef SEGA_SATURN
int SD_PlayDigitized(unsigned short which,int leftpos,int rightpos)
{
    Mix_Chunk *sample;
    int channel;
    if (!DigiMode)
        return 0;

    if (which >= NumDigi)
        Quit("SD_PlayDigitized: bad sound number %i", which);

    channel = SD_GetChannelForDigi(which);
    SD_SetPosition(channel, leftpos,rightpos);

    DigiPlaying = true;
#ifdef USE_AUDIO_CVT
    sample = &SoundChunks[which];
    if(sample->abuf == NULL)
#else
    sample = SoundChunks[which];
    if(sample == NULL)
#endif
    {
        printf("SoundChunks[%i] is NULL!\n", which);
        return 0;
    }

    if(Mix_PlayChannel(channel, sample, 0) == -1)
    {
        printf("Unable to play sound: %s\n", Mix_GetError());
        return 0;
    }

    return channel;
}

void SD_ChannelFinished(int channel)
{
    channelSoundPos[channel].valid = 0;
}


void
SD_SetDigiDevice(SDSMode mode)
{
    boolean devicenotpresent;

    if (mode == DigiMode)
        return;

    SD_StopDigitized();

    devicenotpresent = false;
    switch (mode)
    {
        case sds_SoundBlaster:
            if (!SoundBlasterPresent)
                devicenotpresent = true;
            break;
        default:
            break;       
    }

    if (!devicenotpresent)
    {
        DigiMode = mode;
    }
}
#endif

void
SDL_SetupDigi(void)
{
    /* Correct padding enforced by PM_Startup() */
	int i,page;
    unsigned short *soundInfoPage = (unsigned short *) (void *) PM_GetPage(ChunksInFile-1);
    NumDigi = (unsigned short) PM_GetPageSize(ChunksInFile - 1) / 4;

    DigiList = wlcast_conversion(digiinfo*, SafeMalloc(NumDigi * sizeof(*DigiList)));

    
    for(i = 0; i < NumDigi; i++)
    {
        /* 
        ** Calculate the size of the digi from the sizes of the pages between
        ** the start page and the start page of the next sound
		*/
        int lastPage;
		unsigned int size = 0;
        DigiList[i].startpage = soundInfoPage[i * 2];
        if((int) DigiList[i].startpage >= ChunksInFile - 1)
        {
            NumDigi = i;
            break;
        }

        
        if(i < NumDigi - 1)
        {
            lastPage = soundInfoPage[i * 2 + 2];
            if(lastPage == 0 || lastPage + PMSoundStart > ChunksInFile - 1) lastPage = ChunksInFile - 1;
            else lastPage += PMSoundStart;
        }
        else lastPage = ChunksInFile - 1;

       
        for(page = PMSoundStart + DigiList[i].startpage; page < lastPage; page++)
            size += PM_GetPageSize(page);

        /* Don't include padding of sound info page, if padding was added */
        if(lastPage == ChunksInFile - 1 && PMSoundInfoPagePadded) size--;

        /* Patch lower 16-bit of size with size from sound info page. */
        /*
        ** The original VSWAP contains padding which is included in the page size,
        ** but not included in the 16-bit size. So we use the more precise value.
        */
        if((size & 0xffff0000) != 0 && (size & 0xffff) < soundInfoPage[i * 2 + 1])
            size -= 0x10000;
        size = (size & 0xffff0000) | soundInfoPage[i * 2 + 1];

        DigiList[i].length = size;
    }

    for(i = 0; i < LASTSOUND; i++)
    {
        DigiMap[i] = -1;
        DigiChannel[i] = -1;
    }
}

/*      AdLib Code       */
#ifndef SEGA_SATURN
/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_ALStopSound() - Turns off any sound effects playing through the
//              AdLib card
//
///////////////////////////////////////////////////////////////////////////
*/

static void
SDL_ALStopSound(void)
{
    alSound = 0;
    alOut(alFreqH + 0, 0);
}

static void
SDL_AlSetFXInst(Instrument *inst)
{
    unsigned char c,m;
    m = 0;      /* modulator cell for channel 0 */
    c = 3;      /* carrier cell for channel 0 */
    alOut(m + alChar,inst->mChar);
    alOut(m + alScale,inst->mScale);
    alOut(m + alAttack,inst->mAttack);
    alOut(m + alSus,inst->mSus);
    alOut(m + alWave,inst->mWave);
    alOut(c + alChar,inst->cChar);
    alOut(c + alScale,inst->cScale);
    alOut(c + alAttack,inst->cAttack);
    alOut(c + alSus,inst->cSus);
    alOut(c + alWave,inst->cWave);

    /* Note: Switch commenting on these lines for old MUSE compatibility */
/*    alOutInIRQ(alFeedCon,inst->nConn);      */
    alOut(alFeedCon,0);
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_ALPlaySound() - Plays the specified sound on the AdLib card
//
///////////////////////////////////////////////////////////////////////////
*/

static void
SDL_ALPlaySound(AdLibSound *sound)
{
    Instrument      *inst;
    unsigned char            *data;

    SDL_ALStopSound();

    alLengthLeft = sound->common.length;
    data = sound->data;
    alBlock = ((sound->block & 7) << 2) | 0x20;
    inst = &sound->inst;

    if (!(inst->mSus | inst->cSus))
    {
        Quit("SDL_ALPlaySound() - Bad instrument");
    }

    SDL_AlSetFXInst(inst);
    alSound = (unsigned char *)data;
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_ShutAL() - Shuts down the AdLib card for sound effects
//
///////////////////////////////////////////////////////////////////////////
*/

static void
SDL_ShutAL(void)
{
    alSound = 0;
    alOut(alEffects,0);
    alOut(alFreqH + 0,0);
    SDL_AlSetFXInst(&alZeroInst);
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_StartAL() - Starts up the AdLib card for sound effects
//
///////////////////////////////////////////////////////////////////////////
*/

static void
SDL_StartAL(void)
{
    alOut(alEffects, 0);
    SDL_AlSetFXInst(&alZeroInst);
}

/*
////////////////////////////////////////////////////////////////////////////
//
//      SDL_ShutDevice() - turns off whatever device was being used for sound fx
//
////////////////////////////////////////////////////////////////////////////
*/

static void
SDL_ShutDevice(void)
{
    switch (SoundMode)
    {
        case sdm_PC:
            SDL_ShutPC();
            break;
        case sdm_AdLib:
            SDL_ShutAL();
            break;
        default:
            break;       
    }
    SoundMode = sdm_Off;
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SDL_StartDevice() - turns on whatever device is to be used for sound fx
//
///////////////////////////////////////////////////////////////////////////
*/

static void
SDL_StartDevice(void)
{
    switch (SoundMode)
    {
        case sdm_AdLib:
            SDL_StartAL();
            break;
        default:
            break;        
    }
    SoundNumber = (soundnames) 0;
    SoundPriority = 0;
}

/*      Public routines        */

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_SetSoundMode() - Sets which sound hardware to use for sound effects
//
///////////////////////////////////////////////////////////////////////////
*/

boolean
SD_SetSoundMode(SDMode mode)
{
    boolean result = false;
    unsigned short    tableoffset;

    SD_StopSound();

    if ((mode == sdm_AdLib) && !AdLibPresent)
        mode = sdm_PC;

    switch (mode)
    {
        case sdm_Off:
            tableoffset = STARTADLIBSOUNDS;
            result = true;
            break;
        case sdm_PC:
            tableoffset = STARTPCSOUNDS;
            result = true;
            break;
        case sdm_AdLib:
            tableoffset = STARTADLIBSOUNDS;
            if (AdLibPresent)
                result = true;
            break;
        default:
            Quit("SD_SetSoundMode: Invalid sound mode %i", mode);
            return false;
    }
    SoundTable = &audiosegs[tableoffset];

    if (result && (mode != SoundMode))
    {
        SDL_ShutDevice();
        SoundMode = mode;
        SDL_StartDevice();
    }

    return(result);
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_SetMusicMode() - sets the device to use for background music
//
///////////////////////////////////////////////////////////////////////////
*/

boolean
SD_SetMusicMode(SMMode mode)
{
    boolean result = false;

    SD_FadeOutMusic();
    while (SD_MusicPlaying())
        SDL_Delay(5);

    switch (mode)
    {
        case smm_Off:
            result = true;
            break;
        case smm_AdLib:
            if (AdLibPresent)
                result = true;
            break;
        default:
            break;        
    }

    if (result)
        MusicMode = mode;

    return(result);
}
#endif 

int numreadysamples = 0;
unsigned char* curAlSoundPtr = 0;
unsigned int curAlLengthLeft = 0;
int soundTimeCounter = 5;
int samplesPerMusicTick;
unsigned char* curAlSound = 0;
void SDL_IMFMusicPlayer(void *udata, unsigned char *stream, int len)
{
    int stereolen = len>>1;
    int sampleslen = stereolen>>1;
    short *stream16 = (short *) (void *) stream;    /* expect correct alignment */

    while(1)
    {
        if(numreadysamples)
        {
            if(numreadysamples<sampleslen)
            {
#if defined(USE_DOSBOX) || defined(USE_NUKEDOPL)
                YM3812UpdateOne(&oplChip, stream16, numreadysamples);
#else
                YM3812UpdateOne(oplChip, stream16, numreadysamples);
#endif
                stream16 += numreadysamples*2;
                sampleslen -= numreadysamples;
            }
            else
            {
#if defined(USE_DOSBOX) || defined(USE_NUKEDOPL)
                YM3812UpdateOne(&oplChip, stream16, sampleslen);
#else
                YM3812UpdateOne(oplChip, stream16, sampleslen);
#endif
                numreadysamples -= sampleslen;
                return;
            }
        }
        soundTimeCounter--;
        if(!soundTimeCounter)
        {
            soundTimeCounter = 5;
            if(curAlSound != alSound)
            {
                curAlSound = curAlSoundPtr = alSound;
                curAlLengthLeft = alLengthLeft;
            }
            if(curAlSound)
            {
                if(*curAlSoundPtr)
                {
                    alOut(alFreqL, *curAlSoundPtr);
                    alOut(alFreqH, alBlock);
                }
                else alOut(alFreqH, 0);
                curAlSoundPtr++;
                curAlLengthLeft--;
                if(!curAlLengthLeft)
                {
                    curAlSound = alSound = 0;
                    SoundNumber = (soundnames) 0;
                    SoundPriority = 0;
                    alOut(alFreqH, 0);
                }
            }
        }
        if(sqActive)
        {
            do
            {
                if(sqHackTime > alTimeCount) break;
                sqHackTime = alTimeCount + *(sqHackPtr+1);
                alOut(*(unsigned char *) sqHackPtr, *(((unsigned char *) sqHackPtr)+1));
                sqHackPtr += 2;
                sqHackLen -= 4;
            }
            while(sqHackLen>0);
            alTimeCount++;
            if(!sqHackLen)
            {
                sqHackPtr = sqHack;
                sqHackLen = sqHackSeqLen;
                sqHackTime = 0;
                alTimeCount = 0;
            }
        }
        numreadysamples = samplesPerMusicTick;
    }
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_Startup() - starts up the Sound Mgr
//              Detects all additional sound hardware and installs my ISR
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_Startup(void)
{
#ifdef SEGA_SATURN
 /* AdLibPresent = false; */
    SoundBlasterPresent = true;

        /* 
        ** alTimeCount = 0;
        **
        ** SD_SetSoundMode(sdm_PC);
        ** SD_SetMusicMode(sdm_PC);
        */
    SDL_AudioSpec desired, obtained;
    desired.freq = ORIGSAMPLERATE;
    desired.format = AUDIO_U8;
    desired.channels = 1;
    /*  
    **   desired.samples = 1024;
    **   desired.userdata = NULL;
    */
    SDL_OpenAudio(&desired, &obtained);
#else
    int     i;

    if (SD_Started)
        return;
#if SDL_MAJOR_VERSION == 1 || SDL_MAJOR_VERSION == 2
    if (Mix_OpenAudio(param_samplerate, AUDIO_S16, 2, param_audiobuffer) < 0)
#else
    if(Mix_OpenAudio(param_samplerate, SDL_AUDIO_S16, 2, param_audiobuffer) < 0)
#endif
    {
        printf("Unable to open audio: %s\n", Mix_GetError());
        return;
    }

    Mix_ReserveChannels(2);  /* reserve player and boss weapon channels */
    Mix_GroupChannels(2, MIX_CHANNELS-1, 1); /* group remaining channels */

    /* Init music */

    samplesPerMusicTick = param_samplerate / 700;    /* SDL_t0FastAsmService played at 700Hz */
    if(YM3812Init(1,3579545,param_samplerate))
    {
        printf("Unable to create virtual OPL!!\n");
    }

    for(i=1;i<0xf6;i++)
        alOut(i,0);
    alOut(1,0x20); /* Set WSE=1 */
    /* YM3812Write(0,8,0); /* Set CSM=0 & SEL=0	*/	 /* already set in for statement */
    Mix_HookMusic(SDL_IMFMusicPlayer, 0);
    Mix_ChannelFinished(SD_ChannelFinished);
    AdLibPresent = true;
    SoundBlasterPresent = true;

    alTimeCount = 0;
	
    /* Add PC speaker sound mixer */
    Mix_RegisterEffect(MIX_CHANNEL_POST, SDL_PCMixCallback, NULL, NULL);

    SD_SetSoundMode(sdm_Off);
    SD_SetMusicMode(smm_Off);

    SDL_SetupDigi();

    SD_Started = true;
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_Shutdown() - shuts down the Sound Mgr
//              Removes sound ISR and turns off whatever sound hardware was active
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_Shutdown(void)
{
#ifndef SEGA_SATURN
    int i;

    if (!SD_Started)
        return;

    SD_MusicOff();
    SD_StopSound();

    for(i = 0; i < STARTMUSIC - STARTDIGISOUNDS; i++)
    {
#ifdef USE_AUDIO_CVT
        if(SoundChunks[i].abuf) free(SoundChunks[i].abuf);
        memset(&SoundChunks[i], 0, sizeof(SoundChunks[i]));
#else
        if(SoundChunks[i]) Mix_FreeChunk(SoundChunks[i]);
#endif
    }

    free (DigiList);
    DigiList = NULL;

    SD_Started = false;
#endif
}

#ifndef SEGA_SATURN

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_PositionSound() - Sets up a stereo imaging location for the next
//              sound to be played. Each channel ranges from 0 to 15.
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_PositionSound(int leftvol,int rightvol)
{
    LeftPosition = leftvol;
    RightPosition = rightvol;
    nextsoundpos = true;
}
#endif

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_PlaySound() - plays the specified sound on the appropriate hardware
//
///////////////////////////////////////////////////////////////////////////
*/

boolean
SD_PlaySound(soundnames sound)
{
#ifdef SEGA_SATURN
    /* slPrint("SD_PlaySound",slLocate(10,9)); */		
    /* slPrintHex(sound,slLocate(24,9)); */	
#ifdef PONY
#ifndef USE_ADX	
    if (Mix_PlayChannel(DigiMap[sound], NULL, 0) == -1)
#else
    return Mix_PlayChannel(sound, NULL, 0);
#endif
#else
    Mix_Chunk* sample = SoundChunks[DigiMap[sound]];	 /* DigiMap[sound] */
    if (Mix_PlayChannel(0, sample, 0) == -1)
#endif	
#else
    boolean         ispos;
    SoundCommon     *s;
    int             lp,rp;

    lp = LeftPosition;
    rp = RightPosition;
    LeftPosition = 0;
    RightPosition = 0;

    ispos = nextsoundpos;
    nextsoundpos = false;

    if (sound == (soundnames)-1  || (DigiMode == sds_Off && SoundMode == sdm_Off))
        return 0;

    s = (SoundCommon *) SoundTable[sound];

    if ((SoundMode != sdm_Off) && !s)
            Quit("SD_PlaySound() - Uncached sound");

    if ((DigiMode != sds_Off) && (DigiMap[sound] != -1))
    {
        if ((DigiMode == sds_PC) && (SoundMode == sdm_PC))
        {
            if (s->priority < SoundPriority)
                return 0;

            SDL_PCStopSound();

            SD_PlayDigitized(DigiMap[sound],lp,rp);
            SoundPositioned = ispos;
            SoundNumber = sound;
            SoundPriority = s->priority;
        }
        else
        {
#ifdef NOTYET
            if (s->priority < DigiPriority)
                return(false);
#endif
            int channel = SD_PlayDigitized(DigiMap[sound], lp, rp);
            SoundPositioned = ispos;
            DigiNumber = sound;
            DigiPriority = s->priority;
            return channel + 1;
        }

        return(true);
    }

    if (SoundMode == sdm_Off)
        return 0;

    if (!s->length)
        Quit("SD_PlaySound() - Zero length sound");
    if (s->priority < SoundPriority)
        return 0;

    switch (SoundMode)
    {
        case sdm_PC:
            SDL_PCPlaySound((PCSound *)s);
            break;
        case sdm_AdLib:
            curAlSound = alSound = 0;                /* ADDEDFIX: Tricob */
            alOut(alFreqH, 0);
            SDL_ALPlaySound((AdLibSound *)s);
            break;
        default:
            break;        
    }

    SoundNumber = sound;
    SoundPriority = s->priority;

    return 0;
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_SoundPlaying() - returns the sound number that's playing, or 0 if
//              no sound is playing
//
///////////////////////////////////////////////////////////////////////////
*/

unsigned short
SD_SoundPlaying(void)
{
#ifdef SEGA_SATURN
#ifdef PONY	

#else
    unsigned char i;
    for (i = 0; i < 4; i++)
    {
        if (slPCMStat(&m_dat[i]))
        {
                        /* slSndFlush(); */
                        /* slSynch(); */ /* vbt remis 26/05 */ /* necessaire sinon reste planré à la fin du niveau */
            return true;
        }
    }
    /* slPrintHex(SoundMode,slLocate(10,3)); */
#endif

    return false;
#else
    boolean result = false;

    switch (SoundMode)
    {
        case sdm_PC:
            result = pcSound? true : false;
            break;
        case sdm_AdLib:
            result = alSound? true : false;
            break;
        default:
            break;        
    }

    if (result)
        return(SoundNumber);
    else
        return(false);
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_StopSound(void)
{
#ifndef SEGA_SATURN
    if (DigiPlaying)
        SD_StopDigitized();

    switch (SoundMode)
    {
        case sdm_PC:
            SDL_PCStopSound();
            break;
        case sdm_AdLib:
            SDL_ALStopSound();
            break;
        default:
            break;        
    }

    SoundPositioned = false;

    SDL_SoundFinished();
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_WaitSoundDone() - waits until the current sound is done playing
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_WaitSoundDone(void)
{
#ifndef SEGA_SATURN
    while (SD_SoundPlaying())
        SDL_Delay(5);
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_MusicOn() - turns on the sequencer
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_MusicOn(void)
{
#ifndef SEGA_SATURN
    sqActive = true;
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_MusicOff() - turns off the sequencer and any playing notes
//      returns the last music offset for music continue
//
///////////////////////////////////////////////////////////////////////////
*/

int
SD_MusicOff(void)
{
#ifndef SEGA_SATURN
    unsigned short    i;

    sqActive = false;
    switch (MusicMode)
    {
        case smm_AdLib:
            alOut(alEffects, 0);
            for (i = 0;i < sqMaxTracks;i++)
                alOut(alFreqH + i + 1, 0);
            break;
        default:
            break;    
    }

    return (int) (sqHackPtr-sqHack);
#else
    return 0;
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_StartMusic() - starts playing the music pointed to
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_StartMusic(int chunk)
{
    SD_MusicOff();

    if (MusicMode == smm_AdLib)
    {
        int chunkLen = CA_CacheAudioChunk(chunk);
        sqHack = (unsigned short *)(void *) audiosegs[chunk];     /* alignment is correct */
        if(*sqHack == 0) sqHackLen = sqHackSeqLen = chunkLen;
        else sqHackLen = sqHackSeqLen = *sqHack++;
        sqHackPtr = sqHack;
        sqHackTime = 0;
        alTimeCount = 0;
        SD_MusicOn();
    }
}

void
SD_ContinueMusic(int chunk, int startoffs)
{
#ifndef SEGA_SATURN
    SD_MusicOff();

    if (MusicMode == smm_AdLib)
    {
        int chunkLen = CA_CacheAudioChunk(chunk);
        int i;
        sqHack = (unsigned short*)(void *) audiosegs[chunk];     /* alignment is correct */
        if(*sqHack == 0) sqHackLen = sqHackSeqLen = chunkLen;
        else sqHackLen = sqHackSeqLen = *sqHack++;
        sqHackPtr = sqHack;

        if (startoffs >= sqHackLen)
        {
            startoffs = 0; /* ADDEDFIX: Andy, improved by Chris Chokan */
        }
/*
        else
        {
            Quit("SD_StartMusic: Illegal startoffs provided!");
        }
*/
        /* fast forward to correct position */
        /* (needed to reconstruct the instruments) */

        for(i = 0; i < startoffs; i += 2)
        {
            unsigned char reg = *(unsigned char *)sqHackPtr;
            unsigned char val = *(((unsigned char *)sqHackPtr) + 1);
            if(reg >= 0xb1 && reg <= 0xb8) val &= 0xdf;           /* disable play note flag */
            else if(reg == 0xbd) val &= 0xe0;                     /* disable drum flags */

            alOut(reg,val);
            sqHackPtr += 2;
            sqHackLen -= 4;
        }
        sqHackTime = 0;
        alTimeCount = 0;

        SD_MusicOn();
    }
#else
    satPlayMusic(chunk);
#endif
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_FadeOutMusic() - starts fading out the music. Call SD_MusicPlaying()
//              to see if the fadeout is complete
//
///////////////////////////////////////////////////////////////////////////
*/

void
SD_FadeOutMusic(void)
{
    switch (MusicMode)
    {
        case smm_AdLib:
            /* DEBUG - quick hack to turn the music off */
            SD_MusicOff();
            break;
        default: 
            break;   
    }
}

/*
///////////////////////////////////////////////////////////////////////////
//
//      SD_MusicPlaying() - returns true if music is currently playing, false if
//              not
//
///////////////////////////////////////////////////////////////////////////
*/

boolean
SD_MusicPlaying(void)
{
    boolean result;

    switch (MusicMode)
    {
        case smm_AdLib:
            result = sqActive;
            break;
        default:
            result = false;
            break;
    }

    return(result);
}
#endif
