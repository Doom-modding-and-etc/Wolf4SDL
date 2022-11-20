// ID_PM.C

#include "wl_def.h"

#ifndef SEGA_SATURN
word ChunksInFile;
#endif
word PMSpriteStart;
#ifndef SEGA_SATURN
word PMSoundStart;
bool PMSoundInfoPagePadded = false;
#endif
word *pageLengths;

byte *PMPageData;
byte **PMPages;
#ifdef SEGA_SATURN
byte* PM_DecodeSprites2(unsigned int start, unsigned int endi, uint32_t* pageOffsets, word* pageLengths, uint8_t* ptr, Sint32 fileId);
#endif

#ifdef SEGA_SATURN
void readChunks(Sint32 fileId, uint32_t size, uint32_t* pageOffsets, Uint8* Chunks, uint8_t* ptr)
{
    uint16_t delta = (uint16_t)(*pageOffsets / 2048);
    uint32_t delta2 = (*pageOffsets - delta * 2048);

    GFS_Load(fileId, delta, (void*)Chunks, size + delta2);

    memcpy(ptr, &Chunks[delta2], size);
}
#endif

/*
==================
=
= PM_Startup
=
==================
*/

void PM_Startup (void)
{
    int      i;
    int      padding;
    byte     *page;
    u32 *pageOffsets;
    u32 pagesize;
    s32 
#ifndef SEGA_SATURN
    filesize,
#endif
    datasize;
    FILE *file;
#if defined(SWITCH) || defined (N3DS) || defined (PS2) || defined (SEGA_SATURN)
    char fname[13 + sizeof(DATADIR)] = DATADIR "vswap.";
#else   
    char fname[13] = "vswap.";
#endif 
#ifdef SEGA_SATURN
    Uint8 Chunks;
    Sint32 fileId;
#endif

    strcat (fname,extension);

#ifdef SEGA_SATURN
    fileId = GFS_NameToId((Sint8*)fname);
#ifndef SEGA_SATURN
    filesize = GetFileSize(fileId);
#endif
    int ChunksInFile = 0;
    Chunks = (Uint8*)saturnChunk;
    //	CHECKMALLOCRESULT(Chunks);
    GFS_Load(fileId, 0, (void*)Chunks, 0x80);
    ChunksInFile = Chunks[0] | Chunks[1] << 8;
    //slPrintHex(ChunksInFile, slLocate(10, 18));
#endif
    file = fopen(fname,"rb");

    if (!file)
        CA_CannotOpen(fname);


    //
    // read in header variables
    //
    fread (&ChunksInFile,sizeof(ChunksInFile),1,file);
#ifdef SEGA_SATURN
    PMSpriteStart = Chunks[2] | Chunks[3] << 8;
    PMSoundStart = Chunks[4] | Chunks[5] << 8;
#else
    fread (&PMSpriteStart,sizeof(PMSpriteStart),1,file);
    fread (&PMSoundStart,sizeof(PMSoundStart),1,file);
#endif

#ifdef SEGA_SATURN
    // vbt : on ne charge pas les sons !	
    ChunksInFile = PMSoundStart;

    PMPages = (uint8_t**)SafeMalloc((ChunksInFile + 1) * sizeof(uint8_t*));
    SafeMalloc(PMPages);
#endif
   
#ifdef SEGA_SATURN
    //
    // read in the chunk offsets
    //
    pageOffsets = (uint32_t*)saturnChunk;
#else
    pageOffsets = SafeMalloc((ChunksInFile + 1) * sizeof(*pageOffsets));
#endif
#ifndef SEGA_SATURN
    fread (pageOffsets,sizeof(*pageOffsets),ChunksInFile,file);
#endif

    //
    // read in the chunk lengths
    //

#ifdef SEGA_SATURN
    pageLengths = (word*)saturnChunk + (ChunksInFile + 1) * sizeof(int32_t);
#else
    pageLengths = SafeMalloc(ChunksInFile * sizeof(*pageLengths));
#endif
#ifndef SEGA_SATURN
    fread (pageLengths,sizeof(*pageLengths),ChunksInFile,file);
#endif
/*

*/

#ifndef SEGA_SATURN
    fseek (file,0,SEEK_END);
    filesize = ftell(file);
    datasize = filesize - pageOffsets[0];

    if (datasize < 0)
        Quit ("PM_Startup: The page file \"%s\" is too large!",fname);
#else
    long pageDataSize = fileSize - pageOffsets[0];
    if (pageDataSize > (size_t) -1)
        Quit("The page file \"%s\" is too large!", fname);
#endif
    pageOffsets[ChunksInFile] = filesize;

#ifndef SEGA_SATURN
    //
    // check that all chunk offsets are valid
    //
    for (i = 0; i < ChunksInFile; i++)
    {
        if (!pageOffsets[i])
            continue;           // sparse page

        if (pageOffsets[i] < pageOffsets[0] || pageOffsets[i] >= filesize)
            Quit ("PM_Startup: Illegal page offset for page %i: %u (filesize: %u)",i,pageOffsets[i],filesize);
    }

    //
    // calculate total amount of padding needed for sprites and sound info page
    //
    padding = 0;

    for (i = PMSpriteStart; i < PMSoundStart; i++)
    {
        if (!pageOffsets[i])
            continue;           // sparse page

        if (((pageOffsets[i] - pageOffsets[0]) + padding) & 1)
            padding++;
    }

    if (((pageOffsets[ChunksInFile - 1] - pageOffsets[0]) + padding) & 1)
        padding++;

    //
    // allocate enough memory to hold the whole page file
    //
    PMPageData = SafeMalloc(datasize + padding);

    //
    // [ChunksInFile + 1] pointers to page starts
    // the last pointer points one byte after the last page
    //
    PMPages = SafeMalloc((ChunksInFile + 1) * sizeof(*PMPages));

    //
    // load pages and initialize PMPages pointers
    //
    page = &PMPageData[0];

    for (i = 0; i < ChunksInFile; i++)
    {
        if ((i >= PMSpriteStart && i < PMSoundStart) || i == ChunksInFile - 1)
        {
            //
            // pad with zeros to make it 2-byte aligned
            //
            if ((page - &PMPageData[0]) & 1)
            {
                *page++ = 0;

                if (i == ChunksInFile - 1)
                    PMSoundInfoPagePadded = true;
            }
        }

        PMPages[i] = page;

        if (!pageOffsets[i])
            continue;               // sparse page

        //
        // use specified page length when next page is sparse
        // otherwise, calculate size from the offset difference between this and the next page
        //
        if (!pageOffsets[i + 1])
            pagesize = pageLengths[i];
        else
            pagesize = pageOffsets[i + 1] - pageOffsets[i];

        fseek (file,pageOffsets[i],SEEK_SET);
        fread (page,sizeof(*page),pagesize,file);

        page += pagesize;
    }
#endif
    //
    // last page points after page buffer
    //
    PMPages[ChunksInFile] = page;
#ifndef SEGA_SATURN
    free (pageOffsets);
    fclose (file);
#else
    pageLengths = NULL;
    pageOffsets = NULL;
    Chunks = NULL;
#endif
}

#ifdef SEGA_SATURN
byte* PM_DecodeSprites2(unsigned int start, unsigned int endi, uint32_t* pageOffsets, word* pageLengths, uint8_t* ptr, Sint32 fileId)
{
    uint8_t* Chunks = (uint8_t*)saturnChunk + 0x9000;
    uint8_t* bmpbuff = (uint8_t*)saturnChunk + 0xA000;	//0x00202000;
    uint32_t size;

    for (unsigned int i = start; i < endi; i++)
    {
        PMPages[i] = ptr;

        if (!pageOffsets[i])
            continue;               // sparse page

        // Use specified page length, when next page is sparse page.
        // Otherwise, calculate size from the offset difference between this and the next page.

        if (!pageOffsets[i + 1]) size = pageLengths[i - PMSpriteStart];
        else size = pageOffsets[i + 1] - pageOffsets[i];

        if (!size)
            continue;

        int end = size;
        if (size % 4 != 0)
        {
            end = ((size + (4 - 1)) & -4);
        }
        //		memcpy(ptr,&Chunks[pageOffsets[i]],size);
        readChunks(fileId, size, &pageOffsets[i], Chunks, ptr);

        memset(&ptr[size], 0x00, end - size);

        t_compshape* shape = (t_compshape*)ptr;
        shape->leftpix = SWAP_BYTES_16(shape->leftpix);
        shape->rightpix = SWAP_BYTES_16(shape->rightpix);

        byte* bmpptr, * sprdata8;
        unsigned short* cmdptr;

        // set the texel index to the first texel
        //unsigned char* sprptr = (unsigned char*)shape + (((((shape->rightpix) - (shape->leftpix)) + 1) * 2) + 4);
        // clear the buffers

        // setup a pointer to the column offsets	
        cmdptr = shape->dataofs;
        int count_00 = 63;
        //		int count_01=0;

        for (int x = 0; x <= (shape->rightpix - shape->leftpix); x++)
        {
            sprdata8 = ((unsigned char*)shape + *cmdptr);
            shape->dataofs[x] = SWAP_BYTES_16(shape->dataofs[x]);
            while ((sprdata8[0] | sprdata8[1] << 8) != 0)
            {
                for (int y = (sprdata8[4] | sprdata8[5] << 8) / 2; y < (sprdata8[0] | sprdata8[1] << 8) / 2; y++)
                {
                    unsigned int min_y = (sprdata8[4] | sprdata8[5] << 8) / 2;
                    if (min_y < count_00)
                        count_00 = min_y;

                    //					if(min_y>count_01)
                    //						count_01=min_y;					
                }
                sprdata8 += 6;
            }
            cmdptr++;
        }
        memset(bmpbuff, 0x00, (64 - count_00) << 6);

       unsigned char *sprptr = (unsigned char*)shape + (((((shape->rightpix) - (shape->leftpix)) + 1) * 2) + 4);

        cmdptr = shape->dataofs;

        for (int x = (shape->leftpix); x <= (shape->rightpix); x++)
        {
            sprdata8 = ((unsigned char*)shape + *cmdptr);
            bmpptr = (byte*)bmpbuff + x;

            while ((sprdata8[0] | sprdata8[1] << 8) != 0)
            {
                for (int y = (sprdata8[4] | sprdata8[5] << 8) / 2; y < (sprdata8[0] | sprdata8[1] << 8) / 2; y++)
                    //				for (int y = (sprdata8[4]|sprdata8[5]<<8)/2; y < count_01; y++)
                {
                    bmpptr[(y - count_00) << 6] = *sprptr++;
                    if (bmpptr[(y - count_00) << 6] == 0) bmpptr[(y - count_00) << 6] = 0xa0;
                }
                sprdata8 += 6;
            }
            cmdptr++;
        }
        memcpyl((void*)ptr, bmpbuff, (64 - count_00) << 6);
        ptr += ((64 - count_00) << 6);
    }
    return ptr;
}
#endif

#ifndef SEGA_SATURN
/*
==================
=
= PM_Shutdown
=
==================
*/

void PM_Shutdown (void)
{
    free (pageLengths);
    free (PMPages);
    free (PMPageData);

    pageLengths = NULL;
    PMPages = NULL;
    PMPageData = NULL;
}


/*
==================
=
= PM_GetPageSize
=
==================
*/

uint32_t PM_GetPageSize (int page)
{
    if (page < 0 || page >= ChunksInFile)
        Quit ("PM_GetPageSize: Invalid page request: %i",page);

    return (uint32_t)(PMPages[page + 1] - PMPages[page]);
}
#endif

/*
==================
=
= PM_GetPage
=
= Returns the address of the page
=
==================
*/

byte *PM_GetPage (int page)
{
#ifndef SEGA_SATURN
    if (page < 0 || page >= ChunksInFile)
        Quit ("PM_GetPage: Invalid page request: %i",page);
#endif
    return PMPages[page];
}


/*
==================
=
= PM_GetPageEnd
=
= Returns the address of the last page
=
==================
*/
#ifndef SEGA_SATURN
byte *PM_GetPageEnd (void)
{
    return PMPages[ChunksInFile];
}
#endif

#ifdef SEGA_SATURN
static inline byte* PM_GetTexture(int wallpic)
{
    return PM_GetPage(wallpic);
}

static inline uint16_t* PM_GetSprite(int shapenum)
{
    // correct alignment is enforced by PM_Startup()
    return (uint16_t*)(void*)PM_GetPage(PMSpriteStart + shapenum);
}
#endif
