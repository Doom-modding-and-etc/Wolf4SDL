// WL_HEALTHMETER.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_HEALTHBAR)

#define HEALTHMETER_MAXBARS 4
#define HEALTHMETER_XOFFSET 2
#define HEALTHMETER_YOFFSET 35
#define HEALTHMETER_WIDTH 26
#define HEALTHMETER_FACEHEIGHT 20
#define HEALTHMETER_BORDERWIDTH 1
#define HEALTHMETER_BORDERLINEWIDTH 1
#define HEALTHMETER_XSEP 2
#define HEALTHMETER_FLASHPERIOD 8

typedef struct HealthMeter_BarLook_s
{
    int xoffset;
    int yoffset;
    int xsep;
    int length;
    int width;
    int faceWidth;
    int faceHeight;
    int borderWidth;
    int borderLineWidth;
    uint32_t fullHealthColor;
    uint32_t mediumHealthColor;
    uint32_t lowHealthColor;
    uint32_t backgroundColor;
    uint32_t flashingColor;
} HealthMeter_BarLook_t;

typedef struct HealthMeter_Bar_s
{
    bool used;
    short full_hitpoints;
    int objindex;
    bool flashing;
    int32_t flashtics;
    int shapenum;
    int cropx, cropy;
    int cropw, croph;
    byte *facepic;
} HealthMeter_Bar_t;

typedef struct HealthMeter_s
{
    HealthMeter_Bar_t bars[HEALTHMETER_MAXBARS];
} HealthMeter_t;

static HealthMeter_t HealthMeter;

static const HealthMeter_BarLook_t HealthMeter_BarLook =
{
    HEALTHMETER_XOFFSET, // xoffset
    HEALTHMETER_YOFFSET, // yoffset
    HEALTHMETER_XSEP, // xsep
    (MaxY * 1 / 2), // length
    HEALTHMETER_WIDTH, // width
    HEALTHMETER_WIDTH - (((HEALTHMETER_BORDERLINEWIDTH * 2) + HEALTHMETER_BORDERWIDTH) * 2), // faceWidth
    HEALTHMETER_FACEHEIGHT, // faceHeight
    HEALTHMETER_BORDERWIDTH, // borderWidth
    HEALTHMETER_BORDERLINEWIDTH, // borderLineWidth
    RGBA_GREEN, // fullHealthColor
    RGBA_ORANGE, // mediumHealthColor
    RGBA_RED, // lowHealthColor
    RGBA_BLACK, // backgroundColor
    RGBA_GOLD, // flashingColor
};

extern VBuf_t vbuf;

static void HealthMeter_DrawRect(int x, int y, int width, int height,
    uint32_t color, byte blend)
{
    VBuf_t bufp;
    int scx, scy, scwidth, scheight;

    scx = x * scaleFactor;
    scy = y * scaleFactor;
    scwidth = width * scaleFactor;
    scheight = height * scaleFactor;

    bufp = VBufOffset(vbuf, scy * screen.pitch + scx);

    VBufSetBlendFast(blend);

    while (scheight--)
    {
        VBuf_t row = bufp;
        if (blend != 0)
        {
            for (x = 0; x < scwidth; x++)
            {
                VBufWriteBlendColorFast(bufp, 0, color);
                bufp = VBufOffset(bufp, 1);
            }
        }
        else
        {
            for (x = 0; x < scwidth; x++)
            {
                VBufWriteColor(bufp, 0, color);
                bufp = VBufOffset(bufp, 1);
            }
        }
        bufp = VBufOffset(row, screen.pitch);
    }
}

static void HealthMeter_DrawImage(int x, int y, int width, int height,
    uint32_t *pic)
{
    VBuf_t bufp;
    int scx, scy, scwidth, scheight;

    scx = x * scaleFactor;
    scy = y * scaleFactor;
    scwidth = width * scaleFactor;
    scheight = height * scaleFactor;

    bufp = VBufOffset(vbuf, scy * screen.pitch + scx);

    while (scheight--)
    {
        memcpy(bufp.pix, pic, scwidth * sizeof(uint32_t));
        pic += scwidth;
        bufp = VBufOffset(bufp, screen.pitch);
    }
}

static void HealthMeter_ShapeToImage(int shapenum, byte *image)
{
    int x, y, i;
    word *cmdptr;
    t_compshape *shape;
    byte *sprite;
    short *linecmds;

    memset(image, 0, TEXTURESIZE * TEXTURESIZE);

    shape = (t_compshape *)PM_GetSprite(shapenum);
    sprite = (byte *)shape;

    cmdptr = shape->dataofs;
    for (x = shape->leftpix; x <= shape->rightpix; x++)
    {
        linecmds = (short *)(sprite + *cmdptr++);
        for (; *linecmds; linecmds += 3)
        {
            i = linecmds[2] / 2 + linecmds[1];
            for (y = linecmds[2] / 2; y < linecmds[0] / 2; y++, i++)
            {
                image[y * 64 + x] = sprite[i];
            }
        }
    }
}

static void HealthMeter_InitBarFacePic(int barId)
{
    int index;
    int x, y;
    int k;
    int fx, fy;
    int size;
    uint32_t *faceptr;
    byte col;
    byte shapeImage[TEXTURESIZE * TEXTURESIZE];
    const HealthMeter_BarLook_t *bl;
    HealthMeter_Bar_t *bar;
    fixed u, v, du, dv;
    int scwidth, scheight;

    assert(barId != 0);
    index = barId - 1;
    bar = &HealthMeter.bars[index];
    bl = &HealthMeter_BarLook;

    if (bar->facepic != NULL)
    {
        return;
    }

    HealthMeter_ShapeToImage(bar->shapenum, shapeImage);

    scwidth = bl->faceWidth * scaleFactor;
    scheight = bl->faceHeight * scaleFactor;
    size = scwidth * scheight * sizeof(uint32_t);
    bar->facepic = (byte *)malloc(size);
    CHECKMALLOCRESULT(bar->facepic);
    memset(bar->facepic, 0, size);

    du = fixedpt_fromint(bar->cropw) / scwidth;
    dv = fixedpt_fromint(bar->croph) / scheight;

    faceptr = (uint32_t *)bar->facepic;

    v = fixedpt_fromint(bar->cropy);
    for (fy = 0; fy < scheight; fy++)
    {
        y = fixedpt_toint(v);
        u = fixedpt_fromint(bar->cropx);
        for (fx = 0; fx < scwidth; fx++)
        {
            x = fixedpt_toint(u);
            col = shapeImage[(y << TEXTURESHIFT) + x];
            *faceptr = RGBA_FROMPAL(gamepal, col);
            faceptr++;
            u += du;
        }
        v += dv;
    }
}

int HealthMeter_NewBar(void)
{
    int i;
    HealthMeter_Bar_t *bar;

    for (i = 0; i < HEALTHMETER_MAXBARS; i++)
    {
        if (!HealthMeter.bars[i].used)
        {
            break;
        }
    }

    if (i == HEALTHMETER_MAXBARS)
    {
        return 0;
    }

    bar = &HealthMeter.bars[i];
    memset(bar, 0, sizeof(HealthMeter_Bar_t));
    bar->used = true;

    return i + 1;
}

void HealthMeter_DeleteBar(int barId)
{
    int index;
    HealthMeter_Bar_t *bar;

    if (barId == 0)
    {
        return;
    }
    index = barId - 1;

    bar = &HealthMeter.bars[index];
    free(bar->facepic);
    memset(bar, 0, sizeof(HealthMeter_Bar_t));
}

void HealthMeter_SetBarFlashing(int barId, boolean flashing)
{
    int index;
    HealthMeter_Bar_t *bar;

    assert(barId != 0);
    index = barId - 1;

    bar = &HealthMeter.bars[index];
    bar->flashing = flashing;
    bar->flashtics = 0;
}

void HealthMeter_Reset(void)
{
    int i;
    HealthMeter_Bar_t *bar;

    for (i = 0; i < HEALTHMETER_MAXBARS; i++)
    {
        bar = &HealthMeter.bars[i];
        if (bar->used)
        {
            HealthMeter_DeleteBar(i + 1);
        }
    }
}

static void HealthMeter_DrawBar(int barId)
{
    byte blend;
    int x, y;
    int hp, yused;
    int index;
    HealthMeter_Bar_t *bar;
    const HealthMeter_BarLook_t *bl;
    int fullBorderWidth;
    uint32_t color;
    objtype *ob;

    assert(barId != 0);
    index = barId - 1;

    bar = &HealthMeter.bars[index];
    bl = &HealthMeter_BarLook;

    const int blx[HEALTHMETER_MAXBARS] =
    {
        bl->xoffset,
        (viewwidth / scaleFactor) - bl->xoffset - bl->width,
        bl->xoffset + bl->width + bl->xsep,
        (viewwidth / scaleFactor) - bl->xoffset - (bl->width * 2) - bl->xsep,
    };

    x = blx[index];
    y = bl->yoffset;

    color = RGBA_FROMPAL(gamepal, bordercol);
    // left border
    HealthMeter_DrawRect(x + bl->borderLineWidth,
        y + bl->borderLineWidth, bl->borderWidth,
        bl->length - (bl->borderLineWidth * 2), color, 0);
    // right border
    HealthMeter_DrawRect(x + bl->width - 
        bl->borderLineWidth - bl->borderWidth,
        y + bl->borderLineWidth, bl->borderWidth,
        bl->length - (bl->borderLineWidth * 2), color, 0);
    // top border
    HealthMeter_DrawRect(x + bl->borderLineWidth,
        y + bl->borderLineWidth, 
        bl->width - (bl->borderLineWidth * 2),
        bl->borderWidth, color, 0);
    // bottom border
    HealthMeter_DrawRect(x + bl->borderLineWidth,
        y + bl->length - 
        bl->borderLineWidth - bl->borderWidth, 
        bl->width - (bl->borderLineWidth * 2),
        bl->borderWidth, color, 0);

    color = RGBA_FROMPAL(gamepal, bordercol - 2);
    // left outer border line
    HealthMeter_DrawRect(x, y, bl->borderLineWidth,
        bl->length, color, 0);
    // right inner border line
    HealthMeter_DrawRect(x + bl->width - 
        (bl->borderLineWidth * 2) - bl->borderWidth,
        y + bl->borderLineWidth + bl->borderWidth, 
        bl->borderLineWidth, 
        bl->length - (bl->borderLineWidth + bl->borderWidth) * 2, 
        color, 0);
    // top outer border line
    HealthMeter_DrawRect(x, y, bl->width, bl->borderLineWidth, 
        color, 0);
    // bottom inner border line
    HealthMeter_DrawRect(x + bl->borderLineWidth + 
        bl->borderWidth,
        y + bl->length - 
        (bl->borderLineWidth * 2) - bl->borderWidth, 
        bl->width - (bl->borderLineWidth + bl->borderWidth) * 2,
        bl->borderLineWidth,
        color, 0);

    color = RGBA_FROMPAL(gamepal, 0);
    // right outer border line
    HealthMeter_DrawRect(x + bl->width - 
        bl->borderLineWidth,
        y, bl->borderLineWidth,
        bl->length, color, 0);
    // left inner border line
    HealthMeter_DrawRect(x + bl->borderLineWidth +
        bl->borderWidth,
        y + bl->borderLineWidth + bl->borderWidth, 
        bl->borderLineWidth, 
        bl->length - (bl->borderLineWidth + bl->borderWidth) * 2, 
        color, 0);
    // bottom outer border line
    HealthMeter_DrawRect(x,
        y + bl->length - bl->borderLineWidth,
        bl->width, bl->borderLineWidth, 
        color, 0);
    // top inner border line
    HealthMeter_DrawRect(x + bl->borderLineWidth + 
        bl->borderWidth,
        y + bl->borderLineWidth + bl->borderWidth, 
        bl->width - (bl->borderLineWidth + bl->borderWidth) * 2,
        bl->borderLineWidth,
        color, 0);

    color = RGBA_FROMPAL(gamepal, bordercol - 3);
    // bottom left inner highlight
    HealthMeter_DrawRect(x + bl->borderWidth + 
        bl->borderLineWidth,
        y + bl->length - 
        (bl->borderLineWidth * 2) - bl->borderWidth, 
        bl->borderLineWidth, bl->borderLineWidth,
        color, 0);
    // top right outer highlight
    HealthMeter_DrawRect(x + bl->width - bl->borderLineWidth, y, 
        bl->borderLineWidth, bl->borderLineWidth,
        color, 0);

    fullBorderWidth = (bl->borderLineWidth * 2) + bl->borderWidth;

    ob = objlist + bar->objindex;
    hp = CLIP(ob->hitpoints, 0, ob->starthitpoints);
    yused = (ob->starthitpoints - hp) * 
        (bl->length - (fullBorderWidth * 3) - bl->faceHeight) / ob->starthitpoints;

    if (hp >= ob->starthitpoints * 5 / 8)
    {
        blend = (hp - (ob->starthitpoints * 5 / 8)) * 
            255 / ((ob->starthitpoints * 3 / 8) + 1);
        color = Math_FixedLerpRgba(bl->mediumHealthColor, 
            bl->fullHealthColor, blend);
    }
    else
    {
        blend = hp * 255 / ((ob->starthitpoints * 5 / 8) + 1);
        color = Math_FixedLerpRgba(bl->lowHealthColor, 
            bl->mediumHealthColor, blend);
    }

    if (bar->flashing)
    {
        bar->flashtics += tics;
        if (bar->flashtics > HEALTHMETER_FLASHPERIOD * 2)
        {
            bar->flashtics = 0;
        }
        else if (bar->flashtics > HEALTHMETER_FLASHPERIOD)
        {
            color = bl->flashingColor;
        }
    }

    // health bar
    HealthMeter_DrawRect(x + fullBorderWidth,
        y + (fullBorderWidth * 2) + bl->faceHeight + yused, 
        bl->width - (fullBorderWidth * 2),
        bl->length - ((fullBorderWidth * 3) + bl->faceHeight) - yused,
        color, 128);

    // bar background
    HealthMeter_DrawRect(x + fullBorderWidth,
        y + (fullBorderWidth * 2) + bl->faceHeight, 
        bl->width - (fullBorderWidth * 2),
        yused,
        bl->backgroundColor, 128);

    color = RGBA_FROMPAL(gamepal, bordercol);
    // face separator border
    HealthMeter_DrawRect(x + 
        bl->borderLineWidth + bl->borderWidth,
        y + bl->faceHeight + 
        fullBorderWidth + bl->borderLineWidth, 
        bl->width - ((bl->borderWidth + bl->borderLineWidth) * 2),
        bl->borderWidth, color, 0);

    color = RGBA_FROMPAL(gamepal, 0);
    // bottom face separator border line
    HealthMeter_DrawRect(x + bl->borderLineWidth + 
        bl->borderWidth,
        y + bl->faceHeight + (fullBorderWidth * 2) -
        bl->borderLineWidth, 
        bl->width - (bl->borderLineWidth + bl->borderWidth) * 2,
        bl->borderLineWidth,
        color, 0);

    color = RGBA_FROMPAL(gamepal, bordercol - 2);
    // top face separator border line
    HealthMeter_DrawRect(x + bl->borderLineWidth + 
        bl->borderWidth,
        y + bl->faceHeight + fullBorderWidth, 
        bl->width - (bl->borderLineWidth + bl->borderWidth) * 2,
        bl->borderLineWidth,
        color, 0);

    color = RGBA_FROMPAL(gamepal, bordercol - 3);
    // face separator inner highlight
    HealthMeter_DrawRect(x + fullBorderWidth -
        bl->borderLineWidth,
        y + fullBorderWidth + bl->faceHeight, 
        bl->borderLineWidth, bl->borderLineWidth,
        color, 0);

    // face
    HealthMeter_InitBarFacePic(barId);
    HealthMeter_DrawImage(x + fullBorderWidth, y + fullBorderWidth, 
        bl->faceWidth, bl->faceHeight, (uint32_t *)bar->facepic);
}

void HealthMeter_Draw(void)
{
    int i;
    HealthMeter_Bar_t *bar;

    for (i = 0; i < HEALTHMETER_MAXBARS; i++)
    {
        bar = &HealthMeter.bars[i];
        if (bar->used)
        {
            HealthMeter_DrawBar(i + 1);
        }
    }
}

HealthMeter_Bar_t *HealthMeter_GetBar(int barId)
{
    int index;

    if (barId == 0)
    {
        return NULL;
    }
    index = barId - 1;

    return &HealthMeter.bars[index];
}

int HealthMeter_NewBarEnemy(objtype *ob, int shapenum, int cropx,
    int cropy, int cropw, int croph)
{
    int barId;
    HealthMeter_Bar_t *bar;

    barId = HealthMeter_NewBar();
    bar = HealthMeter_GetBar(barId);
    if (bar != NULL)
    {
        bar->full_hitpoints = ob->starthitpoints;
        bar->objindex = (int)(ob - objlist);
        bar->shapenum = shapenum;
        bar->cropx = cropx;
        bar->cropy = cropy;
        bar->cropw = cropw;
        bar->croph = croph;
    }

    return barId;
}

int32_t HealthMeter_SaveTheGame(FILE *file, int32_t checksum)
{
    SAVE_FIELD(HealthMeter);
    return checksum;
}

int32_t HealthMeter_LoadTheGame(FILE *file, int32_t checksum)
{
    int i;

    LOAD_FIELD(HealthMeter);

    for (i = 0; i < HEALTHMETER_MAXBARS; i++)
    {
        HealthMeter.bars[i].facepic = NULL;
    }

    return checksum;
}
#endif