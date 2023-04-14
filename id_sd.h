//
//      ID Engine
//      ID_SD.h - Sound Manager Header
//      Version for Wolfenstein
//      By Jason Blochowiak
//

#ifndef __ID_SD_H_
#define __ID_SD_H_
#include "version.h"

#ifndef VIEASM
#if defined(USE_DOSBOX) || defined(USE_NUKEDOPL)
#define alOut(n,b) YM3812Write(&oplChip, n, b)
#else
#define alOut(n,b) YM3812Write(oplChip, n, b)
#endif


#define TickBase        70      // 70Hz per tick - used as a base for timer 0

typedef enum
{
    sdm_Off,
    sdm_PC,
#ifndef SEGA_SATURN
    sdm_AdLib,
#endif
} SDMode;

typedef enum
{
    smm_Off,
    smm_AdLib
} SMMode;

typedef enum
{
    sds_Off,
    sds_PC,
    sds_SoundBlaster
} SDSMode;

#ifndef SEGA_SATURN
typedef struct
{
    unsigned int        length;
    unsigned short            priority;
} SoundCommon;
#endif
#define ORIG_SOUNDCOMMON_SIZE 6

//      PC Sound stuff
#define pcTimer         0x42
#define pcTAccess       0x43
#define pcSpeaker       0x61

#define pcSpkBits       3

#ifndef SEGA_SATURN
typedef struct
{
    SoundCommon     common;
    unsigned char            data[1];
} PCSound;
#endif

//      Register addresses
// Operator stuff
#define alChar          0x20
#define alScale         0x40
#define alAttack        0x60
#define alSus           0x80
#define alWave          0xe0
// Channel stuff
#define alFreqL         0xa0
#define alFreqH         0xb0
#define alFeedCon       0xc0
// Global stuff
#define alEffects       0xbd

#ifndef SEGA_SATURN
typedef struct
{
    unsigned char    mChar,cChar,
            mScale,cScale,
            mAttack,cAttack,
            mSus,cSus,
            mWave,cWave,
            nConn,

            // These are only for Muse - these bytes are really unused
            voice,
            mode;
    unsigned char    unused[3];
} Instrument;
#endif
#define ORIG_INSTRUMENT_SIZE 16

#ifndef SEGA_SATURN
typedef struct
{
    SoundCommon     common;
    Instrument      inst;
    unsigned char            block;
    unsigned char            data[1];
} AdLibSound;

#define ORIG_ADLIBSOUND_SIZE (ORIG_SOUNDCOMMON_SIZE + ORIG_INSTRUMENT_SIZE + 2)
#endif
//
//      Sequencing stuff
//
#define sqMaxTracks     10

#ifndef SEGA_SATURN
typedef struct
{
    unsigned short    length;
    unsigned short    values[1];
} MusicGroup;

typedef struct
{
    int valid;
    fixed globalsoundx, globalsoundy;
} globalsoundpos;
#endif
typedef struct
{
    unsigned int startpage;
    unsigned int length;
} digiinfo;

#ifndef SEGA_SATURN
extern globalsoundpos channelSoundPos[];
#endif

// Global variables
extern  boolean         
#ifndef SEGA_SATURN
                        AdLibPresent,
#endif     
                        SoundBlasterPresent,
                        SoundPositioned;
extern  SDMode          SoundMode;
extern  SDSMode         DigiMode;
#ifndef SEGA_SATURN
extern  SMMode          MusicMode;
#endif
extern  unsigned short            NumDigi;
extern  digiinfo        *DigiList;
#if !defined(USE_ADX) && !defined(SEGA_SATURN)
extern  int             DigiMap[];
#endif
#ifndef SEGA_SATURN
extern  int             DigiChannel[];
#endif


// Function prototypes
void            Delay (unsigned int wolfticks);

extern  void    SD_Startup(void),
                SD_Shutdown(void);

extern  int     SD_GetChannelForDigi(int which);
#ifndef SEGA_SATURN
extern  void    SD_PositionSound(int leftvol,int rightvol);
#endif
extern  boolean SD_PlaySound(soundnames sound);
#ifndef SEGA_SATURN
extern  void    SD_SetPosition(int channel, int leftvol,int rightvol);
#endif
extern  void    SD_StopSound(void),
                SD_WaitSoundDone(void);

extern  void    SD_StartMusic(int chunk);
extern  void    SD_ContinueMusic(int chunk, int startoffs);
extern  void    SD_MusicOn(void),
                SD_FadeOutMusic(void);
extern  int     SD_MusicOff(void);

extern  boolean SD_MusicPlaying(void);
#ifndef SEGA_SATURN
extern  boolean SD_SetSoundMode(SDMode mode);
extern  boolean SD_SetMusicMode(SMMode mode);
#endif
extern  unsigned short    SD_SoundPlaying(void);

#ifndef SEGA_SATURN
extern  void    SD_SetDigiDevice(SDSMode);
#endif
extern  void	SD_PrepareSound(int which);
extern  int     SD_PlayDigitized(unsigned short which,int leftpos,int rightpos);
extern  void    SD_StopDigitized(void);
#endif
#endif