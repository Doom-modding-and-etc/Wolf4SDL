// ID_CA.C

// this has been customized for WOLF

/*
=============================================================================

Id Software Caching Manager
---------------------------

Must be started BEFORE the memory manager, because it needs to get the headers
loaded into the data segment

=============================================================================
*/
#ifndef SEGA_SATURN
#include <sys/types.h>
#if defined _WIN32
    #include <io.h>
#elif defined(SWITCH)
	#include <sys/_iovec.h>
#elif defined(N3DS) || defined(PS2) 
struct iovec 
{
     void *iov_base;     
     size_t iov_len;    
};
#elif !defined(_arch_dreamcast)
    #include <sys/uio.h>
    #include <unistd.h>
#else	
    #include <sys/uio.h>
    #include <unistd.h>
#endif
#endif

#include "wl_def.h"

#ifdef SEGA_SATURN
void readChunks(Sint32 fileId, uint32_t size, uint32_t* pageOffsets, Uint8* Chunks, uint8_t* ptr);
#define BUFFERSIZE  0x800
#endif

#define THREEBYTEGRSTARTS

#ifdef SEGA_SATURN
//#define HEAP_WALK 1
#endif

#if defined(HEAP_WALK) && defined(SEGA_SATURN)
extern Uint32  end;
extern Uint32  __malloc_free_list;
extern Uint32  _sbrk(int size);

void heapWalk(void)
{
    Uint32 chunkNumber = 1;
    // The __end__ linker symbol points to the beginning of the heap.
    Uint32 chunkCurr = (Uint32)&end;
    // __malloc_free_list is the head pointer to newlib-nano's link list of free chunks.
    Uint32 freeCurr = __malloc_free_list;
    // Calling _sbrk() with 0 reserves no more memory but it returns the current top of heap.
    Uint32 heapEnd = _sbrk(0);

    //    printf("Heap Size: %lu\n", heapEnd - chunkCurr);
    char msg[100];
    sprintf(msg, "Heap Size: %d  e%08x s%08x                  ", heapEnd - chunkCurr, heapEnd, chunkCurr);
    //	FNT_Print256_2bpp((volatile Uint8 *)SS_FONT,(Uint8 *)toto,12,216);
    slPrint((char*)msg, slLocate(1, 3));

    // Walk through the chunks until we hit the end of the heap.
    while (chunkCurr < heapEnd)
    {
        // Assume the chunk is in use.  Will update later.
        int      isChunkFree = 0;
        // The first 32-bit word in a chunk is the size of the allocation.  newlib-nano over allocates by 8 bytes.
        // 4 bytes for this 32-bit chunk size and another 4 bytes to allow for 8 byte-alignment of returned pointer.
        Uint32 chunkSize = *(Uint32*)chunkCurr;
        // The start of the next chunk is right after the end of this one.
        Uint32 chunkNext = chunkCurr + chunkSize;

        // The free list is sorted by address.
        // Check to see if we have found the next free chunk in the heap.
        if (chunkCurr == freeCurr)
        {
            // Chunk is free so flag it as such.
            isChunkFree = 1;
            // The second 32-bit word in a free chunk is a pointer to the next free chunk (again sorted by address).
            freeCurr = *(Uint32*)(freeCurr + 4);
        }

        // Skip past the 32-bit size field in the chunk header.
        chunkCurr += 4;
        // 8-byte align the data pointer.
        chunkCurr = (chunkCurr + 7) & ~7;
        // newlib-nano over allocates by 8 bytes, 4 bytes for the 32-bit chunk size and another 4 bytes to allow for 8
        // byte-alignment of the returned pointer.
        chunkSize -= 8;
        //        printf("Chunk: %lu  Address: 0xlX  Size: %lu  %s\n", chunkNumber, chunkCurr, chunkSize, isChunkFree ? "CHUNK FREE" : "");

        sprintf(msg, "%d A%04x  S%04d %s", chunkNumber, chunkCurr, chunkSize, isChunkFree ? "CHUNK FREE" : "");
        if (chunkNumber < 20)
            slPrint((char*)msg, slLocate(0, chunkNumber * 10 + 10));
        //	if(chunkNumber>=200)
        //	slPrint((char *)msg,slLocate(0,chunkNumber-200));
        //	if(chunkNumber>=230)
        //	slPrint((char *)msg,slLocate(20,chunkNumber-230));

        chunkCurr = chunkNext;
        chunkNumber++;
    }
}
#endif //To be continued...

/*
=============================================================================
                             LOCAL CONSTANTS
=============================================================================
*/

typedef struct
{
    unsigned short bit0,bit1;       // 0-255 is a character, > is a pointer to a node
} huffnode;


typedef struct
{
    unsigned short RLEWtag;
#if MAPPLANES >= 4
    unsigned short numplanes;       // unused, but WDC needs 2 bytes here for internal usage
#endif
    int32_t headeroffsets[NUMMAPS];
} mapfiletype;


/*
=============================================================================
                             GLOBAL VARIABLES
=============================================================================
*/

unsigned short *mapsegs[MAPPLANES];
#if defined(EMBEDDED) && defined(SEGA_SATURN)
static maptype mapheaderseg;
#else
maptype *mapheaderseg[NUMMAPS];
#endif

#if !defined(VIEASM) || !defined(SEGA_SATURN)
unsigned char *audiosegs[NUMSNDCHUNKS];
#endif
unsigned char *grsegs[NUMCHUNKS];

mapfiletype *tinf;

/*
=============================================================================
                             LOCAL VARIABLES
=============================================================================
*/

char extension[5]; // Need a string, not constant to change cache files
#ifndef SEGA_SATURN
char graphext[5];
char audioext[5];
#endif

#if defined(SWITCH) || defined(SEGA_SATURN) || defined(N3DS) || defined(PS2)
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

#ifndef SEGA_SATURN
void CA_CannotOpen(const char *string);
#endif 
static int32_t  grstarts[NUMCHUNKS + 1];

#if !defined(VIEASM) || !defined(SEGA_SATURN)
static int32_t* audiostarts; // array of offsets in audio / audiot
#endif

#ifdef GRHEADERLINKED
huffnode *grhuffman;
#else
huffnode grhuffman[255];
#endif

int    grhandle = -1;               // handle to EGAGRAPH
int    maphandle = -1;              // handle to MAPTEMP / GAMEMAPS

#if !defined(VIEASM) || !defined(SEGA_SATURN)
int    audiohandle = -1;            // handle to AUDIOT / AUDIO
#endif
int32_t   chunkcomplen,chunkexplen;

#ifndef VIEASM
SDMode oldsoundmode;
#endif


static int32_t GRFILEPOS(const size_t idx)
{
#ifdef SEGA_SATURN
    #define assert8(x) if(!(x)) { slPrint((char *)"asset test failed8", slLocate(10,20));return;}
#else
	assert(idx < lengthof(grstarts));
#endif
    return grstarts[idx];
}

/*
=============================================================================
                            LOW LEVEL ROUTINES
=============================================================================
*/

/*
============================
=
= CAL_GetGrChunkLength
=
= Gets the length of an explicit length chunk (not tiles)
= The file pointer is positioned so the compressed data can be read in next.
=
============================
*/

void CAL_GetGrChunkLength (int chunk)
{
#ifdef SEGA_SATURN
    uint32_t pos = GRFILEPOS(chunk);
    readChunks(grhandle, sizeof(chunkexplen), &pos, saturnChunk, (uint8_t*)&chunkexplen);
#else
    w3slseek(grhandle, GRFILEPOS(chunk), SEEK_SET);
    w3sread(grhandle, &chunkexplen, sizeof(chunkexplen));
#endif
    chunkcomplen = GRFILEPOS(chunk+1)-GRFILEPOS(chunk)-4;
}
#ifndef SEGA_SATURN
/*
==========================
=
= CA_WriteFile
=
= Writes a file from a memory buffer
=
==========================
*/

boolean CA_WriteFile (const char *filename, void *ptr, int32_t length)
{
    const int handle = w3sopen(filename, O_CREAT | O_WRONLY | O_BINARY, 0644);
    if (handle == -1)
        return false;

    if (!w3swrite (handle,ptr,length))
    {
        w3sclose (handle);
        return false;
    }
    w3sclose (handle);
    return true;
}



/*
==========================
=
= CA_LoadFile
=
= Allocate space for and load a file
=
==========================
*/

boolean CA_LoadFile (const char *filename, void **ptr)
{
    int32_t size;

    const int handle = w3sopen(filename, O_RDONLY | O_BINARY);
    if (handle == -1)
        return false;

    size = w3slseek(handle, 0, SEEK_END);
    w3slseek(handle, 0, SEEK_SET);
    *ptr = SafeMalloc(size);

    if (!w3sread (handle,*ptr,size))
    {
        w3sclose (handle);
        return false;
    }
    w3sclose (handle);
    return true;
}
#endif
/*
============================================================================
                COMPRESSION routines, see JHUFF.C for more
============================================================================
*/

static void CAL_HuffExpand(unsigned char *source, unsigned char *dest, int32_t length, huffnode *hufftable)
{
    unsigned char *end;
    huffnode *headptr, *huffptr;
	int written = 0;
	unsigned char val;
	unsigned char mask;
    unsigned short nodeval;

    if(!length || !dest)
    {
        Quit("length or dest is null!");
        return;
    }

    headptr = hufftable+254;        // head node is always node 254



    end=dest+length;

    val = *source++;
    mask = 1;
    
    huffptr = headptr;
    while(1)
    {
        if(!(val & mask))
            nodeval = huffptr->bit0;
        else
            nodeval = huffptr->bit1;
        if(mask==0x80)
        {
            val = *source++;
            mask = 1;
        }
        else mask <<= 1;

        if(nodeval<256)
        {
            *dest++ = (unsigned char) nodeval;
            written++;
            huffptr = headptr;
            if(dest>=end) break;
        }
        else
        {
            huffptr = hufftable + (nodeval - 256);
        }
    }
}

/*
======================
=
= CAL_CarmackExpand
=
= Length is the length of the EXPANDED data
=
======================
*/

#define NEARTAG 0xa7
#define FARTAG  0xa8

void CAL_CarmackExpand (unsigned char *source, unsigned short *dest, int length)
{
    unsigned short ch,chhigh,count,offset;
    unsigned char *inptr;
    unsigned short *copyptr, *outptr;

    length/=2;

    inptr = (unsigned char *) source;
    outptr = dest;

    while (length>0)
    {
        ch = READWORD(inptr);
        inptr += 2;
        chhigh = ch>>8;
        if (chhigh == NEARTAG)
        {
            count = ch&0xff;
            if (!count)
            {                               // have to insert a word containing the tag byte
                ch |= *inptr++;
                *outptr++ = ch;
                length--;
            }
            else
            {
                offset = *inptr++;
                copyptr = outptr - offset;
                length -= count;
                if(length<0) return;
                while (count--)
                    *outptr++ = *copyptr++;
            }
        }
        else if (chhigh == FARTAG)
        {
            count = ch&0xff;
            if (!count)
            {                               // have to insert a word containing the tag byte
                ch |= *inptr++;
                *outptr++ = ch;
                length --;
            }
            else
            {
                offset = READWORD(inptr);
                inptr += 2;
                copyptr = dest + offset;
                length -= count;
                if(length<0) return;
                while (count--)
                    *outptr++ = *copyptr++;
            }
        }
        else
        {
            *outptr++ = ch;
            length --;
        }
    }
}

/*
======================
=
= CA_RLEWcompress
=
======================
*/

int32_t CA_RLEWCompress (unsigned short *source, int32_t length, unsigned short *dest, unsigned short rlewtag)
{
    unsigned short value,count;
    unsigned i;
    unsigned short *start,*end;

    start = dest;

    end = source + (length+1)/2;

    //
    // compress it
    //
    do
    {
        count = 1;
        value = *source++;
        while (*source == value && source<end)
        {
            count++;
            source++;
        }
        if (count>3 || value == rlewtag)
        {
            //
            // send a tag / count / value string
            //
            *dest++ = rlewtag;
            *dest++ = count;
            *dest++ = value;
        }
        else
        {
            //
            // send word without compressing
            //
            for (i=1;i<=count;i++)
                *dest++ = value;
        }

    } while (source<end);

    return (int32_t)(2*(dest-start));
}

#ifndef SEGA_SATURN
/*
======================
=
= CA_RLEWexpand
= length is EXPANDED length
=
======================
*/

void CA_RLEWexpand (unsigned short *source, unsigned short *dest, int32_t length, unsigned short rlewtag)
{
    unsigned short value,count,i;
    unsigned short *end=dest+length/2;

//
// expand it
//
    do
    {
        value = *source++;
        if (value != rlewtag)
            //
            // uncompressed
            //
            *dest++=value;
        else
        {
            //
            // compressed string
            //
            count = *source++;
            value = *source++;
            for (i=1;i<=count;i++)
                *dest++ = value;
        }
    } while (dest<end);
}
#endif


/*
=============================================================================
                                         CACHE MANAGER ROUTINES
=============================================================================
*/


/*
======================
=
= CAL_SetupGrFile
=
======================
*/

void CAL_SetupGrFile (void)
{
#ifdef SEGA_SATURN
    char fname[13];
    //int handle;
//	unsigned int j=0;
    unsigned char* compseg;
    long fileSize;
    Sint32 fileId;

#ifdef GRHEADERLINKED

    grhuffman = (huffnode*)&EGAdict;
    grstarts = (int32_t _seg*)FP_SEG(&EGAhead);

#else

    //
    // load ???dict.ext (huffman dictionary for graphics files)
    //

    strcpy(fname, gdictname);
    strcat(fname, extension);
    /*
        while (fname[j])
        {
            fname[j]= toupper(fname[j]);
            j++;
        }*/
        //	i=0;

            //handle = open(fname, O_RDONLY | O_BINARY);
            //if (handle == -1)
        //	if(stat(fname, NULL))
        //        CA_CannotOpen(fname);

    fileId = GFS_NameToId((Sint8*)fname);
    //	fileSize = GetFileSize(fileId);
    compseg = (Uint8*)saturnChunk + 0x8000;
    //	CHECKMALLOCRESULT(vbtHuff);
    //slPrint((char *)"CAL_SetupGrFile1     ",slLocate(10,12));
    GFS_Load(fileId, 0, (void*)compseg, sizeof(grhuffman)); // lecture VGADICT full
    huffnode* grhuffmanptr = (huffnode*)grhuffman;

    for (unsigned char x = 0; x < 255; x++)
    {
        //	slPrint((char *)"CAL_SetupGrFile2     ",slLocate(10,12));	
        grhuffmanptr->bit0 = compseg[0] | (compseg[1] << 8);
        grhuffmanptr->bit1 = compseg[2] | (compseg[3] << 8);
        grhuffmanptr++;
        compseg += 4;
    }
    //	j=0;

        //read(handle, grhuffman, sizeof(grhuffman));
        //close(handle);

        // load the data offsets from ???head.ext
    strcpy(fname, gheadname);
    strcat(fname, extension);
    /*
        while (fname[j])
        {
            fname[j]= toupper(fname[j]);
            j++;
        }
        j=0;*/

        //handle = open(fname, O_RDONLY | O_BINARY);
        //if (handle == -1)
    //	if(stat(fname, NULL))
    //        CA_CannotOpen(fname);
    //slSynch();
    //slPrint((char *)"CAL_SetupGrFile3     ",slLocate(10,12));
    fileId = GFS_NameToId((Sint8*)fname);
    fileSize = GetFileSize(fileId);
    long headersize = fileSize;//lseek(handle, 0, SEEK_END);
    //lseek(handle, 0, SEEK_SET);
//headersize= 157*3;
#if 0
    if (headersize / 3 != (long)(lengthof(grstarts) - numEpisodesMissing))
    {
        /*        Quit("Wolf4SDL was not compiled for these data files:\n"
                    "        %s contains a wrong number of offsets                          (%i instead of %i)!\n\n"
                    "                          Please check whether you are using the right executable!\n"
                    "(For mod developers: perhaps you forgot to update NUMCHUNKS?)",
                    fname, headersize / 3, lengthof(grstarts) - numEpisodesMissing);
        */
        char msg[200];
        sprintf(msg, "Wolf4SDL was not compiled for these data files:\n"
            "        %s contains a wrong number of offsets                          (%i instead of %i)!\n\n"
            "                          Please check whether you are using the right executable!\n"
            "(For mod developers: perhaps you forgot to update NUMCHUNKS?)",
            fname, headersize / 3, lengthof(grstarts) - numEpisodesMissing);

        slPrint((char*)msg, slLocate(1, 3));
        while (1);
    }
#endif	
    //slPrint((char *)"CAL_SetupGrFile4     ",slLocate(10,12));
    unsigned char data[lengthof(grstarts) * 3];
    GFS_Load(fileId, 0, (void*)data, sizeof(data)); // lecture de VGAHEAD full
    //read(handle, data, sizeof(data));
    //close(handle);

    const unsigned char* d = data;
    for (int32_t* i = grstarts; i != endof(grstarts); ++i)
    {
        //	slPrint((char *)"CAL_SetupGrFile5     ",slLocate(10,12));	
        const int32_t val = d[0] | d[1] << 8 | d[2] << 16;
        *i = (val == 0x00FFFFFF ? -1 : val);
        d += 3;
    }

#endif
    //slPrint((char *)"CAL_SetupGrFile6     ",slLocate(10,12));
    //
    // Open the graphics file, leaving it open until the game is finished
    //
    strcpy(fname, gfilename);
    strcat(fname, extension);
    /*
        while (fname[j])
        {
            fname[j]= toupper(fname[j]);
            j++;
        }
        j=0;*/

        //grhandle = open(fname, O_RDONLY | O_BINARY);
        //if (grhandle == -1)
    //	if(stat(fname, NULL))
    //        CA_CannotOpen(fname);

    grhandle = GFS_NameToId((Sint8*)fname);
    //	fileSize = GetFileSize(grhandle);
    //
    // load the pic and sprite headers into the arrays in the data segment
    //
    //slPrint((char *)"CAL_SetupGrFile7     ",slLocate(10,12));
    pictable = (pictabletype*)malloc(NUMPICS * sizeof(pictabletype));

    int32_t   chunkcomplen = CAL_GetGrChunkLength(STRUCTPIC);                // position file pointer
    //	compseg =(byte*)malloc((chunkcomplen));
    compseg = (unsigned char*)saturnChunk;
    //	CHECKMALLOCRESULT(compseg);
    GFS_Load(grhandle, 0, (void*)compseg, (chunkcomplen));
    //    CAL_HuffExpand(&compseg[4], (byte*)pictable, NUMPICS * sizeof(pictabletype), grhuffman);
    //slPrint((char *)"CAL_SetupGrFile8     ",slLocate(10,12));
    CAL_HuffExpand(&compseg[4], (unsigned char*)pictable, NUMPICS * sizeof(pictabletype), grhuffman);
    //slPrint((char *)"CAL_SetupGrFile9     ",slLocate(10,12));
    for (unsigned long k = 0; k < NUMPICS; k++)
    {
        //slPrint((char *)"CAL_SetupGrFile10     ",slLocate(10,12));		
        pictable[k].height = SWAP_BYTES_16(pictable[k].height);
        pictable[k].width = SWAP_BYTES_16(pictable[k].width);
    }
    //slPrint((char *)"CAL_SetupGrFile11     ",slLocate(10,12));	
    compseg = NULL;
    // VBT correct
#else
    char fname[13];
    int handle;
    unsigned char *compseg;
	long headersize;
	int expectedsize;
	unsigned char data[lengthof(grstarts) * 3];
	const unsigned char* d;
	int32_t* i;
//
// load ???dict.ext (huffman dictionary for graphics files)
//

    strcpy(fname,gdictname);
    strcat(fname,graphext);

    handle = w3sopen(fname, O_RDONLY | O_BINARY);
    if (handle == -1)
        CA_CannotOpen(fname);

    w3sread(handle, grhuffman, sizeof(grhuffman));
    w3sclose(handle);

    // load the data offsets from ???head.ext
    strcpy(fname,gheadname);
    strcat(fname,graphext);

    handle = w3sopen(fname, O_RDONLY | O_BINARY);
    if (handle == -1)
        CA_CannotOpen(fname);

    headersize = w3slseek(handle, 0, SEEK_END);
    w3slseek(handle, 0, SEEK_SET);

	expectedsize = lengthof(grstarts);

    if(!param_ignorenumchunks && headersize / 3 != (long) expectedsize)
        Quit("Wolf4SDL was not compiled for these data files:\n"
            "%s contains a wrong number of offsets (%i instead of %i)!\n\n"
            "Please check whether you are using the right executable!\n"
            "(For mod developers: perhaps you forgot to update NUMCHUNKS?)",
            fname, headersize / 3, expectedsize);


    w3sread(handle, data, sizeof(data));
    w3sclose(handle);

    d = data;
    for (i = grstarts; i != endof(grstarts); ++i)
    {
        const int32_t val = d[0] | d[1] << 8 | d[2] << 16;
        *i = (val == 0x00FFFFFF ? -1 : val);
        d += 3;
    }

//
// Open the graphics file
//
    strcpy(fname,gfilename);
    strcat(fname,graphext);

    grhandle = w3sopen(fname, O_RDONLY | O_BINARY);
    if (grhandle == -1)
        CA_CannotOpen(fname);


//
// load the pic and sprite headers into the arrays in the data segment
//
    pictable = SafeMalloc(NUMPICS * sizeof(*pictable));
    CAL_GetGrChunkLength(STRUCTPIC);                // position file pointer
    compseg = SafeMalloc(chunkcomplen);
    w3sread (grhandle,compseg,chunkcomplen);
    CAL_HuffExpand(compseg, (unsigned char*)pictable, NUMPICS * sizeof(*pictable), grhuffman);
    free(compseg);

    CA_CacheGrChunks ();

    w3sclose (grhandle);
#endif
}

//==========================================================================


/*
======================
=
= CAL_SetupMapFile
=
======================
*/

#ifdef SEGA_SATURN
long CAL_SetupMapFile(void)
#else
void CAL_SetupMapFile (void)
#endif
{
#ifdef SEGA_SATURN
#ifndef SEGA_SATURN
    int     i;
#endif
    int32_t length, pos;
    char fname[13];
    long fileSize;

    //
    // load maphead.ext (offsets and tileinfo for map file)
    //
    strcpy(fname, mheadname);
    strcat(fname, extension);

    Sint32 fileId;

    fileId = GFS_NameToId((Sint8*)fname);
    //	fileSize = GetFileSize(fileId); // utile
    length = NUMMAPS * 4 + 2; // used to be "filelength(handle);"
    //    mapfiletype *tinf=(mapfiletype *) malloc(sizeof(mapfiletype));
    mapfiletype* tinf = (mapfiletype*)saturnChunk;
    GFS_Load(fileId, 0, (void*)tinf, length);
    //read(handle, tinf, length);

    //
    // open the data file
    //
#ifdef CARMACIZED
    strcpy(fname, "GAMEMAPS.");
    strcat(fname, extension);

    /*	while (fname[i])
        {
            fname[i]= toupper(fname[i]);
            i++;
        }*/
    maphandle = GFS_NameToId((Sint8*)fname);
    fileSize = GetFileSize(maphandle);
#else
    strcpy(fname, mfilename);
    strcat(fname, extension);

    maphandle = open(fname, O_RDONLY | O_BINARY);
    if (maphandle == -1)
        CA_CannotOpen(fname);
#endif

    //
    // load all map header
    //

    pos = tinf->headeroffsets[mapnum];
    //slPrintHex(pos, slLocate(10, 2));
    //slPrintHex(fileSize, slLocate(10, 3));
    if (pos < 0)                          // $FFFFFFFF start is a sparse map
        return fileSize;


    slPrintHex(pos, slLocate(10, 2));

    uint8_t* maphandleptr = (uint8_t*)(((int)saturnChunk + length + (4 - 1)) & -4);

    readChunks(maphandle, sizeof(maptype), &pos, maphandleptr, (uint8_t*)&mapheaderseg);
    //	memcpy((memptr)&mapheaderseg,saturnChunk+0x500,sizeof(maptype));
#ifndef EMBEDDED
    mapheaderseg[mapnum] = (maptype*)((saturnChunk + sizeof(mapfiletype) + fileSize + (8 - 1)) & -4);
    //	CHECKMALLOCRESULT(mapheaderseg[mapnum]);
    safe_malloc(mapheaderseg[mapnum]);
        //read (maphandle,(memptr)mapheaderseg[i],sizeof(maptype));
    memcpy((memptr)mapheaderseg[mapnum], maphandleptr, sizeof(maptype));

    //
    // allocate space for 3 64*64 planes
    //
    for (int i = 0; i < MAPPLANES; i++)
    {
        mapheaderseg[mapnum]->planestart[i] = SWAP_BYTES_32(mapheaderseg[mapnum]->planestart[i]);
        mapheaderseg[mapnum]->planelength[i] = SWAP_BYTES_16(mapheaderseg[mapnum]->planelength[i]);
        mapsegs[i] = (unsigned short*)SATURN_MAPSEG_ADDR + (0x2000 * i);
        //		mapsegs[i]=(word *) malloc(maparea*2);
    }
    mapheaderseg[mapnum]->width = SWAP_BYTES_16(mapheaderseg[mapnum]->width);
    mapheaderseg[mapnum]->height = SWAP_BYTES_16(mapheaderseg[mapnum]->height);
#else

//
// allocate space for 3 64*64 planes
//
//	if(mapheaderseg.width>0xff)
    {
        for (i = 0; i < MAPPLANES; i++)
        {
            mapheaderseg.planestart[i] = SWAP_BYTES_32(mapheaderseg.planestart[i]);
           // slPrintHex(mapheaderseg.planestart[i], slLocate(10, 6 + i * 2));
            mapheaderseg.planelength[i] = SWAP_BYTES_16(mapheaderseg.planelength[i]);
            //slPrintHex(mapheaderseg.planelength[i], slLocate(10, 6 + (i * 2) + 1));
            //		mapsegs[i]=(word *)SATURN_MAPSEG_ADDR+(0x2000*i);
            if (mapsegs[i] == NULL) mapsegs[i] = (word*)malloc(maparea * 2);
            //		mapsegs[i]=(word *) malloc(maparea*2);
        }
        mapheaderseg.width = SWAP_BYTES_16(mapheaderseg.width);
        //slPrintHex(mapheaderseg.width, slLocate(10, 4));
        mapheaderseg.height = SWAP_BYTES_16(mapheaderseg.height);
    }
    //slPrintHex(mapheaderseg.height, slLocate(10, 5));

#endif


    maphandleptr = NULL;
    tinf = NULL;

    memcpy(source, Chunks, compressed);

    return fileSize;
#else
    int     i;
    int handle;
    int32_t pos;
#if defined(SWITCH) || defined (N3DS) || defined(SATURN)
    char fname[13 + sizeof(DATADIR)];
#else
    char fname[13];
#endif
//
// load maphead.ext (offsets and tileinfo for map file)
//
    strcpy(fname,mheadname);
    strcat(fname,extension);

    handle = w3sopen(fname, O_RDONLY | O_BINARY);
    if (handle == -1)
        CA_CannotOpen(fname);

    tinf = SafeMalloc(sizeof(*tinf));

    w3sread(handle, tinf, sizeof(*tinf));
    w3sclose(handle);

//
// open the data file
//
#ifdef CARMACIZED
    strcpy(fname, mfilecama);    
    strcat(fname, extension);

    maphandle = w3sopen(fname, O_RDONLY | O_BINARY);
    if (maphandle == -1)
        CA_CannotOpen(fname);
#else
    strcpy(fname,mfilename);
    strcat(fname,extension);

    maphandle = open(fname, O_RDONLY | O_BINARY);
    if (maphandle == -1)
        CA_CannotOpen(fname);
#endif

//
// load all map header
//
    for (i=0;i<NUMMAPS;i++)
    {
        pos = tinf->headeroffsets[i];
        if (pos<0)                          // $FFFFFFFF start is a sparse map
            continue;

        mapheaderseg[i] = SafeMalloc(sizeof(*mapheaderseg[i]));

        w3slseek(maphandle,pos,SEEK_SET);
        w3sread (maphandle,mapheaderseg[i],sizeof(*mapheaderseg[i]));
    }

//
// allocate space for 3 64*64 planes
//
    for (i = 0; i < MAPPLANES; i++)
    {
        mapsegs[i] = SafeMalloc(MAPAREA * sizeof(*mapsegs[i]));
    }
#endif
}


//==========================================================================

/*
======================
=
= CAL_SetupAudioFile
=
======================
*/
#if !defined(VIEASM) || !defined(SEGA_SATURN)
void CAL_SetupAudioFile (void)
{
#if defined(SWITCH) || defined(N3DS) || defined(PS2)
    char fname[13 + sizeof(DATADIR)];
#else    
    char fname[13];
#endif
	void* ptr;
//
// load audiohed.ext (offsets for audio file)
//
#ifndef VIEASM
    strcpy(fname,aheadname);
#endif
    strcat(fname,audioext);

    
    if (!CA_LoadFile(fname, &ptr))
        CA_CannotOpen(fname);
    audiostarts = (int32_t*)ptr;

//
// open the data file
//
    strcpy(fname,afilename);
    strcat(fname,audioext);

    audiohandle = w3sopen(fname, O_RDONLY | O_BINARY);
    if (audiohandle == -1)
        CA_CannotOpen(fname);
}
#endif
//==========================================================================


/*
======================
=
= CA_Startup
=
= Open all files and load in headers
=
======================
*/

void CA_Startup (void)
{
#ifdef PROFILE
    unlink ("PROFILE.TXT");
    profilehandle = open("PROFILE.TXT", O_CREAT | O_WRONLY | O_TEXT);
#endif
#ifndef SEGA_SATURN
#if defined(SWITCH) || defined (N3DS)
    printf("CA_INIT\n");
#endif   
    CAL_SetupMapFile ();
#endif
#if defined (SWITCH) || defined (N3DS)   
    printf("CAL_SetupMapFile ();\n");
#endif    
    CAL_SetupGrFile ();
#if defined (SWITCH) || defined (N3DS)    
    printf("CAL_SetupGrFile ();\n");
#endif    
#ifndef SEGA_SATURN
#ifndef VIEASM
    CAL_SetupAudioFile ();
#endif
#if defined (SWITCH) || defined (N3DS)
#ifndef VIEASM    
    printf("CAL_SetupAudioFile ();\n");
#endif
#endif
#endif
}

//==========================================================================

#if !defined(SEGA_SATURN)
/*
======================
=
= CA_Shutdown
=
= Closes all files
=
======================
*/

void CA_Shutdown (void)
{
    int i,start;

    if (maphandle != -1)
        w3sclose(maphandle);
    
#ifndef VIEASM
    if (audiohandle != -1)
        w3sclose(audiohandle);
#endif
    for (i=0; i<NUMCHUNKS; i++)
    {
        free (grsegs[i]);
        grsegs[i] = NULL;
    }

    free (pictable);
    pictable = NULL;

    for (i = 0; i < NUMMAPS; i++)
    {
        free (mapheaderseg[i]);
        mapheaderseg[i] = NULL;
    }

    for (i = 0; i < MAPPLANES; i++)
    {
        free (mapsegs[i]);
        mapsegs[i] = NULL;
    }

    free (tinf);
    tinf = NULL;
#ifndef VIEASM
    switch(oldsoundmode)
    {
        case sdm_Off:
            return;
        case sdm_PC:
            start = STARTPCSOUNDS;
            break;
        case sdm_AdLib:
            start = STARTADLIBSOUNDS;
            break;
    }
#endif
    for(i=0; i<NUMSOUNDS; i++,start++)
        UNCACHEAUDIOCHUNK(start);
}

//===========================================================================

/*
======================
=
= CA_CacheAudioChunk
=
======================
*/
#ifndef VIEASM
int32_t CA_CacheAudioChunk (int chunk)
{
    int32_t pos = audiostarts[chunk];
    int32_t size = audiostarts[chunk+1]-pos;

    if (audiosegs[chunk])
        return size;                        // already in memory

    audiosegs[chunk] = SafeMalloc(size);

    w3slseek(audiohandle,pos,SEEK_SET);
    w3sread(audiohandle,audiosegs[chunk],size);

    return size;
}
#endif

void CA_CacheAdlibSoundChunk (int chunk)
{
    unsigned char    *bufferseg;
    unsigned char    *ptr;
    int32_t pos = audiostarts[chunk];
    int32_t size = audiostarts[chunk+1]-pos;
	AdLibSound *sound;

    if (audiosegs[chunk])
        return;                        // already in memory

    w3slseek(audiohandle, pos, SEEK_SET);

    bufferseg = (unsigned char*)SafeMalloc(ORIG_ADLIBSOUND_SIZE - 1);
    ptr = bufferseg;

    w3sread(audiohandle, ptr, ORIG_ADLIBSOUND_SIZE - 1);   // without data[1]

    sound = (AdLibSound*)SafeMalloc(size + sizeof(*sound) - ORIG_ADLIBSOUND_SIZE);

    sound->common.length = READLONGWORD(ptr);
    ptr += 4;

    sound->common.priority = READWORD(ptr);
    ptr += 2;

    sound->inst.mChar = *ptr++;
    sound->inst.cChar = *ptr++;
    sound->inst.mScale = *ptr++;
    sound->inst.cScale = *ptr++;
    sound->inst.mAttack = *ptr++;
    sound->inst.cAttack = *ptr++;
    sound->inst.mSus = *ptr++;
    sound->inst.cSus = *ptr++;
    sound->inst.mWave = *ptr++;
    sound->inst.cWave = *ptr++;
    sound->inst.nConn = *ptr++;
    sound->inst.voice = *ptr++;
    sound->inst.mode = *ptr++;
    sound->inst.unused[0] = *ptr++;
    sound->inst.unused[1] = *ptr++;
    sound->inst.unused[2] = *ptr++;
    sound->block = *ptr++;

    w3sread(audiohandle, sound->data, size - ORIG_ADLIBSOUND_SIZE + 1);  // + 1 because of byte data[1]

    audiosegs[chunk]=(unsigned char *) sound;

    free (bufferseg);
}

//===========================================================================

/*
======================
=
= CA_LoadAllSounds
=
= Purges all sounds, then loads all new ones (mode switch)
=
======================
*/
#ifndef VIEASM
void CA_LoadAllSounds (void)
{
    unsigned start,i;

    switch (oldsoundmode)
    {
        case sdm_Off:
            goto cachein;
        case sdm_PC:
            start = STARTPCSOUNDS;
            break;
        case sdm_AdLib:
            start = STARTADLIBSOUNDS;
            break;
    }

    for (i=0;i<NUMSOUNDS;i++,start++)
        UNCACHEAUDIOCHUNK(start);

cachein:

    oldsoundmode = SoundMode;

    switch (SoundMode)
    {
        case sdm_Off:
            start = STARTADLIBSOUNDS;   // needed for priorities...
            break;
        case sdm_PC:
            start = STARTPCSOUNDS;
            break;
        case sdm_AdLib:
            start = STARTADLIBSOUNDS;
            break;
    }

    if(start == STARTADLIBSOUNDS)
    {
        for (i=0;i<NUMSOUNDS;i++,start++)
            CA_CacheAdlibSoundChunk(start);
    }
    else
    {
        for (i=0;i<NUMSOUNDS;i++,start++)
            CA_CacheAudioChunk(start);
    }
}
#endif
#endif
//===========================================================================


/*
======================
=
= CAL_ExpandGrChunk
=
= Does whatever is needed with a pointer to a compressed chunk
=
======================
*/

void CAL_ExpandGrChunk (int chunk, int32_t *source)
{
    int32_t    expanded;

    if (chunk >= STARTTILE8 && chunk < STARTEXTERNS)
    {
        //
        // expanded sizes of tile8/16/32 are implicit
        //

#define BLOCK           64
#define MASKBLOCK       128

        if (chunk<STARTTILE8M)          // tile 8s are all in one chunk!
            expanded = BLOCK*NUMTILE8;
        else if (chunk<STARTTILE16)
            expanded = MASKBLOCK*NUMTILE8M;
        else if (chunk<STARTTILE16M)    // all other tiles are one/chunk
            expanded = BLOCK*4;
        else if (chunk<STARTTILE32)
            expanded = BLOCK*4;
        else if (chunk<STARTTILE32M)
            expanded = BLOCK*16;
        else
            expanded = MASKBLOCK*16;
    }
    else
    {
        //
        // everything else has an explicit size longword
        //
        expanded = *source++;
    }

    //
    // allocate final space and decompress it
    //
    grsegs[chunk] = (unsigned char*)SafeMalloc(expanded);

    CAL_HuffExpand((unsigned char *) source, grsegs[chunk], expanded, grhuffman);
}


/*
======================
=
= CAL_DeplaneGrChunk
=
======================
*/
#ifndef SEGA_SATURN
void CAL_DeplaneGrChunk (int chunk)
{
    int     i;
    int16_t width,height;

    if (chunk == STARTTILE8)
    {
        width = height = 8;

        for (i = 0; i < NUMTILE8; i++)
            VL_DePlaneVGA (grsegs[chunk] + (i * (width * height)),width,height);
    }
    else
    {
        width = pictable[chunk - STARTPICS].width;
        height = pictable[chunk - STARTPICS].height;

        VL_DePlaneVGA (grsegs[chunk],width,height);
    }
}
#endif

/*
======================
=
= CA_CacheGrChunks
=
= Load all graphics chunks into memory
=
======================
*/

void CA_CacheGrChunks (void)
{
    int32_t pos,compressed;
    int32_t *bufferseg;
    int32_t *source;
    int     chunk,next;

    for (chunk = STRUCTPIC + 1; chunk < NUMCHUNKS; chunk++)
    {
        if (grsegs[chunk])
            continue;                             // already in memory

        //
        // load the chunk into a buffer
        //
        pos = GRFILEPOS(chunk);

        if (pos<0)                              // $FFFFFFFF start is a sparse tile
            continue;

        next = chunk + 1;

        while (GRFILEPOS(next) == -1)           // skip past any sparse tiles
            next++;

        compressed = GRFILEPOS(next)-pos;

#ifdef SEGA_SATURN
        uint8_t* Chunks;
        uint16_t delta;
        uint32_t delta2;
        Uint16 i, j = 0;
        delta = (uint16_t)(pos / 2048);
        delta2 = (pos - delta * 2048);

        Chunks = (uint8_t*)saturnChunk;  // d?plac? pour pas ?craser de son
        //	CHECKMALLOCRESULT(Chunks);
        GFS_Load(grhandle, delta, (void*)Chunks, compressed + delta2); // lecture partielle ok
        Chunks += delta2;
        //TODO:
        if (compressed <= BUFFERSIZE)
        {
            for (i = 0; i < compressed; i += 4)
            {
                // evite des erreurs d'alignement de pointeur ?			
                bufferseg[j++] = Chunks[3] | (Chunks[2] << 8) | (Chunks[1] << 16) | (Chunks[0] << 24);
                Chunks += 4;
            }
            source = bufferseg;
        }
        else
        {
            source = (int32_t*)malloc(compressed);
            CHECKMALLOCRESULT(source);
            //read(grhandle,source,compressed);

            for (i = 0; i < compressed; i += 4)
            {
                // evite des erreurs d'alignement de pointeur ?				
                source[j++] = Chunks[3] | (Chunks[2] << 8) | (Chunks[1] << 16) | (Chunks[0] << 24);
                Chunks += 4;
            }

        }
        Chunks = NULL;
        CAL_ExpandGrChunk(chunk, source);

        if (compressed > BUFFERSIZE)
        {
            free(source);
            source = NULL;
        }

#endif

        w3slseek(grhandle,pos,SEEK_SET);

        bufferseg = SafeMalloc(compressed);
        source = bufferseg;

        w3sread(grhandle,source,compressed);

        CAL_ExpandGrChunk (chunk,source);

        if (chunk >= STARTPICS && chunk < STARTEXTERNS)
            CAL_DeplaneGrChunk (chunk);

        free(bufferseg);
    }
}



//==========================================================================


/*
======================
=
= CA_CacheMap
=
= WOLF: This is specialized for a 64*64 map size
=
======================
*/

void CA_CacheMap (int mapnum)
{
    int32_t  pos,compressed;
    int      plane;
    unsigned short *dest;
    unsigned size;
    unsigned short *bufferseg;
    unsigned short *source;
#ifdef CARMACIZED
    unsigned short *buffer2seg;
    int32_t  expanded;
#endif
#ifdef SEGA_SATURN
    long fileSize;
    CAL_SetupMapFile(mapnum);
#endif

    if (mapheaderseg[mapnum]->width != MAPSIZE || mapheaderseg[mapnum]->height != MAPSIZE)
        Quit ("CA_CacheMap: Map not %u*%u!",MAPSIZE,MAPSIZE);

//
// load the planes into the already allocated buffers
//
#ifdef SEGA_SATURN
    //TODO: Adapt this...
    uint32_t pos = GRFILEPOS(chunk);
    readChunks(grhandle, sizeof(chunkexplen), &pos, saturnChunk, (uint8_t*)&chunkexplen);
#else
    size = MAPAREA * sizeof(*dest);
#endif
    for (plane = 0; plane<MAPPLANES; plane++)
    {
#if defined(EMBEDDED) && defined(SEGA_SATURN)
        pos = mapheaderseg.planestart[plane];
        compressed = mapheaderseg.planelength[plane];
#else
        pos = mapheaderseg[mapnum]->planestart[plane];
        compressed = mapheaderseg[mapnum]->planelength[plane];
#endif
        dest = mapsegs[plane];

#ifndef SEGA_SATURN
        w3slseek(maphandle,pos,SEEK_SET);
#endif
        bufferseg = SafeMalloc(compressed);
        source = bufferseg;
#ifdef SEGA_SATURN
        memcpy(source, &Chunks[pos], compressed);
#else
        w3sread(maphandle,source,compressed);
#endif
#ifdef CARMACIZED
        //
        // unhuffman, then unRLEW
        // The huffman'd chunk has a two byte expanded length first
        // The resulting RLEW chunk also does, even though it's not really
        // needed
        //
#ifdef SEGA_SATURN
        expanded = source[0] | (source[1] << 8);
        source += 2;
#else
        expanded = *source;
        source++;
#endif
        buffer2seg = SafeMalloc(expanded);
        CAL_CarmackExpand((unsigned char *) source, buffer2seg,expanded);
        CA_RLEWexpand(buffer2seg+1,dest,size,tinf->RLEWtag);
        free(buffer2seg);
#ifdef SEGA_SATURN
        buffer2seg = NULL;
#endif

#else
        //
        // unRLEW, skipping expanded length
        //
        CA_RLEWexpand (source+1,dest,size,tinf->RLEWtag);
#endif
        free(bufferseg);
    }
}

//===========================================================================

#ifndef SEGA_SATURN
void CA_CannotOpen(const char *string)
{
    char str[30];

    strcpy(str,"Can't open ");
    strcat(str,string);
    strcat(str,"!\n");
    Quit (str);
}
#endif