#ifndef __VERSION_H_
#define __VERSION_H_

#ifndef VERSIONALREADYCHOSEN              // used for batch compiling

/* Defines used for different versions */
#define SPEAR
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

//This defines must be activated with LWLIB + WOLFRAD + USE_SHADE
//#define LWLIB //Actives the LinuxWolf library ported to C99
//#define WOLFRAD //Actives the Wolfrad functions ported to C99
//#define USE_AI //Enables the Artificial intelligence function(see wl_ai.c)
//#define USE_ACTOR //Enables the Actor function(see wl_anyactor.c)
//#define USE_BENCHHOBO //Enables the benchhobo function(see wl_benchhobo.c)
//#define USE_CONARTIST //Enables conartist function(see wl_conartist.c)
//#define USE_CLOCKKING //Enables clockking function(see wl_clockking.c)
//#define USE_EDITOR //Enables the editor(see wl_ed.c)
//#define USE_FIREFLY //Enables the fire effect?(see wl_firefly.c)
//#define USE_HEALTHMETTER //Enables the health metter(see wl_healthmetter.c)
//#define USE_HOOKER //Enables the hooker function(see wl_hooker.c)
//#define USE_LED //Enables the led function(see wl_led.c)
//#define USE_MATH //Enables the math function(see wl_math.c)
//#define USE_PHYSICS //Enables the physic function(see wl_physics.c)
//#define USE_PIMP //Enables the pimp function(see wl_pimp.c)
//#define USE_POLYGON //Enables the polygon function(see wl_polygon.c)
//#define USE_SLEEPHOBO //Enables the polygon function(see wl_sleephobo.c)
//End.

//#define USE_FEATUREFLAGS    // Enables the level feature flags (see bottom of wl_def.h)
//#define USE_SHADING         // Enables shading support (see wl_shade.c)
//#define USE_DIR3DSPR        // Enables directional 3d sprites
//#define USE_FLOORCEILINGTEX // Enables texture-mapped floors and ceilings (see wl_plane.c)
//#define USE_MULTIFLATS      // Enables floor and ceiling textures stored in the third mapplane
//#define USE_PARALLAX 16     // Enables parallax sky with 16 textures per sky (see wl_parallax.c)
//#define USE_SKYWALLPARALLAX 16 // Enables parallax sky on walls with 16 repeats of sky texture
//#define USE_CLOUDSKY        // Enables cloud sky support (see wl_cloudsky.c)
//#define USE_STARSKY         // Enables star sky support (see wl_atmos.c)
//#define USE_RAIN            // Enables rain support (see wl_atmos.c)
//#define USE_SNOW            // Enables snow support (see wl_atmos.c)
//#define FIXRAINSNOWLEAKS    // Enables leaking ceilings fix (by Adam Biser, only needed if maps with rain/snow and ceilings exist)
//#define VIEWMAP             // Enables the overhead map
//#define REVEALMAP           // Enables showing only the areas of the overhead map that have been seen


//#define DEBUGKEYS             // Comment this out to compile without the Tab debug keys
#define ARTSEXTERN
#define DEMOSEXTERN
#define PLAYDEMOLIKEORIGINAL  // When playing or recording demos, several bug fixes do not take
                              // effect to let the original demos work as in the original Wolf3D v1.4
                              // (actually better, as the second demo rarely worked)
#define CRT // Makes the screen fixed in 4:3
//#define SCALE2X //The screen will be scaled in 2x with linear filter. This guy needs to work with CRT defined or else it won´t work..
#define FIXCALCROTATE         // Apply a modified version of Ginyu's fix to make CalcRotate more accurate at high resolutions
//#define VIEASM //Activates: VODKA-INDUCED ENTERTAINMENT ADVANCED SOUND MANAGER v0.9.2(Warning: only works with .wav and .ogg placed in sounds/ follow the asmcref.h for more details).
//#define BANDEDHOLOWALLS     // Use the old DOS-style "banded" wall drawing behaviour when inside walls
//#define USE_DOSBOX //Replaces fmopl sound and uses DOSBOX Sound instead.
//#define GOD_MODE //Use godmode

#if defined(SWITCH) 
#define DATADIR "/switch/wolf4sdl/wolf3d/"
#elif defined(N3DS)
#define DATADIR "/3ds/wolf4sdl/wolf3d/"
#elif defined(PS2)
#define DATADIR "/data"
#elif defined(SEGA_SATURN)
#define DATADIR "/data"
#endif
#endif