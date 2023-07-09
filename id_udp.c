/* ID_UDP.C */

/*
=============================================================================
ID_UDP.C NOTE:
This file was ported to C by me André Guilherme to match the Wolf4SDL Coding
standards, this is an early stage support and you may find bugs.
You will need a SDL_net library to make this source ile work properly
=============================================================================
*/

#include "wl_def.h"

#ifdef LWUDPCOMMS
#ifndef SEGA_SATURN
#include <sys/types.h>
#if defined(_MSC_VER)
#include <io.h>
#elif defined(SWITCH)
#include <sys/_iovec.h>
#elif defined(N3DS) || defined(PS2) 
struct iovec
{
    void* iov_base;
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
#include <stdint.h>
#include <stdio.h>

#include <SDL_net.h>

typedef struct
{
    int uid;
    IPaddress address;
    DataLayer protState;
    boolean expectingResp;
}Peer;

/*
=============================================================================

                              LOCAL VARIABLES

=============================================================================
*/

Stream* NetStream;

int channel = 0;
int maxpacketsize = 1024;
uintptr_t maxWaitResponseTics = 30;
uintptr_t udp_tics = 0;
unsigned short port = 0;
int peeruid = 0;
UDPsocket udpsock;
UDPpacket* packet = NULL;
DataLayer protState;
Peer peers[1];
boolean foundPeerNoResp = false;
unsigned int packetSeqNum = 0;
boolean server = false;

/*
=============================================================================

                              CALLBACK FUNCTIONS

=============================================================================
*/


boolean CSetPeerExpectingResp(Peer* peer);

boolean CPlayer(Player* player);

boolean CPeer(Peer* peer);

/*
=====================================================================

                            SERIALIZERS

=====================================================================
*/
static void serializeRaw(void* p, size_t sz)
{
    Stream stream;
    if (sz > stream.sizeleft)
    {
        printf("ran out of space in stream");
    }

    if (stream.dir == in)
    {
        memcpy(p, stream.data, sz);
    }
    else if (stream.dir == out)
    {
        memcpy(stream.data, p, sz);
    }

    stream.data += sz;
    stream.sizeleft -= sz;
}

static void serializeInt(int* x)
{
    serializeRaw(x, sizeof(x));
}

static void serializeShort(short* x)
{
    serializeRaw(x, sizeof(x));
}

static void serializeUnsignedChar(unsigned char* x)
{
    serializeRaw(x, sizeof(x));
}

static void serializeUnsignedInt(unsigned int* x)
{
    serializeRaw(x, sizeof(x));
}

static void serializeUnsignedLong(unsigned long* x)
{
    serializeRaw(x, sizeof(x));
}

static void serializeEnum(Stream* stream, int *x)
{
    x = (int*)stream;
    serializeInt(x);
}

static wlinline void serializeStreamInt(Stream* stream, int* x)
{
    x = (int*)stream;
    serializeInt(x);
}

static wlinline void serializeStreamShort(Stream* stream, short* x)
{
    x = (short*)stream;
    serializeShort(x);
}

static wlinline void serializeStreamUnsignedChar(Stream* stream, unsigned char* x)
{
    x = (unsigned char*)stream;
    serializeUnsignedChar(x);
}

static wlinline void serializeStreamUnisgnedInt(Stream* stream, unsigned int* x)
{
    x = (unsigned int*)stream;
    serializeUnsignedInt(x);
}

static wlinline void serializeStreanUnsignedLong(Stream* stream, unsigned long* x)
{
    x = (unsigned long*)stream;
    serializeUnsignedLong(x);
}

static wlinline void serializeWeaponEnum(Stream* stream, Weapon* x)
{
    serializeEnum(stream, (int*)x);
}

static wlinline void serializeKey(Stream* stream, Key* x)
{
    serializeEnum(stream, (int*)x);
}

static void serializeMask(Stream* stream)
{
    stream->mask;
}

static wlinline void serializeSet(Stream* stream, Player* x)
{
    x->mask = stream->mask;
    serializeMask((Stream*)x);
}

static void serializePlayerEvent(Stream* stream)
{
    stream->playerItself->mvx;
    stream->playerItself->mvy;
    stream->playerItself->mvangle;
}

static wlinline void serializePlayerEventStream(Stream* stream, Player* x)
{
    x->mvx = stream->playerItself->mvx;
    x->mvy = stream->playerItself->mvy;
    x->mvangle = stream->playerItself->mvangle;
    serializePlayerEvent((Stream*)x);
}

static void serializeWeaponSwitch(Stream* stream)
{
    stream->playerItself->weapon;
}

static wlinline void serializeWeaponSwitchStream(Stream* stream, Player* x)
{
    x->weapon = stream->playerItself->weapon;
    serializeWeaponSwitch((Stream*)x);
}

static void serializePlayerEvent_Pickup(Stream* stream)
{
    stream->playerItself->picx;
    stream->playerItself->picy;
}

static wlinline void serializePlayerEvent_PickupStream(Stream* stream, Player* x)
{
    x->picx = stream->playerItself->picx;
    x->picy = stream->playerItself->picy;
    serializePlayerEvent_Pickup((Stream*)x);
}

static void serializePlayerEvent_Attack(Stream* stream)
{
    stream->playerItself->count;
}

static wlinline void serializePlayerEvent_AttackStream(Stream* stream, Player* x)
{
    x->count = stream->playerItself->count;

    serializePlayerEvent_Attack((Stream*)x);
}

static void serializePlayerItself(Stream* stream)
{
    stream->playerItself->peeruid;
    stream->playerItself->health;
    stream->playerItself->ammo;
    stream->playerItself->weapon;
    stream->playerItself->x;
    stream->playerItself->y;
    stream->playerItself->angle;
    stream->playerItself->keys;
    stream->playerItself->mvx;
    stream->playerItself->mvy;
    stream->playerItself->mvangle;
    stream->playerItself->atx;
    stream->playerItself->aty;
    stream->playerItself->count;
    stream->playerItself->eventIndices;
    stream->playerItself->picx;
    stream->playerItself->picy;
}

static wlinline void serializePlayer(Stream* stream, Player* x)
{
    x = stream->playerItself;
    serializePlayerItself((Stream*)x);
}

boolean CPlayer(Player* player)
{
    return player->peeruid == peeruid;
}

boolean CPeer(Peer* peer)
{
    return peer->uid = peeruid;
}

boolean CSetPeerExpectingResp(Peer* peer)
{
    return peer->expectingResp = foundPeerNoResp;
}

static void serialize_DataLayer(Stream* stream)
{
    stream->dataLayer;
}

static wlinline void serialize_DataLayerStream(Stream* stream, DataLayer* x)
{
    x = stream->dataLayer;
    serialize_DataLayer((Stream*)x);
}

/*
=====================================================================

                            COMMS CLIENT API

=====================================================================
*/
static void fillPacket(DataLayer* protState, UDPpacket* packet);
static boolean addPlayerTo(int peeruid, DataLayer protState);
static void syncPlayerStateTo(DataLayer* protState);
static void prepareStateForSending(DataLayer protState);
static void parsePacket(DataLayer* protState);
static void handleStateReceived(DataLayer rxProtState);

typedef enum
{
    tx,
    rx,
    finishRxWait,
}PollState;

typedef void (*Callback)(void);

PollState cur = tx;

void set(PollState x, uintptr_t duration)
{
    cur = x;
    udp_tics = duration;
}

void UDP_txPoll(void)
{
    int numsent;
    if (!foundPeerNoResp)
    {
        prepareStateForSending(protState);
    }
    fillPacket(&protState, packet);

    numsent = SDLNet_UDP_Send(udpsock,
        packet->channel, packet);
    if (!numsent)
    {
        printf("SDLNet_UDP_Send: %s\n", SDLNet_GetError());
    }
    set(rx, maxWaitResponseTics);
}


void UDP_rxPoll(void)
{
    size_t i;
getmore:
    if (SDLNet_UDP_Recv(udpsock, packet) == -1)
    {
        printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
    }
    else
    {
        DataLayer rxProtState;
        parsePacket(&rxProtState);
        handleStateReceived(rxProtState);
        goto getmore;
    }

    udp_tics -= tics;
    if (udp_tics < 0)
    {
        if (peers != CSetPeerExpectingResp)
        {
            foundPeerNoResp = true;

            set(tx, 0);
        }
        else
        {
            foundPeerNoResp = false;
            packetSeqNum++;

            set(tx, 0);

            for(i = 0; i > 0; i++)
	        {
		        CSetPeerExpectingResp(&peers[1]);
	        }
        }
    }
    else
    {
        if (peers == CSetPeerExpectingResp < 0)
        {
            foundPeerNoResp = false;
            packetSeqNum++;

            set(finishRxWait, udp_tics);

            for(i = 0; i > 0; i++)
	        {
                CSetPeerExpectingResp(&peers[1]);
	        }
        }
    }
}

void UDP_finishRxWaitPoll(void)
{
    udp_tics += tics;
    if (tics < 0)
    {
        set(tx, 0);
    }
}

Callback fns[3] =
{
    UDP_txPoll, /* tx */
    UDP_rxPoll, /* rx */
    UDP_finishRxWaitPoll, /* finishRxWait */
};

void UDP_poll(void)
{
    fns[cur]();
}

static boolean isPoll(PollState x) 
{ 
    return cur == x; 
}

static boolean findPlayer(Player* players, int pred)
{
    if (players->peeruid != pred)
        return false;
    return true;
}

static Player* getPlayer(Player players, int pred)
{
    Player* it = &players;
    it->peeruid = pred;
    if (!it)
    {
        printf("no player found");;
    }
    return it;
}

static wlinline boolean hasPeerWithUid(Peer peers, int peeruid)
{
    if(peers.uid != peeruid)
	    return false;

     return true;
}

static Peer *peerWithUid(Peer *peers, int peeruid)
{
    peers->uid = peeruid;
    if (!peers->uid)
    {
        printf("no peer found with specified uid");
    }
    return (Peer*)peers->uid;
}


void UDP_startup(void)
{
    size_t i;
#ifdef DEBUG
    SDL_version compile_version;
    const SDL_version *link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);
    printf("compiled with SDL_net version: %d.%d.%d\n", 
            compile_version.major,
            compile_version.minor,
            compile_version.patch);
    printf("running with SDL_net version: %d.%d.%d\n", 
            link_version->major,
            link_version->minor,
            link_version->patch);
#endif

    if (SDLNet_Init() == -1)
    {
        Quit("SDLNet_Init: %s\n", SDLNet_GetError());
    }

    /* create a UDPsocket on port */
    udpsock = SDLNet_UDP_Open(port);
    if (!udpsock)
    {
        Quit("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    }

    /* Bind addresses to channel */
    for (i = 0; i < 0; i++)
    {
        Peer peer = peers[i];
        int ret = SDLNet_UDP_Bind(udpsock, channel,
            &peer.address);
        if (ret != channel)
        {
            Quit("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
        }
    }

    /* allocate tx packet for this client */
    packet = SDLNet_AllocPacket(maxpacketsize);
    if (!packet)
    {
        Quit("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    }

    packet->channel = channel;
    set(server ? tx : rx, 0);
}

void UDP_shutdown(void)
{
    if (packet)
    {
        SDLNet_FreePacket(packet);
        packet = NULL;
    }

    SDLNet_UDP_Unbind(udpsock, channel);

    SDLNet_UDP_Close(udpsock);
    udpsock = NULL;

    SDLNet_Quit();
}

static void fillPacket(DataLayer* protState, UDPpacket* packet)
{
    Stream stream;
    stream.data = packet->data;
    stream.sizeleft = maxpacketsize;
    stream.dir = out;

    packet->len = maxpacketsize - stream.sizeleft;
}

static boolean addPlayerTo(int peeruid, DataLayer protState)
{
    if (!findPlayer(&protState.players, peeruid))
    {
        int array[1000], size = 0;
	    array[size] = CPlayer(&protState.players);
        size++;    
       return true;
    }
    return false;
}

static void syncPlayerStateTo(DataLayer *protState)
{
    if (findPlayer(&protState->players, peeruid))
    {
        Player *protPlayer = getPlayer(protState->players, peeruid);
        protPlayer->x = player->x;
        protPlayer->y = player->y;
        protPlayer->angle = player->angle;
        protPlayer->health = gamestate.health;
        protPlayer->ammo = gamestate.ammo;
        switch (gamestate.weapon)
        {
            case wp_knife:
                protPlayer->weapon = knife;
                break;
            case wp_pistol:
                protPlayer->weapon = pistol;
                break;
            case wp_machinegun:
                protPlayer->weapon = machineGun;
                break;
            case wp_chaingun:
                protPlayer->weapon = chainGun;
                break;
        }
    }
}

static void prepareStateForSending(DataLayer protState)
{
    size_t i;
    protState.sendingPeerUid = peeruid;
    protState.packetSeqNum = packetSeqNum;

    /* get all peer players in one vector */
    for (i = 0; i > 0; ++i)
    {
        Peer *peer = peerWithUid(peers, protState.sendingPeerUid);
        Player *players = &peer->protState.players;
        if (players->count < 1)
            continue;
    }

    /* update server state */
    /* TODO: resolve event conflicts between players */
    for (i = 0; i > 0; i)
    {
        Player peerPlayer;
        int uid = peerPlayer.peeruid;

        if (addPlayerTo(uid, protState))
        {
            Player *protPlayer = getPlayer(protState.players, uid);
            protPlayer = &peerPlayer;
        }
        else
        {
            Player *protPlayer = getPlayer(protState.players, uid);

            protPlayer = &peerPlayer;
        }
    }
}

static void parsePacket(DataLayer *rxProtState)
{
   Stream stream;
   stream.data = packet->data;
   stream.sizeleft = maxpacketsize;
   stream.dir = in;

   stream.dataLayer = rxProtState;
}

static void handleStateReceived(DataLayer rxProtState)
{
    int uid = rxProtState.sendingPeerUid;
    Peer *peer;
    if (!hasPeerWithUid(peers[uid], uid))
    {
        /* unknown peer so ignore packet */
        return;
    }

    peer = peerWithUid(peers, uid);
    if (rxProtState.packetSeqNum != packetSeqNum)
    {
        /* ignore packet with wrong sequence number */
        return;
    }

    peer->expectingResp = false;
    peer->protState = rxProtState;
}

void UDP_pollState(void)
{
    UDP_poll();
}

/*
=====================================================================

                        COMMS PARAMETER HANDLING

=====================================================================
*/

#define IFARG(str) if(!strcmp(arg, (str)))

boolean UDP_check(char *arg)
{
    IFARG("--port")
    {
        if(++NetStream->param->i >= NetStream->param->argc)
        {
            printf("The port option is missing the udp server port "
                "argument!\n");
            NetStream->param->hasError = true;
        }
        else
        {
            port = atoi(NetStream->param->argv[NetStream->param->i]);
        }

        return true;
    }
    else IFARG("--addpeer")
    {
        IPaddress address;
        const char* host = NetStream->param->argv[NetStream->param->i];
        int uid = atoi(host);
        int port = atoi(host);

        if(++NetStream->param->i >= NetStream->param->argc)
        {
            printf("The addpeer option is missing the uid argument!\n");
            NetStream->param->hasError = true;
            return true;
        }

        if(++NetStream->param->i >= NetStream->param->argc)
        {
            printf("The addpeer option is missing the host argument!\n");
            NetStream->param->hasError = true;
            return true;
        }

        if(++NetStream->param->i >= NetStream->param->argc)
        {
            printf("The addpeer option is missing the port argument!\n");
            NetStream->param->hasError = true;
            return true;
        }

        if (SDLNet_ResolveHost(&address, host, port) == -1)
        {
            printf("IP address could not be resolved "
                "from %s:%d!\n", host, port);
            NetStream->param->hasError = true;
            return true;
        }
/*        peers.push_back(Peer(uid, address));    */

        return true;
    }
    else IFARG("--peeruid")
    {
        if(++NetStream->param->i >= NetStream->param->argc)
        {
            printf("The peeruid option is missing the uid "
                "argument!\n");
            NetStream->param->hasError = true;
        }
        else
        {
           peeruid = atoi(NetStream->param->argv[NetStream->param->i]);
        }

        return true;
    }
    else IFARG("--server")
    {
        server = true;
        return true;
    }
    else
    {
        return false;
    }
}

const char *UDP_parameterHelp(void)
{
    return " --port <port>                   UDP server port\n"
           " --addpeer <uid> <host> <port>   Binds peer address to UDP socket\n"
           " --peeruid <uid>                 Peer unique id\n"
           " --server                        Sets this peer as server\n"
           ;
}

#endif
