// ID_UDP.C

/*
=============================================================================
ID_UDP.C NOTE:
This file was ported to C by me André Guilherme to match the Wolf4SDL Coding
standards, this is an very early stage and you may be can find bugs.
You will need a SDL_net library to make this file work properly
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
#include <SDL_net.h>

typedef struct Peer_s
{
    //struct Peer_s Vec;
    int uid;
    IPaddress address;
    DataLayer protState;
    boolean expectingResp;
}Peer;

typedef struct 
{
    int uid;
}PeerHasUid;

typedef struct 
{
    boolean expectingResp;
}SetPeerExpectingResp;

/*
=============================================================================

                              LOCAL VARIABLES

=============================================================================
*/

Stream* NetStream;
Parameter* Comms_param;

const int channel = 0;
const int maxpacketsize = 1024;
const int maxWaitResponseTics = 30;

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


const boolean CSetPeerExpectingResp(Peer* peer);

const boolean CPlayer(const Player* player);

const boolean CPeer(Peer* peer);



/*
=====================================================================

                            SERIALIZERS

=====================================================================
*/
void serializeRaw(void* p, size_t sz)
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

void serializeInt(int* x)
{
    serializeRaw(x, sizeof(x));
}

void serializeShort(short* x)
{
    serializeRaw(x, sizeof(x));
}

void serializeUnsignedChar(unsigned char* x)
{
    serializeRaw(x, sizeof(x));
}

void serializeUnsignedInt(unsigned int* x)
{
    serializeRaw(x, sizeof(x));
}

void serializeUnsignedLong(unsigned long* x)
{
    serializeRaw(x, sizeof(x));
}

void serializeEnum(Stream* stream, int *x)
{
    x = stream;
    serializeInt(x);
}

inline void serializeStreamInt(Stream* stream, int* x)
{
    x = stream;
    serializeInt(x);
}

inline void serializeStreamShort(Stream* stream, short* x)
{
    x = stream;
    serializeShort(x);
}

inline void serializeStreamUnsignedChar(Stream* stream, unsigned char* x)
{
    x = stream;
    serializeUnsignedChar(x);
}

inline void serializeStreamUnisgnedInt(Stream* stream, unsigned int* x)
{
    x = stream;
    serializeUnsignedInt(x);
}

inline void serializeStreanUnsignedLong(Stream* stream, unsigned long* x)
{
    x = stream;
    serializeUnsignedLong(x);
}

inline void serializeWeaponEnum(Stream* stream, Weapon* x)
{
    serializeEnum(stream, x);
}

inline void serializeKey(Stream* stream, Key* x)
{
    serializeEnum(stream, x);
}


void add(int x)
{
    NetStream->set->mask |= (1 << x);
}

const boolean has(int x)
{
    return (NetStream->set->mask & (1 << x)) != 0;
}

void serializeMask(Stream* stream)
{
    stream->set->mask;
}

inline void serializeSet(Stream* stream, Set* x)
{

    x = stream->set;
    serializeMask(x);
}

void serializePlayerEvent(Stream* stream)
{
    stream->move->x;
    stream->move->y;
    stream->move->angle;
}

inline void serializePlayerEventStream(Stream* stream, Move* x)
{
    x = stream->move;
    serializePlayerEvent(x);
}

void serializeWeaponSwitch(Stream* stream)
{
    stream->weaponSwitch;
}

inline void serializeWeaponSwitchStream(Stream* stream, WeaponSwitch* x)
{
    x = stream->weaponSwitch;
    serializeWeaponSwitch(x);
}

void serializePlayerEvent_Pickup(Stream* stream)
{
    stream->pick->x;
    stream->pick->y;
}

inline void serializePlayerEvent_PickupStream(Stream* stream, Pickup* x)
{
    x = stream->pick;
    serializePlayerEvent_Pickup(x);
}

void serializePlayerEvent_Attack(Stream* stream)
{
    stream->attack->count;
    stream->attack->peeruid;
}

inline void serializePlayerEvent_AttackStream(Stream* stream, Attack* x)
{
    x = stream->attack;
    serializePlayerEvent_Attack(x);
}

void serializePlayerItself(Stream* stream)
{
    stream->playerItself->peeruid;
    stream->playerItself->health;
    stream->playerItself->ammo;
    stream->playerItself->weapon;
    stream->playerItself->x;
    stream->playerItself->y;
    stream->playerItself->angle;
    stream->playerItself->keys;
    stream->playerItself->moveEvents;
    stream->playerItself->weaponSwitchEvents;
    stream->playerItself->pickupEvents;
    stream->playerItself->attackEvents;
    stream->playerItself->eventIndices;
}

inline void serializePlayer(Stream* stream, Player* x)
{
    x = stream->playerItself;
    serializePlayerItself(x);
}

const boolean CPlayer(const Player* player)
{
    return player->hasuid == peeruid;
}


const boolean CPeer(Peer* peer)
{
    return peer->uid = peeruid;
}

const boolean CSetPeerExpectingResp(Peer* peer)
{
    return peer->expectingResp = foundPeerNoResp;
}

void serialize_DataLayer(Stream* stream)
{
    stream->dataLayer->sendingPeerUid;
    stream->dataLayer->packetSeqNum;
    stream->dataLayer->players;
}

inline void serialize_DataLayerStream(Stream* stream, DataLayer* x)
{
    x = stream->dataLayer;
    serialize_DataLayer(x);
}

/*
=====================================================================

                            COMMS CLIENT API

=====================================================================
*/
#ifdef WIP
void UDP_txPoll(void);
void UDP_rxPoll(void);
#endif
void UDP_finishRxWaitPoll(void);
void UDP_poll(void);

void fillPacket(DataLayer* protState, UDPpacket* packet);
#ifdef WIP
boolean addPlayerTo(int peeruid, DataLayer& protState);
void syncPlayerStateTo(DataLayer& protState);
void prepareStateForSending(DataLayer& protState);
#endif
void parsePacket(DataLayer* protState);
void handleStateReceived(DataLayer* rxProtState);

typedef enum Poll
{
#ifdef WIP
   tx,
   rx,
#endif
   finishRxWait,
}PollState;

typedef void (*Callback)(void);

Callback fns[] =
{
#ifdef WIP
    UDP_txPoll, // tx
    UDP_rxPoll, // rx
#endif
    UDP_finishRxWaitPoll, // finishRxWait
};


//TODO: PollState cur = tx;
PollState cur = finishRxWait;

int udp_tics = 0;

void set(PollState x, int duration)
{
     cur = x;
     udp_tics = duration;
}

void UDP_poll(void)
{
     fns[cur]();
}

boolean is(PollState x) 
{ 
    return cur == x; 
}

/*
    inline boolean hasPeerWithUid(Peer::Vec &peers, int peeruid)
    {
        return std::find_if(peers.begin(), peers.end(),
            PeerHasUid(peeruid)) != peers.end();
    }

    Peer &peerWithUid(Peer::Vec &peers, int peeruid)
    {
        Peer::Vec::iterator it = std::find_if(peers.begin(),
            peers.end(), PeerHasUid(peeruid));
        if (it == peers.end())
        {
            printf("no peer found with specified uid");
        }
        return *it;
    }


*/
void UDP_startup(void)
{
    size_t i = 0;
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

    if (SDLNet_Init() == -1)
    {
        Quit("SDLNet_Init: %s\n", SDLNet_GetError());
    }

    // create a UDPsocket on port
    udpsock = SDLNet_UDP_Open(port);
    if (!udpsock)
    {
        Quit("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    }

    // Bind addresses to channel
    for (i; i < 0; i++)
    {
        Peer peer = peers[i];

        const int ret = SDLNet_UDP_Bind(udpsock, channel,
            &peer.address);
        if (ret != channel)
        {
            Quit("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
        }
    }

    // allocate tx packet for this client
    packet = SDLNet_AllocPacket(maxpacketsize);
    if (!packet)
    {
        Quit("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    }

    packet->channel = channel;
#ifdef WIP
    set(server ? tx : rx, 0);
#else 
    set(server == finishRxWait, 0);
#endif
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
/*
boolean addPayerTo(int peeruid, DataLayer* protState)
{
    if (!findPlayer(protState-players, PlayerHasPeerUid(peeruid)))
    {
        protState.players.push_back(Player(peeruid));
        return true;
    }

    return false;
}

void syncPlayerStateTo(DataLayer *protState)
{
    if (findPlayer(protState->players, PlayerHasPeerUid(peeruid)))
    {
        Player *protPlayer = getPlayer(protState->players,
            PlayerHasPeerUid(peeruid));
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


void prepareStateForSending(DataLayer &protState)
{
    protState.sendingPeerUid = peeruid;
    protState.packetSeqNum = packetSeqNum;

    // get all peer players in one vector
    Player::Vec peerPlayers;
    for (Peer::Vec::iterator it = peers.begin(); it != peers.end(); ++it)
    {
        Peer &peer = *it;
        Player::Vec &players = peer.protState.players;
        if (players.size() < 1)
            continue;

        peerPlayers.push_back(players[0]);
    }

    // update server state
    // TODO: resolve event conflicts between players
    for (Player::Vec::size_type i = 0; i < peerPlayers.size(); i++)
    {
        Player &peerPlayer = peerPlayers[i];
        const int uid = peerPlayer.peeruid;

        if (addPlayerTo(uid, protState))
        {
            Player &protPlayer = getPlayer(protState.players,
                PlayerHasPeerUid(uid));
            protPlayer = peerPlayer;
        }
        else
        {
            Player &protPlayer = getPlayer(protState.players,
                PlayerHasPeerUid(uid));

            protPlayer = peerPlayer;
        }
    }
}
*/
void fillPacket(DataLayer *protState, UDPpacket *packet)
{
    Stream stream;
    stream.data = packet->data;
    stream.sizeleft = maxpacketsize;
    stream.dir = out;

    packet->len = maxpacketsize - stream.sizeleft;
}

void parsePacket(DataLayer *rxProtState)
{
   Stream stream;
   stream.data = packet->data;
   stream.sizeleft = maxpacketsize;
   stream.dir = in;

   stream.dataLayer = rxProtState;
}

#ifdef WIP 

void handleStateReceived(DataLayer &rxProtState)
{
    const int uid = rxProtState.sendingPeerUid;

    if (!hasPeerWithUid(peers, uid))
    {
        // unknown peer so ignore packet
        return;
    }

    Peer &peer = peerWithUid(peers, uid);
    if (rxProtState.packetSeqNum != packetSeqNum)
    {
        // ignore packet with wrong sequence number
        return;
    }

    peer.expectingResp = false;
    peer.protState = rxProtState;
}

void UDP_txPoll(void)
{
    int numsent;
    if (!foundPeerNoResp)
    {
        prepareStateForSending(protState);
    }
    fillPacket(protState, packet);

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
getmore:
    int numrecv = SDLNet_UDP_Recv(udpsock, packet);
    if (numrecv == -1)
    {
        printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
    }
    else if (numrecv)
    {
        DataLayer rxProtState;
        parsePacket(rxProtState);
        handleStateReceived(rxProtState);

        goto getmore;
    }

    udp_tics -= tics;
    if (udp_tics < 0)
    {
        if (std::find_if(peers.begin(), peers.end(),
            PeerIsExpectingResp()) != peers.end())
        {
            foundPeerNoResp = true;

            set(tx, 0);
        }
        else
        {
            foundPeerNoResp = false;
            packetSeqNum++;

            set(tx, 0);

            std::for_each(peers.begin(), peers.end(),
                SetPeerExpectingResp(true));
        }
    }
    else
    {
        if (std::find_if(peers.begin(), peers.end(),
            PeerIsExpectingResp()) == peers.end())
        {
            foundPeerNoResp = false;
            packetSeqNum++;

            set(finishRxWait, udp_tics);

            std::for_each(peers.begin(), peers.end(),
                SetPeerExpectingResp(true));
        }
    }
}
#endif
void UDP_finishRxWaitPoll(void)
{
    udp_tics += tics;
    if (tics < 0)
    {
#ifdef WIP
        set(tx, 0);
#endif
    }
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

boolean UDP_check(const char *arg)
{
    IFARG("--port")
    {
        if(++Comms_param->i >= Comms_param->argc)
        {
            printf("The port option is missing the udp server port "
                "argument!\n");
            Comms_param->hasError = true;
        }
        else
        {
            port = atoi(Comms_param->argv[Comms_param->i]);
        }

        return true;
    }
    else IFARG("--addpeer")
    {
        IPaddress address;
        const int uid = atoi(Comms_param->argv[Comms_param->i]);
        const char* host = Comms_param->argv[Comms_param->i];
        const int port = atoi(Comms_param->argv[Comms_param->i]);

        if(++Comms_param->i >= Comms_param->argc)
        {
            printf("The addpeer option is missing the uid argument!\n");
            Comms_param->hasError = true;
            return true;
        }

        if(++Comms_param->i >= Comms_param->argc)
        {
            printf("The addpeer option is missing the host argument!\n");
            Comms_param->hasError = true;
            return true;
        }

        if(++Comms_param->i >= Comms_param->argc)
        {
            printf("The addpeer option is missing the port argument!\n");
            Comms_param->hasError = true;
            return true;
        }

        if (SDLNet_ResolveHost(&address, host, port) == -1)
        {
            printf("IP address could not be resolved "
                "from %s:%d!\n", host, port);
            Comms_param->hasError = true;
            return true;
        }

        //peers.push_back(Peer(uid, address));

        return true;
    }
    else IFARG("--peeruid")
    {
        if(++Comms_param->i >= Comms_param->argc)
        {
            printf("The peeruid option is missing the uid "
                "argument!\n");
            Comms_param->hasError = true;
        }
        else
        {
           peeruid = atoi(Comms_param->argv[Comms_param->i]);
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