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
//#define BLAKEDOORS     // Blake Stone style Double doors - original code by: Adam Biser
//#define EXTRACONTROLS // switches out to Joystick controls for WASD and mouse movement toggles instead 
//#define AUTOINTER //Enables the automatic intermission screen

//By: WSJ
//#define BURN_ANIM //Add the burn animation
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
