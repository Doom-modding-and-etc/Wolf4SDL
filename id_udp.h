#ifndef __ID_UDP__
#define __ID_UDP__

#ifdef LWUDPCOMMS 
#include <stdint.h>
#include <stdio.h>

/*
=====================================================================

                            COMMS CLIENT API

=====================================================================
*/

void UDP_startup(void);

void UDP_shutdown(void);

void UDP_pollState(void);


/*
=====================================================================

                        COMMS PARAMETER HANDLING

=====================================================================
*/

typedef struct
{
   char **argv[1];
   const int argc;
   int i;
   boolean hasError;
   boolean showHelp;
} Parameter;
extern Parameter *Comms_param;
boolean UDP_check(const char* arg);

const char *UDP_parameterHelp(void);


/*
=====================================================================

                           COMMS PROTOCOL

=====================================================================
*/

typedef enum StreamDirection
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
    int mask;
} Set;

typedef struct PlayerEvent_Movement
{
    //struct PlayerEvent_Movement Vec;
    int x, y;
    short angle;
} Move;

typedef struct PlayerEvent_WeaponSwitch
{
    //struct PlayerEvent_WeaponSwitch Vec;
    Weapon weapon;
} WeaponSwitch;

typedef struct PlayerEvent_Pickup
{
    //struct PlayerEvent_Pickup Vec;
    unsigned char x, y;
} Pickup;

typedef struct PlayerEvent_Attack
{
    //struct PlayerEvent_Attack Vec;
    unsigned char count;
    int peeruid;
} Attack;

typedef struct PlayerEvent_HasPeerUid
{
    int peeruid;
} HasPeerUid;

typedef struct PlayerEvent_PlayerItself
{
    //struct PlayerEvent_PlayerItself Vec;
    int peeruid;
    short health;
    short ammo;
    Weapon weapon;
    int x, y;
    short angle;
    Key keys;
    Move moveEvents;
    WeaponSwitch weaponSwitchEvents;
    Pickup pickupEvents;
    Attack attackEvents;
    unsigned char eventIndices[2];
    HasPeerUid*hasuid;
} Player;

typedef struct
{
    int sendingPeerUid;
    unsigned int packetSeqNum;
    Player *players;
}DataLayer;

typedef struct CommsStream
{
   unsigned char *data;
   size_t sizeleft;
   Direction dir;
   Set* set;
   Parameter* param;
   Move* move;
   WeaponSwitch* weaponSwitch;
   Pickup* pick;
   Attack *attack;
   Player* playerItself;
   DataLayer *dataLayer;
} Stream;

extern Stream *NetStream;

void serializeRaw(void* p, size_t sz);

void serializeInt(int* x);

void serializeShort(short* x);

void serializeUnsignedChar(unsigned char* x);

void serializeUnsignedInt(unsigned int* x);

void serializeUnsignedLong(unsigned long* x);

inline void serializeStreamInt(Stream* stream, int* x);

inline void serializeStreamShort(Stream* stream, short* x);

inline void serializeStreamUnsignedChar(Stream* stream, unsigned char* x);

inline void serializeStreamUnisgnedInt(Stream* stream, unsigned int* x);

inline void serializeStreanUnsignedLong(Stream* stream, unsigned long* x);

void serializeEnum(Stream* stream, int *x);

inline void serializeWeaponEnum(Stream* stream, Weapon* x);

inline void serializeKey(Stream* stream, Key* x);

void add(int x);

const boolean has(int x);

void serializeMask(Stream* stream);

inline void serializeSet(Stream* stream, Set* x);

void serializePlayerEvent(Stream* stream);

inline void serializePlayerEventStream(Stream* stream, Move* x);

void serializeWeaponSwitch(Stream* stream);

inline void serializeWeaponSwitchStream(Stream* stream, WeaponSwitch* x);

void serializePlayerEvent_Pickup(Stream* stream);

inline void serializePlayerEvent_PickupStream(Stream* stream, Pickup* x);

void serializePlayerEvent_Attack(Stream* stream);

inline void serializePlayerEvent_AttackStream(Stream* stream, Attack* x);

void serializePlayerItself(Stream* stream);

inline void serializePlayer(Stream* stream, Player* x);

const boolean CPlayer(const Player* player);

#ifdef WIP
template <class Pred>
boolean findPlayer(Player::Vec& players, Pred pred)
{
    return std::find_if(players.begin(), players.end(), pred) !=
        players.end();
}

template <class Pred>
Player& getPlayer(Player::Vec& players, Pred pred)
{
    Player::Vec::iterator it = std::find_if(players.begin(),
        players.end(), pred);
    if (it == players.end())
    {
        throw "no player found";
    }
    return *it;
}
#endif

void serialize_DataLayer(Stream* stream);

inline void serialize_DataLayerStream(Stream* stream, DataLayer* x);

#endif
#endif
