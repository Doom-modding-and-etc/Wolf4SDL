// WL_LED.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_LED)
typedef struct Led_s
{
    float scx;
    float scy;
    float scwidth;
    float scheight;
    float thickness;
    float edgeGap;
    float charGap;
    uint32_t activeEdgeColor;
    uint32_t inactiveEdgeColor;
    byte activeEdgeBlend;
    byte inactiveEdgeBlend;
    bool showInactiveEdges;
} Led_t;

static const char *LedCharEdges[] =
{
    "\x03\x36\x02\x25\x58\x68", // '0'
    "\x25\x58", // '1'
    "\x02\x25\x35\x36\x68", // '2'
    "\x02\x25\x35\x58\x68", // '3'
    "\x03\x25\x35\x58", // '4'
    "\x02\x03\x35\x58\x68", // '5'
    "\x02\x03\x36\x68\x35\x58", // '6'
    "\x02\x25\x58", // '7'
    "\x02\x35\x68\x03\x36\x25\x58", // '8'
    "\x02\x03\x35\x25\x58\x68", // '9'
    "\x11\x77", // ':'
    "\x88", // '.'
};

static void LedBar (float scx, float scy, float scwidth, 
    float scheight, float thickness, uint32_t color, byte blend)
{
    assert(scx >= 0 && (unsigned) scx + scwidth <= screen.w
            && scy >= 0 && (unsigned) scy + scheight <= screen.h
            && "VL_BarScaledCoord: Destination rectangle out of bounds!");

    int i;
    vec3f_t v;
    vec3f_t pts[8];
    struct PolyPoint ConvexShape[8];

    pts[0] = vec3f(scx, scy + thickness, 1.0);
    pts[1] = vec3f(scx + thickness, scy, 1.0);
    pts[2] = vec3f(scx + scwidth - thickness, scy, 1.0);
    pts[3] = vec3f(scx + scwidth, scy + thickness, 1.0);
    pts[4] = vec3f(scx + scwidth, scy + scheight - thickness, 1.0);
    pts[5] = vec3f(scx + scwidth - thickness, scy + scheight, 1.0);
    pts[6] = vec3f(scx + thickness, scy + scheight, 1.0);
    pts[7] = vec3f(scx, scy + scheight - thickness, 1.0);

    for (i = 0; i < 8; i++)
    {
        v = Math_MatrixMultPoint(pts[i]);
        ConvexShape[i].X = (int)(X(v) + 0.5);
        ConvexShape[i].Y = (int)(Y(v) + 0.5);
    }

    FillConvexPolygonEx(8, ConvexShape, color, blend);
}

static void LedEdge(const Led_t *led, byte edge, bool active)
{
    byte blend;
    uint32_t color;
    float scx, scy, scwidth, scheight;

    scx = scy = scwidth = scheight = 0;

    switch (edge)
    {
    case 0x01:
        scx = led->scx + led->thickness + led->edgeGap;
        scwidth = led->scwidth - led->thickness - (led->edgeGap * 2);
        scy = led->scy;
        scheight = led->thickness * 2;
        break;
    case 0x02:
        scx = led->scx + led->thickness + led->edgeGap;
        scwidth = led->scwidth - ((led->thickness + led->edgeGap) * 2);
        scy = led->scy;
        scheight = led->thickness * 2;
        break;
    case 0x12:
        scx = led->scx + (led->scwidth / 2) + led->edgeGap;
        scwidth = led->scwidth - led->thickness - (led->edgeGap * 2);
        scy = led->scy;
        scheight = led->thickness * 2;
        break;
    case 0x34:
        scx = led->scx + led->thickness + led->edgeGap;
        scwidth = led->scwidth - led->thickness - (led->edgeGap * 2);
        scy = led->scy + (led->scheight / 2) - led->thickness;
        scheight = led->thickness * 2;
        break;
    case 0x35:
        scx = led->scx + led->thickness + led->edgeGap;
        scwidth = led->scwidth - ((led->thickness + led->edgeGap) * 2);
        scy = led->scy + (led->scheight / 2) - led->thickness;
        scheight = led->thickness * 2;
        break;
    case 0x45:
        scx = led->scx + (led->scwidth / 2) + led->edgeGap;
        scwidth = led->scwidth - led->thickness - (led->edgeGap * 2);
        scy = led->scy + (led->scheight / 2) - led->thickness;
        scheight = led->thickness * 2;
        break;
    case 0x67:
        scx = led->scx + led->thickness + led->edgeGap;
        scwidth = led->scwidth - led->thickness - (led->edgeGap * 2);
        scy = led->scy + led->scheight - (led->thickness * 2);
        scheight = led->thickness * 2;
        break;
    case 0x68:
        scx = led->scx + led->thickness + led->edgeGap;
        scwidth = led->scwidth - ((led->thickness + led->edgeGap) * 2);
        scy = led->scy + led->scheight - (led->thickness * 2);
        scheight = led->thickness * 2;
        break;
    case 0x78:
        scx = led->scx + (led->scwidth / 2) + led->edgeGap;
        scwidth = led->scwidth - led->thickness - (led->edgeGap * 2);
        scy = led->scy + led->scheight - (led->thickness * 2);
        scheight = led->thickness * 2;
        break;
    case 0x03:
        scx = led->scx;
        scwidth = led->thickness * 2;
        scy = led->scy + led->thickness + led->edgeGap;
        scheight = (led->scheight / 2) - led->thickness - 
            (led->edgeGap * 2);
        break;
    case 0x06:
        scx = led->scx;
        scwidth = led->thickness * 2;
        scy = led->scy + led->thickness + led->edgeGap;
        scheight = led->scheight - ((led->thickness + led->edgeGap) * 2);
        break;
    case 0x36:
        scx = led->scx;
        scwidth = led->thickness * 2;
        scy = led->scy + (led->scheight / 2) + led->edgeGap;
        scheight = (led->scheight / 2) - led->thickness - 
            (led->edgeGap * 2);
        break;
    case 0x14:
        scx = led->scx + (led->scwidth / 2) - led->thickness;
        scwidth = led->thickness * 2;
        scy = led->scy + led->thickness + led->edgeGap;
        scheight = led->scheight - led->thickness - (led->edgeGap * 2);
        break;
    case 0x17:
        scx = led->scx + (led->scwidth / 2) - led->thickness;
        scwidth = led->thickness * 2;
        scy = led->scy + led->thickness + led->edgeGap;
        scheight = led->scheight - ((led->thickness + led->edgeGap) * 2);
        break;
    case 0x47:
        scx = led->scx + (led->scwidth / 2) - led->thickness;
        scwidth = led->thickness * 2;
        scy = led->scy + (led->scheight / 2) + led->edgeGap;
        scheight = led->scheight - led->thickness - (led->edgeGap * 2);
        break;
    case 0x25:
        scx = led->scx + led->scwidth - (led->thickness * 2);
        scwidth = led->thickness * 2;
        scy = led->scy + led->thickness + led->edgeGap;
        scheight = (led->scheight / 2) - led->thickness - 
            (led->edgeGap * 2);
        break;
    case 0x28:
        scx = led->scx + led->scwidth - (led->thickness * 2);
        scwidth = led->thickness * 2;
        scy = led->scy + led->thickness + led->edgeGap;
        scheight = led->scheight - ((led->thickness + led->edgeGap) * 2);
        break;
    case 0x58:
        scx = led->scx + led->scwidth - (led->thickness * 2);
        scwidth = led->thickness * 2;
        scy = led->scy + (led->scheight / 2) + led->edgeGap;
        scheight = (led->scheight / 2) - led->thickness - 
            (led->edgeGap * 2);
        break;
    case 0x11:
        scx = led->scx + (led->scwidth / 2) - led->thickness;
        scwidth = led->thickness * 2;
        scy = ((led->scy + (led->scheight / 2)) + 
            led->thickness) / 2 - led->thickness;
        scheight = led->thickness * 2;
        break;
    case 0x77:
        scx = led->scx + (led->scwidth / 2) - led->thickness;
        scwidth = led->thickness * 2;
        scy = ((led->scy + (led->scheight / 2)) + 
            (led->scheight - led->thickness)) / 2 - led->thickness;
        scheight = led->thickness * 2;
        break;
    case 0x88:
        scx = led->scx + (led->scwidth / 2) - led->thickness;
        scwidth = led->thickness * 2;
        scy = (led->scy + led->scheight) - (led->thickness * 2);
        scheight = led->thickness * 2;
        break;
    }

    if (active)
    {
        color = led->activeEdgeColor;
        blend = led->activeEdgeBlend;
    }
    else
    {
        color = led->inactiveEdgeColor;
        blend = led->inactiveEdgeBlend;
    }
    LedBar (scx, scy, scwidth, scheight, 
        led->thickness, color, blend);
}

static void LedChar (const Led_t *led, char letter)
{
    int i;
    const char *edges;

    if (letter >= '0' && letter <= '9')
    {
        edges = LedCharEdges[letter - '0'];
    }
    else if (letter == ':')
    {
        edges = LedCharEdges[10];
    }
    else if (letter == '.')
    {
        edges = LedCharEdges[11];
    }

    if (led->showInactiveEdges)
    {
        const char *s = "\x02\x35\x68\x03\x36\x25\x58";
        for (i = 0; s[i] != '\0'; i++)
        {
            LedEdge(led, s[i], false);
        }
    }

    if (edges != NULL)
    {
        for (i = 0; edges[i] != '\0'; i++)
        {
            LedEdge(led, edges[i], true);
        }
    }
}

static void LedString(const Led_t *led, const char *str)
{
    int i;
    Led_t charLed;

    charLed = *led;
    for (i = 0; i < strlen(str); i++)
    {
        LedChar(&charLed, str[i]);
        charLed.scx += led->scwidth + led->charGap;
    }
}

void LedDraw(void)
{
    Led_t led =
    {
        0, // scx
        0, // scy
        1.0, // scwidth
        1.0, // scheight
        0.075, // thickness
        0.03, // edgeGap
        0.06, // charGap
        RGBA_GREEN, // activeEdgeColor
        RGBA_GREEN_DARKER,// inactiveEdgeColor
        0, // activeEdgeBlend
        0, // inactiveEdgeBlend
        true, // showInactiveEdges
    };
    int milli;
    int hundredths;
    int secs;
    char tmpStr[32];
    float hscale, vscale, tx, ty;

    if (gamestate.clockking_timer == 0)
    {
        return;
    }

    milli = TICS2MILLI(gamestate.clockking_timer);
    hundredths = (milli / 10) % 100;
    secs = milli / 1000;

    snprintf(tmpStr, sizeof(tmpStr), "%d:%02d.%02d", 
        secs / 60, (secs % 60), hundredths);

    hscale = 1.0 / 10;
    vscale = 3.0 / 10;
    tx = 0.15;
    ty = 0.05;
    if (secs >= 30)
    {
        hscale *= 0.5;
        vscale *= 0.5;
        tx = 0.3;
    }
    else if (secs >= 10 && secs < 30)
    {
        led.activeEdgeColor = RGBA_ORANGE;
        led.inactiveEdgeColor = RGBA_ORANGE_DARKER;
        hscale *= 0.7;
        vscale *= 0.7;
        tx = 0.2;
    }
    else if (secs < 10)
    {
        led.activeEdgeColor = RGBA_RED;
        led.inactiveEdgeColor = RGBA_RED_DARKER;
    }

    Math_PushMatrix();
    Math_TranslateMatrix(viewscreenx + tx * viewwidth, viewscreeny + ty * viewheight);
    Math_ScaleMatrix(hscale * viewwidth, vscale * viewheight);
    Math_ShearMatrix(-0.35, 0.0);
    
    LedString(&led, tmpStr);

    Math_PopMatrix();
}
#endif