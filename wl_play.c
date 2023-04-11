// WL_PLAY.C

#include "wl_def.h"

#include "wl_cloudsky.h"
#include "wl_shade.h"

/*
=============================================================================

                                                 LOCAL CONSTANTS

=============================================================================
*/

#define sc_Question     0x35

/*
=============================================================================

                                                 GLOBAL VARIABLES

=============================================================================
*/

boolean madenoise;              // true when shooting or screaming

exit_t playstate;

#ifdef MAPCONTROLLEDMUSIC
static musicnames lastmusicchunk;
#else
static musicnames lastmusicchunk = (musicnames) 0;
#endif
#ifndef SEGA_SATURN
boolean     DebugOk;
#endif

#ifdef COMPASS
boolean compass;
#endif

objtype objlist[MAXACTORS];
objtype *newobj, *obj, *player, *lastobj, *objfreelist, *killerobj;
#ifdef SEGA_SATURN
boolean godmode;
#else
#ifdef HIGHLIGHTPUSHWALLS
boolean singlestep,godmode,noclip,ammocheat,mapreveal,highlightmode;
#else
boolean singlestep, godmode, noclip, ammocheat, mapreveal;
#endif
#endif
int     extravbls;

tiletype tilemap[MAPSIZE][MAPSIZE]; // wall values only
boolean     spotvis[MAPSIZE][MAPSIZE];
objtype *actorat[MAPSIZE][MAPSIZE];
#ifdef REVEALMAP
boolean     mapseen[MAPSIZE][MAPSIZE];
#endif
#if defined(FIXEDLOGICRATE) && defined(LAGSIMULATOR)
boolean lagging = true;
#endif

//
// replacing refresh manager
//
unsigned short     mapwidth,mapheight;
size_t tics;

//
// control info
//
#ifndef SEGA_SATURN
boolean mouseenabled, joystickenabled;
#endif
int dirscan[4] =
{
    sc_UpArrow,
    sc_RightArrow,
    sc_DownArrow,
    sc_LeftArrow
};

int buttonscan[NUMBUTTONS] = 
{ 
    sc_Control, 
    sc_Alt, 
    sc_LShift, 
    sc_Space, 
    sc_1, 
    sc_2, 
    sc_3, 
    sc_4, 
#ifdef EXTRACONTROLS
    sc_W,
    sc_S,
    sc_A, 
    sc_D, 
#endif
};

#ifndef SEGA_SATURN
#if SDL_MAJOR_VERSION == 2
int buttongamecontroller[bt_Max] =
{
    bt_use,
    bt_prevweapon,
    bt_nobutton,
    bt_nextweapon,
    bt_pause,
    bt_nobutton,
    bt_esc,
    bt_nobutton,
    bt_nobutton,
    bt_run,
    bt_attack,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
#ifndef XBOX
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton
#endif
};
#endif

int buttonmouse[4] =
{
    bt_attack,
    bt_strafe,
    bt_use,
    bt_nobutton
};

int buttonjoy[32] =
{

#ifdef _arch_dreamcast
    bt_attack,
    bt_strafe,
    bt_use,
    bt_run,
    bt_esc,
    bt_prevweapon,
    bt_nobutton,
    bt_nextweapon,
    bt_pause,
    bt_strafeleft,
    bt_straferight,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
#else
    bt_attack,
    bt_strafe,
    bt_use,
    bt_run,
    bt_strafeleft,
    bt_straferight,
    bt_esc,
    bt_pause,
    bt_prevweapon,
    bt_nextweapon,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
#endif
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton,
    bt_nobutton
};
#endif
int viewsize;

boolean buttonheld[NUMBUTTONS];

boolean demorecord, demoplayback;
char *demoptr, *lastdemoptr;
#ifndef SEGA_SATURN
void   *demobuffer;
#endif
//
// current user input
//
int controlx, controly;         // range from -100 to 100 per tic
#if SDL_MAJOR_VERSION == 2
int gamecontrolstrafe;
#endif

#ifdef EXTRACONTROLS
int controlstrafe;
#endif // EXTRACONTROLS
boolean buttonstate[NUMBUTTONS];

int lastgamemusicoffset = 0;
#if defined(USE_SPRITES) && defined(SEGA_SATURN)
extern unsigned int position_vram;
extern unsigned int static_items;
extern unsigned char wall_buffer[(SATURN_WIDTH + 64) * 64];
extern SPRITE user_walls[MAX_WALLS];
extern char texture_list[SPR_NULLSPRITE];
#endif

//===========================================================================


void CenterWindow (unsigned short w, unsigned short h);
void RemoveObj (objtype * gone);
void PollControls (void);
int StopMusic (void);
void StartMusic (void);
void ContinueMusic (int offs);
void PlayLoop (void);

/*
=============================================================================

                                                 LOCAL VARIABLES

=============================================================================
*/

#ifndef SEGA_SATURN
objtype dummyobj;
#endif
//
// LIST OF SONGS FOR EACH VERSION
//
int songs[] = {
#ifndef SPEAR
    //
    // Episode One
    //
    GETTHEM_MUS,
    SEARCHN_MUS,
    POW_MUS,
    SUSPENSE_MUS,
    GETTHEM_MUS,
    SEARCHN_MUS,
    POW_MUS,
    SUSPENSE_MUS,

    WARMARCH_MUS,               // Boss level
    CORNER_MUS,                 // Secret level

    //
    // Episode Two
    //
    NAZI_OMI_MUS,
    PREGNANT_MUS,
    GOINGAFT_MUS,
    HEADACHE_MUS,
    NAZI_OMI_MUS,
    PREGNANT_MUS,
    HEADACHE_MUS,
    GOINGAFT_MUS,

    WARMARCH_MUS,               // Boss level
    DUNGEON_MUS,                // Secret level

    //
    // Episode Three
    //
    INTROCW3_MUS,
    NAZI_RAP_MUS,
    TWELFTH_MUS,
    ZEROHOUR_MUS,
    INTROCW3_MUS,
    NAZI_RAP_MUS,
    TWELFTH_MUS,
    ZEROHOUR_MUS,

    ULTIMATE_MUS,               // Boss level
    PACMAN_MUS,                 // Secret level

    //
    // Episode Four
    //
    GETTHEM_MUS,
    SEARCHN_MUS,
    POW_MUS,
    SUSPENSE_MUS,
    GETTHEM_MUS,
    SEARCHN_MUS,
    POW_MUS,
    SUSPENSE_MUS,

    WARMARCH_MUS,               // Boss level
    CORNER_MUS,                 // Secret level

    //
    // Episode Five
    //
    NAZI_OMI_MUS,
    PREGNANT_MUS,
    GOINGAFT_MUS,
    HEADACHE_MUS,
    NAZI_OMI_MUS,
    PREGNANT_MUS,
    HEADACHE_MUS,
    GOINGAFT_MUS,

    WARMARCH_MUS,               // Boss level
    DUNGEON_MUS,                // Secret level

    //
    // Episode Six
    //
    INTROCW3_MUS,
    NAZI_RAP_MUS,
    TWELFTH_MUS,
    ZEROHOUR_MUS,
    INTROCW3_MUS,
    NAZI_RAP_MUS,
    TWELFTH_MUS,
    ZEROHOUR_MUS,

    ULTIMATE_MUS,               // Boss level
    FUNKYOU_MUS                 // Secret level
#else

    //////////////////////////////////////////////////////////////
    //
    // SPEAR OF DESTINY TRACKS
    //
    //////////////////////////////////////////////////////////////
    XTIPTOE_MUS,
    XFUNKIE_MUS,
    XDEATH_MUS,
    XGETYOU_MUS,                // DON'T KNOW
    ULTIMATE_MUS,               // Trans Grosse

    DUNGEON_MUS,
    GOINGAFT_MUS,
    POW_MUS,
    TWELFTH_MUS,
    ULTIMATE_MUS,               // Barnacle Wilhelm BOSS

    NAZI_OMI_MUS,
    GETTHEM_MUS,
    SUSPENSE_MUS,
    SEARCHN_MUS,
    ZEROHOUR_MUS,
    ULTIMATE_MUS,               // Super Mutant BOSS

    XPUTIT_MUS,
    ULTIMATE_MUS,               // Death Knight BOSS

    XJAZNAZI_MUS,               // Secret level
    XFUNKIE_MUS,                // Secret level (DON'T KNOW)

    XEVIL_MUS                   // Angel of Death BOSS
#endif
};


/*
=============================================================================

                               USER CONTROL

=============================================================================
*/

/*
===================
=
= PollKeyboardButtons
=
===================
*/

void PollKeyboardButtons (void)
{
    int i;

    for (i = 0; i < NUMBUTTONS; i++)
        if (Keyboard(buttonscan[i]))
            buttonstate[i] = true;
}


/*
===================
=
= PollMouseButtons
=
===================
*/

int LastWheelPos=0;


void PollMouseButtons (void)
{
    int buttons = IN_MouseButtons ();

    if (WheelPos < LastWheelPos)
    {
        buttonstate[bt_prevweapon] = true;
    } 
     
    if (WheelPos > LastWheelPos)
    {
        buttonstate[bt_nextweapon] = true;
    }

    LastWheelPos = WheelPos;

    if (buttons & 1)
        buttonstate[buttonmouse[0]] = true;
    if (buttons & 2)
        buttonstate[buttonmouse[1]] = true;
    if (buttons & 4)
        buttonstate[buttonmouse[2]] = true;
}
#ifndef EXTRACONTROLS
/*
===================
=
= PollJoystickButtons
=
===================
*/

void PollJoystickButtons (void)
{
    int i,val,buttons = IN_JoyButtons();

    for(i = 0, val = 1; i < JoyNumButtons; i++, val <<= 1)
    {
        if(buttons & val)
            buttonstate[buttonjoy[i]] = true;
    }
}
#endif
#if SDL_MAJOR_VERSION == 2
/*
===================
=
= PollGameControllerButtons
=
===================
*/
void PollGameControllerButtons(void)
{
    int i;
    for (i = 0; i < bt_Max; i++)
    {
        if (GameControllerButtons[i])
        {
            if (buttongamecontroller[i] != -1)
                buttonstate[buttongamecontroller[i]] = true;
        }
    }
}
#endif



/*
===================
=
= PollKeyboardMove
=
===================
*/

void PollKeyboardMove (void)
{
    int delta = buttonstate[bt_run] ? RUNMOVE * (int)tics : BASEMOVE * (int)tics;

    if (Keyboard(dirscan[di_north]))
        controly -= delta;
    if (Keyboard(dirscan[di_south]))
        controly += delta;
    if (Keyboard(dirscan[di_west]))
        controlx -= delta;
    if (Keyboard(dirscan[di_east]))
        controlx += delta;
}


/*
===================
=
= PollMouseMove
=
===================
*/

void PollMouseMove (void)
{
    int mousexmove, mouseymove;

#if SDL_MAJOR_VERSION == 1
    SDL_GetMouseState(&mousexmove, &mouseymove);
    if(IN_IsInputGrabbed())
        IN_CenterMouse();

    mousexmove -= screenWidth / 2;
    mouseymove -= screenHeight / 2;
#else
    SDL_GetRelativeMouseState(&mousexmove, &mouseymove);
#endif

    controlx += mousexmove * 10 / (13 - mouseadjustment);
#ifndef EXTRACONTROLS
    controly += mouseymove * 20 / (13 - mouseadjustment);
#else
    if (mousemoveenabled)
    {
        controly += mouseymove * 20 / (13 - mouseadjustment);
    }
#endif
}


/*
===================
=
= PollJoystickMove
=
===================
*/

void PollJoystickMove (void)
{
    int joyx, joyy;
	int delta = buttonstate[bt_run] ? RUNMOVE * (int)tics : BASEMOVE * (int)tics;
    IN_GetJoyDelta (&joyx, &joyy);

    if (joyx > 64 || buttonstate[bt_turnright])
        controlx += delta;
    else if (joyx < -64  || buttonstate[bt_turnleft])
        controlx -= delta;
    if (joyy > 64 || buttonstate[bt_movebackward])
        controly += delta;
    else if (joyy < -64 || buttonstate[bt_moveforward])
        controly -= delta;
}


#if SDL_MAJOR_VERSION == 2
/*
===================
=
= PollGameControllerMove
=
===================
*/
void PollGameControllerMove(void)
{
    int delta = buttonstate[bt_run] ? RUNMOVE * (int)tics : BASEMOVE * (int)tics;

    if (GameControllerRightStick[0] > 64)
        controlx += delta;
    else if (GameControllerRightStick[0] < -64)
        controlx -= delta;

    if (GameControllerLeftStick[1] > 120)
        controly += delta;
    else if (GameControllerLeftStick[1] < -64)
        controly -= delta;

    if (GameControllerLeftStick[0] > 64)
        gamecontrolstrafe += delta;
    else if (GameControllerLeftStick[0] < -64)
        gamecontrolstrafe -= delta;

}
#endif
/*
===================
=
= PollControls
=
= Gets user or demo input, call once each frame
=
= controlx              set between -100 and 100 per tic
= controly
= buttonheld[]  the state of the buttons LAST frame
= buttonstate[] the state of the buttons THIS frame
=
===================
*/

void PollControls (void)
{
    int max, min, i;
    unsigned char buttonbits;
#if defined(SWITCH) 
	unsigned int kDown;
	float turnspeed = 0;
    float movespeed = 0;
	int delta;
#elif defined(N3DS)
	unsigned int kDown;
	int delta;
	circlePosition cpos;
	int cx = 0, cy = 0;
#endif

    IN_ProcessEvents();

//
// get timing info for last frame
//
    if (demoplayback || demorecord)   // demo recording and playback needs to be constant
    {
        // wait up to DEMOTICS Wolf tics
        size_t curtime = WL_GetTicks();
		size_t timediff;
        lasttimecount += DEMOTICS;
        timediff = (lasttimecount * 100) / 7 - curtime;
        if(timediff > 0)
            SDL_Delay((unsigned int)timediff);

        if(timediff < -2 * DEMOTICS)       // more than 2-times DEMOTICS behind?
            lasttimecount = (curtime * 7) / 100;    // yes, set to current timecount

        tics = DEMOTICS;
    }
    else
        CalcTics ();

    controlx = 0;
    controly = 0;
#if SDL_MAJOR_VERSION == 2
    gamecontrolstrafe = 0;
#endif
    memcpy(buttonheld, buttonstate, sizeof(buttonstate));
    memset(buttonstate, 0, sizeof(buttonstate));

    if (demoplayback)
    {
        //
        // read commands from demo buffer
        //
        buttonbits = *demoptr++;
        for (i = 0; i < NUMBUTTONS; i++)
        {
            buttonstate[i] = buttonbits & 1;
            buttonbits >>= 1;
        }

        controlx = *demoptr++;
        controly = *demoptr++;

        if (demoptr == lastdemoptr)
            playstate = ex_completed;   // demo is done

        controlx *= (int) tics;
        controly *= (int) tics;

        return;
    }


//
// get button states
//
#if defined(SWITCH) 
    padUpdate(&pad);

    kDown = padGetButtons(&pad);

    //u32 kToggle = hidKeysDown(CONTROLLER_P1_AUTO);

    if((kDown & HidNpadButton_A) || (kDown & HidNpadButton_ZR))
        buttonstate[bt_attack] = true;

    if((kDown & HidNpadButton_B))
        buttonstate[bt_use] = true;

    if((kDown & HidNpadButton_X))
        buttonstate[bt_strafe] = true;

    if((kDown & HidNpadButton_Y) || (kDown & HidNpadButton_ZL))
        buttonstate[bt_run] = true;

    if((kDown & HidNpadButton_R))
        buttonstate[bt_nextweapon] = true;

    if((kDown & HidNpadButton_L))
        buttonstate[bt_prevweapon] = true;

    if((kDown & HidNpadButton_Minus))
        buttonstate[bt_esc] = true;

    if((kDown & HidNpadButton_Plus))
        buttonstate[bt_pause] = true;
#elif defined(N3DS)
    hidScanInput();

    kDown = hidKeysHeld();

    //u32 kToggle = hidKeysDown(CONTROLLER_P1_AUTO);

    if((kDown & KEY_A) || (kDown & KEY_ZR))
        buttonstate[bt_attack] = true;

    if((kDown & KEY_B))
        buttonstate[bt_use] = true;

    if((kDown & KEY_X))
        buttonstate[bt_strafe] = true;

    if((kDown & KEY_Y) || (kDown & KEY_ZL))
        buttonstate[bt_run] = true;

    if((kDown & KEY_R))
        buttonstate[bt_nextweapon] = true;

    if((kDown & KEY_L))
        buttonstate[bt_prevweapon] = true;

    if((kDown & KEY_START))
        buttonstate[bt_esc] = true;

    if((kDown & KEY_SELECT))
        buttonstate[bt_pause] = true;
#else
    PollKeyboardButtons();
#if SDL_MAJOR_VERSION == 2
    PollGameControllerButtons();
#endif
    if (mouseenabled && IN_IsInputGrabbed())
        PollMouseButtons();
#ifndef EXTRACONTROLS
    if (joystickenabled)
        PollJoystickButtons();
#endif
#endif
//
// get movements
//
#if defined(SWITCH)
    // keyboard movement code
    delta = buttonstate[bt_run] ? RUNMOVE * tics : BASEMOVE * tics;
	
    if((kDown & HidNpadButton_Up))
        controly -= delta;
    if((kDown & HidNpadButton_Right))
        controly += delta;
    if((kDown & HidNpadButton_Left))
        controlx -= delta;
    if((kDown & HidNpadButton_Right))
        controlx += delta;

    //Read the joysticks' position
    pos_left = padGetStickPos(&pos_left, 0);
    pos_right = padGetStickPos(&pos_right, 1);

    if( pos_left.x < 0)
    {
        buttonstate[bt_strafeleft] = true;
    }
    else if( pos_left.x > 0)
    {
        buttonstate[bt_straferight] = true;
    }
    if( pos_left.y < -JOYSTICK_DEAD_ZONE)
    {
        movespeed = floor((float)((float)pos_left.y/(float)32767)*(float)35);
        if( pos_left.y < -JOYSTICK_MAX_ZONE )
        {
            movespeed = -35;
        }
        delta = buttonstate[bt_run] ? (movespeed*2) * tics : movespeed * tics;
        controly -= delta;
    }
    if( pos_left.y > JOYSTICK_DEAD_ZONE)
    {
        movespeed = floor((float)((float)-pos_left.y/(float)32767)*(float)35);
        if( pos_left.y > JOYSTICK_MAX_ZONE)
        {
            movespeed = -35;
        }
        delta = buttonstate[bt_run] ? (movespeed*2) * tics : movespeed * tics;
        controly += delta;
    }
    if( pos_right.x < -JOYSTICK_DEAD_ZONE)
    {
        turnspeed = floor((float)((float)-pos_right.x/(float)32767)*(float)35);
        if( pos_right.x < -JOYSTICK_MAX_ZONE)
        {
            turnspeed = 35;
        }
        delta = buttonstate[bt_run] ? (turnspeed*2) * tics : turnspeed * tics;
        controlx -= delta;
    }
    else if( pos_right.x > JOYSTICK_DEAD_ZONE)
    {
        turnspeed = floor((float)((float)pos_right.x/(float)32767)*(float)35);
        if( pos_right.x > JOYSTICK_MAX_ZONE)
        {
            turnspeed = 35;
        }
        delta = buttonstate[bt_run] ? (turnspeed*2) * tics : turnspeed * tics;
        controlx += delta;
    }
#elif defined(N3DS)
    // keyboard movement code
    delta = buttonstate[bt_run] ? RUNMOVE * tics : BASEMOVE * tics;

    if((kDown & KEY_DUP))
        controly -= delta;
    if((kDown & KEY_DDOWN))
        controly += delta;
    if((kDown & KEY_DLEFT))
        controlx -= delta;
    if((kDown & KEY_DRIGHT))
        controlx += delta;

	hidCircleRead(&cpos);
	if (abs(cpos.dx) > 32) 
        cx = (cpos.dx) >> 1;
	else
        cx = 0;
	
    if (abs(cpos.dy) > 32) 
        cy = (0-(cpos.dy)) >> 2;
	else 
        cy = 0;

    controlx += cx * 10/(13-mouseadjustment);
	controly += cy * 20/(13-mouseadjustment);	
#else
    PollKeyboardMove();
#if SDL_MAJOR_VERSION == 2
    PollGameControllerMove();
#endif

    if (mouseenabled && IN_IsInputGrabbed())
        PollMouseMove ();

    if (joystickenabled)
        PollJoystickMove ();
#endif
//
// bound movement to a maximum
//
    max = 100 * (int)tics;
    min = -max;
    if (controlx > max)
        controlx = max;
    else if (controlx < min)
        controlx = min;

    if (controly > max)
        controly = max;
    else if (controly < min)
        controly = min;

#if SDL_MAJOR_VERSION == 2
    if (gamecontrolstrafe > max)
        gamecontrolstrafe = max;

    else if (gamecontrolstrafe < min)
        gamecontrolstrafe = min;
#endif

    if (demorecord)
    {
        //
        // save info out to demo buffer
        //
        controlx /= (int) tics;
        controly /= (int) tics;

        buttonbits = 0;

        // TODO: Support 32-bit buttonbits
        for (i = NUMBUTTONS - 1; i >= 0; i--)
        {
            buttonbits <<= 1;
            if (buttonstate[i])
                buttonbits |= 1;
        }

        *demoptr++ = buttonbits;
        *demoptr++ = controlx;
        *demoptr++ = controly;

        if (demoptr >= lastdemoptr - 8)
            playstate = ex_completed;
        else
        {
            controlx *= (int) tics;
            controly *= (int) tics;
        }
    }
}



//==========================================================================



///////////////////////////////////////////////////////////////////////////
//
//      CenterWindow() - Generates a window of a given width & height in the
//              middle of the screen
//
///////////////////////////////////////////////////////////////////////////
#ifndef SEGA_SATURN
#define MAXX    320
#define MAXY    160

void CenterWindow (unsigned short w, unsigned short h)
{
    US_DrawWindow (((MAXX / 8) - w) / 2, ((MAXY / 8) - h) / 2, w, h);
}
#endif
//===========================================================================


/*
=====================
=
= CheckKeys
=
=====================
*/

void CheckKeys (void)
{
    ScanCode scan;
	static int wasPressed;
    if (screenfaded || demoplayback)    // don't do anything with a faded screen
        return;

    scan = LastScan;

#ifdef DEBUGKEYS
    if (Keyboard(sc_Tab) &&
        Keyboard(sc_N))			// N = no clip
    {
        noclip ^= 1;
        WindowH = 160;

        SD_StopDigitized();
        CA_CacheGrChunks();
        ClearSplitVWB();
        //VW_ScreenToScreen(screen, screenBuffer, 80, 160);

        if (noclip)
            Message("No clipping ON");
        else
            Message("No clipping OFF");
        /*
            UNCACHEGRCHUNK(STARTFONT + 1);
            PM_CheckMainMem();
        */
        if (noclip)
        {
            SD_PlaySound(DEATHSCREAM6SND);
        }
        else
        {
            SD_PlaySound(ENDBONUS2SND);
        }
        IN_Ack();
        noclip ^= 1;
        DrawPlayBorderSides();
        IN_ClearKeysDown();
        return;	// return 1;
    }
#endif

    wasPressed = 0;
    if (Keyboard(sc_Tab) && Keyboard(sc_P)) //Fabien Sangalard + Ripper Picture Grabber.
    {
        if (!wasPressed)
        {
            wasPressed = 1;
#ifdef CRT
            CRT_Screenshot();
#elif defined(DEBUGKEYS)
            PictureGrabber();
#endif
        }
        else
        {
            wasPressed = 0;
        }
    }
//#endif
#ifndef SEGA_SATURN
#if defined(GOD_MODE) || defined(SPEAR)
    //
    // SECRET CHEAT CODE: TAB-G-F10
    //
    if (Keyboard(sc_Tab) && Keyboard(sc_G) && Keyboard(sc_F10)) 
    {

        SD_StopDigitized();
        // CA_CacheGrChunk(STARTFONT + 1);
        CA_CacheGrChunks();
        ClearSplitVWB();

        if (godmode)
        {
            Message ("God mode OFF");
            //Better use sound.
            SD_PlaySound (DEATHSCREAM1SND);
        }
        else
        {
            Message ("God mode ON");
            SD_PlaySound (ENDBONUS2SND);
        }

        IN_Ack ();
        godmode ^= 1;
        DrawPlayBorderSides ();
        IN_ClearKeysDown ();
        return;
    }
#endif

    //
    // SECRET CHEAT CODE: 'MLI'
    //
    if (Keyboard(sc_M) && Keyboard(sc_L) && Keyboard(sc_I))
    {
        gamestate.health = 100;
        gamestate.ammo = 99;
        gamestate.keys = 3;
        gamestate.score = 0;
        gamestate.TimeCount += 42000L;
        GiveWeapon (wp_chaingun);
        DrawWeapon ();
        DrawHealth ();
        DrawKeys ();
        DrawAmmo ();
        DrawScore ();

        SD_StopDigitized();
        ClearSplitVWB ();

        Message (STR_CHEATER1 "\n"
                 STR_CHEATER2 "\n\n" STR_CHEATER3 "\n" STR_CHEATER4 "\n" STR_CHEATER5);

        IN_ClearKeysDown ();
        IN_Ack ();

        if (viewsize < 17)
            DrawPlayBorder ();
    }
#endif

    //
    // OPEN UP DEBUG KEYS
    //
#ifdef DEBUGKEYS
    if (Keyboard(sc_BackSpace) && Keyboard(sc_LShift) && Keyboard(sc_Alt) && param_debugmode)
    {
        SD_StopDigitized ();
        ClearSplitVWB ();

        Message ("Debugging keys are\nnow available!");
        IN_ClearKeysDown ();
        IN_Ack ();

        DrawPlayBorderSides ();
        DebugOk = true;
    }
#endif

#ifndef SEGA_SATURN
    //
    // TRYING THE KEEN CHEAT CODE!
    //
    if (Keyboard(sc_B) && Keyboard(sc_A) && Keyboard(sc_T))
    {
        SD_StopDigitized();
        ClearSplitVWB();

        Message("Commander Keen is also\n"
            "available from Apogee, but\n"
            "then, you already know\n" "that - right, Cheatmeister?!");

        IN_ClearKeysDown();
        IN_Ack();

        if (viewsize < 18)
            DrawPlayBorder();
    }
//
// pause key weirdness can't be checked as a scan code
//
    if(buttonstate[bt_pause]) Paused = true;
    if(Paused)
    {
        int lastoffs = StopMusic();
        VWB_DrawPic (16 * 8, 80 - 2 * 8, PAUSEDPIC);
        VW_UpdateScreen();
        IN_Ack ();
        Paused = false;
        ContinueMusic(lastoffs);
        if (MousePresent && IN_IsInputGrabbed())
            IN_CenterMouse();     // Clear accumulated mouse movement
        lasttimecount = GetTimeCount();
        return;
    }

//
// F1-F7/ESC to enter control panel
//
    if (
#ifndef DEBCHECK
           scan == sc_F10 ||
#endif
           scan == sc_F9 || scan == sc_F7 || scan == sc_F8)     // pop up quit dialog
    {
        ClearMemory ();
        ClearSplitVWB ();
        US_ControlPanel (scan);

        DrawPlayBorderSides ();

        SETFONTCOLOR (0, 15);
        IN_ClearKeysDown ();
        return;
    }
#endif
#ifdef SEGA_SATURN
    if (scan == sc_Escape)
#else
    if ((scan >= sc_F1 && scan <= sc_F9) || scan == sc_Escape || buttonstate[bt_esc])
#endif
    {
        int lastoffs = StopMusic ();
#ifdef SAVE_GAME_SCREENSHOT
        VL_SetSaveGameSlot();
#endif
        ClearMemory ();
        VW_FadeOut ();

        US_ControlPanel (buttonstate[bt_esc] ? sc_Escape : scan);

        SETFONTCOLOR (0, 15);
        IN_ClearKeysDown ();
        VW_FadeOut();
        if(viewsize != 21)
            DrawPlayScreen ();
        if (!startgame && !loadedgame)
            ContinueMusic (lastoffs);
#ifndef SEGA_SATURN
        if (loadedgame)
            playstate = ex_abort;
#endif
        lasttimecount = GetTimeCount();
        if (MousePresent && IN_IsInputGrabbed())
            IN_CenterMouse();     // Clear accumulated mouse movement
        return;
    }

//
// TAB-? debug keys
//
#ifdef DEBUGKEYS
#ifndef SEGA_SATURN
    if (Keyboard(sc_Tab) && DebugOk)
    {
        fontnumber = 0;
        SETFONTCOLOR (0, 15);
        if (DebugKeys () && viewsize < 20)
        {
            DrawPlayBorder ();       // dont let the blue borders flash

            if (MousePresent && IN_IsInputGrabbed())
                IN_CenterMouse();     // Clear accumulated mouse movement

            lasttimecount = GetTimeCount();
        }
        return;
    }
#endif
#endif

#ifdef VIEWMAP
    if (Keyboard(sc_O))
    {
        ViewMap ();

        if (MousePresent && IN_IsInputGrabbed())
            IN_CenterMouse();     // Clear accumulated mouse movement

        lasttimecount = GetTimeCount();
    }
#endif
#ifdef AUTOMAP
//
// Display full automap
//
    if (Keyboard(sc_M))
    {
        ScanCode key;
        DrawFullmap();
        VH_UpdateScreen(screen);

        IN_ClearKeysDown();
        key = IN_WaitForKey();
        while (key != sc_Tab && key != sc_Escape)
            key = IN_WaitForKey();
        IN_ClearKeysDown();

        if (MousePresent && IN_IsInputGrabbed())
            IN_CenterMouse();     // Clear accumulated mouse movement
        lasttimecount = GetTimeCount();
        return;
    }
#endif
#ifdef COMPASS
    //------------------
    // Compass
    if (Keyboard(sc_C))
    {
        compass ^= true;
        Keyboard(sc_C) | false;
        return;
    }
#endif
}


//===========================================================================

/*
#############################################################################

                                  The objlist data structure

#############################################################################

objlist contains structures for every actor currently playing.  The structure
is accessed as a linked list starting at *player, ending when ob->next ==
NULL.  GetNewObj inserts a new object at the end of the list, meaning that
if an actor spawn another actor, the new one WILL get to think and react the
same frame.  RemoveObj unlinks the given object and returns it to the free
list, but does not damage the objects ->next pointer, so if the current object
removes itself, a linked list following loop can still safely get to the
next element.

<backwardly linked free list>

#############################################################################
*/


/*
=========================
=
= InitActorList
=
= Call to clear out the actor object lists returning them all to the free
= list.  Allocates a special spot for the player.
=
=========================
*/

#ifndef SEGA_SATURN
int objcount;
#endif

void InitActorList (void)
{
    int i;

//
// init the actor lists
//
    for (i = 0; i < MAXACTORS; i++)
    {
        objlist[i].prev = &objlist[i + 1];
        objlist[i].next = NULL;
    }

    objlist[MAXACTORS - 1].prev = NULL;

    objfreelist = &objlist[0];
    lastobj = NULL;

#ifndef SEGA_SATURN
    objcount = 0;
#endif
//
// give the player the first free spots
//
    GetNewActor ();
    player = newobj;

}

//===========================================================================

/*
=========================
=
= GetNewActor
=
= Sets the global variable new to point to a free spot in objlist.
= The free spot is inserted at the end of the liked list
=
= When the object list is full, the caller can either have it bomb out ot
= return a dummy object pointer that will never get used
=
=========================
*/

void GetNewActor (void)
{
    if (!objfreelist)
        Quit ("GetNewActor: No free spots in objlist!");

    newobj = objfreelist;
    objfreelist = newobj->prev;
    memset (newobj, 0, sizeof (*newobj));

    if (lastobj)
        lastobj->next = newobj;
    newobj->prev = lastobj;     // new->next is already NULL from memset

    newobj->active = ac_no;
    lastobj = newobj;
#ifndef SEGA_SATURN
    objcount++;
#endif
}

//===========================================================================

/*
=========================
=
= RemoveObj
=
= Add the given object back into the free list, and unlink it from it's
= neighbors
=
=========================
*/

void RemoveObj (objtype * gone)
{
    if (gone == player)
        Quit ("RemoveObj: Tried to remove the player!");

    gone->state = NULL;

//
// fix the next object's back link
//
    if (gone == lastobj)
        lastobj = (objtype *) gone->prev;
    else
        gone->next->prev = gone->prev;

//
// fix the previous object's forward link
//
    gone->prev->next = gone->next;

//
// add it back in to the free list
//
    gone->prev = objfreelist;
    objfreelist = gone;

#ifndef SEGA_SATURN
    objcount--;
#endif
}

/*
=============================================================================

                                                MUSIC STUFF

=============================================================================
*/


/*
=================
=
= StopMusic
=
=================
*/
int StopMusic(void)
{
#ifdef MAPCONTROLLEDMUSIC
    int lastoffs = SD_MusicOff();
    int holder;

    holder = tilemap[1][0];
    if (holder < 0 || holder >LASTMUSIC)
        holder = 0;
#ifndef SEGA_SATURN
    UNCACHEAUDIOCHUNK(STARTMUSIC + holder);
#endif
    return lastoffs;
#else
    int lastoffs = SD_MusicOff();
#ifndef SEGA_SATURN
    UNCACHEAUDIOCHUNK(STARTMUSIC + lastmusicchunk);
#endif
    return lastoffs;
#endif
}

//==========================================================================


/*
=================
=
= StartMusic
=
=================
*/
#if defined(BOSS_MUSIC) && defined(VIEASM)
void StartMusic()
{
#ifdef MAPPEDCONTROLLEDMUSIC
    int holder;
    //static musicnames lastmusicchunk;
#endif
    SD_MusicOff();

#ifdef MAPPEDCONTROLLEDMUSIC
    holder = tilemap[1][0];
    if (holder < 0 || holder >LASTMUSIC)
        holder = 0;
    lastmusicchunk = (musicnames)(gamestate.music[songs[holder]]);
#else
    lastmusicchunk = (musicnames)(gamestate.music);
#endif
    SD_StartMusic(STARTMUSIC + lastmusicchunk);
}

void ContinueMusic(int offs)
{
#ifdef MAPCONTROLLEDMUSIC
    int holder;
    //static musicnames lastmusicchunk;
#endif
    SD_MusicOff();
#ifdef MAPCONTROLLEDMUSIC
    holder = tilemap[1][0];
    if (holder < 0 || holder >LASTMUSIC)
        holder = 0;
    lastmusicchunk = (musicnames)(gamestate.music[songs[holder]]);
#else
    lastmusicchunk = (musicnames)(gamestate.music);
#endif
    SD_ContinueMusic(STARTMUSIC + lastmusicchunk, offs);
}

void ChangeGameMusic(int song)
{
    if (gamestate.music == song || song >= LASTMUSIC)
        return;

    StopMusic();
    gamestate.music = song;
    StartMusic();
}

void SetLevelMusic(void)
{
    gamestate.music = songs[gamestate.mapon + gamestate.episode * 10];
}
#else
void StartMusic ()
{
#ifdef MAPCONTROLLEDMUSIC
    int holder;
    //static musicnames lastmusicchunk;
#endif
    SD_MusicOff();
#ifdef MAPCONTROLLEDMUSIC
    holder = tilemap[1][0];
    if (holder < 0 || holder >LASTMUSIC)
        holder = 0;
    lastmusicchunk = (musicnames)songs[holder];
#else
    lastmusicchunk = (musicnames)songs[gamestate.mapon + gamestate.episode * 10];
#endif
    SD_StartMusic(STARTMUSIC + lastmusicchunk);
}

void ContinueMusic (int offs)
{
#ifdef MAPCONTROLLEDMUSIC
    int holder;
    //static musicnames lastmusicchunk;
#endif
    SD_MusicOff();
#ifdef MAPCONTROLLEDMUSIC
    holder = tilemap[1][0];
    if (holder < 0 || holder >LASTMUSIC)
        holder = 0;
    lastmusicchunk = (musicnames)songs[holder];
#else
    lastmusicchunk = (musicnames)songs[gamestate.mapon + gamestate.episode * 10];
#endif
    SD_ContinueMusic(STARTMUSIC + lastmusicchunk, offs);
}
#endif
/*
=============================================================================

                                        PALETTE SHIFTING STUFF

=============================================================================
*/

#define NUMREDSHIFTS    6
#define REDSTEPS        8

#define NUMWHITESHIFTS  3
#define WHITESTEPS      20
#define WHITETICS       6


SDL_Color redshifts[NUMREDSHIFTS][256];
SDL_Color whiteshifts[NUMWHITESHIFTS][256];

int damagecount, bonuscount;
boolean palshifted;

/*
=====================
=
= InitRedShifts
=
=====================
*/

void InitRedShifts (void)
{
    SDL_Color *workptr, *baseptr;
    int i, j, delta;


//
// fade through intermediate frames
//
    for (i = 1; i <= NUMREDSHIFTS; i++)
    {
        workptr = redshifts[i - 1];
        baseptr = gamepal;

        for (j = 0; j <= 255; j++)
        {
            delta = 256 - baseptr->r;
            workptr->r = baseptr->r + delta * i / REDSTEPS;
            delta = -baseptr->g;
            workptr->g = baseptr->g + delta * i / REDSTEPS;
            delta = -baseptr->b;
            workptr->b = baseptr->b + delta * i / REDSTEPS;
            baseptr++;
            workptr++;
        }
    }

    for (i = 1; i <= NUMWHITESHIFTS; i++)
    {
        workptr = whiteshifts[i - 1];
        baseptr = gamepal;

        for (j = 0; j <= 255; j++)
        {
            delta = 256 - baseptr->r;
            workptr->r = baseptr->r + delta * i / WHITESTEPS;
            delta = 248 - baseptr->g;
            workptr->g = baseptr->g + delta * i / WHITESTEPS;
            delta = 0-baseptr->b;
            workptr->b = baseptr->b + delta * i / WHITESTEPS;
            baseptr++;
            workptr++;
        }
    }
}


/*
=====================
=
= ClearPaletteShifts
=
=====================
*/

void ClearPaletteShifts (void)
{
    bonuscount = damagecount = 0;
    palshifted = false;
}


/*
=====================
=
= StartBonusFlash
=
=====================
*/

void StartBonusFlash (void)
{
    bonuscount = NUMWHITESHIFTS * WHITETICS;    // white shift palette
}


/*
=====================
=
= StartDamageFlash
=
=====================
*/

void StartDamageFlash (int damage)
{
    damagecount += damage;
}


/*
=====================
=
= UpdatePaletteShifts
=
=====================
*/

void UpdatePaletteShifts (void)
{
    int red, white;

    if (bonuscount)
    {
        white = bonuscount / WHITETICS + 1;
        if (white > NUMWHITESHIFTS)
            white = NUMWHITESHIFTS;
        bonuscount -= (int)tics;
        if (bonuscount < 0)
            bonuscount = 0;
    }
    else
        white = 0;


    if (damagecount)
    {
        red = damagecount / 10 + 1;
        if (red > NUMREDSHIFTS)
            red = NUMREDSHIFTS;

        damagecount -= (int)tics;
        if (damagecount < 0)
            damagecount = 0;
    }
    else
        red = 0;

    if (red)
    {
        VL_SetPalette (redshifts[red - 1], false);
        palshifted = true;
    }
    else if (white)
    {
        VL_SetPalette (whiteshifts[white - 1], false);
        palshifted = true;
    }
    else if (palshifted)
    {
        VL_SetPalette (gamepal, false);        // back to normal
        palshifted = false;
    }
}


/*
=====================
=
= FinishPaletteShifts
=
= Resets palette to normal if needed
=
=====================
*/

void FinishPaletteShifts (void)
{
    if (palshifted)
    {
        palshifted = 0;
        VL_SetPalette (gamepal, true);
    }
}


/*
=============================================================================

                                                CORE PLAYLOOP

=============================================================================
*/
/*
=====================
=
= DoActor
=
=====================
*/

#if defined(EMBEDDED) && defined(SEGA_SATURN)
void DoActor(objtype* ob)
{
    void (*think) (objtype*);
/*
    if (!ob->active && ob->areanumber < NUMAREAS && !areabyplayer[ob->areanumber])
        return;

    if (!(ob->flags & (FL_NONMARK | FL_NEVERMARK)))
        actorat[ob->tilex][ob->tiley] = NULL;
*/
    if (!(ob->flags & (FL_NONMARK | FL_NEVERMARK)))
        clear_actor(ob->tilex, ob->tiley);

    //
    // non transitional object
    //

    if (!ob->ticcount)
    {
        think = (void (*)(objtype*)) ob->state->think;
        if (think)
        {
            think(ob);
            if (!ob->state)
            {
                RemoveObj(ob);
                return;
            }
        }

        if (ob->flags & FL_NEVERMARK)
            return;

        if ((ob->flags & FL_NONMARK) && actorat[ob->tilex][ob->tiley])
            return;


        move_actor(ob);
        return;
    }

    //
    // transitional object
    //
    ob->ticcount -= (short)tics;
    while (ob->ticcount <= 0)
    {
        think = (void (*)(objtype*)) ob->state->action;        // end of state action
        if (think)
        {
            think(ob);
            if (!ob->state)
            {
                RemoveObj(ob);
                return;
            }
        }

        ob->state = ob->state->next;

        if (!ob->state)
        {
            RemoveObj(ob);
            return;
        }

        if (!ob->state->tictime)
        {
            ob->ticcount = 0;
            goto think;
        }

        ob->ticcount += ob->state->tictime;
    }

think:
    //
    // think
    //
    think = (void (*)(objtype*)) ob->state->think;
    if (think)
    {
        think(ob);
        if (!ob->state)
        {
            RemoveObj(ob);
            return;
        }
    }

    if (ob->flags & FL_NEVERMARK)
        return;

    if ((ob->flags & FL_NONMARK) && actorat[ob->tilex][ob->tiley])
        return;


    move_actor(ob);
}
#else
void DoActor (objtype * ob)
{
    void (*think) (objtype *);

    if (!ob->active && ob->areanumber < NUMAREAS && !areabyplayer[ob->areanumber])
        return;

    if (!(ob->flags & (FL_NONMARK | FL_NEVERMARK)))
        actorat[ob->tilex][ob->tiley] = NULL;

//
// non transitional object
//

    if (!ob->ticcount)
    {
        think = (void (*)(objtype *)) ob->state->think;
        if (think)
        {
            think (ob);
            if (!ob->state)
            {
                RemoveObj (ob);
                return;
            }
        }

        if (ob->flags & FL_NEVERMARK)
            return;

        if ((ob->flags & FL_NONMARK) && actorat[ob->tilex][ob->tiley])
            return;

        actorat[ob->tilex][ob->tiley] = ob;
        return;
    }

//
// transitional object
//
    ob->ticcount -= (short) tics;
    while (ob->ticcount <= 0)
    {
        think = (void (*)(objtype *)) ob->state->action;        // end of state action
        if (think)
        {
            think (ob);
            if (!ob->state)
            {
                RemoveObj (ob);
                return;
            }
        }

        ob->state = ob->state->next;

        if (!ob->state)
        {
            RemoveObj (ob);
            return;
        }

        if (!ob->state->tictime)
        {
            ob->ticcount = 0;
            goto think;
        }

        ob->ticcount += ob->state->tictime;
    }

think:
    //
    // think
    //
    think = (void (*)(objtype *)) ob->state->think;
    if (think)
    {
        think (ob);
        if (!ob->state)
        {
            RemoveObj (ob);
            return;
        }
    }

    if (ob->flags & FL_NEVERMARK)
        return;

    if ((ob->flags & FL_NONMARK) && actorat[ob->tilex][ob->tiley])
        return;

    actorat[ob->tilex][ob->tiley] = ob;
}
#endif
//==========================================================================


/*
===================
=
= PlayLoop
=
===================
*/
int funnyticount;


#ifdef FIXEDLOGICRATE
double accumulator, frametime_spent = 0;
double dt = 1.0f / 70.0f; // 70 FPS
unsigned int oldtime = 0;


void ClockGameLogic(void)
{
    size_t curtime;
    unsigned int deltatime;
    double time_to_pass;
    if (demorecord || demoplayback)
    {
        accumulator = dt;
        return;
    }

    curtime = WL_GetTicks();
    deltatime = curtime - oldtime;
    if (oldtime == 0)
        deltatime = 0;
    oldtime = curtime;
    time_to_pass = (double)(deltatime / 1000.0f);

    // Choking, do not overload the timestep
    // to avoid grinding to a halt
    if (time_to_pass > dt * 10)
    {
        time_to_pass = dt * 10;
    }

    // Add some frametime to spend
    accumulator += time_to_pass;
}

#ifdef LAGSIMULATOR
unsigned int next_lag_spike = -1;


void LagSimulator(void)
{
    if (demorecord || demoplayback)
        return;

    if (lagging)
    {
        if (next_lag_spike == -1 || WL_GetTicks() >= next_lag_spike)
        {
            SDL_Delay(80 + (rand() % 200));
            next_lag_spike = WL_GetTicks() + 200 + rand() % 350;
        }
    }
}
#endif
#endif

#ifdef MAPCONTROLLEDLTIME
float leveltime;
int scoreticcount;
int scorebonusAmount;
#endif

void PlayLoop (void)
{
#if defined (SWITCH) || defined (N3DS)
    printf("PLAY LOOP START\n");
#endif
#if defined(USE_FEATUREFLAGS) && defined(USE_CLOUDSKY)
    if(GetFeatureFlags() & FF_CLOUDSKY)
        InitSky();
#endif

#ifdef USE_SHADING
    InitLevelShadeTable();
#endif

    playstate = ex_stillplaying;
    lasttimecount = GetTimeCount();
#ifndef SEGA_SATURN
    frameon = 0;
#endif
    anglefrac = 0;
    facecount = 0;
    funnyticount = 0;
    memset (buttonstate, 0, sizeof (buttonstate));
    ClearPaletteShifts ();

    if (MousePresent && IN_IsInputGrabbed())
        IN_CenterMouse();         // Clear accumulated mouse movement

    if (demoplayback)
        IN_StartAck();
#if defined (SWITCH) || defined (N3DS)    
    printf("LOOP HERE\n");
#endif    
#ifdef SEGA_SATURN
    DrawStatusBar(); // vbt : ajout
#endif
#ifdef FIXEDLOGICRATE
    do
    {
        // Break from the playloop if starting game
        if (startgame || loadedgame)
            break;

#ifdef LAGSIMULATOR
        // Do some lagging
        LagSimulator();
#endif
        // Clock the logictime that has stacked up
        ClockGameLogic();

        // Loop while there is time left to simulate
        while (accumulator >= dt)
        {
            //
            // start a logic frame
            //
            PollControls();

            //
            // actor thinking
            //
            madenoise = false;

            MoveDoors();
            MovePWalls();

            for (obj = player; obj; obj = obj->next)
                DoActor(obj);

            UpdatePaletteShifts();

            //
            // MAKE FUNNY FACE IF BJ DOESN'T MOVE FOR AWHILE
            //
#ifdef SPEAR
            funnyticount += tics;
            if (funnyticount > 30l * 70)
            {
                funnyticount = 0;
                if (viewsize != 21)
                    StatusDrawFace(BJWAITING1PIC + (US_RndT() & 1));
                facecount = 0;
            }
#endif
#ifdef MAPCONTROLLEDLTIME
            if (tilemap[63][4]) {
                leveltime = 2.0;
                if (tilemap[63][5] >= 1 && tilemap[63][5] <= 5) {
                    leveltime = tilemap[63][5];
                    if (tilemap[63][6] >= 1 && tilemap[63][6] <= 3) {
                        switch (tilemap[63][6]) {
                        case 1:
                            leveltime += .25;
                            break;
                        case 2:
                            leveltime += .50;
                            break;
                        case 3:
                            leveltime += .75;
                            break;
                        }
                    }
                }
                leveltime = (leveltime * 4200) / 70;
                if (tilemap[63][7] <= 1) {
                    scorebonusAmount = (tilemap[63][7]) * 1000;
                }
                else if (tilemap[63][7] == 0) {
                    scorebonusAmount = 500;
                }
                scoreticcount += tics;
                if (scoreticcount > 1l * 70) {
                    GivePoints(scorebonusAmount);
                    scoreticcount = 0;
                }
                if (gamestate.TimeCount == leveltime * 70)
                    playstate = ex_completed;
            }
#endif
            // Abort demo?
            if (demoplayback)
            {
                if (IN_CheckAck())
                {
                    IN_ClearKeysDown();
                    playstate = ex_abort;
                }
            }

            // Advance timer
            gamestate.TimeCount += tics;

            // Debug keys
            CheckKeys();

            // End of one frame
            frametime_spent += dt;
            accumulator -= dt;
            frameon += tics;
        }

        // Only refresh screen once per frame, instead of once per logic frame
        ThreeDRefresh();

        UpdateSoundLoc();      // JAB
        if (screenfaded)
            VW_FadeIn();

        // Do single stepping outside of the game logic loop
        if (singlestep)
        {
            VW_WaitVBL(singlestep);
            lasttimecount = GetTimeCount();
        }

        // Extra vbls left outside of game logic
        if (extravbls)
            VW_WaitVBL(extravbls);
    }
#else
    do
    {
        PollControls ();

//
// actor thinking
//
        madenoise = false;

        MoveDoors ();
        MovePWalls ();

        for (obj = player; obj; obj = obj->next)
            DoActor (obj);

        UpdatePaletteShifts ();

        ThreeDRefresh ();

        //
        // MAKE FUNNY FACE IF BJ DOESN'T MOVE FOR AWHILE
        //
#ifdef SPEAR
        funnyticount += tics;
        if (funnyticount > 30l * 70)
        {
            funnyticount = 0;
            if(viewsize != 21)
                StatusDrawFace(BJWAITING1PIC + (US_RndT () & 1));
            facecount = 0;
        }
#endif

        gamestate.TimeCount += tics;
#ifndef SEGA_SATURN
        UpdateSoundLoc ();      // JAB
#endif
        if (screenfaded)
            VW_FadeIn ();

        CheckKeys ();

//
// debug aids
//
#ifndef SEGA_SATURN
        if (singlestep)
        {
            VW_WaitVBL (singlestep);
            lasttimecount = GetTimeCount();
        }
        if (extravbls)
            VW_WaitVBL (extravbls);
#endif
        if (demoplayback)
        {
            if (IN_CheckAck ())
            {
                IN_ClearKeysDown ();
                playstate = ex_abort;
            }
        }
    }
#endif
    while (!playstate && !startgame);

    if (playstate != ex_died)
        FinishPaletteShifts ();
}
