#ifndef __ID_UDP__H
#define __ID_UDP__H

#ifdef LWUDPCOMMS 

#ifdef __cplusplus
extern "C"
{
#endif

/*
=====================================================================

                           COMMS PROTOCOL

=====================================================================
*/

typedef enum
{
    in,
    out
} Direction;

typedef enum
{
    knife,
    pistol,
    machineGun,
    chainGun
}Weapon;

typedef enum
{
    gold,
    silver,
    bronze,
    aqua
} Key;

typedef struct
{
    char* argv[1];
    const int argc;
    int i;
    boolean hasError;
    boolean showHelp;
} Parameter;

typedef struct
{
    int peeruid;
    short health;
    short ammo;
    Weapon weapon;
    int x, y;
    short angle;
    int mask;
    Key keys;
    int mvx, mvy;
    short mvangle;
    unsigned char atx, aty;
    unsigned char count;
    unsigned char eventIndices[2];
    unsigned char picx, picy;
} Player;

typedef struct
{
    int sendingPeerUid;
    unsigned int packetSeqNum;
    Player players;
} DataLayer;

typedef struct
{
    unsigned char* data;
    size_t sizeleft;
    Direction dir;
    int mask;
    Parameter* param;
    Player* playerItself;
    DataLayer* dataLayer;
} Stream;

extern Stream* NetStream;

/*
=====================================================================

                            COMMS CLIENT API

=====================================================================
*/

void UDP_startup(void);

void UDP_shutdown(void);

void UDP_pollState(void);

void UDP_txPoll(void);

void UDP_rxPoll(void);

void UDP_finishRxWaitPoll(void);

/*
=====================================================================

                        COMMS PARAMETER HANDLING

=====================================================================
*/


boolean UDP_check(char* arg);

const char *UDP_parameterHelp(void);


#ifdef __cplusplus
}
#endif

#endif
#endif
