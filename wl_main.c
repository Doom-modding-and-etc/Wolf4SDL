/* WL_MAIN.C */

#ifdef _MSC_VER
    #include <io.h>
#else
    #include <unistd.h>
#endif

#include "wl_def.h"
#include "wl_atmos.h"
#ifndef __APPLE__
#include <SDL_syswm.h>
#endif

#ifdef _arch_dreamcast
#include <SDL.h>
#include <SDL_dreamcast.h>
#endif

/*
=============================================================================

                             WOLFENSTEIN 3-D

                        An Id Software production

                             by John Carmack

=============================================================================
*/

extern unsigned char signon[];

/*
=============================================================================

                             LOCAL CONSTANTS

=============================================================================
*/


#define FOCALLENGTH     (0x5700l)               /* in global coordinates */
#define VIEWGLOBAL      0x10000                 /* globals visable flush to wall */

#define VIEWWIDTH       256                     /* size of view window */
#define VIEWHEIGHT      144

/*
=============================================================================

                            GLOBAL VARIABLES

=============================================================================
*/
#ifdef EXTRACONTROLS
boolean     mousemoveenabled;
#endif /* EXTRACONTROLS */

#ifdef SWITCH
PadState pad;
#endif

char    str[80];
int     dirangle[9] = {0,ANGLES/8,2*ANGLES/8,3*ANGLES/8,4*ANGLES/8,
                       5*ANGLES/8,6*ANGLES/8,7*ANGLES/8,ANGLES};

/*
** proejection variables
*/
fixed    focallength;
unsigned int screenofs;
int      viewscreenx, viewscreeny;
int      viewwidth;
int      viewheight;
short    centerx,centery;
int      shootdelta;           /* pixels away from centerx a target can be */
fixed    scale;
int  heightnumerator;


void    Quit (const char *error,...);

boolean startgame;
boolean loadedgame;
int     mouseadjustment;
#ifdef VIEASM
unsigned char    soundvol, musicvol;
boolean reversestereo;
#endif
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SEGA_SATURN) || defined(PSVITA) || defined(ZIPIT_Z2)
char configdir[256] = DATADIR;
#else
char    configdir[256] = "";
#endif
char    configname[13] = "config.";

/*
** Command line parameter variables
*/
boolean param_debugmode = false;
boolean param_nowait = false;
int     param_difficulty = 1;           /* default is "normal" */
int     param_tedlevel = -1;            /* default is not to start a level */
int     param_joystickindex = 0;

#if defined(_arch_dreamcast)
int     param_joystickhat = 0;
unsigned int     param_samplerate = 11025;       /* higher samplerates result in "out of memory" */
int     param_audiobuffer = 1024;
#elif defined(GP2X_940)
int     param_joystickhat = -1;
unsigned int     param_samplerate = 11025;       /* higher samplerates result in "out of memory" */
int     param_audiobuffer = 128;
#elif defined(SWITCH)
int     param_joystickhat = -1;
unsigned int     param_samplerate = 44100;
int     param_audiobuffer = 2048 / 44100 / param_samplerate;
#elif defined (N3DS)
int     param_joystickhat = -1;
unsigned int     param_samplerate = 44100;
int     param_audiobuffer = 0 / (44100 / param_samplerate);
#else
int     param_joystickhat = -1;
unsigned int     param_samplerate = 48000;
int     param_audiobuffer = 2048;
#endif

int     param_mission = 0;
boolean param_goodtimes = false;
boolean param_ignorenumchunks = false;

#ifndef GOODTIMES
#ifndef SPEARDEMO
extern void CopyProtection(void);
#endif
#endif

/*
=============================================================================

                            LOCAL VARIABLES

=============================================================================
*/


/*
====================
=
= ReadConfig
=
====================
*/

void ReadConfig(void)
{
    SDMode  sd;
    SMMode  sm;
    SDSMode sds;

    char* configpath[300];
    int file;

#ifdef _arch_dreamcast
    DC_LoadFromVMU(configname);
#endif

    if(configdir[0])
        w3ssnprintf(configpath, sizeof(configpath), "%s/%s", configdir, configname);
    else
        strcpy(configpath, configname);
#if defined(DEVCPP)
    file = w3sopen(configpath, O_CREAT | O_WRONLY | O_BINARY);
#else
    file = w3sopen(configpath, O_CREAT | O_WRONLY | O_BINARY, 0644);
#endif

    if (file != -1)
    {
        /*
        ** valid config file
        */
        unsigned short tmp;
#ifndef EXTRACONTROLS
		boolean dummyJoypadEnabled;
        boolean dummyJoystickProgressive;
        int dummyJoystickPort = 0;
#else 
        boolean dummyMouseMoveEnabled;
#endif

        w3sread(file,&tmp,sizeof(tmp));
        if(tmp!=0xfefa)
        {
            w3sclose(file);
            goto noconfig;
        }
        w3sread(file,Scores,sizeof(HighScore) * MaxScores);

        w3sread(file,&sd,sizeof(sd));
        w3sread(file,&sm,sizeof(sm));
        w3sread(file,&sds,sizeof(sds));

        w3sread(file,&mouseenabled,sizeof(mouseenabled));
#ifndef EXTRACONTROLS
        w3sread(file,&joystickenabled,sizeof(joystickenabled));
        w3sread(file,&dummyJoypadEnabled,sizeof(dummyJoypadEnabled));
        w3sread(file,&dummyJoystickProgressive,sizeof(dummyJoystickProgressive));
        w3sread(file,&dummyJoystickPort,sizeof(dummyJoystickPort));
#endif
        w3sread(file,dirscan,sizeof(dirscan));
        w3sread(file,buttonscan,sizeof(buttonscan));
        w3sread(file,buttonmouse,sizeof(buttonmouse));
#ifndef EXTRACONTROLS
        w3sread(file,buttonjoy,sizeof(buttonjoy));
#endif
        w3sread(file,&viewsize,sizeof(viewsize));
        w3sread(file,&mouseadjustment,sizeof(mouseadjustment));
        /* 
        ** To keep backwards compatibility with older save files, we try to
        ** detect if there is still info to read from the file. If so, we
        ** assume it's the mouselook setting. If not, we simply close the file.
        */
        if (w3sread(file,&mouselookenabled,sizeof(mouselookenabled)) != sizeof(mouselookenabled))
            printf ("Original Wolf4SDL config file detected (missing mouselook setting)\n");

        if (w3sread(file,&alwaysrunenabled,sizeof(alwaysrunenabled)) != sizeof(alwaysrunenabled))
            printf ("Original Wolf4SDL config file detected (missing alwaysrun setting)\n");		
#ifdef EXTRACONTROLS
        w3sread(file, &mousemoveenabled, sizeof(mousemoveenabled));
        w3sread(file, &dummyMouseMoveEnabled, sizeof(dummyMouseMoveEnabled));
#endif
#ifdef VIEASM
        w3sread(file, &soundvol, sizeof(soundvol));
        w3sread(file, &musicvol, sizeof(musicvol));
        w3sread(file, &reversestereo, sizeof(reversestereo));
#endif
        /* [FG] toggle crosshair */
        if (w3sread(file,&crosshair,sizeof(crosshair))<0)
        {
            crosshair = false;
        }
        w3sclose(file);

        if ((sd == sdm_AdLib || sm == smm_AdLib) && !AdLibPresent
                && !SoundBlasterPresent)
        {
            sd = sdm_PC;
            sm = smm_Off;
        }

        if ((sds == sds_SoundBlaster && !SoundBlasterPresent))
            sds = sds_Off;

        /* make sure values are correct */

        if(mouseenabled)
        {
            mouseenabled = true;
        }

        if(joystickenabled)
        {
            joystickenabled = true;
        }
        if(mouselookenabled) mouselookenabled=true;
        if(alwaysrunenabled) alwaysrunenabled=true;	
		if(crosshair) crosshair = true;
#ifdef EXTRACONTROLS
        if (mousemoveenabled)
        {
            mousemoveenabled = true;
        }
#endif /* EXTRACONTROLS */
        if (!MousePresent)
        {
            mouseenabled = false;
#ifdef EXTRACONTROLS
            mousemoveenabled = false;
#endif
			mouselookenabled = false;
        }
        if (!IN_JoyPresent())
            joystickenabled = false;

        if(mouseadjustment<0) mouseadjustment=0;
        else if(mouseadjustment>9) mouseadjustment=9;

        if(viewsize<4) viewsize=4;
        else if(viewsize>21) viewsize=21;
#ifdef VIEASM
        if (soundvol > 100) soundvol = 100;
        if (musicvol > 100) musicvol = 100;
#endif
        MainMenu[6].active=1;
        MainItems.curpos=0;
    }
    else
    {
        /*
        ** no config file, so select by hardware
        */
noconfig:
        if (SoundBlasterPresent || AdLibPresent)
        {
            sd = sdm_AdLib;
            sm = smm_AdLib;
        }
        else
        {
            sd = sdm_PC;
            sm = smm_Off;
        }

        if (SoundBlasterPresent)
            sds = sds_SoundBlaster;
        else
            sds = sds_Off;

        if (MousePresent)
        {
            mouseenabled = true;
#ifdef EXTRACONTROLS
            mousemoveenabled = true;
#endif
			mouselookenabled = true;
        }
        if (IN_JoyPresent())
            joystickenabled = true;

        viewsize = 19;                          /* start with a good size */
        mouseadjustment=5;
		alwaysrunenabled = false;	
#ifdef VIEASM
        soundvol = 100;
        musicvol = 100;
        reversestereo = false;
#endif
		crosshair = false;
    }

    SD_SetMusicMode (sm);
    SD_SetSoundMode (sd);
    SD_SetDigiDevice (sds);
#ifdef VIEASM
    SD_ChangeVolume((unsigned char)(soundvol * 1.28), (unsigned char)(musicvol * 1.28));
    SD_Reverse(reversestereo);
#endif
}

/*
====================
=
= WriteConfig
=
====================
*/

void WriteConfig(void)
{
    char *configpath[300];
    int file;

#ifdef _arch_dreamcast
    fs_unlink(configname);
#endif

    if(configdir[0])
        w3ssnprintf(configpath, sizeof(configpath), "%s/%s", configdir, configname);
    else
        strcpy(configpath, configname);
#if defined(DEVCPP)
    file = w3sopen((const char*)configpath, O_CREAT | O_WRONLY | O_BINARY);
#else
    file = w3sopen((const char*)configpath, O_CREAT | O_WRONLY | O_BINARY, 0644);
#endif   
    if (file != -1)
    {
        unsigned short tmp=0xfefa;
#ifndef EXTRACONTROLS
		boolean dummyJoypadEnabled = false;
		boolean dummyJoystickProgressive = false;
		int dummyJoystickPort = 0;
#endif
        w3swrite(file,&tmp,sizeof(tmp));
        w3swrite(file,Scores,sizeof(HighScore) * MaxScores);

        w3swrite(file,&SoundMode,sizeof(SoundMode));
        w3swrite(file,&MusicMode,sizeof(MusicMode));
        w3swrite(file,&DigiMode,sizeof(DigiMode));

        w3swrite(file,&mouseenabled,sizeof(mouseenabled));
#ifndef EXTRACONTROLS
        w3swrite(file,&joystickenabled,sizeof(joystickenabled));
        w3swrite(file,&dummyJoypadEnabled,sizeof(dummyJoypadEnabled));
        w3swrite(file,&dummyJoystickProgressive,sizeof(dummyJoystickProgressive));  
        w3swrite(file,&dummyJoystickPort,sizeof(dummyJoystickPort));
#endif
        w3swrite(file,dirscan,sizeof(dirscan));
        w3swrite(file,buttonscan,sizeof(buttonscan));
        w3swrite(file,buttonmouse,sizeof(buttonmouse));
#ifndef EXTRACONTROLS
        w3swrite(file,buttonjoy,sizeof(buttonjoy));
#endif
        w3swrite(file,&viewsize,sizeof(viewsize));
        w3swrite(file,&mouseadjustment,sizeof(mouseadjustment));
        /* 
        ** Mouselookenabled state is saved last, so if this config file is used
        ** with original Wold4SDL, this property is simply ignored.
        */
        w3swrite(file,&mouselookenabled,sizeof(mouselookenabled));
        w3swrite(file,&alwaysrunenabled,sizeof(alwaysrunenabled));
#ifdef EXTRACONTROLS
        w3swrite(file, &mousemoveenabled, sizeof(mousemoveenabled));
#endif /* EXTRACONTROLS */
#ifdef VIEASM
        w3swrite(file, &soundvol, sizeof(soundvol));
        w3swrite(file, &musicvol, sizeof(musicvol));
        w3swrite(file, &reversestereo, sizeof(reversestereo));
#endif
		/* [FG] toggle crosshair */
        w3swrite(file,&crosshair,sizeof(crosshair));
        w3sclose(file);
    }
#ifdef _arch_dreamcast
    DC_SaveToVMU(configname, NULL);
#elif defined(PS2)
    PS2_SaveFile_Memory_Card0(configname);
    PS2_SaveFile_Memory_Card1(configname);
#endif
}


/* =========================================================================== */

/*
=====================
=
= NewGame
=
= Set up new game to start from the beginning
=
=====================
*/

void NewGame (int difficulty,int episode)
{
    memset (&gamestate,0,sizeof(gamestate));
    gamestate.difficulty = difficulty;
    gamestate.weapon = gamestate.bestweapon
            = gamestate.chosenweapon = wp_pistol;
    gamestate.health = 100;
    gamestate.ammo = STARTAMMO;
    gamestate.lives = 3;
    gamestate.nextextra = EXTRAPOINTS;
    gamestate.episode=episode;
#ifdef AUTOMAP
    memset(automap, 0, sizeof(automap));
#endif
    startgame = true;
}

#ifdef MAPCONTROLLEDPLSETT
/********
**
** ResetPlayer
**
***************/

void ResetPlayer(void)
{
    int ammoAmount, healthAmount;
    if (tilemap[63][1] == 0) { /* Start with a specific weapon */
        gamestate.weapon = gamestate.bestweapon = gamestate.chosenweapon = wp_knife;
    }
    else if (tilemap[63][1] == 1) {
        gamestate.weapon = gamestate.bestweapon = gamestate.chosenweapon = wp_pistol;
    }
    else if (tilemap[63][1] == 2) {
        gamestate.weapon = gamestate.bestweapon = gamestate.chosenweapon = wp_machinegun;
    }
    else if (tilemap[63][1] >= 3) {
        gamestate.weapon = gamestate.bestweapon = gamestate.chosenweapon = wp_chaingun;
    }
    ammoAmount = tilemap[63][2]; /* Start with a specific amount of ammo */
    if (ammoAmount > 99) {
        gamestate.ammo = 99;
    }
    else {
        gamestate.ammo = ammoAmount;
    }
    if (tilemap[63][3] == 0) {   /* Start with a certain percentage of health */
        healthAmount = 25;
    }
    else if (tilemap[63][3] == 1) {
        healthAmount = 50;
    }
    else if (tilemap[63][3] == 2) {
        healthAmount = 75;
    }
    else if (tilemap[63][3] >= 3) {
        healthAmount = 100;
    }
    gamestate.health = healthAmount;
}
#endif

/* =========================================================================== */

void DiskFlopAnim(int x,int y)
{
    static char which=0;
    if (!x && !y)
        return;
    VWB_DrawPic(x,y,C_DISKLOADING1PIC+which);
    if (!usedoublebuffering) VL_UpdateScreen(screenBuffer);    /* ADDEDFIX 4 - Chris */
    which^=1;
}


int DoChecksum(unsigned char *source, unsigned int size,int checksum)
{
    unsigned int i;

    for (i=0;i<size-1;i++)
    checksum += source[i]^source[i+1];

    return checksum;
}


/*
==================
=
= SaveTheGame
=
==================
*/

boolean SaveTheGame(FILE *file,int x,int y)
{
    int i,j;
    int checksum;
    unsigned short actnum,laststatobjnum;
    objtype *ob;
    objtype nullobj;
    statobj_t nullstat;

    checksum = 0;

    DiskFlopAnim(x,y);
    fwrite(&gamestate,sizeof(gamestate),1,file);
    checksum = DoChecksum((unsigned char *)&gamestate,sizeof(gamestate),checksum);

    DiskFlopAnim(x,y);
    fwrite(&LevelRatios[0],sizeof(LRstruct)*LRpack,1,file);
    checksum = DoChecksum((unsigned char *)&LevelRatios[0],sizeof(LRstruct)*LRpack,checksum);

    DiskFlopAnim(x,y);
    fwrite(tilemap,sizeof(tilemap),1,file);
    checksum = DoChecksum((unsigned char *)tilemap,sizeof(tilemap),checksum);
#ifdef REVEALMAP
    DiskFlopAnim(x,y);
    fwrite(mapseen,sizeof(mapseen),1,file);
    checksum = DoChecksum((unsigned char *)mapseen,sizeof(mapseen),checksum);
#endif
#ifdef AUTOMAP
    fwrite(automap, sizeof(automap), 1, file);
    checksum = DoChecksum((unsigned char*)automap, sizeof(automap), checksum);
#endif
    DiskFlopAnim(x,y);

    for(i=0;i<mapwidth;i++)
    {
        for(j=0;j<mapheight;j++)
        {
            ob=actorat[i][j];
            if(ISPOINTER(ob))
                actnum=0x8000 | (unsigned short)(ob-objlist);
            else
                actnum=(unsigned short)(uintptr_t)ob;
            fwrite(&actnum,sizeof(actnum),1,file);
            checksum = DoChecksum((unsigned char *)&actnum,sizeof(actnum),checksum);
        }
    }

    fwrite (areaconnect,sizeof(areaconnect),1,file);
    fwrite (areabyplayer,sizeof(areabyplayer),1,file);

    /*
    ** player object needs special treatment as it's in WL_AGENT.CPP and not in
    ** WL_ACT2.CPP which could cause problems for the relative addressing
    */
    ob = player;
    DiskFlopAnim(x,y);
    memcpy(&nullobj,ob,sizeof(nullobj));
    nullobj.state=(statetype *) ((uintptr_t)nullobj.state-(uintptr_t)&s_player);
    fwrite(&nullobj,sizeof(nullobj),1,file);

    DiskFlopAnim(x,y);
    for (ob = ob->next; ob; ob=ob->next)
    {
        memcpy(&nullobj,ob,sizeof(nullobj));
        nullobj.state=(statetype *) ((uintptr_t)nullobj.state-(uintptr_t)&s_grdstand);
        fwrite(&nullobj,sizeof(nullobj),1,file);
    }
    nullobj.active = ac_badobject;          /* end of file marker */
    DiskFlopAnim(x,y);
    fwrite(&nullobj,sizeof(nullobj),1,file);

    DiskFlopAnim(x,y);
    laststatobjnum=(unsigned short) (laststatobj-statobjlist);
    fwrite(&laststatobjnum,sizeof(laststatobjnum),1,file);
    checksum = DoChecksum((unsigned char *)&laststatobjnum,sizeof(laststatobjnum),checksum);

    DiskFlopAnim(x,y);
    for(i=0;i<MAXSTATS;i++)
    {
        memcpy(&nullstat,statobjlist+i,sizeof(nullstat));
        nullstat.visspot=(unsigned char *) ((uintptr_t) nullstat.visspot-(uintptr_t)spotvis);
        fwrite(&nullstat,sizeof(nullstat),1,file);
        checksum = DoChecksum((unsigned char *)&nullstat,sizeof(nullstat),checksum);
    }

    DiskFlopAnim(x,y);
#ifdef BLAKEDOORS
    fwrite(ldoorposition, sizeof(ldoorposition), 1, file);
    checksum = DoChecksum((unsigned char*)ldoorposition, sizeof(ldoorposition), checksum);
    DiskFlopAnim(x, y);
    fwrite(rdoorposition, sizeof(rdoorposition), 1, file);
    checksum = DoChecksum((unsigned char*)rdoorposition, sizeof(rdoorposition), checksum);
#else
    fwrite(doorposition, sizeof(doorposition), 1, file);
    checksum = DoChecksum((unsigned char*)doorposition, sizeof(doorposition), checksum);
#endif
    DiskFlopAnim(x,y);
    fwrite (doorobjlist,sizeof(doorobjlist),1,file);
    checksum = DoChecksum((unsigned char *)doorobjlist,sizeof(doorobjlist),checksum);

    DiskFlopAnim(x,y);
    fwrite (&pwallstate,sizeof(pwallstate),1,file);
    checksum = DoChecksum((unsigned char *)&pwallstate,sizeof(pwallstate),checksum);
    fwrite (&pwalltile,sizeof(pwalltile),1,file);
    checksum = DoChecksum((unsigned char*)&pwalltile,sizeof(pwalltile),checksum);
    fwrite (&pwallx,sizeof(pwallx),1,file);
    checksum = DoChecksum((unsigned char *)&pwallx,sizeof(pwallx),checksum);
    fwrite (&pwally,sizeof(pwally),1,file);
    checksum = DoChecksum((unsigned char *)&pwally,sizeof(pwally),checksum);
    fwrite (&pwalldir,sizeof(pwalldir),1,file);
    checksum = DoChecksum((unsigned char *)&pwalldir,sizeof(pwalldir),checksum);
    fwrite (&pwallpos,sizeof(pwallpos),1,file);
    checksum = DoChecksum((unsigned char *)&pwallpos,sizeof(pwallpos),checksum);

    /*
    ** WRITE OUT CHECKSUM
    */
    fwrite (&checksum,sizeof(checksum),1,file);

    fwrite (&lastgamemusicoffset,sizeof(lastgamemusicoffset),1,file);

    return(true);
}

/* =========================================================================== */

/*
==================
=
= LoadTheGame
=
==================
*/

boolean LoadTheGame(FILE *file,int x,int y)
{
    int i,j;
    int actnum = 0;
    unsigned short laststatobjnum;
    int checksum,oldchecksum;
    objtype nullobj;
    statobj_t nullstat;

    checksum = 0;

    DiskFlopAnim(x,y);
    fread (&gamestate,sizeof(gamestate),1,file);
    checksum = DoChecksum((unsigned char *)&gamestate,sizeof(gamestate),checksum);

    DiskFlopAnim(x,y);
    fread (&LevelRatios[0],sizeof(LRstruct)*LRpack,1,file);
    checksum = DoChecksum((unsigned char *)&LevelRatios[0],sizeof(LRstruct)*LRpack,checksum);

    DiskFlopAnim(x,y);
    SetupGameLevel ();

    DiskFlopAnim(x,y);
    fread (tilemap,sizeof(tilemap),1,file);
    checksum = DoChecksum((unsigned char *)tilemap,sizeof(tilemap),checksum);
#ifdef REVEALMAP
    DiskFlopAnim(x,y);
    fread (mapseen,sizeof(mapseen),1,file);
    checksum = DoChecksum((unsigned char *)mapseen,sizeof(mapseen),checksum);
#endif
    DiskFlopAnim(x,y);

    for(i=0;i<mapwidth;i++)
    {
        for(j=0;j<mapheight;j++)
        {
            fread (&actnum,sizeof(unsigned short),1,file);
            checksum = DoChecksum((unsigned char *) &actnum,sizeof(unsigned short),checksum);
            if(actnum&0x8000)
                actorat[i][j]=objlist+(actnum&0x7fff);
            else
                actorat[i][j]=(objtype *)(uintptr_t) actnum;
        }
    }

    fread (areaconnect,sizeof(areaconnect),1,file);
    fread (areabyplayer,sizeof(areabyplayer),1,file);

    InitActorList ();
    DiskFlopAnim(x,y);
    fread (player,sizeof(*player),1,file);
    player->state=(statetype *) ((uintptr_t)player->state+(uintptr_t)&s_player);

    while (1)
    {
        DiskFlopAnim(x,y);
        fread (&nullobj,sizeof(nullobj),1,file);
        if (nullobj.active == ac_badobject)
            break;
        GetNewActor ();
        nullobj.state=(statetype *) ((uintptr_t)nullobj.state+(uintptr_t)&s_grdstand);
        /* don't copy over the links */
        memcpy (newobj,&nullobj,sizeof(nullobj)-8);
    }

    DiskFlopAnim(x,y);
    fread (&laststatobjnum,sizeof(laststatobjnum),1,file);
    laststatobj=statobjlist+laststatobjnum;
    checksum = DoChecksum((unsigned char *)&laststatobjnum,sizeof(laststatobjnum),checksum);

    DiskFlopAnim(x,y);
    for(i=0;i<MAXSTATS;i++)
    {
        fread(&nullstat,sizeof(nullstat),1,file);
        checksum = DoChecksum((unsigned char *)&nullstat,sizeof(nullstat),checksum);
        nullstat.visspot=(unsigned char *) ((uintptr_t)nullstat.visspot+(uintptr_t)spotvis);
        memcpy(statobjlist+i,&nullstat,sizeof(nullstat));
    }

    DiskFlopAnim(x,y);
#ifdef BLAKEDOORS
    fread(ldoorposition, sizeof(ldoorposition), 1, file);
    checksum = DoChecksum((unsigned char*)ldoorposition, sizeof(ldoorposition), checksum);
    DiskFlopAnim(x, y);
    fread(rdoorposition, sizeof(rdoorposition), 1, file);
    checksum = DoChecksum((unsigned char*)rdoorposition, sizeof(rdoorposition), checksum);
#else
    fread(doorposition, sizeof(doorposition), 1, file);
    checksum = DoChecksum((unsigned char*)doorposition, sizeof(doorposition), checksum);
#endif
    DiskFlopAnim(x,y);
    fread (doorobjlist,sizeof(doorobjlist),1,file);
    checksum = DoChecksum((unsigned char *)doorobjlist,sizeof(doorobjlist),checksum);

    DiskFlopAnim(x,y);
    fread (&pwallstate,sizeof(pwallstate),1,file);
    checksum = DoChecksum((unsigned char *)&pwallstate,sizeof(pwallstate),checksum);
    fread (&pwalltile,sizeof(pwalltile),1,file);
    checksum = DoChecksum((unsigned char *)&pwalltile,sizeof(pwalltile),checksum);
    fread (&pwallx,sizeof(pwallx),1,file);
    checksum = DoChecksum((unsigned char *)&pwallx,sizeof(pwallx),checksum);
    fread (&pwally,sizeof(pwally),1,file);
    checksum = DoChecksum((unsigned char *)&pwally,sizeof(pwally),checksum);
    fread (&pwalldir,sizeof(pwalldir),1,file);
    checksum = DoChecksum((unsigned char *)&pwalldir,sizeof(pwalldir),checksum);
    fread (&pwallpos,sizeof(pwallpos),1,file);
    checksum = DoChecksum((unsigned char *)&pwallpos,sizeof(pwallpos),checksum);

    if (gamestate.secretcount)      /* assign valid floorcodes under moved pushwalls */
    {
        unsigned short *map, *obj; unsigned short tile, sprite;
        map = mapsegs[0]; obj = mapsegs[1];
        for (y=0;y<mapheight;y++)
            for (x=0;x<mapwidth;x++)
            {
                tile = *map++; sprite = *obj++;
                if (sprite == PUSHABLETILE && !tilemap[x][y]
                    && (tile < AREATILE || tile >= (AREATILE+NUMAREAS)))
                {
                    if (*map >= AREATILE)
                        tile = *map;
                    if (*(map-1-mapwidth) >= AREATILE)
                        tile = *(map-1-mapwidth);
                    if (*(map-1+mapwidth) >= AREATILE)
                        tile = *(map-1+mapwidth);
                    if ( *(map-2) >= AREATILE)
                        tile = *(map-2);

                    *(map-1) = tile; *(obj-1) = 0;
                }
            }
    }

    Thrust(0,0);    /* set player->areanumber to the floortile you're standing on */

    fread (&oldchecksum,sizeof(oldchecksum),1,file);

    fread (&lastgamemusicoffset,sizeof(lastgamemusicoffset),1,file);
    if(lastgamemusicoffset<0) lastgamemusicoffset=0;


    if (oldchecksum != checksum)
    {
        Message(STR_SAVECHT1"\n"
                STR_SAVECHT2"\n"
                STR_SAVECHT3"\n"
                STR_SAVECHT4);

        IN_ClearKeysDown();
        IN_Ack();

        gamestate.oldscore = gamestate.score = 0;
        gamestate.lives = 1;
        gamestate.weapon =
            gamestate.chosenweapon =
            gamestate.bestweapon = wp_pistol;
        gamestate.ammo = 8;
    }

    return true;
}

/* =========================================================================== */

/*
==========================
=
= ShutdownId
=
= Shuts down all ID_?? managers
=
==========================
*/

void ShutdownId (void)
{  
#ifdef LWUDPCOMMS
    UDP_shutdown ();
#endif
    US_Shutdown ();         /* This line is completely useless... */
#if defined(SWITCH) || defined (N3DS)
    printf("US_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY("US_Shutdown DONE\n",  4, 20, 20);
#endif
    SD_Shutdown ();
#if defined(SWITCH) || defined (N3DS)
    printf("SD_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY ("SD_Shutdown DONE\n", 4, 20, 20);
#endif    
    PM_Shutdown ();
#if defined(SWITCH) || defined (N3DS)
    printf("PM_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY("PM_Shutdown DONE\n", 4, 20, 20);
#endif    
    IN_Shutdown ();
#if defined(SWITCH) || defined (N3DS)
    printf("IN_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY("IN_Shutdown DONE\n", 4, 20, 20);
#endif        
#if defined(SDL_MAJOR_VERSION) && (SDL_MAJOR_VERSION == 2) && defined(HAPTIC_SUPPORT)
    HAPTIC_Shutdown();
#if defined(SWITCH) || defined (N3DS)
    printf("HAPTIC_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY("HAPTIC_Shutdown DONE\n", 4, 20, 20);
#endif   
#endif
    VL_Shutdown ();
#if defined(SWITCH) || defined (N3DS)
    printf("VL_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY("VL_Shutdown DONE\n", 4, 20, 20);
#endif    
    CA_Shutdown ();
#if defined(SWITCH) || defined (N3DS)
    printf("CA_Shutdown DONE\n");
#elif defined(PS2)
    ps2_printf_XY("CA_Shutdown DONE\n", 4, 20, 20);
#endif    
#if defined(GP2X_940)
    GP2X_Shutdown();
#endif
#if defined(PS2)
    PS2_Shutdown();
    ps2_printf_XY("PS2_Shutdown DONE\n", 4, 20, 20);
#endif
}


/* =========================================================================== */
 
/*
==================
=
= BuildTables
=
= Calculates:
=
= scale                 projection constant
= sintable/costable     overlapping fractional tables
=
==================
*/

const float radtoint = (float)(FINEANGLES/2/M_PI);

void BuildTables (void)
{
	float angle;
    float anglestep;
    /*
    ** calculate fine tangents
    */

    int i;
    for(i=0;i<FINEANGLES/8;i++)
    {
        double tang=tan((i+0.5)/radtoint);
        finetangent[i]=(int)(tang*GLOBAL1);
        finetangent[FINEANGLES/4-1-i]=(int)((1/tang)*GLOBAL1);
    }

    /*
    ** costable overlays sintable with a quarter phase shift
    ** ANGLES is assumed to be divisable by four
    */

    angle=0;
    anglestep=(float)(M_PI/2/ANGLEQUAD);
    for(i=0; i<ANGLEQUAD; i++)
    {
        fixed value=(int)(GLOBAL1*sin(angle));
        sintable[i]=sintable[i+ANGLES]=sintable[ANGLES/2-i]=value;
        sintable[ANGLES-i]=sintable[ANGLES/2+i]=-value;
        angle+=anglestep;
    }
    sintable[ANGLEQUAD] = 65536;
    sintable[3*ANGLEQUAD] = -65536;

#if defined(USE_STARSKY) || defined(USE_RAIN) || defined(USE_SNOW)
    Init3DPoints();
#endif
}

/* =========================================================================== */


/*
====================
=
= CalcProjection
=
= Uses focallength
=
====================
*/

void CalcProjection (int focal)
{
    int     i;

    int     halfview;
    double  facedist;

    focallength = focal;
    facedist = focal+MINDIST;
    halfview = viewwidth/2;                                 /* half view in pixels */

    /*
    ** calculate scale value for vertical height calculations
    ** and sprite x calculations
    */
    scale = (fixed) (halfview*facedist/(VIEWGLOBAL/2));

    /*
    ** divide heightnumerator by a posts distance to get the posts height for
    ** the heightbuffer.  The pixel height is height>>2
    */
    heightnumerator = (int)(TILEGLOBAL*scale)>>6;

    /*
    ** calculate the angle offset from view angle of each pixel's ray
    */

    for (i=0;i<halfview;i++)
    {
        /* start 1/2 pixel over, so viewangle bisects two middle pixels */
        double  tang = (int)i*VIEWGLOBAL/viewwidth/facedist;
        float   angle = (float) atan(tang);
        int intang = (int) (angle*radtoint);
        pixelangle[halfview-1-i] = intang;
        pixelangle[halfview+i] = -intang;
    }
}



/* =========================================================================== */

/*
===================
=
= SetupWalls
=
= Map tile values to scaled pics
=
===================
*/

void SetupWalls (void)
{
    int     i;

    horizwall[0]=0;
    vertwall[0]=0;

    for (i=1;i<MAXWALLTILES;i++)
    {
        horizwall[i]=(i-1)*2;
        vertwall[i]=(i-1)*2+1;
    }
}

/* =========================================================================== */

/*
==========================
=
= SignonScreen
=
==========================
*/

void SignonScreen (void)                        /* VGA version */
{
    VL_SetVGAPlaneMode ();

    VL_MemToScreen (signon,320,200,0,0);
}


/*
==========================
=
= FinishSignon
=
==========================
*/

void FinishSignon (void)
{
#ifndef SPEAR
#ifdef SAVE_GAME_SCREENSHOT
    VL_Bar(0, 189, 300, 11, VL_GetPixel(screen, 0, 0));
#else
    VL_Bar(0, 189, 300, 11, VL_GetPixel(0, 0));
#endif
    WindowX = 0;
    WindowW = 320;
    PrintY = 190;

    #ifndef JAPAN
    SETFONTCOLOR(14,4);

    #ifdef SPANISH
    US_CPrint ("Oprima una tecla");
    #else
    US_CPrint ("Press a key");
    #endif

    #endif

    VL_UpdateScreen(screenBuffer);

    if (!param_nowait)
        IN_Ack ();

    #ifndef JAPAN
#ifdef SAVE_GAME_SCREENSHOT
    VL_Bar(0, 189, 300, 11, VL_GetPixel(screen, 0, 0));
#else
    VL_Bar(0, 189, 300, 11, VL_GetPixel(0, 0));
#endif
    PrintY = 190;
    SETFONTCOLOR(10,4);

    #ifdef SPANISH
    US_CPrint ("pensando...");
    #else
    US_CPrint ("Working...");
    #endif

    VL_UpdateScreen(screenBuffer);
    #endif

    SETFONTCOLOR(0,15);
#else
    VL_UpdateScreen(screenBuffer);

    if (!param_nowait)
        VL_WaitVBL(3*70);
#endif
}

/* =========================================================================== */
 
/*
=====================
=
= InitDigiMap
=
=====================
*/

/*
** channel mapping:
**  -1: any non reserved channel
**   0: player weapons
**   1: boss weapons
*/

static int wolfdigimap[] =
    {
        /* These first sounds are in the upload version */
#ifndef SPEAR
        HALTSND,                0,  -1,
        DOGBARKSND,             1,  -1,
        CLOSEDOORSND,           2,  -1,
        OPENDOORSND,            3,  -1,
        ATKMACHINEGUNSND,       4,   0,
        ATKPISTOLSND,           5,   0,
        ATKGATLINGSND,          6,   0,
        SCHUTZADSND,            7,  -1,
        GUTENTAGSND,            8,  -1,
        MUTTISND,               9,  -1,
        BOSSFIRESND,            10,  1,
        SSFIRESND,              11, -1,
        DEATHSCREAM1SND,        12, -1,
        DEATHSCREAM2SND,        13, -1,
        DEATHSCREAM3SND,        13, -1,
        TAKEDAMAGESND,          14, -1,
        PUSHWALLSND,            15, -1,

        LEBENSND,               20, -1,
        NAZIFIRESND,            21, -1,
        SLURPIESND,             22, -1,

        YEAHSND,                32, -1,

#ifndef UPLOAD
        /* These are in all other episodes */
        DOGDEATHSND,            16, -1,
        AHHHGSND,               17, -1,
        DIESND,                 18, -1,
        EVASND,                 19, -1,

        TOT_HUNDSND,            23, -1,
        MEINGOTTSND,            24, -1,
        SCHABBSHASND,           25, -1,
        HITLERHASND,            26, -1,
        SPIONSND,               27, -1,
        NEINSOVASSND,           28, -1,
        DOGATTACKSND,           29, -1,
        LEVELDONESND,           30, -1,
        MECHSTEPSND,            31, -1,

        SCHEISTSND,             33, -1,
        DEATHSCREAM4SND,        34, -1,         /* AIIEEE */
        DEATHSCREAM5SND,        35, -1,         /* DEE-DEE */
        DONNERSND,              36, -1,         /* EPISODE 4 BOSS DIE */
        EINESND,                37, -1,         /* EPISODE 4 BOSS SIGHTING */
        ERLAUBENSND,            38, -1,         /* EPISODE 6 BOSS SIGHTING */
        DEATHSCREAM6SND,        39, -1,         /* FART */
        DEATHSCREAM7SND,        40, -1,         /* GASP */
        DEATHSCREAM8SND,        41, -1,         /* GUH-BOY! */
        DEATHSCREAM9SND,        42, -1,         /* AH GEEZ! */
        KEINSND,                43, -1,         /* EPISODE 5 BOSS SIGHTING */
        MEINSND,                44, -1,         /* EPISODE 6 BOSS DIE */
        ROSESND,                45, -1,         /* EPISODE 5 BOSS DIE */

#endif
#else
/*
** SPEAR OF DESTINY DIGISOUNDS
*/
        HALTSND,                0,  -1,
        CLOSEDOORSND,           2,  -1,
        OPENDOORSND,            3,  -1,
        ATKMACHINEGUNSND,       4,   0,
        ATKPISTOLSND,           5,   0,
        ATKGATLINGSND,          6,   0,
        SCHUTZADSND,            7,  -1,
        BOSSFIRESND,            8,   1,
        SSFIRESND,              9,  -1,
        DEATHSCREAM1SND,        10, -1,
        DEATHSCREAM2SND,        11, -1,
        TAKEDAMAGESND,          12, -1,
        PUSHWALLSND,            13, -1,
        AHHHGSND,               15, -1,
        LEBENSND,               16, -1,
        NAZIFIRESND,            17, -1,
        SLURPIESND,             18, -1,
        LEVELDONESND,           22, -1,
        DEATHSCREAM4SND,        23, -1,         /* AIIEEE */
        DEATHSCREAM3SND,        23, -1,         /* DOUBLY-MAPPED!!! */
        DEATHSCREAM5SND,        24, -1,         /* DEE-DEE */
        DEATHSCREAM6SND,        25, -1,         /* FART */
        DEATHSCREAM7SND,        26, -1,         /* GASP */
        DEATHSCREAM8SND,        27, -1,         /* GUH-BOY! */
        DEATHSCREAM9SND,        28, -1,         /* AH GEEZ! */
        GETGATLINGSND,          38, -1,         /* Got Gat replacement */

#ifndef SPEARDEMO
        DOGBARKSND,             1,  -1,
        DOGDEATHSND,            14, -1,
        SPIONSND,               19, -1,
        NEINSOVASSND,           20, -1,
        DOGATTACKSND,           21, -1,
        TRANSSIGHTSND,          29, -1,         /* Trans Sight */
        TRANSDEATHSND,          30, -1,         /* Trans Death */
        WILHELMSIGHTSND,        31, -1,         /* Wilhelm Sight */
        WILHELMDEATHSND,        32, -1,         /* Wilhelm Death */
        UBERDEATHSND,           33, -1,         /* Uber Death */
        KNIGHTSIGHTSND,         34, -1,         /* Death Knight Sight */
        KNIGHTDEATHSND,         35, -1,         /* Death Knight Death */
        ANGELSIGHTSND,          36, -1,         /* Angel Sight */
        ANGELDEATHSND,          37, -1,         /* Angel Death */
        GETSPEARSND,            39, -1,         /* Got Spear replacement */
#endif
#endif
        LASTSOUND
    };


void InitDigiMap (void)
{
    int *map;

    for (map = wolfdigimap; *map != LASTSOUND; map += 3)
    {
#ifndef VIEASM
        DigiMap[map[0]] = map[1];
        DigiChannel[map[1]] = map[2];
#endif
        SD_PrepareSound(map[1]);
    }
}

#ifndef SPEAR
CP_iteminfo MusicItems={CTL_X,CTL_Y,6,0,32};
CP_itemtype MusicMenu[]=
    {
        {1,"Get Them!",0},
        {1,"Searching",0},
        {1,"P.O.W.",0},
        {1,"Suspense",0},
        {1,"War March",0},
        {1,"Around The Corner!",0},

        {1,"Nazi Anthem",0},
        {1,"Lurking...",0},
        {1,"Going After Hitler",0},
        {1,"Pounding Headache",0},
        {1,"Into the Dungeons",0},
        {1,"Ultimate Conquest",0},

        {1,"Kill the S.O.B.",0},
        {1,"The Nazi Rap",0},
        {1,"Twelfth Hour",0},
        {1,"Zero Hour",0},
        {1,"Ultimate Conquest",0},
        {1,"Wolfpack",0}
    };
#else
CP_iteminfo MusicItems={CTL_X,CTL_Y-20,9,0,32};
CP_itemtype MusicMenu[]=
    {
        {1,"Funky Colonel Bill",0},
        {1,"Death To The Nazis",0},
        {1,"Tiptoeing Around",0},
        {1,"Is This THE END?",0},
        {1,"Evil Incarnate",0},
        {1,"Jazzin' Them Nazis",0},
        {1,"Puttin' It To The Enemy",0},
        {1,"The SS Gonna Get You",0},
        {1,"Towering Above",0}
    };
#endif

#ifndef SPEARDEMO
void DoJukebox(void)
{
    int which,lastsong=-1;
    uintptr_t start;
    unsigned songs[]=
        {
#ifndef SPEAR
            GETTHEM_MUS,
            SEARCHN_MUS,
            POW_MUS,
            SUSPENSE_MUS,
            WARMARCH_MUS,
            CORNER_MUS,

            NAZI_OMI_MUS,
            PREGNANT_MUS,
            GOINGAFT_MUS,
            HEADACHE_MUS,
            DUNGEON_MUS,
            ULTIMATE_MUS,

            INTROCW3_MUS,
            NAZI_RAP_MUS,
            TWELFTH_MUS,
            ZEROHOUR_MUS,
            ULTIMATE_MUS,
            PACMAN_MUS
#else
            XFUNKIE_MUS,             /* 0 */
            XDEATH_MUS,              /* 2 */
            XTIPTOE_MUS,             /* 4 */
            XTHEEND_MUS,             /* 7 */
            XEVIL_MUS,               /* 17 */
            XJAZNAZI_MUS,            /* 18 */
            XPUTIT_MUS,              /* 21 */
            XGETYOU_MUS,             /* 22 */
            XTOWER2_MUS              /* 23 */
#endif
        };

    IN_ClearKeysDown();
    if (!AdLibPresent && !SoundBlasterPresent)
        return;

    MenuFadeOut();

#ifndef SPEAR
#ifndef UPLOAD
    start = ((WL_GetTicks()/10)%3)*6;
#else
    start = 0;
#endif
#else
    start = 0;
#endif

    CA_LoadAllSounds ();

    fontnumber=1;
    ClearMScreen ();
    VWB_DrawPic(112,184,C_MOUSELBACKPIC);
    DrawStripes (10);
    SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);

#ifndef SPEAR
    DrawWindow (CTL_X-2,CTL_Y-6,280,13*7,BKGDCOLOR);
#else
    DrawWindow (CTL_X-2,CTL_Y-26,280,13*10,BKGDCOLOR);
#endif

    DrawMenu (&MusicItems,&MusicMenu[start]);

    SETFONTCOLOR (READHCOLOR,BKGDCOLOR);
    PrintY=15;
    WindowX = 0;
    WindowY = 320;
    US_CPrint ("Robert's Jukebox");

    SETFONTCOLOR (TEXTCOLOR,BKGDCOLOR);
    VL_UpdateScreen(screenBuffer);
    MenuFadeIn();

    do
    {
        which = HandleMenu(&MusicItems,&MusicMenu[start],NULL);
        if (which>=0)
        {
            if (lastsong >= 0)
                MusicMenu[start+lastsong].active = 1;

            StartCPMusic(songs[start + which]);
            MusicMenu[start+which].active = 2;
            DrawMenu (&MusicItems,&MusicMenu[start]);
            VL_UpdateScreen(screenBuffer);
            lastsong = which;
        }
    } while(which>=0);

    MenuFadeOut();
    IN_ClearKeysDown();
}
#endif

/*
==========================
=
= InitGame
=
= Load a few things right away
=
==========================
*/

static void InitGame()
{
#ifndef SPEARDEMO
    boolean didjukebox=false;
#endif
	int numJoysticks;
#if defined (SWITCH) || defined (N3DS) 
    printf("GAME START");
#elif defined(PS2)
    ps2_printf_XY("GAME START", 4, 20, 20);
#endif
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

#ifdef _arch_dreamcast
    SDL_DC_ShowAskHz(SDL_FALSE);
    SDL_DC_Default60Hz(SDL_FALSE);
    SDL_DC_VerticalWait(SDL_FALSE);
    SDL_DC_SetVideoDriver(SDL_DC_DMA_VIDEO);
#endif

#if defined(SWITCH) || defined (N3DS) 
#if SDL_MAJOR_VERSION == 1 
    printf("SDL1.2 Initialized");   
#elif SDL_MAJOR_VERSION == 2
    printf("SDL2 Initialized");
#endif
#elif defined (PS2)
#if SDL_MAJOR_VERSION == 1 
    ps2_printf_XY("SDL1.2 Initialized", 4, 20, 20);   
#elif SDL_MAJOR_VERSION == 2
    ps2_printf_XY("SDL2 Initialized", 4, 20, 20);
#endif
#endif

    numJoysticks = SDL_NumJoysticks();
    
    if(param_joystickindex && (param_joystickindex < -1 || param_joystickindex >= numJoysticks))
    {
        if(!numJoysticks)
            printf("No joysticks are available to SDL!\n");
        else
            printf("The joystick index must be between -1 and %i!\n", numJoysticks - 1);
        exit(1);
    }

#if defined(GP2X_940)
    GP2X_MemoryInit();
#endif

    SignonScreen ();

    VL_Startup();
#if defined(SWITCH) || defined (N3DS) 
    printf("VL Started DONE\n");
#elif defined(PS2)
    ps2_printf_XY("VL Started DONE\n", 5, 20, 20);
#endif
    IN_Startup ();
#if defined(SWITCH) || defined (N3DS) 
    printf("IN Started DONE\n");
#elif defined(PS2)
    ps2_printf_XY("IN Started DONE\n", 4, 20, 20);
#endif
#if defined(SDL_MAJOR_VERSION) && (SDL_MAJOR_VERSION == 2) && defined(HAPTIC_SUPPORT)
    HAPTIC_Startup ();
#if defined(SWITCH) || defined (N3DS) 
    printf("HAPTIC Started DONE\n");
#elif defined(PS2)
    ps2_printf_XY("HAPTIC Started DONE\n", 4, 20, 20);
#endif
#endif
    PM_Startup ();
#if defined(SWITCH) || defined (N3DS) 
    printf("PM Started");
#elif defined(PS2)
    ps2_printf_XY("PM Started DONE\n", 4, 20, 20);
#endif
    SD_Startup ();
#if defined(SWITCH) || defined (N3DS)
    printf("SD Started DONE\n");
#elif defined(PS2)
    ps2_printf_XY("SD Started DONE\n", 4, 20, 20);
#endif
    CA_Startup ();
#if defined(SWITCH) || defined (N3DS) 
    printf("CA Started");
#elif defined(PS2)
    ps2_printf_XY("CA Started DONE\n", 4, 20, 20);
#endif
    US_Startup ();
#if defined(SWITCH) || defined (N3DS)
    printf("US Started");
#elif defined(PS2)
    ps2_printf_XY("US Started DONE\n", 4);
#endif
#ifdef LWUDPCOMMS
    UDP_startup();
#endif

    /* TODO: Will any memory checking be needed someday?? */
#ifdef NOTYET
#ifndef SPEAR
    if (mminfo.mainmem < 235000L)
#else
    if (mminfo.mainmem < 257000L && !MS_CheckParm("debugmode"))
#endif
    {
        unsigned char *screen;

        screen = grsegs[ERRORSCREEN];
        ShutdownId();
/*        memcpy((byte *)0xb8000,screen+7+7*160,17*160);
        gotoxy (1,23);*/
        exit(1);
    }
#endif


/*
** build some tables
*/
    InitDigiMap ();

    ReadConfig ();

    SetupSaveGames();

/*
** HOLDING DOWN 'M' KEY?
*/
	IN_ProcessEvents();

#ifndef SPEARDEMO
    if (Keyboard(sc_M))
    {
        DoJukebox();
        didjukebox=true;
    }
    else
#endif

/*
** draw intro screen stuff
*/
    IntroScreen ();

/*
** load in and lock down some basic chunks
*/
    BuildTables ();          /* trig tables */
    SetupWalls ();

    NewViewSize (viewsize);

/*
** initialize variables
*/
    InitRedShifts ();
#ifndef SPEARDEMO
    if(!didjukebox)
#endif
        FinishSignon();

#ifdef NOTYET
    vdisp = (unsigned char *) (0xa0000+PAGE1START);
    vbuf = (unsigned char *) (0xa0000+PAGE2START);
#endif
}

/* =========================================================================== */

/*
==========================
=
= SetViewSize
=
==========================
*/

boolean SetViewSize (unsigned width, unsigned height)
{
    viewwidth = width&~15;                  /* must be divisable by 16 */
    viewheight = height&~1;                 /* must be even */
    centerx = viewwidth/2-1;
    centery = viewheight / 2;
    shootdelta = viewwidth/10;
    if((unsigned) viewheight == screenHeight)
        viewscreenx = viewscreeny = screenofs = 0;
    else
    {
        viewscreenx = (screenWidth-viewwidth) / 2;
        viewscreeny = (screenHeight-scaleFactor*STATUSLINES-viewheight)/2;
        screenofs = viewscreeny*screenWidth+viewscreenx;
    }

/*
** calculate trace angles and projection constants
*/
    CalcProjection (FOCALLENGTH);

    return true;
}


void ShowViewSize (int width)
{
    int oldwidth,oldheight;

    oldwidth = viewwidth;
    oldheight = viewheight;

    if(width == 21)
    {
        viewwidth = screenWidth;
        viewheight = screenHeight;
        VL_BarScaledCoord (0, 0, screenWidth, screenHeight, 0);
    }
    else if(width == 20)
    {
        viewwidth = screenWidth;
        viewheight = screenHeight - scaleFactor*STATUSLINES;
        DrawPlayBorder ();
    }
    else
    {
        viewwidth = width*16*screenWidth/320;
        viewheight = (int) (width*16*HEIGHTRATIO*screenHeight/200);
        DrawPlayBorder ();
    }

    viewwidth = oldwidth;
    viewheight = oldheight;
}


void NewViewSize (int width)
{
    viewsize = width;
    if(viewsize == 21)
        SetViewSize(screenWidth, screenHeight);
    else if(viewsize == 20)
        SetViewSize(screenWidth, screenHeight - scaleFactor * STATUSLINES);
    else
        SetViewSize(width*16*screenWidth/320, (unsigned) (width*16*HEIGHTRATIO*screenHeight/200));
}



/* =========================================================================== */

/*
==========================
=
= Quit
=
==========================
*/

void Quit (const char *errorStr, ...)
{
#ifndef SEGA_SATURN
    unsigned char *screen;
    char error[256];
    if(errorStr != NULL)
    {
        va_list vlist;
        va_start(vlist, errorStr);
        vsprintf(error, errorStr, vlist);
        va_end(vlist);
    }
    else error[0] = 0;

    if (!pictable)  /* don't try to display the red box before it's loaded */
    {
        ShutdownId();
        if (error)
        {
#ifdef NOTYET
            SetTextCursor(0,0);
#endif
            puts(error);
#ifdef NOTYET
            SetTextCursor(0,2);
#endif
        }
        exit(1);
    }

    if (!*error)
    {
        #ifndef JAPAN
        screen = grsegs[ORDERSCREEN];
        #endif
        WriteConfig ();
    }
    else
        screen = grsegs[ERRORSCREEN];

    ShutdownId ();

    if (!error)
    {
#ifdef NOTYET
        memcpy((unsigned char *)0xb8000,screen+7,7*160);
        SetTextCursor(9,3);
#endif
        puts(error);
#ifdef NOTYET
        SetTextCursor(0,7);
#endif
        exit(1);
    }
    else if (!*error)
    {
#ifdef NOTYET
        #ifndef JAPAN
        memcpy((unsigned char *)0xb8000,screen+7,24*160); /* 24 for SPEAR/UPLOAD compatibility */
        #endif
        SetTextCursor(0,23);
#endif
    }

    exit(0);
#else
    SYS_Exit(0);
#endif
}

/* =========================================================================== */



/*
=====================
=
= DemoLoop
=
=====================
*/


static void DemoLoop()
{
    int LastDemo = 0;
/*
** check for launch from ted
*/
    if (param_tedlevel != -1)
    {
        param_nowait = true;
        EnableEndGameMenuItem();
        NewGame(param_difficulty,0);

#ifndef SPEAR
        gamestate.episode = param_tedlevel/10;
        gamestate.mapon = param_tedlevel%10;
#else
        gamestate.episode = 0;
        gamestate.mapon = param_tedlevel;
#endif
        GameLoop();
        Quit (NULL);
    }


/*
** main game cycle
*/

#ifndef DEMOTEST

    #ifndef UPLOAD

        #ifndef GOODTIMES
        #ifndef SPEAR
        #ifndef JAPAN
        if (!param_nowait)
            NonShareware();
        #endif
        #else
            #ifndef GOODTIMES
            #ifndef SPEARDEMO
            if(!param_goodtimes)
                CopyProtection();
            #endif
            #endif
        #endif
        #endif
    #endif

    StartCPMusic(INTROSONG);

#ifndef JAPAN
    if (!param_nowait)
        PG13 ();
#endif

#endif

    while (1)
    {
        while (!param_nowait)
        {
/*
** title page
*/
#ifndef DEMOTEST

#ifdef SPEAR
            SDL_Color pal[256];
            VL_ConvertPalette(grsegs[TITLEPALETTE], pal, 256);

            VWB_DrawPic (0,0,TITLE1PIC);
            VWB_DrawPic (0,80,TITLE2PIC);

            VL_UpdateScreen (screenBuffer);
            VL_FadeIn(0,255,pal,30);
#else
            VWB_DrawPic (0,0,TITLEPIC);
            VL_UpdateScreen(screenBuffer);
            VL_FadeIn(0, 255, gamepal, 30);
#endif
            if (IN_UserInput(TickBase*15))
                break;
            VL_FadeOut (0, 255, 0, 0, 0, 30);
/*
** credits page
*/
            VWB_DrawPic (0,0,CREDITSPIC);
            VL_UpdateScreen(screenBuffer);
            VL_FadeIn (0, 255, gamepal, 30);
            if (IN_UserInput(TickBase*10))
                break;
            VL_FadeOut (0, 255, 0, 0, 0, 30);
/*
** high scores
*/
            DrawHighScores ();
            VL_UpdateScreen (screenBuffer);
            VL_FadeIn (0, 255, gamepal, 30);

            if (IN_UserInput(TickBase*10))
                break;
#endif
/*
** demo
*/


            #ifndef SPEARDEMO
            PlayDemo (LastDemo++%NUMDEMOS);
            #else
            PlayDemo (0);
            #endif

            if (playstate == ex_abort)
                break;
            VL_FadeOut (0, 255, 0, 0, 0, 30);
            if(screenHeight % 200 != 0)
                VL_ClearScreen(0);

            StartCPMusic(INTROSONG);

        }

        VL_FadeOut (0, 255, 0, 0, 0, 30);

#ifdef DEBUGKEYS
        if (Keyboard(sc_Tab) && param_debugmode)
            RecordDemo ();
        else
            US_ControlPanel (0);
#else
        US_ControlPanel (0);
#endif

        if (startgame || loadedgame)
        {
            GameLoop ();
            if(!param_nowait)
            {
                VL_FadeOut (0, 255, 0, 0, 0, 30);
                StartCPMusic(INTROSONG);
            }
        }
    }
}


/* =========================================================================== */

#define IFARG(str) if(!strcmp(arg, (str)))

void CheckParameters(int argc, char *argv[])
{
    boolean hasError = false, showHelp = false;
    boolean sampleRateGiven = false, audioBufferGiven = false;
    int i,defaultSampleRate = param_samplerate;

    for(i = 1; i < argc; i++)
    {
        char *arg = argv[i];
#ifndef SPEAR
        IFARG("--goobers")
#else
        IFARG("--debugmode")
#endif
            param_debugmode = true;
        else IFARG("--baby")
            param_difficulty = 0;
        else IFARG("--easy")
            param_difficulty = 1;
        else IFARG("--normal")
            param_difficulty = 2;
        else IFARG("--hard")
            param_difficulty = 3;
        else IFARG("--nowait")
            param_nowait = true;
        else IFARG("--tedlevel")
        {
            if(++i >= argc)
            {
                printf("The tedlevel option is missing the level argument!\n");
                hasError = true;
            }
            else param_tedlevel = atoi(argv[i]);
        }
        else IFARG("--windowed")
            fullscreen = false;
        else IFARG("--windowed-mouse")
        {
            fullscreen = false;
            forcegrabmouse = true;
        }
        else IFARG("--res")
        {
            if(i + 2 >= argc)
            {
                printf("The res option needs the width and/or the height argument!\n");
                hasError = true;
            }
            else
            {
				unsigned int factor;
                screenWidth = atoi(argv[++i]);
                screenHeight = atoi(argv[++i]);
                factor = screenWidth / 320;
                if((screenWidth % 320) || (screenHeight != 200 * factor && screenHeight != 240 * factor))
                    printf("Screen size must be a multiple of 320x200 or 320x240!\n"), hasError = true;
            }
        }
        else IFARG("--resf")
        {
            if(i + 2 >= argc)
            {
                printf("The resf option needs the width and/or the height argument!\n");
                hasError = true;
            }
            else
            {
                screenWidth = atoi(argv[++i]);
                screenHeight = atoi(argv[++i]);
                if(screenWidth < 320)
                    printf("Screen width must be at least 320!\n"), hasError = true;
                if(screenHeight < 200)
                    printf("Screen height must be at least 200!\n"), hasError = true;
            }
        }
        else IFARG("--bits")
        {
            if(++i >= argc)
            {
                printf("The bits option is missing the color depth argument!\n");
                hasError = true;
            }
            else
            {
                screenBits = atoi(argv[i]);
                switch(screenBits)
                {
                    case 8:
                    case 16:
                    case 24:
                    case 32:
                        break;

                    default:
                        printf("Screen color depth must be 8, 16, 24, or 32!\n");
                        hasError = true;
                        break;
                }
            }
        }
        else IFARG("--nodblbuf")
            usedoublebuffering = false;
        else IFARG("--extravbls")
        {
            if(++i >= argc)
            {
                printf("The extravbls option is missing the vbls argument!\n");
                hasError = true;
            }
            else
            {
                extravbls = atoi(argv[i]);
                if(extravbls < 0)
                {
                    printf("Extravbls must be positive!\n");
                    hasError = true;
                }
            }
        }
        else IFARG("--joystick")
        {
            if(++i >= argc)
            {
                printf("The joystick option is missing the index argument!\n");
                hasError = true;
            }
            else param_joystickindex = atoi(argv[i]);   /* index is checked in InitGame */
        }
        else IFARG("--joystickhat")
        {
            if(++i >= argc)
            {
                printf("The joystickhat option is missing the index argument!\n");
                hasError = true;
            }
            else param_joystickhat = atoi(argv[i]);
        }
        else IFARG("--samplerate")
        {
            if(++i >= argc)
            {
                printf("The samplerate option is missing the rate argument!\n");
                hasError = true;
            }
            else param_samplerate = atoi(argv[i]);
            sampleRateGiven = true;
        }
        else IFARG("--audiobuffer")
        {
            if(++i >= argc)
            {
                printf("The audiobuffer option is missing the size argument!\n");
                hasError = true;
            }
            else param_audiobuffer = atoi(argv[i]);
            audioBufferGiven = true;
        }
        else IFARG("--mission")
        {
            if(++i >= argc)
            {
                printf("The mission option is missing the mission argument!\n");
                hasError = true;
            }
            else
            {
                param_mission = atoi(argv[i]);
                if(param_mission < 0 || param_mission > 3)
                {
                    printf("The mission option must be between 0 and 3!\n");
                    hasError = true;
                }
            }
        }
        else IFARG("--configdir")
        {
            if(++i >= argc)
            {
                printf("The configdir option is missing the dir argument!\n");
                hasError = true;
            }
            else
            {
                size_t len = strlen(argv[i]);
                if(len + 2 > sizeof(configdir))
                {
                    printf("The config directory is too long!\n");
                    hasError = true;
                }
                else
                {
                    strcpy(configdir, argv[i]);
                    if(argv[i][len] != '/' && argv[i][len] != '\\')
                        strcat(configdir, "/");
                }
            }
        }
        else IFARG("--goodtimes")
            param_goodtimes = true;
        else IFARG("--ignorenumchunks")
            param_ignorenumchunks = true;
        else IFARG("--help")
            showHelp = true;
#ifdef LWUDPCOMMS
        else IFARG(UDP_check(arg))
        {
                                    /* do nothing */
        }
#endif
        else hasError = true;

    }
    if(hasError || showHelp)
    {
        if(hasError) printf("\n");
        printf(
            "Wolf4SDL v2.3\n"
            "Ported by Chaos-Software, additions by the community\n"
            "Original Wolfenstein 3D by id Software\n\n"
            "Usage: Wolf4SDL [options]\n"
            "Options:\n"
            " --help                 This help page\n"
            " --tedlevel <level>     Starts the game in the given level\n"
            " --baby                 Sets the difficulty to baby for tedlevel\n"
            " --easy                 Sets the difficulty to easy for tedlevel\n"
            " --normal               Sets the difficulty to normal for tedlevel\n"
            " --hard                 Sets the difficulty to hard for tedlevel\n"
            " --nowait               Skips intro screens\n"
            " --windowed[-mouse]     Starts the game in a window [and grabs mouse]\n"
            " --res <width> <height> Sets the screen resolution\n"
            "                        (must be multiple of 320x200 or 320x240)\n"
            " --resf <w> <h>         Sets any screen resolution >= 320x200\n"
            "                        (which may result in graphic errors)\n"
            " --bits <b>             Sets the screen color depth\n"
            "                        (use this when you have palette/fading problems\n"
            "                        allowed: 8, 16, 24, 32, default: \"best\" depth)\n"
            " --nodblbuf             Don't use SDL's double buffering\n"
            " --extravbls <vbls>     Sets a delay after each frame, which may help to\n"
            "                        reduce flickering (unit is currently 8 ms, default: 0)\n"
            " --joystick <index>     Use the index-th joystick if available\n"
            "                        (-1 to disable joystick, default: 0)\n"
            " --joystickhat <index>  Enables movement with the given coolie hat\n"
            " --samplerate <rate>    Sets the sound sample rate (given in Hz, default: %i)\n"
            " --audiobuffer <size>   Sets the size of the audio buffer (-> sound latency)\n"
            "                        (given in bytes, default: 2048 / (44100 / samplerate))\n"
            " --ignorenumchunks      Ignores the number of chunks in VGAHEAD.*\n"
            "                        (may be useful for some broken mods)\n"
            " --configdir <dir>      Directory where config file and save games are stored\n"
#if defined(_arch_dreamcast) || defined(_WIN32) || defined(PS2)
            "                        (default: current directory)\n"
#else
            "                        (default: $HOME/.wolf4sdl)\n"
#endif
#if defined(SPEAR) && !defined(SPEARDEMO)
            " --mission <mission>    Mission number to play (0-3)\n"
            "                        (default: 0 -> .sod, 1-3 -> .sd*)\n"
            " --goodtimes            Disable copy protection quiz\n"
#endif
            , defaultSampleRate
#ifdef LWUDPCOMMS
            ,
            UDP_parameterHelp
#endif
        );
        exit(1);
    }

    if(sampleRateGiven && !audioBufferGiven)
#ifdef _arch_dreamcast
        param_audiobuffer = 4096 / (44100 / param_samplerate);
#else
        param_audiobuffer = 2048 / (44100 / param_samplerate);
#endif
}

/*
==========================
=
= main
=
==========================
*/

int main (int argc, char *argv[])
{
#ifdef SWITCH
    /* nxlink */
    socketInitializeDefault();
	nxlinkStdio();

    /* emulator
    consoleDebugInit(debugDevice_SVC);
	stdout = stderr; */
	printf("nxlink printf\n");
    printf("MAIN ENTRY\n");
	
	printf("Configure Nintendo Switch gamepad");
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);		
#endif	
	
#if defined(_arch_dreamcast)
    DC_Main();
    DC_CheckParameters();
#else
#ifdef PS2
    PS2_Started();
#endif
    CheckParameters(argc, argv);
#endif
#if defined(SWITCH) || defined (N3DS) 
    printf("CheckParameters() DONE\n");
#elif defined(PS2)
    ps2_printf_XY("CheckParameters DONE\n", 4, 20, 20);
#endif
    CheckForEpisodes(); 
#if defined(SWITCH) || defined (N3DS) 
    printf("CheckForEpisodes() DONE\n");
#elif defined(PS2)
    ps2_printf_XY("ChceckForEpisodes() DONE\n" , 4, 20, 20);
#endif
    InitGame();
#if defined(SWITCH) || defined (N3DS)
    printf("InitGame() DONE\n");
#elif defined(PS2)
    ps2_printf_XY("InitGame()", 4, 20, 20);
#endif
    DemoLoop();
#if defined(SWITCH) || defined (N3DS)
    printf("DemoLoop() DONE\n");
#elif defined(PS2)
    ps2_printf_XY("DemoLoop() DONE\n", 4, 20, 20);
#endif
    Quit("Demo loop exited???");
    return 1;
}
