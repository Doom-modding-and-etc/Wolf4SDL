/* WL_INTER.C */

#include "wl_def.h"

LRstruct LevelRatios[LRpack];
size_t lastBreathTime = 0;

#ifdef SEGA_SATURN
extern unsigned char* wallData;

unsigned char* PM_DecodeSprites2(unsigned int start, unsigned int endi, unsigned int* pageOffsets, unsigned short* pageLengths, unsigned char* ptr, int fileId);
void readChunks(int fileId, unsigned int size, unsigned int* pageOffsets, unsigned char* Chunks, unsigned char* ptr);
#endif

void Write (int x, int y, const char *string);

/* ========================================================================== */

/*
==================
=
= CLearSplitVWB
=
==================
*/

void
ClearSplitVWB (void)
{
    WindowX = 0;
    WindowY = 0;
#ifdef SEGA_SATURN
    WindowW = SATURN_WIDTH;
#else
    WindowW = 320;
#endif
    WindowH = 160;
}


/* ========================================================================== */

#ifdef SPEAR
#ifndef SPEARDEMO
/*
////////////////////////////////////////////////////////
//
// End of Spear of Destiny
//
////////////////////////////////////////////////////////
*/

void
EndScreen (int palette, int screen)
{
    SDL_Color pal[256];
    VWB_DrawPic (0,0,screen);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen (screenBuffer);
#endif
    VL_ConvertPalette(grsegs[palette], pal, 256);
    VL_FadeIn (0, 255, pal, 30);
    IN_ClearKeysDown ();
    IN_Ack ();
    VL_FadeOut (0, 255, 0, 0, 0, 30);
}


void
EndSpear (void)
{
    SDL_Color pal[256];

    EndScreen (END1PALETTE, ENDSCREEN11PIC);

    VWB_DrawPic (0,0,ENDSCREEN3PIC);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen (screenBuffer);
#endif
    VL_ConvertPalette(grsegs[END3PALETTE], pal, 256);
    VL_FadeIn (0, 255, pal, 30);
    fontnumber = 0;
    fontcolor = 0xd0;
    WindowX = 0;
    WindowW = 320;
    PrintX = 0;
    PrintY = 180;
    US_CPrint (STR_ENDGAME1 "\n");
    US_CPrint (STR_ENDGAME2);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    IN_UserInput(700);

    PrintX = 0;
    PrintY = 180;
    VWB_Bar (0, 180, 320, 20, 0);
    US_CPrint (STR_ENDGAME3 "\n");
    US_CPrint (STR_ENDGAME4);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    IN_UserInput(700);

    VL_FadeOut (0, 255, 0, 0, 0, 30);

    EndScreen (END4PALETTE, ENDSCREEN4PIC);
    EndScreen (END5PALETTE, ENDSCREEN5PIC);
    EndScreen (END6PALETTE, ENDSCREEN6PIC);
    EndScreen (END7PALETTE, ENDSCREEN7PIC);
    EndScreen (END8PALETTE, ENDSCREEN8PIC);
    EndScreen (END9PALETTE, ENDSCREEN9PIC);

    EndScreen (END2PALETTE, ENDSCREEN12PIC);

    MainMenu[savegame].active = 0;
}
#endif
#endif

/* ========================================================================== */

/*
==================
=
= Victory
=
==================
*/

void
Victory (void)
{
#ifndef SPEARDEMO
    int sec;
    int i, min, kr, sr, tr, x;
    char tempstr[8];

#define RATIOX  6
#define RATIOY  14
#define TIMEX   14
#define TIMEY   8


#ifdef SPEAR
    StartCPMusic (XTHEEND_MUS);
#ifdef SEGA_SATURN
    VWB_Bar(0, 0, SATURN_WIDTH, 200, VIEWCOLOR);
#else
    VWB_Bar (0, 0, 320, 200, VIEWCOLOR);
#endif
    VWB_DrawPic (124, 44, BJCOLLAPSE1PIC);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    VL_FadeIn (0, 255, gamepal, 30);
    VL_WaitVBL (2 * 70);
    VWB_DrawPic (124, 44, BJCOLLAPSE2PIC);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    VL_WaitVBL (105);
    VWB_DrawPic (124, 44, BJCOLLAPSE3PIC);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    VL_WaitVBL (105);
    VWB_DrawPic (124, 44, BJCOLLAPSE4PIC);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    VL_WaitVBL (3 * 70);

    VL_FadeOut (0, 255, 0, 17, 17, 5);
#endif

    StartCPMusic (URAHERO_MUS);
    ClearSplitVWB ();

    VWB_Bar (0, 0, 320, screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);
    if (bordercol != VIEWCOLOR)
        DrawStatusBorder (VIEWCOLOR);

#ifdef JAPAN
#ifndef JAPDEMO
    VWB_DrawPic (0, 0, C_ENDRATIOSPIC);
#endif
#else
    Write (18, 2, STR_YOUWIN);

    Write (TIMEX, TIMEY - 2, STR_TOTALTIME);

    Write (12, RATIOY - 2, "averages");

#ifdef SPANISH
    Write (RATIOX + 2, RATIOY, STR_RATKILL);
    Write (RATIOX + 2, RATIOY + 2, STR_RATSECRET);
    Write (RATIOX + 2, RATIOY + 4, STR_RATTREASURE);
#else
    Write (RATIOX + 8, RATIOY, STR_RATKILL);
    Write (RATIOX + 4, RATIOY + 2, STR_RATSECRET);
    Write (RATIOX, RATIOY + 4, STR_RATTREASURE);
#endif
#ifdef SEGA_SATURN
    DrawPlayScreen(); /* vbt ajout */
    VWB_Bar(0, 0, SATURN_WIDTH, screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);
    if (bordercol != VIEWCOLOR)
        DrawStatusBorder(VIEWCOLOR);
#endif

#endif

#ifndef JAPDEMO
    VWB_DrawPic (8, 4, L_BJWINSPIC);
#endif
#ifdef SEGA_SATURN
    slScrTransparent(2);
    slSynch();
#endif

    for (kr = sr = tr = sec = i = 0; i < LRpack; i++)
    {
        sec += LevelRatios[i].time;
        kr += LevelRatios[i].kill;
        sr += LevelRatios[i].secret;
        tr += LevelRatios[i].treasure;
    }

#ifndef SPEAR
    kr /= LRpack;
    sr /= LRpack;
    tr /= LRpack;
#else
    kr /= 14;
    sr /= 14;
    tr /= 14;
#endif

    min = sec / 60;
    sec %= 60;

    if (min > 99)
        min = sec = 99;

    i = TIMEX * 8 + 1;
    VWB_DrawPic (i, TIMEY * 8, L_NUM0PIC + (min / 10));
    i += 2 * 8;
    VWB_DrawPic (i, TIMEY * 8, L_NUM0PIC + (min % 10));
    i += 2 * 8;
    Write (i / 8, TIMEY, ":");
    i += 1 * 8;
    VWB_DrawPic (i, TIMEY * 8, L_NUM0PIC + (sec / 10));
    i += 2 * 8;
    VWB_DrawPic (i, TIMEY * 8, L_NUM0PIC + (sec % 10));
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
    wlitoa (kr, tempstr, 10);
    x = RATIOX + 24 - (int) strlen(tempstr) * 2;
    Write (x, RATIOY, tempstr);
    wlitoa(sr, tempstr, 10);
    x = RATIOX + 24 - (int) strlen(tempstr) * 2;
    Write(x, RATIOY + 2, tempstr);
    wlitoa(tr, tempstr, 10);
    x = RATIOX + 24 - (int) strlen(tempstr) * 2;
    Write (x, RATIOY + 4, tempstr);

#ifndef SPANISH
#ifndef UPLOAD
#ifndef SPEAR
    /*
    ** TOTAL TIME VERIFICATION CODE
    */
    if (gamestate.difficulty >= gd_medium)
    {
        VWB_DrawPic (30 * 8, TIMEY * 8, C_TIMECODEPIC);
        fontnumber = 0;
        fontcolor = READHCOLOR;
        PrintX = 30 * 8 - 3;
        PrintY = TIMEY * 8 + 8;
        PrintX += 4;
        tempstr[0] = (((min / 10) ^ (min % 10)) ^ 0xa) + 'A';
        tempstr[1] = (int) ((((sec / 10) ^ (sec % 10)) ^ 0xa) + 'A');
        tempstr[2] = (tempstr[0] ^ tempstr[1]) + 'A';
        tempstr[3] = 0;
        US_Print (tempstr);
    }
#endif
#endif
#endif

    fontnumber = 1;

#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif
#ifdef SEGA_SATURN
    DrawStatusBar(); /* vbt ajout */
#endif

    VL_FadeIn (0, 255, gamepal, 30);

    IN_Ack ();

    VL_FadeOut (0, 255, 0, 0, 0, 30);
    if(screenHeight % 200 != 0)
        VL_ClearScreen(0);

    MainMenu[savegame].active = 0;  /* ADDEDFIX 3 - Tricob */

#ifndef SPEAR
    EndText ();
#else
    EndSpear ();
#endif

#endif /* SPEARDEMO */
}


/* ========================================================================== */

#ifndef JAPAN
/*
==================
=
= PG13
=
==================
*/

void
PG13 (void)
{
    VL_FadeOut (0, 255, 0, 0, 0, 30);
    VWB_Bar (0, 0, 320, 200, 0x82);     /* background */

    VWB_DrawPic (216, 110, PG13PIC);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
    VL_UpdateScreen(screenBuffer);
#endif

    VL_FadeIn (0, 255, gamepal, 30);
    IN_UserInput (TickBase * 7);

    VL_FadeOut (0, 255, 0, 0, 0, 30);
}
#endif


/* ========================================================================== */

void
Write (int x, int y, const char *string)
{
    static const int alpha[] = { L_NUM0PIC, L_NUM1PIC, L_NUM2PIC, L_NUM3PIC, L_NUM4PIC, L_NUM5PIC,
        L_NUM6PIC, L_NUM7PIC, L_NUM8PIC, L_NUM9PIC, L_COLONPIC, 0, 0, 0, 0, 0, 0, L_APIC, L_BPIC,
        L_CPIC, L_DPIC, L_EPIC, L_FPIC, L_GPIC, L_HPIC, L_IPIC, L_JPIC, L_KPIC,
        L_LPIC, L_MPIC, L_NPIC, L_OPIC, L_PPIC, L_QPIC, L_RPIC, L_SPIC, L_TPIC,
        L_UPIC, L_VPIC, L_WPIC, L_XPIC, L_YPIC, L_ZPIC
    };

    int i, ox, nx, ny, len = (int) strlen(string);
    char ch;

    ox = nx = x * 8;
    ny = y * 8;
    for (i = 0; i < len; i++)
    {
        if (string[i] == '\n')
        {
            nx = ox;
            ny += 16;
        }
        else
        {
            ch = string[i];
            if (ch >= 'a')
                ch -= ('a' - 'A');
            ch -= '0';

            switch (string[i])
            {
                case '!':
                    VWB_DrawPic (nx, ny, L_EXPOINTPIC);
                    nx += 8;
                    continue;

#ifndef APOGEE_1_0
                case '\'':
                    VWB_DrawPic (nx, ny, L_APOSTROPHEPIC);
                    nx += 8;
                    continue;
#endif

                case ' ':
                    break;

                case 0x3a:     /* ':' */
                    VWB_DrawPic (nx, ny, L_COLONPIC);
                    nx += 8;
                    continue;

                case '%':
                    VWB_DrawPic (nx, ny, L_PERCENTPIC);
                    break;

                default:
                    VWB_DrawPic (nx, ny, alpha[ch]);
            }
            nx += 16;
        }
    }
}


/*
** Breathe Mr. BJ!!!
*/
void
BJ_Breathe (void)
{
    static int which = 0, max = 10;
    int pics[2] = { L_GUYPIC, L_GUY2PIC };

#ifdef SEGA_SATURN
    DrawStatusBar(); /* vbt : ajout */
#endif

    SDL_Delay(5);

    if (GetTimeCount () - lastBreathTime > (size_t)max)
    {
        which ^= 1;
        VWB_DrawPic (0, 16, pics[which]);
#if !defined(SEGA_SATURN) && !defined(USE_SPRITES)
        VL_UpdateScreen(screenBuffer);
#endif
        lastBreathTime = GetTimeCount();
        max = 35;
    }
#ifdef SEGA_SATURN
    slSynch();	/* vbt 19/08/20 ajout pour eviter plantage ecran fin niveau */
#endif
}



/*
==================
=
= LevelCompleted
=
= Entered with the screen faded out
= Still in split screen mode with the status bar
=
= Exit with the screen faded out
=
==================
*/

void
LevelCompleted (void)
{
#define VBLWAIT 30
#define PAR_AMOUNT      500
#define PERCENT100AMT   10000
#ifdef MAPCONTROLPARTIME
    float times_time;
    int t;
#else
    typedef struct
    {
        float time;
        char timestr[6];
    } times;
#endif
    int x, i, min, sec, ratio, kr, sr, tr;
    char tempstr[10];
    int bonus, timeleft = 0;
#ifdef MAPCONTROLPARTIME
    char   times_timestr[6] = "02:00 ";
    times_time = 2.0;
    t = 2;
    if (tilemap[61][0] >= 1 && tilemap[61][0] <= 20)
    {
        t = tilemap[61][0];
        times_time = t;
        times_timestr[0] = '0' + t / 10;
        times_timestr[1] = '0' + t % 10;
    }
    switch (tilemap[62][0] >= 1 && tilemap[62][0] <= 3)
    {
    case 1: /* 15 */
        times_time += .25;
        times_timestr[3] = '1';
        times_timestr[4] = '5';
        break;
    case 2: /* 30 */
        times_time += .5;
        times_timestr[3] = '3';
        break;
    case 3: /* 45 */
        times_time += .75;
        times_timestr[3] = '4';
        times_timestr[4] = '5';
        break;
    }
#else
    times parTimes[] = {
#ifndef SPEAR
        /*
        ** Episode One Par Times
        */
        {1.5, "01:30"},
        {2, "02:00"},
        {2, "02:00"},
        {3.5, "03:30"},
        {3, "03:00"},
        {3, "03:00"},
        {2.5, "02:30"},
        {2.5, "02:30"},
        {0, "??:??"},           /* Boss level */
        {0, "??:??"},           /* Secret level */

        /*
        ** Episode Two Par Times
        */
        {1.5, "01:30"},
        {3.5, "03:30"},
        {3, "03:00"},
        {2, "02:00"},
        {4, "04:00"},
        {6, "06:00"},
        {1, "01:00"},
        {3, "03:00"},
        {0, "??:??"},
        {0, "??:??"},

        /**
        ** Episode Three Par Times
        */
        {1.5, "01:30"},
        {1.5, "01:30"},
        {2.5, "02:30"},
        {2.5, "02:30"},
        {3.5, "03:30"},
        {2.5, "02:30"},
        {2, "02:00"},
        {6, "06:00"},
        {0, "??:??"},
        {0, "??:??"},

        /*
        ** Episode Four Par Times
        */
        {2, "02:00"},
        {2, "02:00"},
        {1.5, "01:30"},
        {1, "01:00"},
        {4.5, "04:30"},
        {3.5, "03:30"},
        {2, "02:00"},
        {4.5, "04:30"},
        {0, "??:??"},
        {0, "??:??"},

        /*
        ** Episode Five Par Times
        */
        {2.5, "02:30"},
        {1.5, "01:30"},
        {2.5, "02:30"},
        {2.5, "02:30"},
        {4, "04:00"},
        {3, "03:00"},
        {4.5, "04:30"},
        {3.5, "03:30"},
        {0, "??:??"},
        {0, "??:??"},

        /*
        ** Episode Six Par Times
        */
        {6.5, "06:30"},
        {4, "04:00"},
        {4.5, "04:30"},
        {6, "06:00"},
        {5, "05:00"},
        {5.5, "05:30"},
        {5.5, "05:30"},
        {8.5, "08:30"},
        {0, "??:??"},
        {0, "??:??"}
#else
        /*
        ** SPEAR OF DESTINY TIMES
        */
        {1.5, "01:30"},
        {3.5, "03:30"},
        {2.75, "02:45"},
        {3.5, "03:30"},
        {0, "??:??"},           /* Boss 1 */
        {4.5, "04:30"},
        {3.25, "03:15"},
        {2.75, "02:45"},
        {4.75, "04:45"},
        {0, "??:??"},           /* Boss 2 */
        {6.5, "06:30"},
        {4.5, "04:30"},
        {2.75, "02:45"},
        {4.5, "04:30"},
        {6, "06:00"},
        {0, "??:??"},           /* Boss 3 */
        {6, "06:00"},
        {0, "??:??"},           /* Boss 4 */
        {0, "??:??"},           /* Secret level 1 */
        {0, "??:??"},           /* Secret level 2 */
#endif
    };
#endif
    ClearSplitVWB ();           /* set up for double buffering in split screen */
#ifdef SEGA_SATURN   
    slScrTransparent(2);
    slSynch();
#endif
#ifdef SEGA_SATURN
    VWB_Bar(0, 0, SATURN_WIDTH, screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);
#else
    VWB_Bar (0, 0, 320, screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);
#endif
 
    if (bordercol != VIEWCOLOR)
        DrawStatusBorder (VIEWCOLOR);

    StartCPMusic (ENDLEVEL_MUS);

/*
** do the intermission
*/
    IN_ClearKeysDown ();
    IN_StartAck ();

#ifdef JAPAN
    VWB_DrawPic (0, 0, C_INTERMISSIONPIC);
#endif
    VWB_DrawPic (0, 16, L_GUYPIC);

#ifdef MAPCONTROLLEDLTIME
    if (tilemap[63][4] >= 1) {
        Write(14, 2, "Level\ncompleted");

        if (gamestate.health <= 0) {
            Write(8, 15, "Sorry you died!");
            Write(8, 18, "50 bonus!");
            GivePoints(50);
        }
        else {
            Write(8, 15, "Congratulations");
            Write(8, 18, "15000 bonus!");
            GivePoints(15000);
        }

        gamestate.health = 100;
        VL_UpdateScreen(screenBuffer);
        VL_FadeIn(0, 255, gamepal, 30);

    }
    else {
#endif
#ifndef SPEAR
        if (gamestate.mapon < 8)
#else
        if (gamestate.mapon != 4 && gamestate.mapon != 9 && gamestate.mapon != 15 && gamestate.mapon < 17)
#endif
        {
#ifndef JAPAN
#ifdef SPANISH
            Write(14, 2, "piso\ncompletado");
#else
            Write(14, 2, "floor\ncompleted");
#endif
#ifdef SEGA_SATURN
            Write(14 + SATURN_ADJUST / 8, 7, STR_BONUS "     0");
            Write(16 + SATURN_ADJUST / 8, 10, STR_TIME);
            Write(16 + SATURN_ADJUST / 8, 12, STR_PAR);
#else
            Write(14, 7, STR_BONUS "     0");
            Write(16, 10, STR_TIME);
            Write(16, 12, STR_PAR);
#endif
#ifdef SPANISH
#ifdef SEGA_SATURN
            Write(11 + SATURN_ADJUST / 8, 14, STR_RAT2KILL);
            Write(11 + SATURN_ADJUST / 8, 16, STR_RAT2SECRET);
            Write(11 + SATURN_ADJUST / 8, 18, STR_RAT2TREASURE);
#else
            Write(11, 14, STR_RAT2KILL);
            Write(11, 16, STR_RAT2SECRET);
            Write(11, 18, STR_RAT2TREASURE);
#endif
#else
#ifdef SEGA_SATURN
            Write(9 + SATURN_ADJUST / 8, 14, STR_RAT2KILL);
            Write(5 + SATURN_ADJUST / 8, 16, STR_RAT2SECRET);
            Write(1 + SATURN_ADJUST / 8, 18, STR_RAT2TREASURE);
#else
            Write(9, 14, STR_RAT2KILL);
            Write(5, 16, STR_RAT2SECRET);
            Write(1, 18, STR_RAT2TREASURE);
#endif
#endif

#ifdef SEGA_SATURN
            Write(26 + SATURN_ADJUST / 8, 12, parTimes[gamestate.episode * 10 + mapon].timestr);
#else
            Write(26, 2, wlitoa(gamestate.mapon + 1, tempstr, 10));
#endif
#endif
#ifdef MAPCONTROLPARTIME
#ifdef SPANISH
#ifdef SEGA_SATURN
            Write(30 + SATURN_ADJUST / 8, 12, times_timestr);
#else
            Write(30, 12, times_timestr);
#endif
#else
#ifdef SEGA_SATURN
            Write(26 + SATURN_ADJUST / 8, 12, times_timestr);
#else
            Write(26, 12, times_timestr);
#endif
#endif
#else
#ifdef SPANISH
#ifdef SEGA_SATURN        
            Write(30 + SATURN_ADJUST / 8, 12, parTimes[gamestate.episode * 10 + gamestate.mapon].timestr);
#else
            Write(30, 12, parTimes[gamestate.episode * 10 + gamestate.mapon].timestr);
#endif
#else
#ifdef SEGA_SATURN
            Write(26 + SATURN_ADJUST / 8, 12, parTimes[gamestate.episode * 10 + mapon].timestr);

#else
            Write(26, 12, parTimes[gamestate.episode * 10 + gamestate.mapon].timestr);
#endif
#endif
#endif

            /*
            ** PRINT TIME
            */
            sec = (int)gamestate.TimeCount / 70;

            if (sec > 99 * 60)      /* 9 minutes max */
                sec = 99 * 60;
#ifdef MAPCONTROLPARTIME
            if (gamestate.TimeCount < times_time * 4200)
                timeleft = (fixed)(times_time * 4200) / 70 - sec;
#else   
            if (gamestate.TimeCount < parTimes[gamestate.episode * 10 + gamestate.mapon].time * 4200)
                timeleft = (fixed)((parTimes[gamestate.episode * 10 + gamestate.mapon].time * 4200) / 70 - sec);
#endif
            min = sec / 60;
            sec %= 60;

#ifdef SPANISH
            i = 30 * 8;
#else
            i = 26 * 8;
#endif
#ifdef SEGA_SATURN
            VWB_DrawPic(i + SATURN_ADJUST, 10 * 8, L_NUM0PIC + (min / 10));
            i += 2 * 8;
            VWB_DrawPic(i + SATURN_ADJUST, 10 * 8, L_NUM0PIC + (min % 10));
            i += 2 * 8;
            Write(i / 8 + SATURN_ADJUST / 8, 10, ":");
            i += 1 * 8;
            VWB_DrawPic(i + SATURN_ADJUST, 10 * 8, L_NUM0PIC + (sec / 10));
            i += 2 * 8;
            VWB_DrawPic(i + SATURN_ADJUST, 10 * 8, L_NUM0PIC + (sec % 10));
#else
            VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (min / 10));
            i += 2 * 8;
            VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (min % 10));
            i += 2 * 8;
            Write(i / 8, 10, ":");
            i += 1 * 8;
            VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (sec / 10));
            i += 2 * 8;
            VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (sec % 10));
#endif

#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)
            VL_UpdateScreen(screenBuffer);
#endif
            VL_FadeIn(0, 255, gamepal, 30);
#ifdef SEGA_SATURN
            DrawStatusBar(); /* vbt : ajout */
#endif

            /*
            ** FIGURE RATIOS OUT BEFOREHAND
            */
            kr = sr = tr = 0;
            if (gamestate.killtotal)
                kr = (gamestate.killcount * 100) / gamestate.killtotal;
            if (gamestate.secrettotal)
                sr = (gamestate.secretcount * 100) / gamestate.secrettotal;
            if (gamestate.treasuretotal)
                tr = (gamestate.treasurecount * 100) / gamestate.treasuretotal;


            /*
            ** PRINT TIME BONUS
            */
            bonus = timeleft * PAR_AMOUNT;
            if (bonus)
            {
                for (i = 0; i <= timeleft; i++)
                {
                    wlitoa(i * PAR_AMOUNT, tempstr, 10);
                    x = 36 - (int)strlen(tempstr) * 2;
                    Write(x, 7, tempstr);
                    if (!(i % (PAR_AMOUNT / 10)))
                        SD_PlaySound(ENDBONUS1SND);
                    if (!usedoublebuffering || !(i % (PAR_AMOUNT / 50)))
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)                    
                        VL_UpdateScreen(screenBuffer);
#endif
                    while (SD_SoundPlaying())
                        BJ_Breathe();
                    if (IN_CheckAck())
                        goto done;
                }
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)				
                VL_UpdateScreen(screenBuffer);
#else
                /*		DrawStatusBar(); */ /* vbt ajout */				
#endif

                SD_PlaySound(ENDBONUS2SND);
                while (SD_SoundPlaying())
                    BJ_Breathe();
            }


#ifdef SPANISH
#define RATIOXX                33
#else
#define RATIOXX                37
#endif
            /*
            ** KILL RATIO
            */
            ratio = kr;
            for (i = 0; i <= ratio; i++)
            {
                wlitoa(i, tempstr, 10);
                x = RATIOXX - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
                Write(x + SATURN_ADJUST / 14, 7, tempstr);
#else
                Write(x, 14, tempstr);
#endif
                if (!(i % 10))
                    SD_PlaySound(ENDBONUS1SND);
                if (!usedoublebuffering || !(i & 1))
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
                    VL_UpdateScreen(screenBuffer);
#endif

                while (SD_SoundPlaying())
                    BJ_Breathe();

                if (IN_CheckAck())
                    goto done;
            }
            if (ratio >= 100)
            {
                VL_WaitVBL(VBLWAIT);
                SD_StopSound();
                bonus += PERCENT100AMT;
                wlltoa(bonus, tempstr, 10);
                x = (RATIOXX - 1) - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
                Write(x + SATURN_ADJUST / 8, 7, tempstr);
#else
                Write(x, 7, tempstr);
#endif
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
                VL_UpdateScreen(screenBuffer);
#endif
                SD_PlaySound(PERCENT100SND);
            }
            else if (!ratio)
            {
                VL_WaitVBL(VBLWAIT);
                SD_StopSound();
                SD_PlaySound(NOBONUSSND);
            }
            else
                SD_PlaySound(ENDBONUS2SND);

#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
            VL_UpdateScreen(screenBuffer);
#endif
            while (SD_SoundPlaying())
                BJ_Breathe();

            /*
            ** SECRET RATIO
            */
            ratio = sr;
            for (i = 0; i <= ratio; i++)
            {
                wlltoa(i, tempstr, 10);
                x = RATIOXX - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
                Write(x + SATURN_ADJUST / 8, 16, tempstr);
#else
                Write(x, 16, tempstr);
#endif
                if (!(i % 10))
                    SD_PlaySound(ENDBONUS1SND);
                if (!usedoublebuffering || !(i & 1))
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)                
                    VL_UpdateScreen(screenBuffer);
#endif
                while (SD_SoundPlaying())
                    BJ_Breathe();

                if (IN_CheckAck())
                    goto done;
            }
            if (ratio >= 100)
            {
                VL_WaitVBL(VBLWAIT);
                SD_StopSound();
                bonus += PERCENT100AMT;
                wlltoa(bonus, tempstr, 10);
                x = (RATIOXX - 1) - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
                Write(x + SATURN_WIDTH / 8, 7, tempstr);
#else
                Write(x, 7, tempstr);
#endif
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
                VL_UpdateScreen(screenBuffer);
#endif
                SD_PlaySound(PERCENT100SND);
            }
            else if (!ratio)
            {
                VL_WaitVBL(VBLWAIT);
                SD_StopSound();
                SD_PlaySound(NOBONUSSND);
            }
            else
                SD_PlaySound(ENDBONUS2SND);
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
            VL_UpdateScreen(screenBuffer);
#endif
            while (SD_SoundPlaying())
                BJ_Breathe();

            /*
            ** TREASURE RATIO
            */
            ratio = tr;
            for (i = 0; i <= ratio; i++)
            {
                wlitoa(i, tempstr, 10);
                x = RATIOXX - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
                Write(x + SATURN_ADJUST / 8, 18, tempstr);
#else
                Write(x, 18, tempstr);
#endif
                if (!(i % 10))
                    SD_PlaySound(ENDBONUS1SND);
                if (!usedoublebuffering || !(i & 1))
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
                    VL_UpdateScreen(screenBuffer);
#endif
                while (SD_SoundPlaying())
                    BJ_Breathe();
                if (IN_CheckAck())
                    goto done;
            }
            if (ratio >= 100)
            {
                VL_WaitVBL(VBLWAIT);
                SD_StopSound();
                bonus += PERCENT100AMT;
                wlltoa(bonus, tempstr, 10);
                x = (RATIOXX - 1) - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
                Write(x + SATURN_WIDTH / 8, 7, tempstr);
#else
                Write(x, 7, tempstr);
#endif
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
                VL_UpdateScreen(screenBuffer);
#endif
                SD_PlaySound(PERCENT100SND);
            }
            else if (!ratio)
            {
                VL_WaitVBL(VBLWAIT);
                SD_StopSound();
                SD_PlaySound(NOBONUSSND);
            }
            else
                SD_PlaySound(ENDBONUS2SND);
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
            VL_UpdateScreen(screenBuffer);
#endif
            while (SD_SoundPlaying())
                BJ_Breathe();


            /*
            ** JUMP STRAIGHT HERE IF KEY PRESSED
            */
        done:   wlitoa(kr, tempstr, 10);
        x = RATIOXX - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
            Write(x + SATURN_WIDTH / 8, 14, tempstr);
#else
            Write(x, 14, tempstr);
#endif
            wlitoa(sr, tempstr, 10);
            x = RATIOXX - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
            Write(x + SATURN_WIDTH / 8, 16, tempstr);
#else
            Write(x, 16, tempstr);
#endif
            wlitoa(tr, tempstr, 10);
            x = RATIOXX - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
            Write(x + SATURN_WIDTH / 8, 18, tempstr);
#else
            Write(x, 18, tempstr);
#endif
            bonus = (int)timeleft * PAR_AMOUNT +
                (PERCENT100AMT * (kr >= 100)) +
                (PERCENT100AMT * (sr >= 100)) + (PERCENT100AMT * (tr >= 100));

            GivePoints(bonus);
            wlitoa(bonus, tempstr, 10);
            x = 36 - (int)strlen(tempstr) * 2;
#ifdef SEGA_SATURN
            Write(x + SEGA_SATURN / 8, 7, tempstr);
#else
            Write(x, 7, tempstr);
#endif
            /*
            ** SAVE RATIO INFORMATION FOR ENDGAME
            */
            LevelRatios[gamestate.mapon].kill = kr;
            LevelRatios[gamestate.mapon].secret = sr;
            LevelRatios[gamestate.mapon].treasure = tr;
            LevelRatios[gamestate.mapon].time = min * 60 + sec;
        }
        else
        {
#ifdef SPEAR
#ifndef SPEARDEMO
            switch (gamestate.mapon)
            {
#ifdef SEGA_SATURN
            case 4:
                Write(14 + SATURN_ADJUST / 8, 4, " trans\n" " grosse\n" STR_DEFEATED);
                break;
            case 9:
                Write(14 + SATURN_ADJUST / 8, 4, "barnacle\n" "wilhelm\n" STR_DEFEATED);
                break;
            case 15:
                Write(14 + SATURN_ADJUST / 8, 4, "ubermutant\n" STR_DEFEATED);
                break;
            case 17:
                Write(14 + SATURN_ADJUST / 8, 4, " death\n" " knight\n" STR_DEFEATED);
                break;
            case 18:
                Write(13 + SATURN_ADJUST / 8, 4, "secret tunnel\n" "    area\n" "  completed!");
                break;
            case 19:
                Write(13 + SATURN_ADJUST / 8, 4, "secret castle\n" "    area\n" "  completed!");
                break;
#else
            case 4:
                Write(14, 4, " trans\n" " grosse\n" STR_DEFEATED);
                break;
            case 9:
                Write(14, 4, "barnacle\n" "wilhelm\n" STR_DEFEATED);
                break;
            case 15:
                Write(14, 4, "ubermutant\n" STR_DEFEATED);
                break;
            case 17:
                Write(14, 4, " death\n" " knight\n" STR_DEFEATED);
                break;
            case 18:
                Write(13, 4, "secret tunnel\n" "    area\n" "  completed!");
                break;
            case 19:
                Write(13, 4, "secret castle\n" "    area\n" "  completed!");
                break;
#endif
            }
#endif
#else
            Write(14, 4, "secret floor\n completed!");
#endif

            Write(10, 16, "15000 bonus!");
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
            VL_UpdateScreen(screenBuffer);
#endif
            VL_FadeIn(0, 255, gamepal, 30);

            GivePoints(15000);
        }
#ifdef MAPCONTROLLEDLTIME
    }
#endif

    DrawScore ();
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
    VL_UpdateScreen(screenBuffer);
#else
    DrawStatusBar(); /* vbt ajout */

#endif

    lastBreathTime = GetTimeCount();
    IN_StartAck ();
    while (!IN_CheckAck ())
        BJ_Breathe ();

/*
** done
*/
#ifdef SPEARDEMO
    if (gamestate.mapon == 1)
    {
        SD_PlaySound (BONUS1UPSND);

        Message ("This concludes your demo\n"
                 "of Spear of Destiny! Now,\n" "go to your local software\n" "store and buy it!");

        IN_ClearKeysDown ();
        IN_Ack ();
    }
#endif

#ifdef JAPDEMO
    if (gamestate.mapon == 3)
    {
        SD_PlaySound (BONUS1UPSND);

        Message ("This concludes your demo\n"
                 "of Wolfenstein 3-D! Now,\n" "go to your local software\n" "store and buy it!");

        IN_ClearKeysDown ();
        IN_Ack ();
    }
#endif

    VL_FadeOut (0, 255, 0, 0, 0, 30);
#ifndef SEGA_SATURN
    DrawPlayBorder();
#endif
}



/* ========================================================================== */


/*
=================
=
= PreloadGraphics
=
= Fill the cache up
=
=================
*/

boolean
PreloadUpdate (unsigned current, unsigned total)
{
    unsigned w = WindowW - scaleFactor * 10;

    VL_BarScaledCoord (WindowX + scaleFactor * 5, WindowY + WindowH - scaleFactor * 3,
        w, scaleFactor * 2, BLACK);
    w = ((int) w * current) / total;
    if (w)
    {
        VL_BarScaledCoord (WindowX + scaleFactor * 5, WindowY + WindowH - scaleFactor * 3,
            w, scaleFactor * 2, 0x37);       /* SECONDCOLOR); */
        VL_BarScaledCoord (WindowX + scaleFactor * 5, WindowY + WindowH - scaleFactor * 3,
            w - scaleFactor * 1, scaleFactor * 1, 0x32);

    }
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
    VL_UpdateScreen(screenBuffer);
#endif
/*      
**      if (LastScan == sc_Escape)
**      {
**              IN_ClearKeysDown();
**              return(true);
**      }
**      else
*/
    return (false);
}

void
PreloadGraphics (void)
{
    DrawLevel ();
    ClearSplitVWB ();           /* set up for double buffering in split screen */
#ifdef SEGA_SATURN
    slScrTransparent(2);
    slSynch();
#endif

    VL_BarScaledCoord (0, 0, screenWidth, screenHeight - scaleFactor * (STATUSLINES - 1), bordercol);
    VWB_DrawPicScaledCoord (((screenWidth-scaleFactor*224)/16) * 8,
        (screenHeight-scaleFactor*(STATUSLINES+48))/2, GETPSYCHEDPIC);

    WindowX = (screenWidth - scaleFactor*224)/2;
    WindowY = (screenHeight - scaleFactor*(STATUSLINES+48))/2;
    WindowW = scaleFactor * 28 * 8;
    WindowH = scaleFactor * 48;

#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
    VL_UpdateScreen(screenBuffer);
#endif
#ifdef SEGA_SATURN
    DrawStatusBar(); // vbt ajout
#endif
/*
#ifdef SEGA_SATURN
#if 0
    //----------------------------------------------------------------------
    char fname[13] = "VSWAP.";
    unsigned int y;
    extern int ChunksInFile;

    strcat(fname, extension);

    Sint32 fileId;

    fileId = GFS_NameToId((Sint8*)fname);

    word* pageLengths = (word*)saturnChunk + (ChunksInFile + 1) * sizeof(int32_t);
    unsigned int* pageOffsets = (unsigned int*)saturnChunk + 0x2000;
    unsigned char* itemmap = (unsigned char*)saturnChunk + 0x4000;
    unsigned char* Chunks = (unsigned char*)saturnChunk + 0xC000;

    if (wallData == NULL) wallData = (unsigned char*)malloc(((NB_WALL_HWRAM * 2) + 8) * 0x1000);
    unsigned char* ptr = (unsigned char*)wallData;
    loaded += (12 + (SPR_NULLSPRITE - SPR_KNIFEREADY));
    for (y = 1; y < 64; y++)
    {
        if (itemmap[y + 1] == 1)
            loaded += 2;
    }

    int i = 0;

    // walls 0/1
    PMPages[0] = ptr;
    readChunks(fileId, 0x2000, &pageOffsets[0], Chunks, ptr);
    PMPages[1] = ptr + 0x1000;
    ptr += 0x2000;
    PreloadUpdate((i += 2), loaded);
    // walls 40/41
    PMPages[42] = ptr;
    readChunks(fileId, 0x2000, &pageOffsets[40], Chunks, ptr);
    PMPages[43] = ptr + 0x1000;
    ptr += 0x2000;
    PreloadUpdate((i += 2), loaded);

    // doors
    for (y = PMSpriteStart - 8; y < PMSpriteStart; y++)
    {
        if (!pageOffsets[y])
            continue;

        PMPages[y] = ptr;
        readChunks(fileId, 0x1000, &pageOffsets[y], Chunks, ptr);
        ptr += 0x1000;
        PreloadUpdate(i++, loaded);
    }

    // walls in map
    for (y = 1; y < NB_WALL_HWRAM / 2; y++)
    {
        if (itemmap[y + 1] == 1)
        {
            PMPages[(y * 2)] = ptr;
            readChunks(fileId, 0x2000, &pageOffsets[(y * 2)], Chunks, ptr);
            PMPages[(y * 2) + 1] = ptr + 0x1000;
            ptr += 0x2000;
            PreloadUpdate((i += 2), loaded);
        }
    }

    int* val = (int*)ptr;
    slPrintHex((int)val, slLocate(10, 21));

    ptr = (unsigned char*)0x00202000;

    for (y = NB_WALL_HWRAM / 2; y < 64; y++)
    {
        if (itemmap[y + 1] == 1)
        {
            PMPages[(y * 2)] = ptr;
            readChunks(fileId, 0x2000, &pageOffsets[(y * 2)], Chunks, ptr);
            PMPages[(y * 2) + 1] = ptr + 0x1000;
            ptr += 0x2000;
            PreloadUpdate((i += 2), loaded);
        }
    }
    // last page points after page buffer
    PMPages[ChunksInFile] = ptr; // retourner l'adresse du pointeur
    // ennemies
    for (y = PMSpriteStart; y < PMSpriteStart + SPR_KNIFEREADY; y++)
    {
        if (itemmap[y] == 1)
        {
#ifdef APOGEE_1_1				
            if (y >= PMSpriteStart + SPR_BJ_W1 - 2 && y <= PMSpriteStart + SPR_BJ_JUMP4)
                ptr = PM_DecodeSprites2(y, y + 1, pageOffsets + 2, pageLengths + 2, ptr, fileId);
            else
#endif
                ptr = PM_DecodeSprites2(y, y + 1, pageOffsets, pageLengths, ptr, fileId);
            PreloadUpdate(i++, loaded);
        }
        else
            PMPages[y] = ptr;
    }
    // weapons  doit �tre apr�s les ennemis
    for (y = PMSpriteStart + SPR_KNIFEREADY; y < PMSpriteStart + SPR_NULLSPRITE; y++)
    {
#ifdef APOGEE_1_1	
        ptr = PM_DecodeSprites2(y, y + 1, pageOffsets + 2, pageLengths + 2, ptr, fileId);
#else
        ptr = PM_DecodeSprites2(y, y + 1, pageOffsets, pageLengths, ptr, fileId);
#endif
        PreloadUpdate(i++, loaded);
    }
    PMPages[PMSpriteStart + SPR_NULLSPRITE] = ptr;
    PreloadUpdate(10, 10);
    val = (int*)ptr;
    slPrintHex((int)val, slLocate(10, 22));
    //----------------------------------------------------------------------
#endif
#endif
*/
    VL_FadeIn (0, 255, gamepal, 30);

    PreloadUpdate (10, 10);
    IN_UserInput (70);
    VL_FadeOut (0, 255, 0, 0, 0, 30);
#ifndef SEGA_SATURN
    DrawPlayBorder ();
#endif
#ifdef SEGA_SATURN
    slScrTransparent(0);
    slSynch();
#endif
    VL_UpdateScreen (screenBuffer);
}


/* ========================================================================== */

/*
==================
=
= DrawHighScores
=
==================
*/

void
DrawHighScores (void)
{
#ifdef SEGA_SATURN
    slScrTransparent(2);
    slSynch();
#endif
    char buffer[16];
#ifndef SPEAR
    char *str;
#ifndef UPLOAD
    char buffer1[5];
#endif
#endif
    unsigned short i, w, h;
    HighScore *s;

#ifndef SPEAR
    ClearMScreen ();
    DrawStripes (10);

    VWB_DrawPic (48, 0, HIGHSCORESPIC);

#ifndef APOGEE_1_0
#ifdef SEGA_SATURN
    VWB_DrawPic(SATURN_ADJUST + 4 * 8, 68, C_NAMEPIC);
    VWB_DrawPic(SATURN_ADJUST + 20 * 8, 68, C_LEVELPIC);
    VWB_DrawPic(SATURN_ADJUST + 28 * 8, 68, C_SCOREPIC);
#else
    VWB_DrawPic (4 * 8, 68, C_NAMEPIC);
    VWB_DrawPic (20 * 8, 68, C_LEVELPIC);
    VWB_DrawPic (28 * 8, 68, C_SCOREPIC);
#endif
#else
#ifdef SEGA_SATURN
    VWB_DrawPic(SATURN_ADJUST + 35 * 8, 68, C_CODEPIC);
#else
    VWB_DrawPic(35*8,68,C_CODEPIC);
#endif
#endif
    fontnumber = 0;

#else
    ClearMScreen ();
    DrawStripes (10);

#ifdef SEGA_SATURN
    VWB_DrawPic(48 + SATURN_ADJUST, 0, HIGHSCORESPIC);
#else
    VWB_DrawPic (0, 0, HIGHSCORESPIC);
#endif
    fontnumber = 1;
#endif


#ifndef SPEAR
    SETFONTCOLOR (15, 0x29);
#else
    SETFONTCOLOR (HIGHLIGHT, 0x29);
#endif

    for (i = 0, s = Scores; i < MaxScores; i++, s++)
    {
        PrintY = 76 + (16 * i);

        /*
        ** name
        */
#ifndef SPEAR
#ifdef SEGA_SATURN
        PrintX = SATURN_ADJUST + 4 * 8;
#else
        PrintX = 4 * 8;
#endif
#else
#ifdef SEGA_SATURN
        PrintX = SATURN_ADJUST + 16;
#else
        PrintX = 16;
#endif
#endif
        US_Print (s->name);

        /*
        ** level
        */
        wlitoa(s->completed, buffer, 10);
#ifndef SPEAR
        for (str = buffer; *str; str++)
            *str = *str + (129 - '0');  /* Used fixed-width numbers (129...) */
        USL_MeasureString (buffer, &w, &h);
#ifdef SEGA_SATURN
        PrintX = SATURN_ADJUST + (22 * 8) - w;
#else
        PrintX = (22 * 8) - w;
#endif
#else
        USL_MeasureString (buffer, &w, &h);
#ifdef SEGA_SATURN
        PrintX = SATURN_ADJUST + 194 - w;
#else
        PrintX = 194 - w;
#endif
#endif

#ifndef UPLOAD
#ifndef SPEAR
        PrintX -= 6;
        wlitoa(s->episode + 1, buffer1, 10);
        US_Print ("E");
        US_Print (buffer1);
        US_Print ("/L");
#endif
#endif

#ifdef SPEAR
        if (s->completed == 21)
#ifdef SEGA_SATURN
            VWB_DrawPic(SATURN_ADJUST + PrintX + 8, PrintY - 1, C_WONSPEARPIC);
#else
            VWB_DrawPic (PrintX + 8, PrintY - 1, C_WONSPEARPIC);
#endif
        else
#endif
            US_Print (buffer);

        /*
        ** score
        */
        wlitoa(s->score, buffer, 10);
#ifndef SPEAR
        for (str = buffer; *str; str++)
            *str = *str + (129 - '0');  /* Used fixed-width numbers (129...) */
        USL_MeasureString (buffer, &w, &h);
#ifdef SEGA_SATURN
        PrintX = SATURN_ADJUST + (34 * 8) - 8 - w;
#else
        PrintX = (34 * 8) - 8 - w;
#endif
#else
        USL_MeasureString (buffer, &w, &h);
#ifdef SEGA_SATURN
        PrintX = SATURN_ADJUST + 292 - w;
#else
        PrintX = 292 - w;
#endif
#endif
        US_Print (buffer);

#ifdef APOGEE_1_0
/* #ifndef UPLOAD */
#ifndef SPEAR
        /*
        ** verification #
        */
        if (!i)
        {
            char temp = (((s->score >> 28) & 0xf) ^ ((s->score >> 24) & 0xf)) + 'A';
            char temp1 = (((s->score >> 20) & 0xf) ^ ((s->score >> 16) & 0xf)) + 'A';
            char temp2 = (((s->score >> 12) & 0xf) ^ ((s->score >> 8) & 0xf)) + 'A';
            char temp3 = (((s->score >> 4) & 0xf) ^ ((s->score >> 0) & 0xf)) + 'A';

            SETFONTCOLOR (0x49, 0x29);
            PrintX = 35 * 8;
            buffer[0] = temp;
            buffer[1] = temp1;
            buffer[2] = temp2;
            buffer[3] = temp3;
            buffer[4] = 0;
            US_Print (buffer);
            SETFONTCOLOR (15, 0x29);
        }
#endif
/* #endif */
#endif
    }

#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
    VL_UpdateScreen(screenBuffer);
#endif

#ifdef SPEAR
    fontnumber = 0;
#endif
}

/* =========================================================================== */


/*
=======================
=
= CheckHighScore
=
=======================
*/

void
CheckHighScore (int score, unsigned short other)
{
    unsigned short i, j;
    int n;
    HighScore myscore;

    memset(myscore.name, 0, sizeof(myscore.name));	/* clear the string buffer */
    strcpy (myscore.name, "");
    myscore.score = score;
    myscore.episode = gamestate.episode;
    myscore.completed = other;

    for (i = 0, n = -1; i < MaxScores; i++)
    {
        if ((myscore.score > Scores[i].score)
            || ((myscore.score == Scores[i].score) && (myscore.completed > Scores[i].completed)))
        {
            for (j = MaxScores; --j > i;)
                Scores[j] = Scores[j - 1];
            Scores[i] = myscore;
            n = i;
            break;
        }
    }

#ifdef SPEAR
    StartCPMusic (XAWARD_MUS);
#else
    StartCPMusic (ROSTER_MUS);
#endif
    DrawHighScores ();

    VL_FadeIn (0, 255, gamepal, 30);

    if (n != -1)
    {
        /*
        ** got a high score
        */
        PrintY = 76 + (16 * n);
#ifndef SPEAR
        PrintX = 4 * 8;
        backcolor = BORDCOLOR;
        fontcolor = 15;
#ifndef SEGA_SATURN
        US_LineInput (PrintX, PrintY, Scores[n].name, 0, true, MaxHighName, 100);
#endif
#else
        PrintX = 16;
        fontnumber = 1;
        VWB_Bar (PrintX - 2, PrintY - 2, 145, 15, 0x9c);
#if !defined(USE_SPRITES) && !defined(SEGA_SATURN)            
        VL_UpdateScreen(screenBuffer);
#endif        
        backcolor = 0x9c;
        fontcolor = 15;
#ifndef SEGA_SATURN
        US_LineInput (PrintX, PrintY, Scores[n].name, 0, true, MaxHighName, 130);
#endif
#endif
    }
    else
    {
        IN_ClearKeysDown ();
        IN_UserInput (500);
    }

}

#ifndef SEGA_SATURN
#ifndef UPLOAD
#ifndef SPEAR
#ifndef JAPAN
/*
////////////////////////////////////////////////////////
//
// NON-SHAREWARE NOTICE
//
////////////////////////////////////////////////////////
*/

void
NonShareware (void)
{
    VL_FadeOut (0, 255, 0, 0, 0, 30);

    ClearMScreen ();
    DrawStripes (10);

    fontnumber = 1;

    SETFONTCOLOR (READHCOLOR, BKGDCOLOR);
    PrintX = 110;
    PrintY = 15;

#ifdef SPANISH
    US_Print ("Atencion");
#else
    US_Print ("Attention");
#endif

    SETFONTCOLOR (HIGHLIGHT, BKGDCOLOR);
    WindowX = PrintX = 40;
    PrintY = 60;
#ifdef SPANISH
    US_Print ("Este juego NO es gratis y\n");
    US_Print ("NO es Shareware; favor de\n");
    US_Print ("no distribuirlo.\n\n");
#else
    US_Print ("This game is NOT shareware.\n");
    US_Print ("Please do not distribute it.\n");
    US_Print ("Thanks.\n\n");
#endif
    US_Print ("        Id Software\n");

    VL_UpdateScreen(screenBuffer);
    VL_FadeIn (0, 255, gamepal, 30);
    IN_Ack ();
}
#endif
#endif
#endif

#ifdef SPEAR
#ifndef GOODTIMES
#ifndef SPEARDEMO

/*
////////////////////////////////////////////////////////
//
// COPY PROTECTION FOR FormGen
//
////////////////////////////////////////////////////////
*/

char CopyProFailedStrs[][100] = {
    STR_COPY1,
    STR_COPY2,

    STR_COPY3,
    STR_COPY4,

    STR_COPY5,
    STR_COPY6,

    STR_COPY7,
    STR_COPY8,

    STR_COPY9,
    "",

    STR_COPY10,
    STR_COPY11,

    STR_COPY12,
    "",

    STR_COPY13,
    "",

    STR_COPY14,
    ""
};

char BackDoorStrs[5][16] = {
    "a spoon?",
    "bite me!",
    "joshua",
    "pelt",
    "snoops"
};

char GoodBoyStrs[10][40] = {
    "...is the CORRECT ANSWER!",
    "",

    "Consider yourself bitten, sir.",
    "",

    "Greetings Professor Falken, would you",
    "like to play Spear of Destiny?",

    "Do you have any gold spray paint?",
    "",

    "I wish I had a 21\" monitor...",
    ""
};

char bossstrs[4][24] = {
    "DEATH KNIGHT",
    "BARNACLE WILHELM",
    "UBERMUTANTUBER MUTANT",
    "TRANS GROSSE"
};

char WordStr[5][20] = {
    "New Game",
    "Sound...F4",
    "Control...F6",
    "Change View...F5",
    "Quit...F10"
};

char WordCorrect[5][2] = { "3", "4", "4", "5", "5" };

char MemberStr[10][40] = {
    STR_COPY15,
    "",

    STR_COPY16,
    "",

    STR_COPY17,
    STR_COPY18,

    STR_COPY19,
    STR_COPY20,

    STR_COPY21,
    STR_COPY22
};

char MemberCorrect[5][24] = {
    "adrian carmack",
    "john carmackjohn romero",
    "tom hall",
    "jay wilbur",
    "kevin cloud"
};

char DosMessages[9][80] = {
    STR_NOPE1,
    STR_NOPE2,
    STR_NOPE3,
    STR_NOPE4,
    STR_NOPE5,
    STR_NOPE6,
    STR_NOPE7,
    STR_NOPE8,
    STR_NOPE9
};

char MiscTitle[4][20] = {
    "BLOOD TEST",
    "STRAIGHT-LACED",
    "QUITE SHAPELY",
    "I AM WHAT I AMMO"
};

char MiscStr[12][40] = {
    STR_MISC1,
    STR_MISC2,
    "",

    STR_MISC3,
    STR_MISC4,
    "",

    STR_MISC5,
    STR_MISC6,
    "",

    STR_MISC7,
    STR_MISC8,
    STR_MISC9
};

char MiscCorrect[4][5] = { "ss", "8", STR_STAR, "45" };


int
BackDoor (char *s)
{
    int i;

    for (i = 0; i < 5; i++)
    {
        if (!w3sstrcasecmp (s, BackDoorStrs[i]))
        {
            SETFONTCOLOR (14, 15);
            fontnumber = 0;
            PrintY = 175;
            VWB_DrawPic (0, 20 * 8, COPYPROTBOXPIC);
            US_CPrint (GoodBoyStrs[i * 2]);
            US_CPrint (GoodBoyStrs[i * 2 + 1]);
            VL_UpdateScreen (screenBuffer);
            return 1;
        }
    }

    return 0;
}


void
CopyProtection (void)
{
#define TYPEBOX_Y       177
#define TYPEBOX_BKGD    0x9c
#define PRINTCOLOR      HIGHLIGHT

    unsigned i;
    int match, whichboss, bossnum, attempt, whichline;
    int enemypicked[4] = { 0, 0, 0, 0 };
    int bosses[4] = { BOSSPIC1PIC, BOSSPIC2PIC, BOSSPIC3PIC, BOSSPIC4PIC };
    int whichpicked[4] = { 0, 0, 0, 0 };
    int whichone, quiztype, whichmem, whichword;
    int memberpicked[5] = { 0, 0, 0, 0, 0 };
    int wordpicked[5] = { 0, 0, 0, 0, 0 };

    char inputbuffer[20];
    char message[80];

    enum
    {
        debriefing,
        checkmanual,
        staffquiz,
        miscquiz,

        totaltypes
    };



    attempt = 0;
    VL_FadeOut (0, 255, 0, 0, 0, 30);
    CA_LoadAllSounds ();
    StartCPMusic (COPYPRO_MUS);
    US_InitRndT (true);

    while (attempt < 3)
    {
        fontnumber = 1;
        SETFONTCOLOR (PRINTCOLOR - 2, 15);
        VWB_DrawPic (0, 0, C_BACKDROPPIC);
        VWB_DrawPic (0, 0, COPYPROTTOPPIC);
        VWB_DrawPic (0, 20 * 8, COPYPROTBOXPIC);
        WindowX = WindowY = 0;
        WindowW = 320;
        WindowH = 200;
        PrintY = 65;

        quiztype = US_RndT () % totaltypes;
        switch (quiztype)
        {
            /*
            ** BOSSES QUIZ
            */
            case debriefing:
            {
				size_t inputlen;
                PrintX = 0;
                US_Print (STR_DEBRIEF);
                SETFONTCOLOR (PRINTCOLOR, 15);

                while (enemypicked[whichboss = US_RndT () & 3]);
                enemypicked[whichboss] = 1;
                bossnum = bosses[whichboss];
                VWB_DrawPic (128, 60, bossnum);
                fontnumber = 0;
                PrintY = 130;
                US_CPrint (STR_ENEMY1 "\n");
                US_CPrint (STR_ENEMY2 "\n\n");

                VL_UpdateScreen (screenBuffer);
                VL_FadeIn (0, 255, gamepal, 30);

                PrintX = 100;
                fontcolor = 15;
                backcolor = TYPEBOX_BKGD;
                inputbuffer[0] = 0;
                PrintY = TYPEBOX_Y;
                fontnumber = 1;
                US_LineInput (PrintX, PrintY, inputbuffer, 0, true, 20, 100);

                match = 0;
                inputlen = strlen(inputbuffer);
                if(inputlen > 3)
                {
                    size_t bosslen = strlen(bossstrs[whichboss]);
                    for (i = 0; i < bosslen; i++)
                    {
                        if (!w3sstrncasecmp (inputbuffer, bossstrs[whichboss] + i, inputlen))
                        {
                            match = 1;
                            break;
                        }
                    }
                }

                match += BackDoor (inputbuffer);
                break;
            }

            /*
            ** MANUAL CHECK
            */
            case checkmanual:
            {
                while (wordpicked[whichword = US_RndT () % 5]);
                wordpicked[whichword] = 1;
                US_CPrint (STR_CHECKMAN);
                SETFONTCOLOR (PRINTCOLOR, 15);
                PrintY += 25;
                US_CPrint (STR_MAN1);
                US_CPrint (STR_MAN2);
                sprintf(message, STR_MAN3 " \"%s\" " STR_MAN4, WordStr[whichword]);
                US_CPrint (message);
                VL_UpdateScreen (screenBuffer);
                VL_FadeIn (0, 255, gamepal, 30);

                PrintX = 146;
                fontcolor = 15;
                backcolor = TYPEBOX_BKGD;
                inputbuffer[0] = 0;
                PrintY = TYPEBOX_Y;
                US_LineInput (PrintX, PrintY, inputbuffer, 0, true, 6, 100);

                match = 1 - (w3sstrcasecmp (inputbuffer, WordCorrect[whichword]) != 0);
                match += BackDoor (inputbuffer);
                break;
            }

            /*
            ** STAFF QUIZ
            */
            case staffquiz:
            {
				size_t inputlen;
                while (memberpicked[whichmem = US_RndT () % 5]);
                memberpicked[whichmem] = 1;
                US_CPrint (STR_ID1);
                SETFONTCOLOR (PRINTCOLOR, 15);
                PrintY += 25;
                US_CPrint (MemberStr[whichmem * 2]);
                US_CPrint (MemberStr[whichmem * 2 + 1]);
                VL_UpdateScreen (screenBuffer);
                VL_FadeIn (0, 255, gamepal, 30);

                PrintX = 100;
                fontcolor = 15;
                backcolor = TYPEBOX_BKGD;
                inputbuffer[0] = 0;
                PrintY = TYPEBOX_Y;
                US_LineInput (PrintX, PrintY, inputbuffer, 0, true, 20, 120);

                match = 0;
                inputlen = strlen(inputbuffer);
                if(inputlen > 2)
                {
                    size_t memberlen = strlen(MemberCorrect[whichmem]);
                    for (i = 0; i < memberlen; i++)
                    {
                        if (!w3sstrncasecmp (inputbuffer, MemberCorrect[whichmem] + i, inputlen))
                        {
                            match = 1;
                            break;
                        }
                    }
                }

                match += BackDoor (inputbuffer);
                break;
            }

            /*
            ** MISCELLANEOUS QUESTIONS
            */
            case miscquiz:
            {
                while (whichpicked[whichone = US_RndT () & 3]);
                whichpicked[whichone] = 1;
                US_CPrint (MiscTitle[whichone]);
                SETFONTCOLOR (PRINTCOLOR, 15);
                PrintY += 25;
                US_CPrint (MiscStr[whichone * 3]);
                US_CPrint (MiscStr[whichone * 3 + 1]);
                US_CPrint (MiscStr[whichone * 3 + 2]);
                VL_UpdateScreen (screenBuffer);
                VL_FadeIn (0, 255, gamepal, 30);

                PrintX = 146;
                fontcolor = 15;
                backcolor = TYPEBOX_BKGD;
                inputbuffer[0] = 0;
                PrintY = TYPEBOX_Y;
                US_LineInput (PrintX, PrintY, inputbuffer, 0, true, 6, 100);

                match = 1 - (w3sstrcasecmp (inputbuffer, MiscCorrect[whichone]) != 0);
                match += BackDoor (inputbuffer);
                break;
            }
        }

        /*
        ** IF NO MATCH, WE'VE GOT A (MINOR) PROBLEM!
        */

        if (!match)
        {
            whichline = 2 * (US_RndT () % 9);
            SETFONTCOLOR (14, 15);
            fontnumber = 0;
            PrintY = 175;
            VWB_DrawPic (0, 20 * 8, COPYPROTBOXPIC);
            US_CPrint (CopyProFailedStrs[whichline]);
            US_CPrint (CopyProFailedStrs[whichline + 1]);

            VL_UpdateScreen (screenBuffer);
            SD_PlaySound (NOWAYSND);
            IN_UserInput (TickBase * 6);
            VL_FadeOut (0, 255, 0, 0, 0, 30);
            attempt++;
        }
        else
        {
            int start;

            SD_PlaySound (BONUS1UPSND);
            SD_WaitSoundDone ();

            switch (SoundMode)
            {
                case sdm_Off:
                    return;
                case sdm_PC:
                    start = STARTPCSOUNDS;
                    break;
                case sdm_AdLib:
                    start = STARTADLIBSOUNDS;
            }

            return;
        }
    }

   SD_StopDigitized ();
 /*   ShutdownId ();    */

 /*   printf ("%s\n", DosMessages[US_RndT () % 9]);   */
 /*   exit (1);   */
}
#endif /* SEGA_SATURN */
#endif /* SPEARDEMO */
#endif /* GOODTIMES */
#endif /* SPEAR */
/* =========================================================================== */
