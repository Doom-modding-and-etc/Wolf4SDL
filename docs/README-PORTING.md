# PORTING (Last reviewed on: 05:28:2023)

  This document abords to how you can port with details about how to port/support Wolf4SDL into a new platform so i highly recomend to you for read this even this is a big text, before you start porting to other platform.

# CODING RULES:

- The most basic one:  you must have the knowledge about that platform before anything.
  Reason: i´m saying this because some peapole when start porting they usually prefer to port some parts but they can´t solve like: how to port the renderer x or in worst cases the person give up because he doesn´t have the proper knowledge about the sdk or hardware. so if you want to post your port here first be experienciated on that platform with the proper knowledge before anything else.
- Use C89(Aka: ANSI C) coding standard
  Reason: if you use standard the C11/C17 when you are porting to another platform you are difficulting the others which prefers some old platforms that doesn´t support C11/C17  for example Commodore64 which the compiler probablly supports only C89, Although this can might be problem since Wolf4SDL uses C++ comments and inline of C99, so if you want to use other standard for your desired platform, please use `#define C11` or `#define C17` and if your compiler supports C99 or C89 please avoid the use the C11 and C17 standard.
- don´t use type defintions including: `int8_t`, `int16_t`, `int32_t`, `int64_t` and etc
  Reason: same as the second one, if you use type definition please go to `wl_def.h` or `id_w3swrap.h` for abstract the type definition,
  example: `/*Type definition of short errno_t(the most hated type) */ typedef int errno_t;` you see that it can makes easier when you manually type the type definition, it makes easier to the compiler see the type itself without using inclusion headers, so if you plan to use types use it however define it first or preference avoid the usage of types.
- Use ` #ifdef` when you are porting to a new desired platform
  Reason: when are you porting to other platform you must use `#ifdef` like this:

  ```
  #ifdef SAMPLE_PLAT
  /* Put your ported code here: */
  #else
  /*Put the original code which you ported to your platform
  #endif
  ```

  but if you want to use `#ifndef`, use like this:
  `#ifndef SAMPLE_PLAT`
  `/* The code that doesn´t work on your platform */`
  `#endif`
  because if you use like this:

  ```
  #ifndef SAMPLE_PLATFORM
  /* Original code that doesn´t work on your platform */
  #else
  /* Code ported to your platform standards */
  #endif
  ```

  you can easily make the code lazy and not easier depending of the situation for the programmer.
  Q: but how i can port since there´s other `#ifdef` in place? well you must use `#if defined(SAMPLE_PLATFORM)` and then
  for your desired platform you use `#elif defined(SAMPLE_PLATFORM)`, if you do this you´re done!!!
- Avoid C++ Source files/Code
  Reason: i don´t like seeing c++ code on Wolf4SDL even if it was ported from Wolf4SDL 1.9 or Less, this the reason why i hate the wolf3d community they prefer on easy solution they should instead go and figure out the problem and solve it instead of porting the whole code based in c to C++99, 03 and 11 in some parts this why many developers/programmers including [linuxwolf](https://github.com/linuxwolf6), this is fault of ripper which on the time doesn´t have the proper knowledge about this detail.
  so to not commit the same mistake instead don´t use C++ files except if you wrap a .h file with c content but the source file fill with c++ wrapped, you can also if your platform supports only C++ you can easily compile as C++ instead of porting the whole code to C++ standards.
- When are you creating a file for your specific platform use the Platform dir for creating it
  Reason: don´t create file for your specific platform on the project root instead go to platform and create a folder with the platform name and write it and then include into wl_def.h or any file on the root dir.
- Use the platform prefix while creating a function or encapsulated function for your platform.
  Reason: This makes easier to identify what platform are been ported in and keeps the standards of id tech 0 engine
  here as example:
  platform_sample.h

  ```
  #ifndef PLATFORM_SAMPLE_H
  #define PLATFOMR_SAMPLE_H
  #include <platform_header.h>
  void PLATFORM_functionx();
  boolean PLATFORM_functiony();
  #endif
  ```

  if you do this you´re helping the others identify what platform are you porting into your project.

  That´s all folks now let´s go to do the guide :)

# First Step: Loading data from directory

- you need to create your project file as usual and include all the source files to your project
  and then go to version.h file and if you see this thing here:

```
#ifndef __VERSION_H_
#define __VERSION_H_

#ifndef VERSIONALREADYCHOSEN              // used for batch compiling

/* Defines used for different versions */
//#define SPEAR
//#define SPEARDEMO
//#define UPLOAD
#define GOODTIMES
#define CARMACIZED
//#define APOGEE_1_0
//#define APOGEE_1_1
//#define APOGEE_1_2
//#define JAPAN

/*
    Wolf3d Full v1.1 Apogee (with ReadThis)   - define CARMACIZED and APOGEE_1_1
    Wolf3d Full v1.4 Apogee (with ReadThis)   - define CARMACIZED
    Wolf3d Full v1.4 GT/ID/Activision         - define CARMACIZED and GOODTIMES
    Wolf3d Full v1.4 Imagineer (Japanese)     - define CARMACIZED and JAPAN
    Wolf3d Shareware v1.0                     - define UPLOAD and APOGEE_1_0
    Wolf3d Shareware v1.1                     - define CARMACIZED and UPLOAD and APOGEE_1_1
    Wolf3d Shareware v1.2                     - define CARMACIZED and UPLOAD and APOGEE_1_2
    Wolf3d Shareware v1.4                     - define CARMACIZED and UPLOAD
    Spear of Destiny Full and Mission Disks   - define CARMACIZED and SPEAR
                                                (and GOODTIMES for no FormGen quiz)
    Spear of Destiny Demo                     - define CARMACIZED and SPEAR and SPEARDEMO
*/

#endif

//#define USE_FEATUREFLAGS    // Enables the level feature flags (see bottom of wl_def.h)
//#define USE_SHADING         // Enables shading support (see wl_shade.c)
//#define USE_DIR3DSPR        // Enables directional 3d sprites
//#define USE_FLOORCEILINGTEX // Enables texture-mapped floors and ceilings (see wl_plane.c)
//#define USE_MULTIFLATS      // Enables floor and ceiling textures stored in the third mapplane
//#define USE_PARALLAX        // Enables parallax sky with 16 textures per sky (see wl_parallax.c)
//#define USE_SKYWALLPARALLAX // Enables parallax sky on walls with 16 repeats of sky texture
//#define USE_CLOUDSKY        // Enables cloud sky support (see wl_cloudsky.c)
//#define USE_STARSKY         // Enables star sky support (see wl_atmos.c)
//#define USE_RAIN            // Enables rain support (see wl_atmos.c)
//#define USE_SNOW            // Enables snow support (see wl_atmos.c)
//#define FIXRAINSNOWLEAKS    // Enables leaking ceilings fix (by Adam Biser, only needed if maps with rain/snow and ceilings exist)
//#define VIEWMAP             // Enables the overhead map
//#define REVEALMAP           // Enables showing only the areas of the overhead map that have been seen


//#define DEBUGKEYS           // Comment this out to compile without the Tab debug keys
#define ARTSEXTERN
#define DEMOSEXTERN
#define PLAYDEMOLIKEORIGINAL  // When playing or recording demos, several bug fixes do not take
                              // effect to let the original demos work as in the original Wolf3D v1.4
                              // (actually better, as the second demo rarely worked)
//#define CRT // Makes the screen fixed in 4:3
//#define SCALE2X //The screen will be scaled in 2x with linear filter. This guy needs to work with CRT defined or else it won´t work..
#define FIXCALCROTATE         // Apply a modified version of Ginyu's fix to make CalcRotate more accurate at high resolutions
//#define VIEASM //Activates: VODKA-INDUCED ENTERTAINMENT ADVANCED SOUND MANAGER v0.9.2(Warning: only works with .wav and .ogg placed in sounds/ follow the asmcref.h for more details).
//#define BANDEDHOLOWALLS     // Use the old DOS-style "banded" wall drawing behaviour when inside walls
//#define USE_DOSBOX //Replaces fmopl sound and uses DOSBOX Sound instead.
//#define GOD_MODE //Use godmode
//#define FIXEDLOGICRATE       // Enables framerate independent game logic (aka: uncapped refreshrate) (by Codetech84)
//#define LAGSIMULATOR          // Enables lag simulator debug option, depends on fixed logic rate (by Codetech84)
//#define AUTOMAP //Enables the HUD automap feature by AlumiuN
//#define VANILLA //Deactivates some features that Wolf3d or Sod doesn´t have on orignial game
//Credits: Codetech84
//#define MAPCONTROLLEDMUSIC //Activates the map controlled music.
//#define MAPCONTROLPARTIME //Activates the map partial time control
//#define MAPCONTROLLEDSHADE //Activates the controled shade
//#define MAPCONTROLLEDPLSETT //Activates the map control plsett
//#define MAPCONTROLLEDLTIME //Activates the map control time
//#define MAPCONTROLLEDSKY //Activates the sky control
//#define MAPCONTROLLEDCEILING// Map Controlled Ceiling colour (8,0)
//#define MAPCONTROLLEDFLOOR    // Map Controlled Floor colour (9,0)
//#define SEEKER_MISSILES //Add Seeker Missiles
//end
//#define BLAKEDOORS     // Blake Stone style Double doors - original code by: Adam Biser
//#define EXTRACONTROLS // switches out to Joystick controls for WASD and mouse movement toggles instead 
//#define AUTOINTER //Enables the automatic intermission screen
//#define CHRIS //Enables the chris chokan patches.
//#define BULLET_CALC //Enable the Bullet Calculation
//#define LOGFILE //Activate the log file feature, Credits: Havoc
//By: WSJ
//#define BURN_ANIM //Add the burn animation
//#define BOSS_MUSIC //Add some boss music it needs VIEASM defined
//end
//#define HIGHLIGHTPUSHWALLS //Enables the "highlight" pushwalls by: Merthsoft
//#define SAVE_GAME_SCREENSHOT //Add screenshot save support.
//By: WallHack
//#define COMPASS //Add the compass 
//end

#if defined(SWITCH) 
#define DATADIR "/switch/wolf4sdl/wolf3d/"
#elif defined(N3DS)
#define DATADIR "/3ds/wolf4sdl/wolf3d/"
#elif defined(PS2)
#define DATADIR "host0://data"
#elif defined(SEGA_SATURN)
#define DATADIR "/data"
#endif
#endif
```

You see that there is a 4 platforms defined here on the end of the line? so this is the directory data for other platforms. if you want to use the data directories go and use like this

```
#if defined(SWITCH) 
#define DATADIR "/switch/wolf4sdl/wolf3d/"
#elif defined(N3DS)
#define DATADIR "/3ds/wolf4sdl/wolf3d/"
#elif defined(SAMPLE_PLATFORM)
#define DATADIR "/sample/wolf4sdl"
#elif defined(PS2)
#define DATADIR "host0://data"
#elif defined(SEGA_SATURN)
#define DATADIR "/data"
#endif
#endif
```

then go to your compiler and declare the defintion as flag example: `-DSAMPLE_PLATFORM`, then go to id_ca.c and do the following here on line 169:

```
#if defined(SWITCH) || defined(SEGA_SATURN) || defined(N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
#ifdef SEGA_SATURN
static const char gheadname[] = DATADIR "VGAHEAD.";
static const char gfilename[] = DATADIR "VGAGRAPH.";
static const char gdictname[] = DATADIR "VGADICT.";
static const char mheadname[] = DATADIR "MAPHEAD.";
#else
static const char gheadname[] = DATADIR "vgahead.";
static const char gfilename[] = DATADIR "vgagraph.";
static const char gdictname[] = DATADIR "vgadict.";
static const char mheadname[] = DATADIR "maphead.";
#endif
#ifndef SEGA_SATURN
static const char mfilename[] = DATADIR "maptemp.";
#ifdef CARMACIZED
static const char mfilecama[] = DATADIR "gamemaps.";
#endif
#ifndef VIEASM
static const char aheadname[] = DATADIR "audiohed.";
#endif
static const char afilename[] = DATADIR "audiot.";
#endif
#else
static const char gheadname[] = "vgahead.";
static const char gfilename[] = "vgagraph.";
static const char gdictname[] = "vgadict.";
static const char mheadname[] = "maphead.";
#ifdef CARMACIZED
static const char mfilecama[] = "gamemaps.";
#endif
static const char mfilename[] = "maptemp.";
#ifndef VIEASM
static const char aheadname[] = "audiohed.";
#endif
static const char afilename[] = "audiot.";
#endif
```

after this go to line 830 on CAL_SetupMapFile
and do this:

```
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SEGA_SATURN) || defined(SAMPLE_PLATFORM) 
    char fname[13 + sizeof(DATADIR)];
#else
    char fname[13];
#endif
```

now go to the line 1029 on void CAL_SetupAudioFile (void)
and do this also

```
#if defined(SWITCH) || defined(N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
    char fname[13 + sizeof(DATADIR)];
#else  
    char fname[13];
#endif
```

after you done with the id_ca.c

go to wl_menu.c and go to line 4711 inside of void CheckForEpisodes (void)
you will see a thing like this:

```

///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR EPISODES
//
///////////////////////////////////////////////////////////////////////////
void
CheckForEpisodes (void)
{
    struct stat statbuf;

    // On Linux like systems, the configdir defaults to $HOME/.wolf4sdl
#if !defined(_WIN32) && !defined(_arch_dreamcast)
    if(configdir[0] == 0)
    {
        // Set config location to home directory for multi-user support
        char *homedir = getenv("HOME");
        if(homedir == NULL)
        {
            Quit("Your $HOME directory is not defined. You must set this before playing.");
        }
        #define WOLFDIR "/.wolf4sdl"
        if(strlen(homedir) + sizeof(WOLFDIR) > sizeof(configdir))
        {
            Quit("Your $HOME directory path is too long. It cannot be used for saving games.");
        }
        w3ssnprintf(configdir, sizeof(configdir), "%s" WOLFDIR, homedir);
    }
#endif

    if(configdir[0] != 0)
    {
        // Ensure config directory exists and create if necessary
        if(stat(configdir, &statbuf) != 0)
        {
#ifdef _WIN32
            if(_mkdir(configdir) != 0)
#else
            if(mkdir(configdir, 0755) != 0)
#endif
            {
                Quit("The configuration directory \"%s\" could not be created.", configdir);
            }
        }
    }

//
// JAPANESE VERSION
//
#ifdef JAPAN
#ifdef JAPDEMO
#if defined(SWITCH) || defined (N3DS) || defined(PS2) 
    if(!stat(DATADIR "vswap.wj1", &statbuf))
#else  
    if (!stat("vswap.wj1", &statbuf))
#endif  
    {
        strcpy (extension, "wj1");
#else

#if defined(SWITCH) || defined (N3DS) || defined(PS2) 
    if(!stat(DATADIR "vswap.wj6", &statbuf))
#else
    if(!stat("vswap.wj6", &statbuf))
#endif  
    {
        strcpy(extension, "wj6");
#endif
        strcat(configname, extension);
        strcat(SaveName, extension);
        strcat(demoname, extension);
        EpisodeSelect[1] =
            EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
    }
    else
        Quit ("NO JAPANESE WOLFENSTEIN 3-D DATA FILES to be found!");
    strcpy (graphext, extension);
    strcpy (audioext, extension);
#else

//
// ENGLISH
//
#ifdef UPLOAD
#if defined(SWITCH) || defined (N3DS) || defined(PS2) 
    if (!stat(DATADIR "vswap.wl1", &statbuf)) 
    {
  
    }
#else  
    if(!stat("vswap.wl1", &statbuf))
    {
  
    }
#endif
    else
        Quit ("NO WOLFENSTEIN 3-D DATA FILES to be found!");

#else
#ifndef SPEAR
#if defined(SWITCH) || defined (N3DS) || defined(PS2) 
    if(!stat(DATADIR "vswap.wl6", &statbuf))
#else
    if(!stat("vswap.wl6", &statbuf))
#endif
    {
        strcpy (extension, "wl6");
        NewEmenu[2].active =
            NewEmenu[4].active =
            NewEmenu[6].active =
            NewEmenu[8].active =
            NewEmenu[10].active =
            EpisodeSelect[1] =
            EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
    }
    else
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2)  
        if(!stat(DATADIR "vswap.wl3", &statbuf))
#else 
        if (!stat("vswap.wl3", &statbuf))
#endif
        {
            strcpy (extension, "wl3");
            NewEmenu[2].active = NewEmenu[4].active = EpisodeSelect[1] = EpisodeSelect[2] = 1;
        }
        else
        {
#if defined(SWITCH) || defined (N3DS) || defined(PS2)  
            if (!stat(DATADIR "vswap.wl1", &statbuf))
#else
            if (!stat("vswap.wl1", &statbuf))
#endif

                strcpy (extension, "wl1");
            else
                Quit ("NO WOLFENSTEIN 3-D DATA FILES to be found!");
        }
    }
#endif
#endif


#ifdef SPEAR
#ifndef SPEARDEMO
    if(param_mission == 0)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2)  
        if(!stat(DATADIR"vswap.sod", &statbuf))
#else
        if(!stat("vswap.sod", &statbuf))
#endif
            strcpy (extension, "sod");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 1)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2)  
        if(!stat(DATADIR"vswap.sd1", &statbuf))
#else
        if(!stat("vswap.sd1", &statbuf))
#endif
            strcpy (extension, "sd1");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 2)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2)  
        if(!stat(DATADIR"vswap.sd2", &statbuf))
#else
        if(!stat("vswap.sd2", &statbuf))
#endif
            strcpy (extension, "sd2");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 3)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2)  
        if(!stat(DATADIR"vswap.sd3", &statbuf))
#else  
        if(!stat("vswap.sd3", &statbuf))
#endif  
            strcpy (extension, "sd3");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else
        Quit ("UNSUPPORTED MISSION!");
    strcpy (graphext, "sod");
#ifndef VIEASM
    strcpy (audioext, "sod");
#endif
#else
#if defined(SWITCH) || defined (N3DS) || defined(PS2) 
    if(!stat(DATADIR "vswap.sdm", &statbuf))
#else
    if(!stat("vswap.sdm", &statbuf))
#endif
    {
        strcpy (extension, "sdm");
    }
    else
        Quit ("NO SPEAR OF DESTINY DEMO DATA FILES TO BE FOUND!");
    strcpy (graphext, "sdm");
    strcpy (audioext, "sdm");
#endif
#else
    strcpy (graphext, extension);
    strcpy (audioext, extension);
#endif

    strcat (configname, extension);
    strcat (SaveName, extension);
    strcat (demoname, extension);

#ifndef SPEAR
#ifndef GOODTIMES
    strcat (helpfilename, extension);
#endif
    strcat (endfilename, extension);
#endif
#endif
}

```

you will see the same thing that i told you when we edited the version.h
so do the same thing on the line 4570:

```
//
// JAPANESE VERSION
//
#ifdef JAPAN
#ifdef JAPDEMO
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
    if(!stat(DATADIR "vswap.wj1", &statbuf))
#else  
    if (!stat("vswap.wj1", &statbuf))
#endif  
    {
        strcpy (extension, "wj1");
#else

#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
    if(!stat(DATADIR "vswap.wj6", &statbuf))
#else
    if(!stat("vswap.wj6", &statbuf))
#endif  
    {
        strcpy(extension, "wj6");
#endif
        strcat(configname, extension);
        strcat(SaveName, extension);
        strcat(demoname, extension);
        EpisodeSelect[1] =
            EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
    }
    else
        Quit ("NO JAPANESE WOLFENSTEIN 3-D DATA FILES to be found!");
    strcpy (graphext, extension);
    strcpy (audioext, extension);
#else

//
// ENGLISH
//
#ifdef UPLOAD
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
    if (!stat(DATADIR "vswap.wl1", &statbuf)) 
    {
  
    }
#else  
    if(!stat("vswap.wl1", &statbuf))
    {
  
    }
#endif
    else
        Quit ("NO WOLFENSTEIN 3-D DATA FILES to be found!");

#else
#ifndef SPEAR
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
    if(!stat(DATADIR "vswap.wl6", &statbuf))
#else
    if(!stat("vswap.wl6", &statbuf))
#endif
    {
        strcpy (extension, "wl6");
        NewEmenu[2].active =
            NewEmenu[4].active =
            NewEmenu[6].active =
            NewEmenu[8].active =
            NewEmenu[10].active =
            EpisodeSelect[1] =
            EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
    }
    else
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)
        if(!stat(DATADIR "vswap.wl3", &statbuf))
#else 
        if (!stat("vswap.wl3", &statbuf))
#endif
        {
            strcpy (extension, "wl3");
            NewEmenu[2].active = NewEmenu[4].active = EpisodeSelect[1] = EpisodeSelect[2] = 1;
        }
        else
        {
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)   
            if (!stat(DATADIR "vswap.wl1", &statbuf))
#else
            if (!stat("vswap.wl1", &statbuf))
#endif

                strcpy (extension, "wl1");
            else
                Quit ("NO WOLFENSTEIN 3-D DATA FILES to be found!");
        }
    }
#endif
#endif


#ifdef SPEAR
#ifndef SPEARDEMO
    if(param_mission == 0)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)   
        if(!stat(DATADIR"vswap.sod", &statbuf))
#else
        if(!stat("vswap.sod", &statbuf))
#endif
            strcpy (extension, "sod");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 1)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)   
        if(!stat(DATADIR"vswap.sd1", &statbuf))
#else
        if(!stat("vswap.sd1", &statbuf))
#endif
            strcpy (extension, "sd1");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 2)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)  
        if(!stat(DATADIR"vswap.sd2", &statbuf))
#else
        if(!stat("vswap.sd2", &statbuf))
#endif
            strcpy (extension, "sd2");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 3)
    {
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)   
        if(!stat(DATADIR"vswap.sd3", &statbuf))
#else  
        if(!stat("vswap.sd3", &statbuf))
#endif  
            strcpy (extension, "sd3");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else
        Quit ("UNSUPPORTED MISSION!");
    strcpy (graphext, "sod");
#ifndef VIEASM
    strcpy (audioext, "sod");
#endif
#else
#if defined(SWITCH) || defined (N3DS) || defined(PS2) || defined(SAMPLE_PLATFORM)  
    if(!stat(DATADIR "vswap.sdm", &statbuf))
#else
    if(!stat("vswap.sdm", &statbuf))
#endif
    {
        strcpy (extension, "sdm");
    }
    else
        Quit ("NO SPEAR OF DESTINY DEMO DATA FILES TO BE FOUND!");
    strcpy (graphext, "sdm");
    strcpy (audioext, "sdm");
#endif
#else
    strcpy (graphext, extension);
    strcpy (audioext, extension);
#endif

    strcat (configname, extension);
    strcat (SaveName, extension);
    strcat (demoname, extension);

#ifndef SPEAR
#ifndef GOODTIMES
    strcat (helpfilename, extension);
#endif
    strcat (endfilename, extension);
#endif
#endif
}
```

after this whole work you made the data directory work on your platform, so you can test/debug it whatever you want but if this step doesn´t work on your platform you should adapt it based on the rules.

# Second step: Change key event to your platform

After making the data directory work you will need to make work the input/event manager so first thing you will need to do is go to:
id_in.h and open it you will see this:

```
//
//	ID Engine
//	ID_IN.h - Header file for Input Manager
//	v1.0d1
//	By Jason Blochowiak
//

#ifndef	__ID_IN_H_
#define	__ID_IN_H_
#include "wl_def.h"

#ifdef	__DEBUG__
#define	__DEBUG_InputMgr__
#endif

#if defined(SWITCH) || defined (N3DS)
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_SCROLLLOCK	SDLK_SCROLLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif
typedef	int		ScanCode;
#define	sc_None			0
#define	sc_Bad			0xff
#define	sc_Return		SDLK_MINUS
#define	sc_Enter		sc_Return // ZR
#define	sc_Escape		SDLK_PLUS //SDLK_j // ZL
#define	sc_Space		SDLK_b
#define	sc_BackSpace	SDLK_BACKSPACE
#define	sc_Tab			SDLK_TAB
#define	sc_Alt			SDLK_x
#define	sc_Control		SDLK_a
#define	sc_CapsLock		SDLK_CAPSLOCK
#define	sc_LShift		SDLK_y
#define	sc_RShift		SDLK_RSHIFT
#define	sc_UpArrow		SDLK_UP
#define	sc_DownArrow	SDLK_DOWN
#define	sc_LeftArrow	SDLK_LEFT
#define	sc_RightArrow	SDLK_RIGHT
#define	sc_Insert		SDLK_INSERT
#define	sc_Delete		SDLK_DELETE
#define	sc_Home			SDLK_HOME
#define	sc_End			SDLK_END
#define	sc_PgUp			SDLK_PAGEUP
#define	sc_PgDn			SDLK_PAGEDOWN
#define	sc_F1			SDLK_F1
#define	sc_F2			SDLK_F2
#define	sc_F3			SDLK_F3
#define	sc_F4			SDLK_F4
#define	sc_F5			SDLK_F5
#define	sc_F6			SDLK_F6
#define	sc_F7			SDLK_F7
#define	sc_F8			SDLK_F8
#define	sc_F9			SDLK_F9
#define	sc_F10			SDLK_F10
#define	sc_F11			SDLK_F11
#define	sc_F12			SDLK_F12

#define sc_ScrollLock		SDLK_SCROLLOCK
#define sc_PrintScreen		SDLK_PRINT

#define	sc_1			SDLK_q
#define	sc_2			SDLK_q
#define	sc_3			SDLK_q
#define	sc_4			SDLK_q
#define	sc_5			SDLK_q
#define	sc_6			SDLK_q
#define	sc_7			SDLK_q
#define	sc_8			SDLK_q
#define	sc_9			SDLK_q
#define	sc_0			SDLK_q

#define	sc_A			SDLK_q
#define	sc_B			SDLK_q
#define	sc_C			SDLK_q
#define	sc_D			SDLK_q
#define	sc_E			SDLK_q
#define	sc_F			SDLK_q
#define	sc_G			SDLK_q
#define	sc_H			SDLK_q
#define	sc_I			SDLK_q
#define	sc_J			SDLK_q
#define	sc_K			SDLK_q
#define	sc_L			SDLK_q
#define	sc_M			SDLK_q
#define	sc_N			SDLK_q
#define	sc_O			SDLK_q
#define	sc_P			SDLK_q
#define	sc_Q			SDLK_q
#define	sc_R			SDLK_q
#define	sc_S			SDLK_q
#define	sc_T			SDLK_q
#define	sc_U			SDLK_q
#define	sc_V			SDLK_q
#define	sc_W			SDLK_q
#define	sc_X			SDLK_q
#define	sc_Y			SDLK_q
#define	sc_Z			SDLK_q

#define	key_None		0

#if SDL_MAJOR_VERSION == 2
#define bt_None SDL_CONTROLLER_BUTTON_INVALID
#define bt_A SDL_CONTROLLER_BUTTON_A
#define bt_B SDL_CONTROLLER_BUTTON_B
#define bt_X SDL_CONTROLLER_BUTTON_X
#define bt_Y SDL_CONTROLLER_BUTTON_Y
#define bt_Back SDL_CONTROLLER_BUTTON_BACK
#define bt_Guide SDL_CONTROLLER_BUTTON_GUIDE
#define bt_Start SDL_CONTROLLER_BUTTON_START
#define bt_LeftStick SDL_CONTROLLER_BUTTON_LEFTSTICK
#define bt_RightStick SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define bt_LeftShoulder SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define bt_RightShoulder SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define bt_DpadUp SDL_CONTROLLER_BUTTON_DPAD_UP
#define bt_DpadDown SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define bt_DpadLeft SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define bt_DpadRight SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define bt_touchpad SDL_CONTROLLER_BUTTON_TOUCHPAD
#define bt_Max SDL_CONTROLLER_BUTTON_MAX

//Axis stuff
#define gc_axis_invalid SDL_CONTROLLER_AXIS_INVALID
#define gc_axis_leftx SDL_CONTROLLER_AXIS_LEFTX
#define gc_axis_lefty SDL_CONTROLLER_AXIS_LEFTY
#define gc_axis_rightx SDL_CONTROLLER_AXIS_RIGHTX
#define gc_axis_righty SDL_CONTROLLER_AXIS_RIGHTY
#define gc_trigger_left SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define gc_trigger_right SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#define gc_axis_max SDL_CONTROLLER_AXIS_MAX
#endif
#define KEYCOUNT 129
#elif defined(PS2)
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_SCROLLLOCK	SDLK_SCROLLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif
#define KEYCOUNT 129
typedef	int		ScanCode;
#define	sc_None			0
#define	sc_Bad			0xff
#define	sc_Return		sc_B
#define	sc_Enter		sc_Return
#define	sc_Escape		SDLK_ESCAPE
#define	sc_Space		sc_X
#define	sc_BackSpace	SDLK_BACKSPACE
#define	sc_Tab			SDLK_TAB
#define	sc_Alt			SDLK_LALT
#define	sc_Control		SDLK_LCTRL
#define	sc_CapsLock		SDLK_CAPSLOCK
#define	sc_LShift		SDLK_LSHIFT
#define	sc_RShift		SDLK_RSHIFT
#define	sc_UpArrow		SDLK_UP
#define	sc_DownArrow	SDLK_DOWN
#define	sc_LeftArrow	SDLK_LEFT
#define	sc_RightArrow	SDLK_RIGHT
#define	sc_Insert		SDLK_INSERT
#define	sc_Delete		SDLK_DELETE
#define	sc_Home			SDLK_HOME
#define	sc_End			SDLK_END
#define	sc_PgUp			SDLK_PAGEUP
#define	sc_PgDn			SDLK_PAGEDOWN
#define	sc_F1			SDLK_F1
#define	sc_F2			SDLK_F2
#define	sc_F3			SDLK_F3
#define	sc_F4			SDLK_F4
#define	sc_F5			SDLK_F5
#define	sc_F6			SDLK_F6
#define	sc_F7			SDLK_F7
#define	sc_F8			SDLK_F8
#define	sc_F9			SDLK_F9
#define	sc_F10			SDLK_F10
#define	sc_F11			SDLK_F11
#define	sc_F12			SDLK_F12

#define sc_ScrollLock		SDLK_SCROLLLOCK
#define sc_PrintScreen		SDLK_PRINTSCREEN

#define	sc_1			SDLK_1
#define	sc_2			SDLK_2
#define	sc_3			SDLK_3
#define	sc_4			SDLK_4
#define	sc_5			SDLK_5
#define	sc_6			SDLK_6
#define	sc_7			SDLK_7
#define	sc_8			SDLK_8
#define	sc_9			SDLK_9
#define	sc_0			SDLK_0

#define	sc_A			SDLK_a
#define	sc_B			SDLK_b
#define	sc_C			SDLK_c
#define	sc_D			SDLK_d
#define	sc_E			SDLK_e
#define	sc_F			SDLK_f
#define	sc_G			SDLK_g
#define	sc_H			SDLK_h
#define	sc_I			SDLK_i
#define	sc_J			SDLK_j
#define	sc_K			SDLK_k
#define	sc_L			SDLK_l
#define	sc_M			SDLK_m
#define	sc_N			SDLK_n
#define	sc_O			SDLK_o
#define	sc_P			SDLK_p
#define	sc_Q			SDLK_q
#define	sc_R			SDLK_r
#define	sc_S			SDLK_s
#define	sc_T			SDLK_t
#define	sc_U			SDLK_u
#define	sc_V			SDLK_v
#define	sc_W			SDLK_w
#define	sc_X			SDLK_x
#define	sc_Y			SDLK_y
#define	sc_Z			SDLK_z

#define	key_None		0
#define key_unknown SDLK_UNKNOWN

#define bt_None SDL_CONTROLLER_BUTTON_INVALID
#define bt_A SDL_CONTROLLER_BUTTON_A
#define bt_B SDL_CONTROLLER_BUTTON_B
#define bt_X SDL_CONTROLLER_BUTTON_X
#define bt_Y SDL_CONTROLLER_BUTTON_Y
#define bt_Back SDL_CONTROLLER_BUTTON_BACK
#define bt_Guide SDL_CONTROLLER_BUTTON_GUIDE
#define bt_Start SDL_CONTROLLER_BUTTON_START
#define bt_LeftStick SDL_CONTROLLER_BUTTON_LEFTSTICK
#define bt_RightStick SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define bt_LeftShoulder SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define bt_RightShoulder SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define bt_DpadUp SDL_CONTROLLER_BUTTON_DPAD_UP
#define bt_DpadDown SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define bt_DpadLeft SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define bt_DpadRight SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define bt_touchpad SDL_CONTROLLER_BUTTON_TOUCHPAD
#define bt_Max SDL_CONTROLLER_BUTTON_MAX

//Axis stuff
#define gc_axis_invalid SDL_CONTROLLER_AXIS_INVALID
#define gc_axis_leftx SDL_CONTROLLER_AXIS_LEFTX
#define gc_axis_lefty SDL_CONTROLLER_AXIS_LEFTY
#define gc_axis_rightx SDL_CONTROLLER_AXIS_RIGHTX
#define gc_axis_righty SDL_CONTROLLER_AXIS_RIGHTY
#define gc_trigger_left SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define gc_trigger_right SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#define gc_axis_max SDL_CONTROLLER_AXIS_MAX
#if SDL_MAJOR_VERSION == 2
extern boolean GameControllerButtons[bt_Max];
extern int GameControllerLeftStick[2];
extern int GameControllerRightStick[2];
extern SDL_GameController* GameController;
#endif
#else
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_SCROLLLOCK	SDLK_SCROLLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif
#define KEYCOUNT 129
typedef	int		ScanCode;
#define	sc_None			0
#define	sc_Bad			0xff
#define	sc_Return		SDLK_RETURN
#define	sc_Enter		sc_Return
#define	sc_Escape		SDLK_ESCAPE
#define	sc_Space		SDLK_SPACE
#define	sc_BackSpace	SDLK_BACKSPACE
#define	sc_Tab			SDLK_TAB
#define	sc_Alt			SDLK_LALT
#define	sc_Control		SDLK_LCTRL
#define	sc_CapsLock		SDLK_CAPSLOCK
#define	sc_LShift		SDLK_LSHIFT
#define	sc_RShift		SDLK_RSHIFT
#define	sc_UpArrow		SDLK_UP
#define	sc_DownArrow	SDLK_DOWN
#define	sc_LeftArrow	SDLK_LEFT
#define	sc_RightArrow	SDLK_RIGHT
#define	sc_Insert		SDLK_INSERT
#define	sc_Delete		SDLK_DELETE
#define	sc_Home			SDLK_HOME
#define	sc_End			SDLK_END
#define	sc_PgUp			SDLK_PAGEUP
#define	sc_PgDn			SDLK_PAGEDOWN
#define	sc_F1			SDLK_F1
#define	sc_F2			SDLK_F2
#define	sc_F3			SDLK_F3
#define	sc_F4			SDLK_F4
#define	sc_F5			SDLK_F5
#define	sc_F6			SDLK_F6
#define	sc_F7			SDLK_F7
#define	sc_F8			SDLK_F8
#define	sc_F9			SDLK_F9
#define	sc_F10			SDLK_F10
#define	sc_F11			SDLK_F11
#define	sc_F12			SDLK_F12

#define sc_ScrollLock		SDLK_SCROLLLOCK
#define sc_PrintScreen		SDLK_PRINTSCREEN

#define	sc_1			SDLK_1
#define	sc_2			SDLK_2
#define	sc_3			SDLK_3
#define	sc_4			SDLK_4
#define	sc_5			SDLK_5
#define	sc_6			SDLK_6
#define	sc_7			SDLK_7
#define	sc_8			SDLK_8
#define	sc_9			SDLK_9
#define	sc_0			SDLK_0

#define	sc_A			SDLK_a
#define	sc_B			SDLK_b
#define	sc_C			SDLK_c
#define	sc_D			SDLK_d
#define	sc_E			SDLK_e
#define	sc_F			SDLK_f
#define	sc_G			SDLK_g
#define	sc_H			SDLK_h
#define	sc_I			SDLK_i
#define	sc_J			SDLK_j
#define	sc_K			SDLK_k
#define	sc_L			SDLK_l
#define	sc_M			SDLK_m
#define	sc_N			SDLK_n
#define	sc_O			SDLK_o
#define	sc_P			SDLK_p
#define	sc_Q			SDLK_q
#define	sc_R			SDLK_r
#define	sc_S			SDLK_s
#define	sc_T			SDLK_t
#define	sc_U			SDLK_u
#define	sc_V			SDLK_v
#define	sc_W			SDLK_w
#define	sc_X			SDLK_x
#define	sc_Y			SDLK_y
#define	sc_Z			SDLK_z

#define	key_None		0
#define key_unknown SDLK_UNKNOWN

#define bt_None SDL_CONTROLLER_BUTTON_INVALID
#define bt_A SDL_CONTROLLER_BUTTON_A
#define bt_B SDL_CONTROLLER_BUTTON_B
#define bt_X SDL_CONTROLLER_BUTTON_X
#define bt_Y SDL_CONTROLLER_BUTTON_Y
#define bt_Back SDL_CONTROLLER_BUTTON_BACK
#define bt_Guide SDL_CONTROLLER_BUTTON_GUIDE
#define bt_Start SDL_CONTROLLER_BUTTON_START
#define bt_LeftStick SDL_CONTROLLER_BUTTON_LEFTSTICK
#define bt_RightStick SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define bt_LeftShoulder SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define bt_RightShoulder SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define bt_DpadUp SDL_CONTROLLER_BUTTON_DPAD_UP
#define bt_DpadDown SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define bt_DpadLeft SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define bt_DpadRight SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define bt_touchpad SDL_CONTROLLER_BUTTON_TOUCHPAD
#define bt_Max SDL_CONTROLLER_BUTTON_MAX

//Axis stuff
#define gc_axis_invalid SDL_CONTROLLER_AXIS_INVALID
#define gc_axis_leftx SDL_CONTROLLER_AXIS_LEFTX
#define gc_axis_lefty SDL_CONTROLLER_AXIS_LEFTY
#define gc_axis_rightx SDL_CONTROLLER_AXIS_RIGHTX
#define gc_axis_righty SDL_CONTROLLER_AXIS_RIGHTY
#define gc_trigger_left SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define gc_trigger_right SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#define gc_axis_max SDL_CONTROLLER_AXIS_MAX

#if SDL_MAJOR_VERSION == 2
extern boolean GameControllerButtons[bt_Max];
extern int GameControllerLeftStick[2];
extern int GameControllerRightStick[2];
extern SDL_GameController* GameController;
#endif
#endif

typedef	enum
{
	demo_Off,
	demo_Record,
	demo_Playback,
	demo_PlayDone
} Demo;

typedef	enum
{
	ctrl_Keyboard,
	ctrl_Keyboard1 = ctrl_Keyboard,
	ctrl_Keyboard2,
	ctrl_Joystick,
	ctrl_Joystick1 = ctrl_Joystick,
	ctrl_Joystick2,
	ctrl_Mouse
} ControlType;

typedef	enum
{
	motion_Left = -1,
	motion_Up = -1,
	motion_None = 0,
	motion_Right = 1,
	motion_Down = 1
} Motion;

typedef	enum
{
	dir_North,
	dir_NorthEast,
	dir_East,
	dir_SouthEast,
	dir_South,
	dir_SouthWest,
	dir_West,
	dir_NorthWest,
	dir_None
} Direction;

typedef	struct
{
	boolean	button0, button1, button2, button3;
	short x, y;
	Motion xaxis, yaxis;
	Direction dir;
} CursorInfo;

typedef	CursorInfo ControlInfo;

typedef	struct
{
	ScanCode button0, button1,
		upleft, up, upright,
		left, right,
		downleft, down, downright;
} KeyboardDef;

typedef	struct
{
	unsigned short joyMinX, joyMinY;
	unsigned short threshMinX, threshMinY;
	unsigned short threshMaxX, threshMaxY;
	unsigned short joyMaxX, joyMaxY;
	unsigned short joyMultXL, joyMultYL;
	unsigned short joyMultXH, joyMultYH;
} JoystickDef;

//Global variables
extern volatile boolean KeyboardState[129];
extern boolean MousePresent;
extern volatile boolean Paused;
extern volatile char LastASCII;
extern volatile ScanCode LastScan;
extern int JoyNumButtons;
extern boolean forcegrabmouse;
extern volatile int WheelPos;


boolean Keyboard(int key);
void KeyboardSet(int key, boolean state);
int KeyboardLookup(int key);

// Function prototypes
#define	IN_KeyDown(code)	(Keyboard((code)))
#define	IN_ClearKey(code)	{KeyboardSet(code, false);\
							if (code == LastScan) LastScan = sc_None;}

// DEBUG - put names in prototypes
extern	void		IN_Startup(void), IN_Shutdown(void);
extern	void		IN_ClearKeysDown(void);
extern	void		IN_ReadControl(ControlInfo*);
extern  void        IN_Ack(void);
extern	boolean		IN_UserInput(unsigned int delay);
extern	char		IN_WaitForASCII(void);
extern	ScanCode	IN_WaitForKey(void);
extern	const char* IN_GetScanName(ScanCode);

void    IN_WaitAndProcessEvents();
void    IN_ProcessEvents();

int     IN_MouseButtons(void);

boolean IN_JoyPresent();
int     IN_JoyButtons(void);
void    IN_GetJoyDelta(int* dx, int* dy);
void    IN_GetJoyFineDelta(int* dx, int* dy);

void    IN_StartAck(void);
boolean IN_CheckAck(void);
boolean    IN_IsInputGrabbed();
void    IN_CenterMouse();

#endif

```

you will notice on line 282 some input lines used for the keyboard events, so you probablly you know the drill
guard these events on 282 to 404 for your platform standards like this:

```

#elif defined(SAMPLE_PLATFORM)
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_SCROLLLOCK	SDLK_SCROLLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif
#define KEYCOUNT 129
typedef	int		ScanCode;
#define	sc_None			0
#define	sc_Bad			0xff
#define	sc_Return		SDLK_RETURN
#define	sc_Enter		sc_Return
#define	sc_Escape		SDLK_ESCAPE
#define	sc_Space		SDLK_SPACE
#define	sc_BackSpace	SDLK_BACKSPACE
#define	sc_Tab			SDLK_TAB
#define	sc_Alt			SDLK_LALT
#define	sc_Control		SDLK_LCTRL
#define	sc_CapsLock		SDLK_CAPSLOCK
#define	sc_LShift		SDLK_LSHIFT
#define	sc_RShift		SDLK_RSHIFT
#define	sc_UpArrow		SDLK_UP
#define	sc_DownArrow	SDLK_DOWN
#define	sc_LeftArrow	SDLK_LEFT
#define	sc_RightArrow	SDLK_RIGHT
#define	sc_Insert		SDLK_INSERT
#define	sc_Delete		SDLK_DELETE
#define	sc_Home			SDLK_HOME
#define	sc_End			SDLK_END
#define	sc_PgUp			SDLK_PAGEUP
#define	sc_PgDn			SDLK_PAGEDOWN
#define	sc_F1			SDLK_F1
#define	sc_F2			SDLK_F2
#define	sc_F3			SDLK_F3
#define	sc_F4			SDLK_F4
#define	sc_F5			SDLK_F5
#define	sc_F6			SDLK_F6
#define	sc_F7			SDLK_F7
#define	sc_F8			SDLK_F8
#define	sc_F9			SDLK_F9
#define	sc_F10			SDLK_F10
#define	sc_F11			SDLK_F11
#define	sc_F12			SDLK_F12

#define sc_ScrollLock		SDLK_SCROLLLOCK
#define sc_PrintScreen		SDLK_PRINTSCREEN

#define	sc_1			SDLK_1
#define	sc_2			SDLK_2
#define	sc_3			SDLK_3
#define	sc_4			SDLK_4
#define	sc_5			SDLK_5
#define	sc_6			SDLK_6
#define	sc_7			SDLK_7
#define	sc_8			SDLK_8
#define	sc_9			SDLK_9
#define	sc_0			SDLK_0

#define	sc_A			SDLK_a
#define	sc_B			SDLK_b
#define	sc_C			SDLK_c
#define	sc_D			SDLK_d
#define	sc_E			SDLK_e
#define	sc_F			SDLK_f
#define	sc_G			SDLK_g
#define	sc_H			SDLK_h
#define	sc_I			SDLK_i
#define	sc_J			SDLK_j
#define	sc_K			SDLK_k
#define	sc_L			SDLK_l
#define	sc_M			SDLK_m
#define	sc_N			SDLK_n
#define	sc_O			SDLK_o
#define	sc_P			SDLK_p
#define	sc_Q			SDLK_q
#define	sc_R			SDLK_r
#define	sc_S			SDLK_s
#define	sc_T			SDLK_t
#define	sc_U			SDLK_u
#define	sc_V			SDLK_v
#define	sc_W			SDLK_w
#define	sc_X			SDLK_x
#define	sc_Y			SDLK_y
#define	sc_Z			SDLK_z

#define	key_None		0
#define key_unknown SDLK_UNKNOWN
#if SDL_MAJOR_VERSION == 2
#define bt_None SDL_CONTROLLER_BUTTON_INVALID
#define bt_A SDL_CONTROLLER_BUTTON_A
#define bt_B SDL_CONTROLLER_BUTTON_B
#define bt_X SDL_CONTROLLER_BUTTON_X
#define bt_Y SDL_CONTROLLER_BUTTON_Y
#define bt_Back SDL_CONTROLLER_BUTTON_BACK
#define bt_Guide SDL_CONTROLLER_BUTTON_GUIDE
#define bt_Start SDL_CONTROLLER_BUTTON_START
#define bt_LeftStick SDL_CONTROLLER_BUTTON_LEFTSTICK
#define bt_RightStick SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define bt_LeftShoulder SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define bt_RightShoulder SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define bt_DpadUp SDL_CONTROLLER_BUTTON_DPAD_UP
#define bt_DpadDown SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define bt_DpadLeft SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define bt_DpadRight SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define bt_touchpad SDL_CONTROLLER_BUTTON_TOUCHPAD
#define bt_Max SDL_CONTROLLER_BUTTON_MAX

//Axis stuff
#define gc_axis_invalid SDL_CONTROLLER_AXIS_INVALID
#define gc_axis_leftx SDL_CONTROLLER_AXIS_LEFTX
#define gc_axis_lefty SDL_CONTROLLER_AXIS_LEFTY
#define gc_axis_rightx SDL_CONTROLLER_AXIS_RIGHTX
#define gc_axis_righty SDL_CONTROLLER_AXIS_RIGHTY
#define gc_trigger_left SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define gc_trigger_right SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#define gc_axis_max SDL_CONTROLLER_AXIS_MAX
#endif

```

Then fill your keys there on your platform.

Well that´s it folks now you´re ready to modding i´will provide more instructions later and when i have i time, so good luck for everyone who wants to port/support it.

# Copyright:

André "Wolf3s" Guilherme
