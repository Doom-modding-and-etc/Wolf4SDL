#ifndef __VERSION_H_
#define __VERSION_H_

#ifndef VERSIONALREADYCHOSEN              /* used for batch compiling */

/* Defines used for different versions */
/* #define SPEAR */
/* #define SPEARDEMO */
/* #define UPLOAD */
#define GOODTIMES
#define CARMACIZED
/* #define APOGEE_1_0 */
/* #define APOGEE_1_1 */
/* #define APOGEE_1_2 */
/* #define JAPAN */

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

/* #define USE_DIR3DSPR_SAMPLE */        /* Enables directional 3d sprites sample */
/* #define FIXRAINSNOWLEAKS */    /* Enables leaking ceilings fix (by Adam Biser, only needed if maps with rain/snow and ceilings exist) */
/* #define VIEWMAP */             /* Enables the overhead map */
/* #define REVEALMAP */           /* Enables showing only the areas of the overhead map that have been seen */
/* #define MENU_DEMOS */          /* Adds the menu demos support */
/* #define DEBUGKEYS */           /* Comment this out to compile without the Tab debug keys */
#define ARTSEXTERN
#define DEMOSEXTERN
#define PLAYDEMOLIKEORIGINAL  /* 
                              ** When playing or recording demos, several bug fixes do not take 
                              ** effect to let the original demos work as in the original Wolf3D v1.4 
                              ** (actually better, as the second demo rarely worked)
                              */
/* #define LWUDPCOMMS */            /* Enables UDP networked communications for multiplayer support */
/* #define CRT */ /* Makes the screen fixed in 4:3 */
/* #define SCALE2X */ /*The screen will be scaled in 2x with linear filter. This guy needs to work with CRT defined or else it won´t work.. */
/* #define FIXCALCROTATE */         /* Apply a modified version of Ginyu's fix to make CalcRotate more accurate at high resolutions */
/* #define VIEASM */ /* Activates: VODKA-INDUCED ENTERTAINMENT ADVANCED SOUND MANAGER v0.9.2(Warning: only works with .wav and .ogg placed in sounds/ follow the asmcref.h for more details). */
/* #define USE_DOSBOX */  /* Replaces fmopl sound and uses DOSBOX Sound instead. */
/* #define USE_NUKEDOPL */ /* Replaces fmopl sound and uses NUKEDOPL instead. */ 
/* #define USE_FBNEO_FMOPL */ /* Replaces fmopl sound and uses FBNEO_FMOPL instead. */
/* #define USE_AUDIO_CVT */  /* Replaces original implmentation of wave file reading to a modern way by Fabian Greffrath */
/* #define GOD_MODE */ /* Use godmode */
/* Credits: Codetech84 */
/* #define MAPCONTROLLEDMUSIC */  /* Activates the map controlled music. */
/* #define MAPCONTROLPARTIME */   /* Activates the map partial time control */
/* #define MAPCONTROLLEDSHADE */  /* Activates the controled shade */
/* #define MAPCONTROLLEDPLSETT */ /* Activates the map control plsett */
/* #define MAPCONTROLLEDLTIME */  /* Activates the map control time */
/* #define MAPCONTROLLEDSKY */    /* Activates the sky control */
/* #define MAPCONTROLLEDCEILING */ /* Map Controlled Ceiling colour (8,0) */
/* #define MAPCONTROLLEDFLOOR */   /* Map Controlled Floor colour (9,0) */
/* #define SEEKER_MISSILES */ /* Add Seeker Missiles */
/* end */
/* #define EXTRACONTROLS */  /* switches out to Joystick controls for WASD and mouse movement toggles instead */ 
/* #define AUTOINTER */ /* Enables the automatic intermission screen */
/* #define CHRIS */ /* Enables the chris chokan patches */
/* #define BULLET_CALC */ /* Enable the Bullet Calculation */
/* #define LOGFILE */ /* Activate the log file feature, Credits: Havoc */
/* By: WSJ */
/* #define BURN_ANIM */ /* Add the burn animation */
/* #define BOSS_MUSIC */ /* Add some boss music, it needs VIEASM defined */
/* end */
/* #define SAVE_GAME_SCREENSHOT */ /* Add screenshot save support. */
/* By: WallHack */
/* #define COMPASS */ /* Add the compass */
/* end */
/* By: ncornette(aka: Nicolas Cornette) */
/* #define CSGO_STRAFE */ /* Hack to enable CS-Like WASD keys strafe + Mouse. */
/* end */
/* By: FraserChapman */
/* #define GAMEBOY_ZERO */ /* Replaces keyboard buttons to GAMEBOY_ZERO buttons */
/* end */
/* By: ksv1986(aka: Andrey Kuleshov) */
/* #define HAPTIC_SUPPORT */ /* Enables the Haptic support(atm it only supports XInput rumble) */
/* end */

#ifdef USE_DATADIR
#if defined(SWITCH) 
#define DATADIR "/switch/wolf4sdl/wolf3d/"
#elif defined(N3DS)
#define DATADIR "/3ds/wolf4sdl/wolf3d/"
#elif defined(PS2)
#if defined(USB)
#define DATADIR "mass0:/data"
#elif defined(HDD)
#define DATADIR "hdd0:/data"
#elif defined(CDFS) /* dvd support */
#define DATADIR "cdrom:/data"
#else
#define DATADIR "host0:/data"
#endif
#elif defined(SEGA_SATURN)
#define DATADIR "/data"
#elif defined(PSVITA)
#define DATADIR "ux0:data/Wolfenstein 3D/"
#elif defined(ZIPIT_Z2)
#define DATADIR "/usr/share/games/wolf3d/"
#else
#define DATADIR "data/"
#endif
#endif

#endif
