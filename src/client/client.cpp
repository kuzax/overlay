#include "client.h"
#include <globals.h>
#include <data/tables.h>
#include <data/pattern.h>
#include <data/elements.h>
#include <data/variables.h>
#include <menu/menu.h>
#include <console/console.h>
#include <utilities/utilities.h>
#include "includes.h"
#include "imguis/Imnotif.h"
#include <xor.h>
#include <game/manager/manager.h>
#include <game/aimbot/aimbot.h>
#include <netc/netc.h>
#include <cmath>
#include <Windows.h>
#include <lua.h>
#pragma comment(lib, "kernel32.lib")

#include "xorstr.h"
#include <SlientAim.h>

CClientPlayer* g_pLineTarget = nullptr;
CClientPlayer* selected_player = nullptr;
void* Call_ECX[50000];
std::string Script_Name[50000];
int Script_ID = 0;
void* Call_This[50000];


void __fastcall h_init_client(CStaticFunctionDefinitions* pStatic, void* EDX, CLuaManager* pLuaManager, CEvents* pEvents, CCoreInterface* pCore, CGame* pGame, CClientGame* pClientGame, CClientManager* pManager)
{
    global->m_pLuaManager = pLuaManager;
    global->m_pEvents = pEvents;
    global->m_pCore = pCore;
    global->m_pGame = pGame;
    global->m_pClientGame = pClientGame;    
    global->m_pManager = pManager;
    global->m_pRootEntity = pClientGame->GetRootEntity();
    global->m_pPedManager = pClientGame->GetPedManager();
    global->m_pPlayerManager = pManager->GetPlayerManager();
    global->m_pVehicleManager = pManager->GetVehicleManager();
    global->m_pObjectManager = pManager->GetObjectManager();
    global->m_pExplosionManager = pManager->GetExplosionManager();
    global->m_pProjectileManager = pManager->GetProjectileManager();
    global->m_pWeaponManager = pManager->GetWeaponManager();
    global->m_pResourceManager = pManager->GetResourceManager();    
    
    global->m_pClientCamera = pManager->GetCamera();
    global->m_pMultiPlayer = pCore->GetMultiplayer();    

    client->o_init_client(pStatic, pLuaManager, pEvents, pCore, pGame, pClientGame, pManager);
}


void MH_CreateAndEnableHook(unsigned __int32 TargetAddress, LPVOID pDetour, LPVOID* ppOriginal) {
    MH_CreateHook(reinterpret_cast<LPVOID>(TargetAddress), pDetour, ppOriginal);
    MH_EnableHook(reinterpret_cast<LPVOID>(TargetAddress));
}

bool __fastcall h_on_player_damage(CClientGame* game, void* edx, eWeaponType weaponUsed, float fDamage, unsigned __int8 hitZone, CClientPed* pDamagedPed, CClientEntity* pInflictingEntity, CEventDamage* pEvent)
{
    if (element->player.godmode)
        return false;

    if (element->player.semimode) 
    {

        float randomChance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        if (randomChance < 0.5f)  
        {
            return false;  
        }
    }

    return client->o_on_player_damage(game, weaponUsed, fDamage, hitZone, pDamagedPed, pInflictingEntity, pEvent);
}




void* __fastcall h_resource_constructor(void* ecx, void* edx, unsigned short usNetID, const char* szResourceName, void* pResourceEntity, void* pResourceDynamicEntity, const std::string& strMinServerReq, const std::string& strMinClientReq, bool bEnableOOP)
{
    client->c_resource_manager = ecx;

    void* result = client->o_resource_constructor(ecx, usNetID, szResourceName, pResourceEntity, pResourceDynamicEntity, strMinServerReq, strMinClientReq, bEnableOOP);

    if (result)
    {
        for (s_resource resource : element->resource.resources_list)
        {
            if (strcmp(szResourceName, resource.resource_name.c_str()) == 0)
                return result;
        }

        s_resource resource = {};
        resource.resource_name = szResourceName;
        if (resource.resource_name.find(" (Started)") == std::string::npos)
            resource.resource_name += " (Started)";
        resource.net_id = usNetID; 

        element->resource.resources_list.push_back(resource);
    }

    return result;
}


void* h_take_player_screenshot() {


    if (element->info.Admin_alert)
    {
        ImGui::InsertNotification({ ImGuiToastType::Warning, 5000, _xor_("An Admin tried to get a screenshot of your game!").c_str() });
     
    }

    return client->o_take_player_screenshot();
}


typedef bool(__cdecl* tVehicleCollisionHandler)(CClientVehicle** pCollidingVehicle, CClientEntity* pCollidedEntity, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16_t usPieceType, CVector vecCollisionPos, CVector vecCollisionVelocity);


DWORD  VehColHandler = NULL;

void SyncUnoccupiedData(CClientVehicle* vehicle)
{
    if (!vehicle)
        return;

    CClientPed* localPed = static_cast<CClientPed*>(manager->get_local_player());
    if (!localPed || VehColHandler == NULL)
        return;

    CClientEntity* pedEntity = static_cast<CClientEntity*>(localPed);

    CVector vecPos;
    vehicle->GetPosition(vecPos); 


    tVehicleCollisionHandler callCollision = reinterpret_cast<tVehicleCollisionHandler>(VehColHandler);
    callCollision(&vehicle, pedEntity, localPed->GetModel(), 1.0f, 1.0f, 3, vecPos, CVector(0.2f, 0.0f, 0.0f));
    console->error("worked");
}

typedef void(__thiscall* Blow_t)(CClientVehicle* ECX, VehicleBlowFlags blow);
Blow_t oBlow = nullptr;




void DeleteAllVehicle()
{
    const std::vector<CClientVehicle*>& vehicles = manager->get_vehicles();
    CClientPed* localPed = static_cast<CClientPed*>(manager->get_local_player());

    if (!localPed)
        return;

    CVector myPos;
    localPed->GetPosition(myPos); 

    myPos.fZ += 1.5f;

    for (CClientVehicle* vehicle : vehicles)
    {
        if (!vehicle || !vehicle->GetGameVehicle())
            continue;

        CVector kuzaxpos(0.0f, 0.0f, 0.0f);
        SyncUnoccupiedData(vehicle);
        vehicle->SetPosition(kuzaxpos);
        SyncUnoccupiedData(vehicle);
    }
}

void TeleportAllVehicle()
{
    const std::vector<CClientVehicle*>& vehicles = manager->get_vehicles();
    CClientPed* localPed = static_cast<CClientPed*>(manager->get_local_player());

    if (!localPed)
        return;

    CVector myPos;
    localPed->GetPosition(myPos);

    myPos.fZ += 1.5f;

    for (CClientVehicle* vehicle : vehicles)
    {
        if (!vehicle || !vehicle->GetGameVehicle())
            continue;

        CVehicle* Test = vehicle->GetGameVehicle();
        SyncUnoccupiedData(vehicle);
        vehicle->SetPosition(myPos);
        SyncUnoccupiedData(vehicle);
    }
}


void WriteCameraOrientation32(const CVector& vecPositionBase, NetBitStreamInterface& BitStream)
{
    // Calc the camera position and rotation
    CMatrix camMatrix;
    global->m_pGame->GetCamera()->GetMatrix(&camMatrix);
    const CVector& vecCamPosition = camMatrix.vPos;
    const CVector& vecCamFwd = camMatrix.vFront;
    float          fCamRotZ = atan2(vecCamFwd.fX, vecCamFwd.fY);
    float          fCamRotX = atan2(vecCamFwd.fZ, DistanceBetweenPoints2D(CVector(), vecCamFwd));

    //
    // Write rotations
    //
    const float         fPI = 3.14159265f;
    SFloatAsBitsSync<8> rotation(-fPI, fPI, false);

    rotation.data.fValue = fCamRotZ;
    BitStream.Write(&rotation);

    rotation.data.fValue = fCamRotX;
    BitStream.Write(&rotation);

    //
    // Write offset
    //
    // Calc the camera offset from the base position
    CVector vecCamOffset = vecPositionBase - vecCamPosition;

    // Calc biggest value
    float fMaxRelValue = std::max(abs(vecCamOffset.fX), std::max(abs(vecCamOffset.fY), abs(vecCamOffset.fZ)));

    // Calc biggest value if used absolute position
    float fMaxAbsValue = std::max(abs(vecCamPosition.fX), std::max(abs(vecCamPosition.fY), abs(vecCamPosition.fZ)));

    // Choose which one
    bool    bUseAbsolutePosition;
    CVector vecUsePosition;
    float   fUseMaxValue;

    if (fMaxAbsValue < fMaxRelValue)
    {
        bUseAbsolutePosition = true;
        vecUsePosition = vecCamPosition;
        fUseMaxValue = fMaxAbsValue;
    }
    else
    {
        bUseAbsolutePosition = false;
        vecUsePosition = vecCamOffset;
        fUseMaxValue = fMaxRelValue;
    }

    // See how many bits we need ( using step size of 1.0f )
    struct
    {
        uint  uiNumBits;
        float fRange;
    } bitCountTable[4] = {
        {3, 4.0f},                // 3 bits is +-4        12 bits total
        {5, 16.0f},               // 5 bits is +-16       18 bits total
        {9, 256.0f},              // 9 bits is +-256      30 bits total
        {14, 8192.0f},            // 14 bits is +-8192    45 bits total
    };
    char idx;
    for (idx = 0; idx < 3; idx++)
    {
        if (bitCountTable[idx].fRange > fUseMaxValue)
            break;            // We have enough bits now
    }
    const uint  uiNumBits = bitCountTable[idx].uiNumBits;
    const float fRange = bitCountTable[idx].fRange;

    // Write flag
    BitStream.WriteBit(bUseAbsolutePosition);

    // Write table look up for num of bits
    BitStream.WriteBits(&idx, 2);

    // Write each component
    SFloatAsBitsSyncBase position(uiNumBits, -fRange, fRange, false);

    position.data.fValue = vecUsePosition.fX;
    BitStream.Write(&position);

    position.data.fValue = vecUsePosition.fY;
    BitStream.Write(&position);

    position.data.fValue = vecUsePosition.fZ;
    BitStream.Write(&position);
}



typedef void(__thiscall* t_add_interpolation)(void* ecx, const CVector* vecPosition);
t_add_interpolation o_add_interpolation;

void RotationTest()
{
    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;
    CClientPed* entity = static_cast<CClientPed*>(local);
}



bool SendFakeDead(float health)
{
    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();

    if (bitStream)
    {
        SFullKeysyncSync     keys;
        SPlayerPuresyncFlags flags;
        SPositionSync        positionSync(false);
        SPedRotationSync     rotationSync;
        SVelocitySync        velocitySync;
        SWeaponSlotSync      WeaponSlotSync;
        SPlayerHealthSync    healthSync;
        SPlayerArmorSync     armorSync;
        SCameraRotationSync  camRotationSync;




        
        healthSync.data.fValue = health;



        bitStream->Write(unsigned char(0));
        bitStream->Write(&healthSync);



        bitStream->WriteBit(false);

        bool result = global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, bitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bitStream);
        return result;
    }

    return false;
}



bool SendFakeJetpack(bool fakejetpack)
{
    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();

    if (bitStream)
    {
        SFullKeysyncSync     keys;
        SPlayerPuresyncFlags flags;
        SPositionSync        positionSync(false);
        SPedRotationSync     rotationSync;
        SVelocitySync        velocitySync;
        SWeaponSlotSync      WeaponSlotSync;
        SPlayerHealthSync    healthSync;
        SPlayerArmorSync     armorSync;
        SCameraRotationSync  camRotationSync;



        flags.data.bHasJetPack = fakejetpack;


        

        bitStream->Write(unsigned char(0));

        bitStream->Write(&flags);


        bitStream->WriteBit(false);

        bool result = global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, bitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bitStream);
        return result;
    }

    return false;
}


bool SendFakeDuck(bool bIsDucked)
{
    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();

    if (bitStream)
    {
        SFullKeysyncSync     keys;
        SPlayerPuresyncFlags flags;
        SPositionSync        positionSync(false);
        SPedRotationSync     rotationSync;
        SVelocitySync        velocitySync;
        SWeaponSlotSync      WeaponSlotSync;
        SPlayerHealthSync    healthSync;
        SPlayerArmorSync     armorSync;
        SCameraRotationSync  camRotationSync;


        flags.data.bIsDucked = bIsDucked;
        
       

        bitStream->Write(unsigned char(0));
        bitStream->Write(&flags);

        bitStream->WriteBit(false);

        bool result = global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, bitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bitStream);
        return result;
    }

    return false;
}

bool SendAttack(const CVector& position)
{
    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();

    if (bitStream)
    {
        SFullKeysyncSync     keys;
        SPlayerPuresyncFlags flags;
        SPositionSync        positionSync(false);
        SPedRotationSync     rotationSync;
        SVelocitySync        velocitySync;
        SWeaponSlotSync      WeaponSlotSync;
        SPlayerHealthSync    healthSync;
        SPlayerArmorSync     armorSync;
        SCameraRotationSync  camRotationSync;

        
        flags.data.bAkimboTargetUp = false;
        flags.data.bHasAWeapon = false;
        flags.data.bHasContact = false;
        flags.data.bHasJetPack = false;
        flags.data.bIsChoking = false;
        flags.data.bIsDucked = false;
        flags.data.bIsInWater = false;
        flags.data.bIsOnFire = false;
        flags.data.bStealthAiming = false;
        flags.data.bWearsGoogles = false;
        flags.data.bIsOnGround = true;
        flags.data.bSyncingVelocity = true;
        flags.data2.isReloadingWeapon = false;

        
        positionSync.data.vecPosition = position;


        bitStream->Write(unsigned char(0));
        bitStream->Write(&positionSync);

        WriteCameraOrientation32(position, *bitStream);

        bitStream->WriteBit(false);

        bool result = global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, bitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bitStream);
        return result;
    }

    return false;
}

bool SendPosition(const CVector& position, float rotation, CVector velocity,
    float health, float armor, float camRotation)
{
    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();

    if (bitStream)
    {
        SFullKeysyncSync     keys;
        SPlayerPuresyncFlags flags;
        SPositionSync        positionSync(false);
        SPedRotationSync     rotationSync;
        SVelocitySync        velocitySync;
        SWeaponSlotSync      WeaponSlotSync;
        SPlayerHealthSync    healthSync;
        SPlayerArmorSync     armorSync;
        SCameraRotationSync  camRotationSync;

        
        flags.data.bAkimboTargetUp = false;
        flags.data.bHasAWeapon = true;
        flags.data.bHasContact = false;
        flags.data.bHasJetPack = false;
        flags.data.bIsChoking = false;
        flags.data.bIsDucked = false;
        flags.data.bIsInWater = false;
        flags.data.bIsOnFire = false;
        flags.data.bStealthAiming = true;
        flags.data.bWearsGoogles = false;
        flags.data.bIsOnGround = true;
        flags.data.bSyncingVelocity = false;
        flags.data2.isReloadingWeapon = false;

        
        positionSync.data.vecPosition = position;

        
        rotationSync.data.fRotation = rotation;

        
        velocitySync.data.vecVelocity = velocity;

        
        healthSync.data.fValue = health;

        WeaponSlotSync.data.uiSlot = 1;
        
        armorSync.data.fValue = armor;

        
        camRotationSync.data.fRotation = camRotation;

        bitStream->Write(unsigned char(0));
        bitStream->Write(&keys);
        bitStream->Write(&flags);
        bitStream->Write(&positionSync);
        bitStream->Write(&rotationSync);
        bitStream->Write(&velocitySync);
        bitStream->Write(&healthSync);
        bitStream->Write(&WeaponSlotSync);
        bitStream->Write(&armorSync);
        bitStream->Write(&camRotationSync);


    
        WriteCameraOrientation32(position, *bitStream);

        bitStream->WriteBit(false);

        bool result = global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, bitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bitStream);
        return result;
    }

    return false;
}

bool SendRotation(float rotation, float camRotation)
{
    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();

    if (bitStream)
    {
        SFullKeysyncSync     keys;
        SPlayerPuresyncFlags flags;
        SPositionSync        positionSync(false);
        SPedRotationSync     rotationSync;
        SVelocitySync        velocitySync;
        SWeaponSlotSync      WeaponSlotSync;
        SPlayerHealthSync    healthSync;
        SPlayerArmorSync     armorSync;
        SCameraRotationSync  camRotationSync;


        flags.data.bAkimboTargetUp = false;
        flags.data.bHasAWeapon = true;
        flags.data.bHasContact = false;
        flags.data.bHasJetPack = false;
        flags.data.bIsChoking = false;
        flags.data.bIsDucked = false;
        flags.data.bIsInWater = false;
        flags.data.bIsOnFire = false;
        flags.data.bWearsGoogles = false;
        flags.data.bIsOnGround = true;
        flags.data.bSyncingVelocity = false;
        flags.data2.isReloadingWeapon = false;



        rotationSync.data.fRotation = rotation;




        camRotationSync.data.fRotation = camRotation;

        bitStream->Write(unsigned char(0));
        bitStream->Write(&keys);
        bitStream->Write(&flags);
        bitStream->Write(&rotationSync);
        bitStream->Write(&camRotationSync);




        bitStream->WriteBit(false);

        bool result = global->m_pNet->SendPacket(PACKET_ID_PLAYER_PURESYNC, bitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bitStream);
        return result;
    }

    return false;
}

void OnFootSlapper()
{
    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        return;

    CVector localPos = *local->GetGamePlayer()->GetPosition();

    localPos.fZ += localPos.fZ * 0.86f;

    SendPosition(localPos);
}






std::vector<CClientPlayer*> players; 







void SendInfinityVelocityToPlayer()
{
    CClientEntity* localEntity = static_cast<CClientEntity*>(manager->get_local_player());
    if (!localEntity)
        return;

    CVector position;
    localEntity->GetPosition(position);

    SendPosition(position);
}





bool WarpPlayerIntoVehicle(CClientVehicle* pEntryVehicle, unsigned int uiSeat = 0, bool bJack = true)
{
    CVector       vehPos;
    unsigned char ucAction;
    pEntryVehicle->GetPosition(vehPos);
    SendPosition(vehPos);
    CClientPlayer* pPlayer = global->m_pClientGame->GetPlayerManager()->GetLocalPlayer();
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        pBitStream->Write(pPlayer->GetID());
        pBitStream->Write(pEntryVehicle->GetID());
        ucAction = static_cast<unsigned char>(CClientGame::VEHICLE_REQUEST_IN);
        unsigned char ucSeat = static_cast<unsigned char>(uiSeat);
        unsigned char ucDoor = static_cast<unsigned char>(0);
        pBitStream->WriteBits(&ucAction, 4);
        pBitStream->WriteBits(&ucSeat, 4);
        pBitStream->WriteBit(false);
        pBitStream->WriteBits(&ucDoor, 3);
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_INOUT, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_RELIABLE_ORDERED);
        global->m_pNet->DeallocateNetBitStream(pBitStream);

        pEntryVehicle->GetPosition(vehPos);
        SendPosition(vehPos);
        pBitStream = global->m_pNet->AllocateNetBitStream();
        if (pBitStream)
        {
            pBitStream->Write(pPlayer->GetID());
            pBitStream->Write(pEntryVehicle->GetID());
            ucAction = static_cast<unsigned char>(bJack ? CClientGame::VEHICLE_NOTIFY_JACK : CClientGame::VEHICLE_NOTIFY_IN);
            pBitStream->WriteBits(&ucAction, 4);
            bool result = global->m_pNet->SendPacket(PACKET_ID_VEHICLE_INOUT, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_RELIABLE_ORDERED);
            global->m_pNet->DeallocateNetBitStream(pBitStream);
            return result;
        }
    }

    return false;
}


std::thread fireThread;
std::atomic<bool> fireThreadRunning(false);

void StartFirePlayer(CClientPlayer* selected_player, CClientStreamElement* stream)
{
    if (fireThreadRunning.load())
        return; 

    fireThreadRunning = true;

    fireThread = std::thread([selected_player, stream]()
        {
            FirePlayer(selected_player, stream);

            fireThreadRunning = false;

        });

    fireThread.detach();
}

void FirePlayer(CClientPlayer* pPlayer, CClientStreamElement* pStreamElement)
{
    static bool bAlreadyFirePlayer = false;

    if (!global->m_pCore->IsConnected())
        return;

    if (bAlreadyFirePlayer)
        return;

    CClientPlayer* localPlayer = global->m_pPlayerManager->GetLocalPlayer();

    bAlreadyFirePlayer = true;


            int  iFireAttemptCount = 0;
            bool bFired = true;

            while (!pPlayer->IsOnFire())
            {
                if (iFireAttemptCount > 200)
                {
                    bFired = false;
                    break;
                }
                else
                {
                    if (!pPlayer->IsStreamedIn())
                        client->oSetStream(pStreamElement, true);

                    CClientEntity* entity = static_cast<CClientEntity*>(pPlayer);

                    CVector vecPlayer;
                    entity->GetPosition(vecPlayer);

                    CVector moveSpeed = *pPlayer->GetGamePlayer()->GetMoveSpeed(&moveSpeed);
                    vecPlayer = (vecPlayer + moveSpeed);

                    NetBitStreamInterface* bitStream = global->m_pNet->AllocateNetBitStream();
                    if (bitStream)
                    {
                        SFullKeysyncSync     keys;
                        SPlayerPuresyncFlags flags;
                        SPositionSync        positionSync(false);
                        SPedRotationSync     rotationSync;
                        SVelocitySync        velocitySync;
                        SPlayerHealthSync    healthSync;
                        SPlayerArmorSync     armorSync;
                        SCameraRotationSync  camRotationSync;

                        
                        flags.data.bAkimboTargetUp = false;
                        flags.data.bHasAWeapon = false;
                        flags.data.bHasContact = false;
                        flags.data.bHasJetPack = false;
                        flags.data.bIsChoking = false;
                        flags.data.bIsDucked = false;
                        flags.data.bIsInWater = false;
                        flags.data.bIsOnFire = true;
                        flags.data.bStealthAiming = false;
                        flags.data.bWearsGoogles = false;
                        flags.data.bIsOnGround = true;
                        flags.data.bSyncingVelocity = false;
                        flags.data2.isReloadingWeapon = false;

                        
                        positionSync.data.vecPosition = vecPlayer;

                        
                        velocitySync.data.vecVelocity = CVector(0, 0, 0);

                        bitStream->Write(unsigned char(0));
                        bitStream->Write(&keys);
                        bitStream->Write(&flags);
                        bitStream->Write(&positionSync);
                        bitStream->Write(&rotationSync);
                        bitStream->Write(&velocitySync);
                        bitStream->Write(&healthSync);
                        bitStream->Write(&armorSync);
                        bitStream->Write(&camRotationSync);

                        WriteCameraOrientation32(vecPlayer, *bitStream);

                        bitStream->WriteBit(false);

                        global->m_pNet->SendPacket(
                            PACKET_ID_PLAYER_PURESYNC,
                            bitStream,
                            PACKET_PRIORITY_HIGH,
                            PACKET_RELIABILITY_UNRELIABLE_SEQUENCED
                        );

                        global->m_pNet->DeallocateNetBitStream(bitStream);
                    }

                    iFireAttemptCount++;
                    Sleep(5);
                }
            }

            if (!bFired || !pPlayer->IsOnFire())
            {
                bAlreadyFirePlayer = false;
                return;
            }

            bAlreadyFirePlayer = false;
}



float DistanceBetween(const CVector& a, const CVector& b)
{
    float dx = a.fX - b.fX;
    float dy = a.fY - b.fY;
    float dz = a.fZ - b.fZ;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}



void LaunchVehicleAboveTarget(CClientPlayer* targetPlayer)
{
    if (!targetPlayer)
        return;

    CClientEntity* ent = static_cast<CClientEntity*>(targetPlayer);
    if (!ent)
        return;

    CVector targetPos;
    ent->GetPosition(targetPos);

    auto allVehicles = manager->get_vehicles();
    auto allPlayers = manager->get_players();

    std::vector<CClientVehicle*> occupied;
    for (auto* player : allPlayers)
    {
        if (auto* veh = player->GetOccupiedVehicle())
            occupied.push_back(veh);
    }

    std::vector<CClientVehicle*> nearbyEmpty;
    for (auto* veh : allVehicles)
    {
        if (!veh || std::find(occupied.begin(), occupied.end(), veh) != occupied.end())
            continue;

        CVector pos;
        veh->GetPosition(pos);

        if (DistanceBetween(pos, targetPos) <= 100.0f)
            nearbyEmpty.push_back(veh);
    }

    if (nearbyEmpty.empty())
        return;

    int randomIndex = rand() % nearbyEmpty.size();
    CClientVehicle* selected = nearbyEmpty[randomIndex];

    CVector abovePos = targetPos + CVector(0.0f, 0.0f, -1.0f);
    selected->SetPosition(abovePos);




    SendSkyDropTarget(selected, abovePos, CVector(0.0f, 0.0f, 0.0f)); 
}




typedef bool(__thiscall* towed_vehicle_t)(CClientVehicle* pLocalVehicle, CClientVehicle* pVehicle, CVector* vecRotationDegrees);
towed_vehicle_t towed_vehicle;






void SendVehicleTrailerAttach(CClientVehicle* pTrailer)
{
    CClientPlayer* local = manager->get_local_player();
    CClientVehicle* myVeh = local->GetOccupiedVehicle();

    if (!myVeh || !pTrailer)
        return;

    CVector Degress(0, 0, 0);


    towed_vehicle(myVeh, pTrailer, &Degress);
}


void PullAllVehicleT()
{
    CClientPlayer* local = manager->get_local_player();
    if (!local)
        return;

    CClientVehicle* myVeh = local->GetOccupiedVehicle();
    if (!myVeh)
        return;

    std::vector<CClientPlayer*> players = manager->get_players();
    CVector Degrees(100, 100, 100);

    for (CClientPlayer* pPlayer : players)
    {
        if (!pPlayer || pPlayer == local)
            continue; 

        towed_vehicle(myVeh, reinterpret_cast<CClientVehicle*>(pPlayer), &Degrees);

        
        if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
        {
            bs->WriteBit(true);
            bs->Write(myVeh->GetID());
            bs->Write(pPlayer->GetID()); 

            global->m_pNet->SendPacket(PACKET_ID_VEHICLE_TRAILER, bs,
                PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_RELIABLE_ORDERED);

            global->m_pNet->DeallocateNetBitStream(bs);
        }
    }
}


void SendSkyDropTarget(CClientVehicle* pVehicle, const CVector& targetPos,const CVector& vecMoveSpeed)
{
    if (!pVehicle)
        return;


    CVector angularVelocity(9999999999999999, 0.0f, 0.f);

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        bs->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync v;
        v.data.vehicleID = pVehicle->GetID();
        v.data.ucTimeContext = 0;

        v.data.vecPosition = targetPos;
        v.data.vecRotation = CVector(0, 0, 0);
        v.data.vecVelocity = vecMoveSpeed;
        v.data.vecTurnVelocity = angularVelocity;

        v.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);
        v.data.bSyncPosition = true;
        v.data.bSyncVelocity = true;
        v.data.bSyncRotation = true;
        v.data.bSyncTurnVelocity = true;
        v.data.bSyncHealth = false;
        v.data.bSyncTrailer = false;

        v.data.bEngineOn = true;
        v.data.bDerailed = false;
        v.data.bIsInWater = false;

        bs->Write(&v);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }
}

CClientPlayer* rainTargetPlayer = nullptr;
bool vehicleRainEnabled = false;

void SetRainTarget(CClientPlayer* player)
{
    if (!element->vehicle.vehicle_rain)
        return;

    rainTargetPlayer = player;
    vehicleRainEnabled = (player != nullptr);
}

void VehicleRainLoop()
{
    if (!vehicleRainEnabled || !rainTargetPlayer)
        return;

    if (!element->vehicle.vehicle_rain)
        return;

    VehicleRain(rainTargetPlayer);
}

void VehicleRain(CClientPlayer* targetPlayer) {
    if (!targetPlayer)
        return;

    CVector targetPos;
    CClientEntity* ent = static_cast<CClientEntity*>(targetPlayer);
    if (!ent) return;
    ent->GetPosition(targetPos);


    auto allVehicles = manager->get_vehicles();
    auto allPlayers = manager->get_players();


    std::vector<CClientVehicle*> occupied;
    for (auto* player : allPlayers) {
        if (auto* veh = player->GetOccupiedVehicle())
            occupied.push_back(veh);
    }

    
    std::vector<CClientVehicle*> available;
    for (auto* veh : allVehicles) {
        if (!veh) continue;
        if (std::find(occupied.begin(), occupied.end(), veh) != occupied.end())
            continue;
        available.push_back(veh);
    }

    if (available.empty())
        return;


    CClientVehicle* vehicle = available[0];
    if (!vehicle) return;

    CVector currentPos;
    vehicle->GetPosition(currentPos);

    CVector speed;
    speed.fX = (targetPos.fX - currentPos.fX) * 2.0f;
    speed.fY = (targetPos.fY - currentPos.fY) * 2.0f;
    speed.fZ = (targetPos.fZ - currentPos.fZ) * 2.0f;

    VehicleSendTarget(vehicle, targetPos, speed);
}

void VehRotationDegis()
{
    auto allVehicles = manager->get_vehicles();
    if (allVehicles.empty())
        return;

    CClientPlayer* local = manager->get_local_player();


    CClientVehicle* myVeh = local->GetOccupiedVehicle();


    CVector Rotation = CVector(INFINITE, INFINITE, INFINITE);
    VehRotation(myVeh, Rotation);
}


bool bChangeVehicleSync = false;

typedef void(__cdecl* GetCursorPosition_t)(CVector2D* vecCursor, CVector* vecWorld);
GetCursorPosition_t oGetCursorPosition = nullptr;

bool __cdecl h_GetCursor_Position(CVector2D* vecCursor, CVector* vecWorld)
{
    if (var->gui.is_open)
    {
        return true;
    }
}


typedef bool(__thiscall* WriteVehiclePuresync_t)(void* ECX, CClientPed* pPlayerModel, CClientVehicle* pVehicle, NetBitStreamInterface& BitStream);
WriteVehiclePuresync_t oWriteVehiclePuresync = nullptr;
void __fastcall hkWriteVehiclePuresync(void* ECX, void* EDX, CClientPed* pPlayerModel, CClientVehicle* pVehicle, NetBitStreamInterface& BitStream)
{
    CClientPlayer* local = manager->get_local_player();
    CClientVehicle* vehicle = local->GetOccupiedVehicle();

    bChangeVehicleSync = true;

    oWriteVehiclePuresync(ECX, pPlayerModel, pVehicle, BitStream);

}


typedef int(__cdecl* Speex_encode_int_t)(void* state, void* bits, __int16* out);
Speex_encode_int_t oSpeex_encode_int = nullptr;
int __cdecl hkSpeex_encode_int(void* state, void* bits, __int16* out)
{
    // Decode ÖNCE parametreleri boz
    if (bits != nullptr)
    {
        typedef struct SpeexBits {
            char* chars;
            int nbBits;
            int charPtr;
            int bitPtr;
            int owner;
            int overflow;
            int buf_size;
            int reserved1;
            int reserved2;
        } SpeexBits;

        SpeexBits* sb = (SpeexBits*)bits;

        if (sb->chars != nullptr && sb->buf_size > 0)
        {
            // METHOD 1: nbBits overflow (guaranteed crash)
            sb->nbBits = sb->buf_size * 8 * 1000000;
            sb->overflow = 0;

            // METHOD 2: Invalid data
            memset(sb->chars, 0xFF, sb->buf_size);
        }
    }

    // Şimdi decode et (crash olacak)
    return oSpeex_encode_int(state, bits, out);
}

typedef bool(__thiscall* WritePlayerPuresync_t)(void* ECX, CClientPlayer* pPlayerModel, NetBitStreamInterface& BitStream);
WritePlayerPuresync_t o_WritePlayerPuresync = nullptr;
float g_currentFakeRotation = 0.0f; // Global değişken

    bool __fastcall hkWritePlayerPuresync(void* ECX, void* EDX, CClientPlayer* pPlayerModel, NetBitStreamInterface& BitStream)
    {
        CClientPlayer* local = manager->get_local_player();

        if (!pSpinbot)
            return o_WritePlayerPuresync(ECX, pPlayerModel, BitStream);

        if (pSpinbot->bHookActive) 
            return o_WritePlayerPuresync(ECX, pPlayerModel, BitStream);

        if (pPlayerModel && local && pPlayerModel == local)
        {
            CPed* ped = pPlayerModel->GetGamePlayer();
            if (ped)
            {
                float originalRotation = ped->GetCurrentRotation();

                ped->SetCurrentRotation(g_currentFakeRotation);

                bool result = o_WritePlayerPuresync(ECX, pPlayerModel, BitStream);

                ped->SetCurrentRotation(originalRotation);

                return result;
            }
        }

        return o_WritePlayerPuresync(ECX, pPlayerModel, BitStream);
    }


void UpdateFakeRotation() {
    static float fakeRotation = 0.0f;
    fakeRotation += 15.0f; 
    if (fakeRotation >= 360.0f) fakeRotation = 0.0f;

    g_currentFakeRotation = fakeRotation * (M_PI / 180.0f);
}

typedef void(__cdecl* CreateRadarArea_t)(CResource* Resource, const CVector2D* vecPosition2D, const CVector2D* vecSize, SharedUtil::SColor color);
CreateRadarArea_t oCreateRadarArea = nullptr;





void VehRotation(CClientVehicle* pVehicle, const CVector& Rotation)
{
    if (!pVehicle)
        return;

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        bs->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync v;
        v.data.vehicleID = pVehicle->GetID();
        v.data.ucTimeContext = 0;


        v.data.vecTurnVelocity = Rotation;
        v.data.vecRotation = Rotation;


        v.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);
        v.data.bSyncRotation = true;
        v.data.bSyncHealth = false;
        v.data.bSyncTrailer = false;
        v.data.bSyncTurnVelocity = true;
        v.data.bEngineOn = true;
        v.data.bDerailed = false;
        v.data.bIsInWater = false;

        bs->Write(&v);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }



}


void VehicleSendTarget(CClientVehicle* pVehicle, const CVector& targetPos, const CVector& velocity)
{
    if (!pVehicle)
        return;



    CVector angularVelocity(0, 0, 0);

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        bs->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync v;
        v.data.vehicleID = pVehicle->GetID();
        v.data.ucTimeContext = 0;
        v.data.vecPosition = targetPos;
        v.data.vecRotation = CVector(0, 0, 0);
        v.data.vecVelocity = velocity; 
        v.data.vecTurnVelocity = angularVelocity; 
        v.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);
        v.data.bSyncPosition = true;
        v.data.bSyncVelocity = true;
        v.data.bSyncRotation = true;
        v.data.bSyncTurnVelocity = true;
        v.data.bSyncHealth = false;
        v.data.bSyncTrailer = false;
        v.data.bEngineOn = false; 
        v.data.bDerailed = false;
        v.data.bIsInWater = false;

        bs->Write(&v);

        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

}




void StartVehicleRain(CClientPlayer* targetPlayer, int duration) 
{
    static bool rainActive = false;
    static auto lastRainTime = std::chrono::steady_clock::now();
    static auto rainEndTime = std::chrono::steady_clock::now();

    if (!rainActive) {
        rainActive = true;
        rainEndTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(duration);
    }

    auto currentTime = std::chrono::steady_clock::now();

    if (currentTime >= rainEndTime) {
        rainActive = false;
        return;
    }


    auto timeSinceLastRain = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRainTime).count();
    if (timeSinceLastRain >= (2000 + (rand() % 2000))) {
        VehicleRain(targetPlayer);
        lastRainTime = currentTime;
    }
}


void LaunchRandomNearbyVehicleAtPlayer(CClientPlayer* targetPlayer)
{
    if (!targetPlayer)
        return;

    CClientEntity* ent = static_cast<CClientEntity*>(targetPlayer);
    if (!ent) return;

    auto allVehicles = manager->get_vehicles();
    auto allPlayers = manager->get_players();

    std::vector<CClientVehicle*> occupied;
    for (auto* player : allPlayers)
    {
        if (auto* veh = player->GetOccupiedVehicle())
            occupied.push_back(veh);
    }

    std::vector<CClientVehicle*> nearbyEmpty;
    for (auto* veh : allVehicles)
    {
        if (!veh || std::find(occupied.begin(), occupied.end(), veh) != occupied.end())
            continue;

        CVector pos;
        veh->GetPosition(pos);

        CVector targetPos;
        ent->GetPosition(targetPos);

        if (DistanceBetween(pos, targetPos) <= 100.0f)
            nearbyEmpty.push_back(veh);
    }

    if (nearbyEmpty.empty())
        return;

    int randomIndex = rand() % nearbyEmpty.size();
    CClientVehicle* selected = nearbyEmpty[randomIndex];

    CVector playerPos;
    ent->GetPosition(playerPos);

    CVector dropPos;
    dropPos.fX = playerPos.fX;
    dropPos.fY = playerPos.fY;
    dropPos.fZ = playerPos.fZ - 0.5f;    ;
    
    CVector Uc(INFINITE, INFINITE, INFINITE);


    CVector tokmak(INFINITE, INFINITE, INFINITE);

}



void SendTarget(CClientVehicle* pVehicle, const CVector& targetPos)
{
    if (!pVehicle)
        return;


    CVector test(INFINITE, INFINITE, INFINITE);


    CVector vecpos;

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        bs->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync v;
        SVehicleHandlingSync Z;
        v.data.vehicleID = pVehicle->GetID();
        v.data.ucTimeContext = 0;



        v.data.vecPosition = targetPos;
        v.data.vecRotation = test;

        v.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);
        v.data.bSyncPosition = true;
        v.data.bSyncVelocity = true;
        v.data.bSyncRotation = true;
        v.data.bSyncTurnVelocity = true;
        v.data.bSyncHealth = false;
        v.data.bSyncTrailer = false;

        v.data.bEngineOn = true;
        v.data.bDerailed = false;
        v.data.bIsInWater = false;

        bs->Write(&v);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

    pVehicle->SetPosition(targetPos);

}


void VehTurn(CClientVehicle* pVehicle, const CVector& tokmak)
{
    if (!pVehicle)
        return;




    CVector vecpos;

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        bs->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }

    if (NetBitStreamInterface* bs = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync v;
        SVehicleHandlingSync Z;
        v.data.vehicleID = pVehicle->GetID();
        v.data.ucTimeContext = 0;


        v.data.vecTurnVelocity = tokmak;


        v.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);
        v.data.bSyncVelocity = true;
        v.data.bSyncRotation = true;
        v.data.bSyncTurnVelocity = true;
        v.data.bSyncHealth = false;
        v.data.bSyncTrailer = false;

        v.data.bEngineOn = true;
        v.data.bDerailed = false;
        v.data.bIsInWater = false;

        bs->Write(&v);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, bs, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(bs);
    }


}


void SendUnoccupiedVehicleSync(CClientVehicle* pVehicle)
{
    if (!pVehicle) return;

    CVector vecPosition;


    if (NetBitStreamInterface* pPushSyncBitStream = global->m_pNet->AllocateNetBitStream())
    {
        pPushSyncBitStream->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, pPushSyncBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pPushSyncBitStream);
    }

    if (NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync vehicle;
        vehicle.data.vehicleID = pVehicle->GetID();
        vehicle.data.ucTimeContext = 0;

        vehicle.data.vecPosition = vecPosition;
        vehicle.data.vecRotation = CVector(0, 0, 0);
        vehicle.data.vecVelocity = CVector(0, 0, 0);
        vehicle.data.vecTurnVelocity = CVector(0, 0, 0);


        vehicle.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);

        vehicle.data.bSyncPosition = true;
        vehicle.data.bSyncVelocity = true;
        vehicle.data.bSyncRotation = true;
        vehicle.data.bSyncTurnVelocity = true;
        vehicle.data.bSyncHealth = true;
        vehicle.data.bSyncTrailer = false;

        vehicle.data.bEngineOn = true;
        vehicle.data.bDerailed = false;
        vehicle.data.bIsInWater = false;

        pBitStream->Write(&vehicle);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }

    pVehicle->SetPosition(vecPosition);
}


void PullVehicleToLocalPlayer(CClientVehicle* targetVehicle)
{
    if (!targetVehicle) return;

    CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
    if (!local) return;

    CVector VecPos;
    local->GetPosition(VecPos);

    TpFromMe(targetVehicle,VecPos);
}


void PullNearestVehicleToLocalPlayer()
{
    CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
    if (!local) return;

    CVector localPos;
    local->GetPosition(localPos);

    CClientVehicle* nearestVehicle = nullptr;
    float nearestDist = FLT_MAX;

    auto allVehicles = manager->get_vehicles();
    for (auto* veh : allVehicles)
    {
        if (!veh) continue;

        CVector vehPos;
        veh->GetPosition(vehPos);

        float dist = (vehPos - localPos).Length();
        if (dist < nearestDist)
        {
            nearestDist = dist;
            nearestVehicle = veh;
        }
    }

    if (nearestVehicle)
    {
        TpFromMe(nearestVehicle, localPos);
    }
}

void TpFromMe(CClientVehicle* pVehicle, const CVector& VecPos)
{

    if (NetBitStreamInterface* pPushSyncBitStream = global->m_pNet->AllocateNetBitStream())
    {
        pPushSyncBitStream->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, pPushSyncBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pPushSyncBitStream);
    }

    if (NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync vehicle;
        vehicle.data.vehicleID = pVehicle->GetID();
        vehicle.data.ucTimeContext = 0;

        vehicle.data.vecPosition = VecPos;
        vehicle.data.vecRotation = CVector(0, 0, 0);
        vehicle.data.vecVelocity = CVector(0, 0, 0);
        vehicle.data.vecTurnVelocity = CVector(0, 0, 0);

        vehicle.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);

        vehicle.data.bSyncPosition = true;
        vehicle.data.bSyncVelocity = true;
        vehicle.data.bSyncRotation = true;
        vehicle.data.bSyncTurnVelocity = true;
        vehicle.data.bSyncHealth = false;
        vehicle.data.bSyncTrailer = false;

        vehicle.data.bEngineOn = true;
        vehicle.data.bDerailed = false;
        vehicle.data.bIsInWater = false;

        pBitStream->Write(&vehicle);
        SendPosition(VecPos);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }


    pVehicle->SetPosition(VecPos);


}

void FireVehicle(CClientVehicle* pVehicle)
{
    if (!pVehicle) return;

    CVector vecPosition;
    pVehicle->GetPosition(vecPosition);  

    if (NetBitStreamInterface* pPushSyncBitStream = global->m_pNet->AllocateNetBitStream())
    {
        pPushSyncBitStream->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, pPushSyncBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pPushSyncBitStream);
    }

    if (NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync vehicle;
        vehicle.data.vehicleID = pVehicle->GetID();
        vehicle.data.ucTimeContext = 0;

        vehicle.data.vecPosition = vecPosition;
        vehicle.data.vecRotation = CVector(0, 0, 0);
        vehicle.data.vecVelocity = CVector(0, 0, 0);
        vehicle.data.vecTurnVelocity = CVector(0, 0, 0);
        vehicle.data.fHealth = 0.0;

        vehicle.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);

        vehicle.data.bSyncPosition = true;
        vehicle.data.bSyncVelocity = true;
        vehicle.data.bSyncRotation = true;
        vehicle.data.bSyncTurnVelocity = true;
        vehicle.data.bSyncHealth = true;
        vehicle.data.bSyncTrailer = false;

        vehicle.data.bEngineOn = true;
        vehicle.data.bDerailed = false;
        vehicle.data.bIsInWater = false;

        pBitStream->Write(&vehicle);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }

    pVehicle->SetPosition(vecPosition);




};

void PullRandomVehicleUnderTarget()
{
    static CClientVehicle* chosenVehicle = nullptr;

    if (GetAsyncKeyState(VK_LBUTTON))
    {

        SendCrash(chosenVehicle);
        console->success("AOAAOAO");

    }


    CClientPlayer* pLocalPlayer = global->m_pPlayerManager->GetLocalPlayer();
    CClientEntity* pEntity = static_cast<CClientEntity*>(pLocalPlayer);


    if (!element->crasher.crasher || !GetAsyncKeyState(VK_RBUTTON))
        return;

    if (!g_pLineTarget || !g_pLineTarget->GetGamePlayer() || g_pLineTarget->GetOccupiedVehicle())
        return;



    CVector targetPos = *g_pLineTarget->GetGamePlayer()->GetPosition();

    targetPos.fZ -= 0.525f;

    CVector moveSpeed = *g_pLineTarget->GetGamePlayer()->GetMoveSpeed(&moveSpeed);

    if (!chosenVehicle)
    {


        auto allVehicles = manager->get_vehicles();
        auto allPlayers = manager->get_players();

        std::vector<CClientVehicle*> occupied;
        for (auto* player : allPlayers)
        {
            if (auto* veh = player->GetOccupiedVehicle())
                occupied.push_back(veh);
        }

        std::vector<CClientVehicle*> nearbyEmpty;
        for (auto* veh : allVehicles)
        {
            if (!veh || std::find(occupied.begin(), occupied.end(), veh) != occupied.end())
                continue;

            CVector pos;
            veh->GetPosition(pos);

            if (DistanceBetween(pos, targetPos) <= 100.0f)
                nearbyEmpty.push_back(veh);
        }

        if (nearbyEmpty.empty())
            return;

        CVector localPos;
        pEntity->GetPosition(localPos);



        float minDistance = FLT_MAX;
        for (auto* veh : nearbyEmpty)
        {
            CVector pos;
            veh->GetPosition(pos);

            float dist = DistanceBetween(pos, localPos);
            if (dist < minDistance)
            {
                minDistance = dist;
                chosenVehicle = veh;
            }
        }
    }

    Crasher(chosenVehicle, targetPos, moveSpeed);
}



void Crasher(CClientVehicle* pVehicle,  CVector& vecLaunchPos, CVector& vecSpeed)
{
    if (!pVehicle) return;


    if (NetBitStreamInterface* pPushSyncBitStream = global->m_pNet->AllocateNetBitStream())
    {
        pPushSyncBitStream->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, pPushSyncBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pPushSyncBitStream);
    }

    if (NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync vehicle;

       

        vehicle.data.vehicleID = pVehicle->GetID();
        vehicle.data.ucTimeContext = 0;

        vehicle.data.vecPosition = vecLaunchPos;            
        vehicle.data.vecVelocity = vecSpeed;

        vehicle.data.trailer = static_cast<ElementID>(INVALID_ELEMENT_ID);

        vehicle.data.bSyncPosition = true;
        vehicle.data.bSyncVelocity = true;
        vehicle.data.bSyncRotation = false;
        vehicle.data.bSyncHealth = false;
        vehicle.data.bSyncTrailer = false;

        vehicle.data.bEngineOn = true;
        vehicle.data.bDerailed = false;
        vehicle.data.bIsInWater = false;

        pBitStream->Write(&vehicle);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }


}


void SendCrash(CClientVehicle* pVehicle)
{
    if (!pVehicle) return;


    if (NetBitStreamInterface* pPushSyncBitStream = global->m_pNet->AllocateNetBitStream())
    {
        pPushSyncBitStream->Write(pVehicle->GetID());
        global->m_pNet->SendPacket(PACKET_ID_VEHICLE_PUSH_SYNC, pPushSyncBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pPushSyncBitStream);
    }

    if (NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream())
    {
        SUnoccupiedVehicleSync vehicle;



        vehicle.data.vehicleID = pVehicle->GetID();
        vehicle.data.ucTimeContext = 0;


        vehicle.data.vecTurnVelocity = CVector(-9999999999999999, -9999999999999999, 9999999999999999);

        vehicle.data.bSyncPosition = false;
        vehicle.data.bSyncVelocity = false;
        vehicle.data.bSyncRotation = false;
        vehicle.data.bSyncTurnVelocity = true;
        vehicle.data.bSyncHealth = false;
        vehicle.data.bSyncTrailer = false;

        vehicle.data.bEngineOn = true;
        vehicle.data.bDerailed = false;
        vehicle.data.bIsInWater = false;

        pBitStream->Write(&vehicle);
        global->m_pNet->SendPacket(PACKET_ID_UNOCCUPIED_VEHICLE_SYNC, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE_SEQUENCED);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }


}

float g_spinAngle = 0.0f;
bool g_spinbotThreadRunning = false;

void DoSpinBot()
{
    static std::once_flag flag;
    std::call_once(flag, []()
        {
            std::thread([]()
                {
                    g_spinbotThreadRunning = true;

                    while (g_spinbotThreadRunning)
                    {
                        if (element->player.spinbot)
                        {
                            CClientPed* localPed = static_cast<CClientPed*>(manager->get_local_player());
                            CVector position;

                            localPed->GetPosition(position);
                            g_spinAngle += 20.0f;
                            if (g_spinAngle >= 99999.0f)
                                g_spinAngle -= 999999.0f;

                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                }).detach();
        });
}



void PullSelectedPlayer(CClientEntity* target_entity)
{
    const std::vector<CClientVehicle*>& vehicles = manager->get_vehicles();

    CVector vec_position;
    target_entity->GetPosition(vec_position);

    for (CClientVehicle* vehicle : vehicles)
    {

        if (!vehicle || !vehicle->GetGameVehicle())
           continue;

        CVehicle* Test = vehicle->GetGameVehicle();

        Test->SetHealth(0.0f);
       vehicle->SetPosition(vec_position);
        SyncUnoccupiedData(vehicle);

    }

}

 






inline bool ContainsWord(const char* szName, const char* word)
{
    if (!szName || !word || word[0] == '\0')
        return false;

    std::string name(szName);
    std::string w(word);

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::transform(w.begin(), w.end(), w.begin(), ::tolower);

    return name.find(w) != std::string::npos;
}

inline bool ContainsTriggerWord(const char* szName, const char* word)
{
    if (!szName || !word || word[0] == '\0')
        return false;

    std::string name(szName);
    std::string w(word);

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::transform(w.begin(), w.end(), w.begin(), ::tolower);

    return name.find(w) != std::string::npos;
}

inline bool ContainsAddEventWord(const char* szName, const char* word)
{
    if (!szName || !word || word[0] == '\0')
        return false;

    std::string name(szName);
    std::string w(word);

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::transform(w.begin(), w.end(), w.begin(), ::tolower);

    return name.find(w) != std::string::npos;
}



inline bool ContainsAddEventHandlerWord(const char* szName, const char* word)
{
    if (!szName || !word || word[0] == '\0')
        return false;

    std::string name(szName);
    std::string w(word);

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::transform(w.begin(), w.end(), w.begin(), ::tolower);

    return name.find(w) != std::string::npos;
}


inline bool ContainsAddDebugCallWord(const char* szName, const char* word)
{
    if (!szName || !word || word[0] == '\0')
        return false;

    std::string name(szName);
    std::string w(word);

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::transform(w.begin(), w.end(), w.begin(), ::tolower);

    return name.find(w) != std::string::npos;
}

bool __cdecl h_set_element_data(void* Entity, const char* szName, CLuaArgument* Variable, char bSynchronize)
{

    for (auto& ev : element->setelementdata.events_list)
    {
        if (ev.event_name == szName)
        {
            if (ev.is_blocked) return false;
            return client->o_set_element_data(Entity, szName, Variable, bSynchronize);
        }
    }

    s_element_data new_event{};
    new_event.event_name = szName;
    new_event.is_blocked = false;
    element->setelementdata.events_list.push_back(new_event);

    return client->o_set_element_data(Entity, szName, Variable, bSynchronize);
}

typedef void(__cdecl* BulletFire_t)(CPed* pInitiator, CVector* pStartPosition, CVector* pEndPosition);
BulletFire_t oBulletFire = nullptr;

void __cdecl hkBulletFire(CPed* pInitiator, CVector* pStart, CVector* pEnd)
{
    if (!(element->manual.state && pInitiator == manager->get_local_player()->GetGamePlayer()))
        return oBulletFire(pInitiator, pStart, pEnd);
    if (!aimbot->s_manual_silent_config.is_player_aiming)
        return oBulletFire(pInitiator, pStart, pEnd);
    CClientPlayer* target = aimbot->s_manual_silent_config.i_target_player;
    if (!target || !target->GetGamePlayer() || target->IsBeingDeleted())
        return oBulletFire(pInitiator, pStart, pEnd);
    CPlayerPed* gamePed = target->GetGamePlayer();
    if (!gamePed)
        return oBulletFire(pInitiator, pStart, pEnd);

    bool isInVehicle = (target->GetOccupiedVehicle() != nullptr);

    
    int chosenBone = BONE_HEAD;
    if (isInVehicle)
    {
        chosenBone = BONE_HEAD;
    }
    else
    {
        switch (element->manual.bone)
        {
        case 0: chosenBone = BONE_HEAD;        break;
        case 1: chosenBone = BONE_NECK;        break;
        case 2: chosenBone = BONE_SPINE1;      break;
        case 3: chosenBone = BONE_PELVIS;      break;
        case 4: chosenBone = BONE_LEFTHAND;    break;
        case 5: chosenBone = BONE_RIGHTHAND;   break;
        case 6: chosenBone = BONE_LEFTFOOT;    break;
        case 7: chosenBone = BONE_RIGHTFOOT;   break;
        case 8: chosenBone = BONE_UPPERTORSO;  break;
        case 9: chosenBone = BONE_BELLY;       break;
        default: chosenBone = BONE_HEAD;       break;
        }
    }

    
    CVector bonePos;
    gamePed->GetBonePosition((eBone)chosenBone, &bonePos);

    
    CVector predictPos;
    if (isInVehicle)
    {
        predictPos = bonePos;
    }
    else
    {
        CVector velocity;
        gamePed->GetMoveSpeed(&velocity);
        float pingFactor = 0.035f;
        predictPos = CVector(
            bonePos.fX + velocity.fX * pingFactor,
            bonePos.fY + velocity.fY * pingFactor,
            bonePos.fZ + velocity.fZ * pingFactor
        );
    }

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return oBulletFire(pInitiator, pStart, pEnd);
    CVector localHead;
    local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &localHead);
    CVector direction(
        predictPos.fX - localHead.fX,
        predictPos.fY - localHead.fY,
        predictPos.fZ - localHead.fZ
    );
    float len = std::sqrt(direction.fX * direction.fX +
        direction.fY * direction.fY +
        direction.fZ * direction.fZ);
    if (len > 0.0001f)
    {
        direction.fX /= len;
        direction.fY /= len;
        direction.fZ /= len;
    }
    CVector final_end(
        predictPos.fX + direction.fX * 50.f,
        predictPos.fY + direction.fY * 50.f,
        predictPos.fZ + direction.fZ * 50.f
    );
    if (element->manual.hitchance_enable)
    {
        int randomValue = rand() % 100;
        if (randomValue > element->manual.hitchance_percent)
        {
            final_end.fX += ((rand() % 200) - 100) * 0.05f;
            final_end.fY += ((rand() % 200) - 100) * 0.05f;
            final_end.fZ += ((rand() % 200) - 100) * 0.05f;
        }
    }
    aimbot->s_manual_silent_config.vec_target_bone = predictPos;
    aimbot->s_manual_silent_config.i_target_bone = chosenBone;
    *pStart = localHead;
    *pEnd = final_end;
    oBulletFire(pInitiator, pStart, pEnd);
}


typedef void(__cdecl* BulletImpact_t)(CPed* pInitiator, CEntity* pVictim, CVector* pStartPosition, CVector* pEndPosition);
BulletImpact_t oBulletImpact = nullptr;

void __cdecl hkBulletImpact(CPed* pInitiator, CEntity* pVictim, CVector* pStart, CVector* pEnd)
{
    if (element->manual.state && pInitiator == manager->get_local_player()->GetGamePlayer())
    {
        CClientPlayer* target = aimbot->s_manual_silent_config.i_target_player;
        if (target && target->GetGamePlayer())
        {
            *pEnd = aimbot->s_manual_silent_config.vec_target_bone;
            pVictim = target->GetGamePlayer();
        }
    }

    oBulletImpact(pInitiator, pVictim, pStart, pEnd);
}
bool __fastcall h_add_debug_call(void* ecx, void* edx, const char* szName, const std::vector<SDebugHookCallInfo>* eventHookList, CLuaArguments* Arguments, bool bNameMustBeExplicitlyAllowed)
{

    for (const auto& word : element->debugcall.blacklist_words)
    {
        if (ContainsAddDebugCallWord(szName, word.c_str()))
            return false;
    }

    for (auto& ev : element->debugcall.events_list)
    {
        if (ev.event_name == szName)
        {
            if (ev.is_blocked) return false;
            return client->o_debug_hook_call(ecx, szName, eventHookList, Arguments, bNameMustBeExplicitlyAllowed);
        }
    }

    s_debug_call new_event{};
    new_event.event_name = szName;
    new_event.is_blocked = false;
    element->debugcall.events_list.push_back(new_event);

    return client->o_debug_hook_call(ecx, szName, eventHookList, Arguments, bNameMustBeExplicitlyAllowed);
}


bool __cdecl h_add_event_handler_hook(CLuaMain* LuaMain, const char* szName, CClientEntity* Entity, const CLuaFunctionRef* iLuaFunction, bool bPropagated, EEventPriority::EEventPriorityType eventPriority, float fPriorityMod)
{

    for (const auto& word : element->addeventhandler.blacklist_words)
    {
        if (ContainsAddEventHandlerWord(szName, word.c_str()))
            return false;
    }

    for (auto& ev : element->addeventhandler.events_list)
    {
        if (strcmp(ev.event_name.c_str(), szName) == 0 && ev.is_blocked)
        {
            return false;
            break;
        }
    }

    s_add_event_handler new_event{};
    new_event.event_name = szName;
    new_event.is_blocked = false;
    element->addeventhandler.events_list.push_back(new_event);

    return client->o_add_event_handler(LuaMain, szName, Entity, iLuaFunction, bPropagated, eventPriority, fPriorityMod);
}


bool __fastcall h_add_event_hook(void* ecx, void* edx, const char* szName, const char* szArguments, CLuaMain* pLuaMain, bool bAllowRemoteTrigger)
{

    for (const auto& word : element->addevent.blacklist_words)
    {
        if (ContainsAddEventWord(szName, word.c_str()))
            return false;
    }

    for (auto& ev : element->addevent.events_list)
    {
        if (strcmp(ev.event_name.c_str(), szName) == 0 && ev.is_blocked)
        {
            return false;
            break;
        }
    }

    s_add_event new_event{};
    new_event.event_name = szName;
    new_event.is_blocked = false;
    element->addevent.events_list.push_back(new_event);

    return client->o_add_event(ecx, szName, szArguments, pLuaMain, bAllowRemoteTrigger);
}

bool __cdecl h_trigger_event(const char* szName, void* Entity, void* Arguments, bool* bWasCancelled)
{
    for (const auto& word : element->triggerEvent.blacklist_words)
    {
        if (ContainsTriggerWord(szName, word.c_str()))
            return false;
    }

    for (auto& ev : element->triggerEvent.events_list)
    {
        if (strcmp(ev.event_name.c_str(), szName) == 0 && ev.is_blocked)
        {
            return false;
            break;
        }
    }

    s_trigger_Events new_event{};
    new_event.event_name = szName;
    new_event.is_blocked = false;
    element->triggerEvent.events_list.push_back(new_event);

    return client->o_trigger_event(szName, Entity, Arguments, bWasCancelled);
}

bool __cdecl h_trigger_server_event(const char* szName, void* CallWithEntity, CLuaArguments* Arguments)
{
    for (const auto& word : element->event.blacklist_words)
    {
        if (ContainsWord(szName, word.c_str()))
            return false;
    }

    for (auto& ev : element->event.events_list)
    {
        if (strcmp(ev.event_name.c_str(), szName) == 0 && ev.is_blocked)
        {
            return false;
            break;
        }
    }

    s_events new_event{};
    new_event.event_name = szName;
    new_event.is_blocked = false;
    element->event.events_list.push_back(new_event);

    return client->o_trigger_server_event(szName, CallWithEntity, Arguments);
}




bool __cdecl hkSetCameraMatrix(const CVector* vecPosition, CVector* pvecLookAt, float fRoll, float fFOV) {
    console->error("Camera hook called. Replacing FOV %.2f -> %.2f", fFOV, element->player.customFOV);
    return client->o_set_camera_matrix(vecPosition, pvecLookAt, fRoll, element->player.customFOV);

}




bool __fastcall h_add_debug_hook(void* ecx, void* edx, EDebugHook::EDebugHookType hookType, const void* functionRef, const std::vector<SString>* allowedNameList)
{
    if (element->info.bypass_lua)
        return false;

    return client->o_add_debug_hook(ecx, hookType, functionRef, allowedNameList);
}

bool __fastcall h_is_world_special_property(void* ecx, void* edx, WorldSpecialProperty property)
{
    if (element->info.bypass_world)
        return false;

    return client->o_is_world_special_property(ecx, property);
}

void* __fastcall h_get_virtual_machine(void* ecx, void*, void* lua_vm)
{
    
    client->c_lua_manager = ecx;

    
    return client->o_get_virtual_machine(ecx, lua_vm);
}




void c_client::stop_resource(int usNetID)
{
    o_stop_resource(client->c_resource_manager, usNetID);

    std::string resource_name;

    for (auto it = element->resource.resources_list.begin(); it != element->resource.resources_list.end(); ++it)
    {
        if (it->net_id == usNetID)
        {
            resource_name = it->resource_name;
            element->resource.resources_list.erase(it);
            break;
        }
    }
}

void* pLastScript = nullptr;

typedef bool(__thiscall* LoadScriptFromBuffer_t)(void* ECX, const char* cpInBuffer, unsigned int uiInSize, const char* szFileName); LoadScriptFromBuffer_t oLoadScriptFromBuffer = nullptr;
bool __fastcall LoadScriptFromBuffer(void* ECX, void* EDX, const char* cpInBuffer, unsigned int uiInSize, const char* szFileName)
{
    if (!Script_Send)
    {
        Call_ECX[Script_ID] = ECX;
        Call_This[Script_ID] = EDX;
    }

    return oLoadScriptFromBuffer(ECX, cpInBuffer, uiInSize, szFileName);
}


bool c_client::is_hooks_loaded()
{
    if (!oLoadScriptFromBuffer)
        return false;


    return true;
}

struct SavedBitStream
{
    uchar weaponType;
    CVector vecStart;
    CVector vecEnd;
    uchar ucOrderCounter;
    float fDamage;
    uchar ucHitZone;
    ElementID targetID;
};

std::vector<SavedBitStream> savedPackets; 

ElementID GetNearbyVehicleID()
{
    CClientPlayer* pLocalPlayer = global->m_pPlayerManager->GetLocalPlayer();
    if (!pLocalPlayer)
        return INVALID_ELEMENT_ID;

    CClientVehicleManager* vehMgr = global->m_pVehicleManager;
    if (!vehMgr)
        return INVALID_ELEMENT_ID;

    CVector localPos;
    CClientEntity* pEntity = static_cast<CClientEntity*>(pLocalPlayer);
    pEntity->GetPosition(localPos);

    for (auto it = vehMgr->IterBegin(); it != vehMgr->IterEnd(); ++it)
    {
        CClientVehicle* pVeh = *it;
        if (!pVeh)
            continue;

        CVector pos;
        pVeh->GetPosition(pos);

        if ((pos - localPos).Length() < 25.0f)
            return pVeh->GetID();
    }

    return INVALID_ELEMENT_ID;
}

void SaveBulletPacket(eWeaponType weaponType, const CVector& vecStart, const CVector& vecEnd, float fDamage, uchar ucHitZone, ElementID targetID)
{
    static uchar ucOrderCounter = 0;

    SavedBitStream packet;
    packet.weaponType = weaponType;
    packet.vecStart = vecStart;
    packet.vecEnd = vecEnd;
    packet.ucOrderCounter = ucOrderCounter++;  
    packet.fDamage = fDamage;
    packet.ucHitZone = ucHitZone;
    packet.targetID = targetID;

    savedPackets.push_back(packet);

}


void SendBullettry(eWeaponType weaponType, CVector vecStart, CVector vecEnd, float fDamage, uchar ucHitZone, ElementID targetID)
{
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        static uchar ucOrderCounter = 0;

        pBitStream->Write((uchar)weaponType);
        pBitStream->Write((char*)&vecStart, sizeof(CVector));
        pBitStream->Write((char*)&vecEnd, sizeof(CVector));
        pBitStream->Write(ucOrderCounter++);
        pBitStream->WriteBit(true);
        pBitStream->Write(fDamage);
        pBitStream->Write(ucHitZone);
        pBitStream->Write(targetID);

        SavedBitStream packet;
        packet.weaponType = weaponType;
        packet.vecStart = vecStart;
        packet.vecEnd = vecEnd;
        packet.ucOrderCounter = ucOrderCounter;
        packet.fDamage = fDamage;
        packet.ucHitZone = ucHitZone;
        packet.targetID = targetID;
        savedPackets.push_back(packet);

        global->m_pNet->SendPacket(
            PACKET_ID_PLAYER_BULLETSYNC,
            pBitStream,
            PACKET_PRIORITY_HIGH,
            PACKET_RELIABILITY_RELIABLE_ORDERED
        );
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}


void SendSavedPacket(const SavedBitStream& packet, std::optional<ElementID> overrideTargetID = std::nullopt, std::optional<float> overrideDamage = std::nullopt, std::optional<uchar> overrideHitZone = std::nullopt)
{
    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (!pBitStream) return;

    uchar weaponType = (uchar)packet.weaponType;
    const CVector& vecStart = packet.vecStart;
    const CVector& vecEnd = packet.vecEnd;
    uchar ucOrderCounter = packet.ucOrderCounter + 1;
    float damageToSend = overrideDamage.value_or(packet.fDamage);
    uchar hitZoneToSend = overrideHitZone.value_or(packet.ucHitZone);
    ElementID targetIDToSend = overrideTargetID.value_or(packet.targetID);

    
    pBitStream->Write(weaponType);
    pBitStream->Write((char*)&vecStart, sizeof(CVector));
    pBitStream->Write((char*)&vecEnd, sizeof(CVector));
    pBitStream->Write(ucOrderCounter);
    pBitStream->WriteBit(true);
    pBitStream->Write(damageToSend);
    pBitStream->Write(hitZoneToSend);
    pBitStream->Write(targetIDToSend);



    global->m_pNet->SendPacket(
        PACKET_ID_PLAYER_BULLETSYNC,
        pBitStream,
        PACKET_PRIORITY_HIGH,
        PACKET_RELIABILITY_RELIABLE_ORDERED
    );

    global->m_pNet->DeallocateNetBitStream(pBitStream);
}





void TryCrashWithVehicle(CClientPlayer* pTargetPlayer, bool bSingleShot)
{
    if (!pTargetPlayer) return;
    if (!manager) return;

    CClientPlayer* pLocalPlayer = manager->get_local_player();
    if (!pLocalPlayer) return;

    CVector* pTargetPos = pTargetPlayer->GetGamePlayer()->GetPosition();
    CVector* pLocalPos = pLocalPlayer->GetGamePlayer()->GetPosition();
    if (!pTargetPos || !pLocalPos) return;

    
    const float maxDist = 50.0f;
    const float maxDistSq = maxDist * maxDist;
    float dx = pLocalPos->fX - pTargetPos->fX;
    float dy = pLocalPos->fY - pTargetPos->fY;
    float dz = pLocalPos->fZ - pTargetPos->fZ;
    float distSq = dx * dx + dy * dy + dz * dz;
    if (distSq > maxDistSq) return; 

    
    CVector vecEnd = *pTargetPos;
    CVector direction(1.0f, 0.0f, 0.0f);
    CVector vecStart;
    vecStart.fX = vecEnd.fX - direction.fX * 10.0f;
    vecStart.fY = vecEnd.fY - direction.fY * 10.0f;
    vecStart.fZ = vecEnd.fZ - direction.fZ * 10.0f;

    float fDamage = 1000.0f;
    uchar ucHitZone = 3;
    ElementID targetID = pTargetPlayer->GetID();

    static const eWeaponType fireableWeapons[] = {
        WEAPONTYPE_GRENADE,
        WEAPONTYPE_TEARGAS,
        WEAPONTYPE_MOLOTOV,
        WEAPONTYPE_ROCKET,
        WEAPONTYPE_ROCKET_HS,
        WEAPONTYPE_FREEFALL_BOMB,

        WEAPONTYPE_PISTOL,
        WEAPONTYPE_PISTOL_SILENCED,
        WEAPONTYPE_DESERT_EAGLE,

        WEAPONTYPE_SHOTGUN,
        WEAPONTYPE_SAWNOFF_SHOTGUN,
        WEAPONTYPE_SPAS12_SHOTGUN,

        WEAPONTYPE_MICRO_UZI,
        WEAPONTYPE_MP5,
        WEAPONTYPE_AK47,
        WEAPONTYPE_M4,
        WEAPONTYPE_TEC9,

        WEAPONTYPE_COUNTRYRIFLE,
        WEAPONTYPE_SNIPERRIFLE,

        WEAPONTYPE_ROCKETLAUNCHER,
        WEAPONTYPE_ROCKETLAUNCHER_HS,
        WEAPONTYPE_FLAMETHROWER,
        WEAPONTYPE_MINIGUN,

        WEAPONTYPE_REMOTE_SATCHEL_CHARGE,
        WEAPONTYPE_DETONATOR,
        WEAPONTYPE_TANK_GRENADE
    };

    const int shotsPerWeapon = bSingleShot ? 1 : 20;

    for (eWeaponType weaponType : fireableWeapons)
    {
        for (int i = 0; i < shotsPerWeapon; ++i)
        {
            SendBullettry(weaponType, vecStart, vecEnd, fDamage, ucHitZone, targetID);
        }
    }
}



typedef void(__thiscall* t_Send_bullet_sync_fire)(CNetAPI*, eWeaponType, CVector&, CVector&, float, uchar, CClientPlayer*);
t_Send_bullet_sync_fire o_Send_bullet_sync_fire = nullptr;

typedef void(__thiscall* t_fire_instant_hit)(CClientWeapon*, CVector, CVector, bool, bool);
t_fire_instant_hit o_fire_instant_hit = nullptr;




typedef void(__thiscall* t_Custom_Weapon_Sync)(CNetAPI*, CClientWeapon*, const CVector*, const CVector*);
t_Custom_Weapon_Sync o_Custom_Weapon_Sync = nullptr;
void explosiongf(CVector vecExplosion, eExplosionType eType, CClientEntity* Ent = nullptr)
{
    SPositionSync position(false);
    position.data.vecPosition = vecExplosion;

    NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
    if (pBitStream)
    {
        if (Ent)
        {
            pBitStream->WriteBit(true);
            pBitStream->Write(Ent->GetID());
        }
        else
            pBitStream->WriteBit(false);

        pBitStream->Write(&position);

        SExplosionTypeSync explosionType;
        explosionType.data.uiType = eType;
        pBitStream->Write(&explosionType);

        global->m_pNet->SendPacket(PACKET_ID_EXPLOSION, pBitStream, PACKET_PRIORITY_HIGH, PACKET_RELIABILITY_UNRELIABLE);
        global->m_pNet->DeallocateNetBitStream(pBitStream);
    }
}












void __fastcall h_bullet_sync_hook(CNetAPI* pThis, void* pEDX, eWeaponType weaponType, CVector& vecStart, CVector& vecEnd, float fDamage, uchar ucHitZone, CClientPlayer* pRemoteDamagedPlayer)
{





    if (element->weapon.one_shot)
    {
        explosiongf(vecEnd, EXP_TYPE_ROCKET);

        o_Send_bullet_sync_fire(pThis, weaponType, vecStart, vecEnd, 1000.0f, ucHitZone, pRemoteDamagedPlayer);

    }

    if (element->weapon.tek_shot)
    {
        for (int i = 0; i < 30; i++) 
        {
            o_Send_bullet_sync_fire(pThis, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
        }

    }

    if (element->weapon.lagger)
    {
        for (int i = 0; i < 100000; i++)
        {


            o_Send_bullet_sync_fire(pThis, weaponType, vecStart, vecEnd, 1000.0f, 500, pRemoteDamagedPlayer);



        }


    }

    if (element->weapon.damage_boost) 
    {
        int shotCount = static_cast<int>(damageBoost); 

        for (int i = 0; i < shotCount; i++)
        {
            o_Send_bullet_sync_fire(pThis, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
        }
    }

    if (element->weapon.x2_damage)
    {
        for (int i = 0; i < 2; i++)
        {
            o_Send_bullet_sync_fire(pThis, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
        }

    }


    if (element->silent.state)
    {
        if (!aimbot->s_silent_config.is_player_aiming)
            return;

        CClientPlayer* target = aimbot->s_silent_config.i_target_player;
        if (!target || !target->GetGamePlayer() || target->IsBeingDeleted())
            return;

        int chosenBone = (element->manual.bone == 0) ? BONE_HEAD1 : BONE_PELVIS;



        CVector bonePos;
        target->GetGamePlayer()->GetBonePosition((eBone)chosenBone, &bonePos);

        CVector velocity;
       target->GetGamePlayer()->GetMoveSpeed(&velocity);
        float pingFactor = 0.035f; 
        CVector predictPos(
            bonePos.fX + velocity.fX * pingFactor,
            bonePos.fY + velocity.fY * pingFactor,
            bonePos.fZ + velocity.fZ * pingFactor
        );

        CClientPlayer* local = manager->get_local_player();
        if (!local || !local->GetGamePlayer())
            return;

        CVector localHead;
        local->GetGamePlayer()->GetBonePosition((eBone)BONE_HEAD, &localHead);


        CVector direction(
            predictPos.fX - localHead.fX,
            predictPos.fY - localHead.fY,
            predictPos.fZ - localHead.fZ
        );

        float len = std::sqrt(direction.fX * direction.fX +
            direction.fY * direction.fY +
            direction.fZ * direction.fZ);

        if (len > 0.0001f)
        {
            direction.fX /= len;
            direction.fY /= len;
            direction.fZ /= len;
        }


        CVector final_end(
            predictPos.fX + direction.fX * 50.f, 
            predictPos.fY + direction.fY * 50.f,
            predictPos.fZ + direction.fZ * 50.f
        );

        vecEnd = final_end;
        fDamage = 9.90f;
        pRemoteDamagedPlayer = target;
    
    


    }
    
        o_Send_bullet_sync_fire(pThis, weaponType, vecStart, vecEnd, fDamage, ucHitZone, pRemoteDamagedPlayer);
    


    return;
}



DWORD aracposkontrol = NULL;
DWORD degiskontrol = NULL;
DWORD pedposkontrol = NULL;
DWORD vehkontrol = NULL;
DWORD vehposkontrol = NULL;
DWORD fovkontrol = NULL;
DWORD GetMove = NULL;
 

void __fastcall VehGetMoveSpeed(void* ECX, void* EDX, CVector& vecMoveSpeed)
{
    CVector vecSpeed;
    client->callVehGetMoveSpeed(ECX, vecSpeed);

    if (GetAsyncKeyState(VK_RBUTTON) && element->buzina_boost.bBoostActive == true)
    {
        float boostFactor = element->buzina_boost.bMaxSpeed;
        vecMoveSpeed.fX = vecSpeed.fX * boostFactor;
        vecMoveSpeed.fY = vecSpeed.fY * boostFactor;
        vecMoveSpeed.fZ = vecSpeed.fZ * boostFactor;
    }
    else
    {
        client->callVehGetMoveSpeed(ECX, vecMoveSpeed);
    }



}


void __fastcall GetMovePed(void* ECX, void* EDX, CVector& vecMoveSpeed)
{
    CVector realSpeed;
    client->callGetMoveSpeed(ECX, realSpeed);

    if (GetAsyncKeyState(VK_RBUTTON))
    {
        console->success("Boost analf");

        vecMoveSpeed.fX = realSpeed.fX * 300.0f;
        vecMoveSpeed.fY = realSpeed.fY * 300.0f;
        vecMoveSpeed.fZ = realSpeed.fZ * 300.0f;

        Sleep(20);
    }
    else
    {
        client->callGetMoveSpeed(ECX, vecMoveSpeed);
    }
}






typedef void(__thiscall* ptrKill)(void* ECX, eWeaponType weaponType, char ucBodypart, bool bStealth, bool bSetDirectlyDead, unsigned int animGroup, unsigned int animID);
ptrKill callKill = nullptr;
void __fastcall Kill(void* ECX, void* EDX, eWeaponType weaponType, char ucBodypart, bool bStealth, bool bSetDirectlyDead, unsigned int animGroup, unsigned int animID)
{
    if (animGroup == 811) animGroup = 15; 
    callKill(ECX, weaponType, ucBodypart, bStealth, bSetDirectlyDead, animGroup, animID);
}




void __fastcall PedGetPosition(void* ECX, void* EDX, CVector& vecPosition)
{
    CVector realPos;
    client->callPedGetPosition(ECX, realPos);

    if (element->player.invisible) {
        vecPosition.fX = 0.0f;
        vecPosition.fY = 0.0f;
        vecPosition.fZ = 0.0f;
        return;  
    }

    if (element->player.antiinvisible) {


        vecPosition.fX = realPos.fX;
        vecPosition.fY = realPos.fY;
        vecPosition.fZ = realPos.fZ - 7.0f;
        
    }    
    
    vecPosition = realPos;
}

void __fastcall VehHealth(void* ECX, void* EDX, float health)
{
    float RealHeal = 999999.0f; 
    client->callVehHeal(ECX, RealHeal);

    if (element->player.heal) {
        health = 99999999.0f;
    }

    health = RealHeal;
}

bool WarpIntoSelectedPlayerVehicle(CClientPlayer* targetPlayer, unsigned int uiSeat, bool bJack)
{
    if (!targetPlayer)
        return false;

    CClientPlayer* pPed = targetPlayer; 
    if (!pPed)
        return false;

    CClientVehicle* pVehicle = pPed->GetOccupiedVehicle(); 
    if (!pVehicle)
        return false;

    return WarpPlayerIntoVehicle(pVehicle, uiSeat, bJack); 
}


bool __cdecl KillPed(CClientEntity* entity, CClientEntity* pKiller, unsigned char ucKillerWeapon, unsigned char ucBodyPart, bool bStealth)
{
    if (!entity || !pKiller)
    {
        return false;
    }


    console->error("Worked.");


    return client->callKillPed(entity, pKiller, ucKillerWeapon, ucBodyPart, bStealth);
}


void KillNearestRandomPlayer()
{

    CClientPlayer* localPlayer = manager->get_local_player();
    if (!localPlayer)
        return;

    std::vector<CClientPlayer*> players = manager->get_players();
    if (players.empty()) return;

    
    std::vector<CClientEntity*> candidates;
    for (auto* player : players)
    {
        if (!player || player == localPlayer) continue;

        CClientEntity* entity = static_cast<CClientEntity*>(player);
        if (!entity) continue;

        candidates.push_back(entity);
    }

    if (candidates.empty()) return;

    int randomIndex = rand() % candidates.size();
    CClientEntity* targetEntity = candidates[randomIndex];
    if (!targetEntity) return;

    CClientEntity* localEntity = static_cast<CClientEntity*>(localPlayer);

    unsigned char weapon = 4;     
    unsigned char bodyPart = 9;   
    bool stealth = true;

        KillPed(targetEntity, localEntity, weapon, bodyPart, stealth);
        console->error("KillPed: Yakındaki oyuncu öldürüldü.");

}



typedef void(__thiscall* SetModelBlocking_t)(CClientVehicle* ECX, unsigned short usModel, unsigned char ucVariant, unsigned char ucVariant2);
SetModelBlocking_t oSetModelBlocking = nullptr;

typedef void(__thiscall* SetColli_t)(CClientEntity* ECX, CClientEntity* pEntity, bool bCanCollide);
SetColli_t oSetColli = nullptr;

typedef void(__thiscall* SetCamera_t)(CClientCamera* ECX, CClientEntity* pEntity,eCamMode eMode, bool bSmoothTransition);
SetCamera_t oSetCamera = nullptr;

typedef bool(__cdecl* SetElementDimension_t)(CClientEntity* pEntity, int usDimension);
SetElementDimension_t oSetElementDimension = nullptr;


typedef bool(__cdecl* SetElementInterior_t)(CClientEntity* pEntity, int ucInterior, bool bSetPosition, CVector* vecPosition);
SetElementInterior_t oSetElementInterior = nullptr;

typedef bool(__cdecl* SetVehicleHandling_t)(CClientVehicle* pVehicle, eHandlingProperty eProperty, float fValue);
SetVehicleHandling_t oSetVehicleHandling = nullptr;




void ChangeCurrentInterior(int ucInterior, CVector* vecPosition)
{
    CClientPlayer* localPlayer = manager->get_local_player();
    if (!localPlayer)
        return;
    CClientEntity* pTarget = static_cast<CClientEntity*>(localPlayer);

    oSetElementInterior(pTarget, ucInterior, true, vecPosition);
}

void SetHandling(eHandlingProperty eProperty, float fValue)
{
    CClientPlayer* local = manager->get_local_player();
    CClientVehicle* pVehicle = local->GetOccupiedVehicle();


    oSetVehicleHandling(pVehicle, eProperty, fValue);

}

void ChangeCurrentDimension(int usDimension)
{
    CClientPlayer* localPlayer = manager->get_local_player();
    if (!localPlayer)
        return;
    CClientEntity* pTarget = static_cast<CClientEntity*>(localPlayer);

   oSetElementDimension(pTarget, usDimension);
}


typedef void(__thiscall* Rotation_t)(CClientVehicle* ECX, CVector* vecRotation);
Rotation_t oRotation = nullptr;

typedef void(__thiscall* Reset_t)(CClientCamera* ECX);
Reset_t oResetCam = nullptr;



void SpectateEntity(CClientEntity* pTarget, CClientStreamElement* pStreamElement)
{
    if (!pTarget || !global || !global->m_pClientCamera || !oSetCamera)
        return;

    client->oSetStream(pStreamElement, true);
    if (!client->oSetStream)
        return;
    CClientCamera* cam = global->m_pManager->GetCamera();

    if (element->player.spectate)
    {
    oSetCamera(cam, pTarget, MODE_CAM_ON_A_STRING, false);
    }
    else
    {
        global->m_pGame->GetCamera()->Restore();    
        client->oSetStream(pStreamElement, true);

    }
}


typedef float(__thiscall* VehGetHealth_t)(CClientVehicle* ECX);
VehGetHealth_t oVehHealth = nullptr;

float GetVehHealth(CClientVehicle* vehicle)
{
    if (!vehicle || !oVehHealth)
        return 0.0f;

    return oVehHealth(vehicle);
}

std::thread collisionThread;
bool collisionThreadRunning = false;

void StartCollisionDisabler()
{
    if (collisionThreadRunning)
        return;

    collisionThreadRunning = true;
    collisionThread = std::thread([]()
        {
            bool hasDisabledOnce = false; 

            while (collisionThreadRunning)
            {
                if (element->vehicle.Cool)
                {
                    DisableVehicleCollision(false); 
                    hasDisabledOnce = false;   
                }
                else
                {
                    if (!hasDisabledOnce)
                    {
                        DisableVehicleCollision(true); 
                        hasDisabledOnce = true;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
    collisionThread.detach();
}


void DisableVehicleCollision(bool enabled)
{
    CClientPlayer* localPlayer = manager->get_local_player();
    if (!localPlayer)
        return;

    CClientVehicle* pVehicle = localPlayer->GetOccupiedVehicle();
    if (!pVehicle)
        return;

    for (auto Iter = global->m_pPedManager->IterBegin(); Iter != global->m_pPedManager->IterEnd(); ++Iter)
    {
        oSetColli(pVehicle, *Iter, enabled);
    }

    for (auto Iter = global->m_pVehicleManager->IterBegin(); Iter != global->m_pVehicleManager->IterEnd(); ++Iter)
    {
        oSetColli(pVehicle, *Iter, enabled);
    }

    for (auto& pObject : global->m_pClientGame->GetObjectManager()->GetObjects())
    {
        oSetColli(pVehicle, pObject, enabled);
    }
}


void ChangeModel(int iNewModel)
{
    CClientPlayer* localPlayer = manager->get_local_player();

    CClientVehicle* myVehicle = localPlayer->GetOccupiedVehicle();

    if (!myVehicle)
    {
        ImGui::InsertNotification({ ImGuiToastType::Warning, 3000, "Please enter a vehicle to spoof the model.", "Vehicle Tools" });
        return; 
    }

    if (!element->content.loaded_client || !global->m_pCore->IsConnected() || !myVehicle)
        return;

    if (!(iNewModel >= 400 && iNewModel <= 611))
        return;

    if (oSetModelBlocking != nullptr)
        oSetModelBlocking(myVehicle, iNewModel, 255, 255);
}




std::vector<CClientPlayer*> playersToPull;
 size_t pullIndex;
CVector originalVehPos;
 bool bReturning;
 bool bInitialized;


void __fastcall VehGetPosition(void* ECX, void* EDX, CVector& vecPosition)
{
    CVector GercekPos;
    client->callVehGetPosition(ECX, GercekPos);

    CClientPlayer* local = manager->get_local_player();
    if (!local) { vecPosition = GercekPos; return; }

    if (element->player.start || (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
    {
        if (pullIndex == 0 && !bReturning)
        {
            playersToPull = manager->get_players();
            originalVehPos = GercekPos;
        }

        if (pullIndex < playersToPull.size())
        {
            CClientPlayer* targetPlayer = playersToPull[pullIndex];
            if (targetPlayer && targetPlayer->GetGamePlayer() && targetPlayer != local)
            {
                CVector targetPos = *targetPlayer->GetGamePlayer()->GetPosition();
                targetPos.fZ -= 0.5f;
                vecPosition = targetPos;
            }
            pullIndex++;
        }
        else if (!bReturning)
        {
            vecPosition = originalVehPos;
            bReturning = true;
        }
        else
        {
            pullIndex = 0;
            bReturning = false;
            playersToPull.clear();
            element->player.start = false; 
        }

        return;
    }

    vecPosition = GercekPos;
}






void AracPosKralim() {

    std::thread([] {
        while (!GetModuleHandleA("client.dll")) {
            Sleep(10);
        }

        const char pattern[] = { "\x55\x8B\xEC\x8B\xC1\x80\xB8" };
        const char mask[] = { "xxxxxxx" };
        SigScan scan;
        aracposkontrol = scan.FindPattern("client.dll", pattern, mask);
        console->success("yuklendiarac");

        if (aracposkontrol != NULL) {

            MH_RemoveHook(client->callVehGetMoveSpeed);
            MH_CreateHook(reinterpret_cast<LPVOID>(aracposkontrol), &VehGetMoveSpeed, reinterpret_cast<LPVOID*>(&client->callVehGetMoveSpeed));
            MH_EnableHook(MH_ALL_HOOKS);
        }
        }).detach();
};





void PedPosHookla() {
    std::thread([] {
        while (!netc->  o_get_connected_server) {

        }

        const char pattern[] = { "\x55\x8B\xEC\x57\x8B\xF9\x8B\x8F\x00\x00\x00\x00\x85\xC9\x74\x00\x8B\x41\x00\x83\xC1\x00\x03\x48\x00\x8B\x01\xFF\x50\x00\x85\xC0" };
        const char mask[] = { "xxxxxxxx????xxx?xx?xx?xx?xxxx?xx" };

        SigScan scan;
         pedposkontrol = scan.FindPattern("client.dll", pattern, mask);
        console->success("yuklendiped");

        if (pedposkontrol != NULL) {
            MH_RemoveHook(client->callPedGetPosition);
            MH_CreateHook(reinterpret_cast<LPVOID>(pedposkontrol), &PedGetPosition, reinterpret_cast<LPVOID*>(&client->callPedGetPosition));
            MH_EnableHook(MH_ALL_HOOKS);
        }
        }).detach();
}

void VehHeal() {
    std::thread([] {
        while (!netc->o_get_connected_server) {

        }

        const char pattern[] = { "\x55\x8B\xEC\xF3\x0F\x10\x45\x00\x0F\x57\xC9" };
        const char mask[] = { "xxxxxxx?xxx" };

        SigScan scan;
        vehkontrol = scan.FindPattern("client.dll", pattern, mask);
        console->success("Veh tes");

        if (vehkontrol != NULL) {
            MH_RemoveHook(client->callVehHeal);
            MH_CreateHook(reinterpret_cast<LPVOID>(vehkontrol), &VehHealth, reinterpret_cast<LPVOID*>(&client->callVehHeal));
            MH_EnableHook(MH_ALL_HOOKS);
        }
        }).detach();
}


void VehiclePosHookla() {
    std::thread([] {
        while (!netc->o_get_connected_server) {

        }

        const char pattern[] = { "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF9\x80\xBF\x00\x00\x00\x00\x00\x74" };
        const char mask[] = { "xxxx?x????xx????xxx?xxxx????xxxxx?xx????xxxx?????x" };

        SigScan scan;
         vehposkontrol = scan.FindPattern("client.dll", pattern, mask);
        console->success("yuklendi vehicle");

        if (vehposkontrol != NULL) {
            MH_RemoveHook(client->callVehGetPosition);
            MH_CreateHook(reinterpret_cast<LPVOID>(vehposkontrol), &VehGetPosition, reinterpret_cast<LPVOID*>(&client->callVehGetPosition));
            MH_EnableHook(MH_ALL_HOOKS);
        }
        }).detach();
}








void FovHookla() {
    std::thread([] {
        while (!netc->o_get_connected_server) {

        }

        const char* pattern = "\x55\x8B\xEC\x8B\xC1\x8B\x88\x00\x00\x00\x00\x85\xC9\x74\x00\x8B\x41\x00\x8D\x49\x00\x8B\x40\x00\x8D\x0C\x01\x8B\x01\x5D\xFF\x20\x8B\x4D\x00\xF3\x0F\x7E\x80\x00\x00\x00\x00\x66\x0F\xD6\x01\x8B\x80\x00\x00\x00\x00\x89\x41\x00\x5D\xC2\x00\x00\xCC\xCC\xCC\xCC\x55\x8B\xEC\x83\xEC";
        const char* mask = "xxxxxxx????xxx?xx?xx?xx?xxxxxxxxxx?xxxx????xxxxxx????xx?xx??xxxxxxxxx";


        SigScan scan;
         GetMove = scan.FindPattern("client.dll", pattern, mask);


        if (GetMove != NULL) {
            MH_RemoveHook(client->callGetMoveSpeed);
            MH_CreateHook(reinterpret_cast<LPVOID>(GetMove), &GetMovePed, reinterpret_cast<LPVOID*>(&client->callGetMoveSpeed));
            MH_EnableHook(MH_ALL_HOOKS);
            console->success("okey fov\n");
        }

        }).detach();
}


void manage_hook(void** original_function, void* hook_function, const char* function_name) {
    if (*original_function != nullptr) {
        if (MH_RemoveHook(*original_function) == MH_OK) {
            console->success((std::string("Successfully removed hook for ") + function_name + ".\n").c_str());
        }
        else {
            console->error((std::string("Failed to remove hook for ") + function_name + ".\n").c_str());
        }
    }

    if (MH_CreateHook(*original_function, hook_function, reinterpret_cast<LPVOID*>(original_function)) == MH_OK) {
        if (MH_EnableHook(*original_function) == MH_OK) {
            console->success((std::string("Successfully created and enabled hook for ") + function_name + ".\n").c_str());
        }
        else {
            console->error((std::string("Failed to enable hook for ") + function_name + ".\n").c_str());
        }
    }
    else {
        console->error((std::string("Failed to create hook for ") + function_name + ".\n").c_str());
    }
}


typedef double(__cdecl* ptrTotalPhysicalMemory)();
ptrTotalPhysicalMemory push_TotalPhysicalMemory = nullptr;

double __cdecl TotalPhysicalMemory()
{
    return 16384.0; 
}


typedef UINT64(__cdecl* ptrGetWMITotalPhysicalMemory)();
ptrGetWMITotalPhysicalMemory original_GetWMITotalPhysicalMemory = nullptr;

UINT64 __cdecl hooked_GetWMITotalPhysicalMemory()
{
    return 16ULL * 1024 * 1024 * 1024; 
}



typedef int(__cdecl* lua_pushstring_t)(lua_State* L, const char* s);
typedef int(__cdecl* lua_pushnumber_t)(lua_State* L, double n);

lua_pushstring_t o_lua_pushstring = nullptr;
lua_pushnumber_t o_lua_pushnumber = nullptr;


bool next_is_total_memory = false;


int __cdecl h_lua_pushstring(lua_State* L, const char* s)
{
    if (s && strcmp(s, "TotalPhysicalMemory") == 0) {
        next_is_total_memory = true;
    }

    return o_lua_pushstring(L, s);
}

int __cdecl h_lua_pushnumber(lua_State* L, double n)
{
    if (next_is_total_memory) {
        next_is_total_memory = false;
        return o_lua_pushnumber(L, 16384.0); 
    }

    return o_lua_pushnumber(L, n);
}

void SetupMemoryHooks()
{
    o_lua_pushstring = (lua_pushstring_t)utilities::c_pattern::find_pattern(
        "client.dll",
        "\xE8\x00\x00\x00\x00\x83\xC4\x00\xE8\x00\x00\x00\x00\x8B\xC8\xE8",
        "x????xx?x????xxx"
    );

    if (o_lua_pushstring != nullptr) {
        MH_RemoveHook(o_lua_pushstring);
        MH_CreateHook(o_lua_pushstring, &h_lua_pushstring, reinterpret_cast<LPVOID*>(&o_lua_pushstring));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_lua_pushstring' address founded.\n");
    }
    else {
        console->error("'o_lua_pushstring' address not found.\n");
    }

    o_lua_pushnumber = (lua_pushnumber_t)utilities::c_pattern::find_pattern(
        "client.dll",
        "\xE8\x00\x00\x00\x00\x6A\x00\x56\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x56\xE8\x00\x00\x00\x00\x83\xC4\x00\x8D\x8D",
        "x????x?xx????x????xx????xx?xx"
    );

    if (o_lua_pushnumber != nullptr) {
        MH_RemoveHook(o_lua_pushnumber);
        MH_CreateHook(o_lua_pushnumber, &h_lua_pushnumber, reinterpret_cast<LPVOID*>(&o_lua_pushnumber));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_lua_pushnumber' address founded.\n");
    }
    else {
        console->error("'o_lua_pushnumber' address not found.\n");
    }
}

typedef bool(__cdecl* RemoveFromVehicle_t)(DWORD dwPlayer);
RemoveFromVehicle_t oRemoveFromVehicle = nullptr;

void c_client::hkRemoveFromDrogas(DWORD dwPlayer) {
    oRemoveFromVehicle(dwPlayer);
}


void c_client::release()
{


  


    o_init_client = (init_client_t)utilities::c_pattern::find_pattern(patterns->init_client.module, patterns->init_client.pattern, patterns->init_client.mask);
    if (o_init_client != nullptr)
    {
        MH_RemoveHook(o_init_client);
        MH_CreateHook(o_init_client, &h_init_client, reinterpret_cast<LPVOID*>(&o_init_client));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_init_client' address founded.\n");
    }
    else
    {
        console->error("'o_init_client' address not founded.\n");
    }




    o_Send_bullet_sync_fire = (t_Send_bullet_sync_fire)utilities::c_pattern::find_pattern(patterns->bullet_sync.module, patterns->bullet_sync.pattern, patterns->bullet_sync.mask);
    if (o_Send_bullet_sync_fire != nullptr)
    {
        MH_RemoveHook(o_Send_bullet_sync_fire);
        MH_CreateHook(o_Send_bullet_sync_fire, &h_bullet_sync_hook, reinterpret_cast<LPVOID*>(&o_Send_bullet_sync_fire));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_Send_bullet_sync_fire' address founded.\n");
    }
    else
    {
        console->error("'o_Send_bullet_sync_fire' address not founded.\n");
    }        
    

    

    



    o_on_player_damage = (on_player_damage_t)utilities::c_pattern::find_pattern(patterns->on_player_damage.module, patterns->on_player_damage.pattern, patterns->on_player_damage.mask);
    if (o_on_player_damage != nullptr)
    {
        MH_RemoveHook(o_on_player_damage);
        MH_CreateHook(o_on_player_damage, &h_on_player_damage, reinterpret_cast<LPVOID*>(&o_on_player_damage));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_on_player_damage' address founded.\n");
    }
    else
    {
        console->error("'o_on_player_damage' address not founded.\n");
    }



    o_get_weapon_type = (get_weapon_type_t)utilities::c_pattern::find_pattern(patterns->get_weapon_type.module, patterns->get_weapon_type.pattern, patterns->get_weapon_type.mask);
    if (o_get_weapon_type != nullptr)
    {
        console->success("'o_get_weapon_type' address founded.\n");
    }
    else
    {
        console->error("'o_get_weapon_type' address not founded.\n");
    }    
    
    o_get_weapon_type = (get_weapon_type_t)utilities::c_pattern::find_pattern(patterns->get_weapon_type.module, patterns->get_weapon_type.pattern, patterns->get_weapon_type.mask);
    if (o_get_weapon_type != nullptr)
    {
        console->success("'o_get_weapon_type' address founded.\n");
    }
    else
    {
        console->error("'o_get_weapon_type' address not founded.\n");
    }


    o_set_element_model = (set_element_model_t)utilities::c_pattern::find_pattern(patterns->set_element_model.module, patterns->set_element_model.pattern, patterns->set_element_model.mask);
    if (o_set_element_model != nullptr)
    {
        console->success("'o_set_element_model' address founded.\n");
    }
    else
    {
        console->error("'o_set_element_model' address not founded.\n");
    }    
    


    o_output_chat_box = (output_chat_box_t)utilities::c_pattern::find_pattern(patterns->output_chat_box.module, patterns->output_chat_box.pattern, patterns->output_chat_box.mask);
    if (o_output_chat_box != nullptr)
    {
        console->success("'out_chat_box' address founded.\n");
    }
    else
    {
        console->error("'out_chat_box' address not founded.\n");
    }


    oWarpPlayerIntoVehicle = (WarpPlayerIntoVehicle_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x83\xE4\x00\x83\xEC\x00\x53\x56\x8B\xF1", "xxxxx?xx?xxxx");
    if (oWarpPlayerIntoVehicle != nullptr)
    {
        console->success("'WarpPlayerIntoVehicle' address founded.\n");
    }
    else
    {
        console->error("'sessss' address not founded.\n");
    }

    oSetModelBlocking = (SetModelBlocking_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x53\x8B\xD9", "xxxxx????x????xxxx?xxx");
    if (oSetModelBlocking != nullptr)
    {
        console->success("'oSetModelBlocking' address founded.\n");
    }
    else
    {
        console->error("'oSetModelBlocking' address not founded.\n");
    }     
    
    
    towed_vehicle = (towed_vehicle_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x83\xEC\x00\x53\x8B\xD9\x56\x57\x8B\x7D\x00\x83\xBB", "xxxxx?xxxxxxx?xx");
    if (towed_vehicle != nullptr)
    {
        console->success("'towed_vehicle' address founded.\n");
    }
    else
    {
        console->error("'towed_vehicle' address not founded.\n");
    }        
    
  
    


    oSetColli = (SetColli_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x83\xEC\x00\x53\x56\x8B\xF1\x8D\x45", "xxxxx?xxxxxx");
    if (oSetColli != nullptr)
    {
        console->success("'oSetColli' address founded.\n");
    }
    else
    {
        console->error("'oSetColli' address not founded.\n");
    }       
    
    oSetCamera = (SetCamera_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x51\x53\x56\x8B\x75\x00\x57\x8B\xF9\x85\xF6", "xxxxxxxx?xxxxx");
    if (oSetCamera != nullptr)
    {
        console->success("'oSetCamera' address founded.\n");
    }
    else
    {
        console->error("'oSetCamera' address not founded.\n");
    }       
    
    oSetStream = (Stream_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x80\xB9\x00\x00\x00\x00\x00\x75\x00\x80\xB9", "xxxxx?????x?xx");
    if (oSetStream != nullptr)
    {
        console->success("'oSetStream' address founded.\n");
    }
    else
    {
        console->error("'oSetStream' address not founded.\n");
    }      
    
    oSetElementDimension = (SetElementDimension_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\x7D\x00\x8B\x5D\x00\x83\x7F\x00\x00\x74\x00\x80\xBF\x00\x00\x00\x00\x00\x74\x00\x8D\x45\x00\x8B\xCF\x50\xE8\x00\x00\x00\x00\x8B\x4D\x00\xC7\x45\x00\x00\x00\x00\x00\x8B\x31\x3B\x71\x00\x74\x00\x0F\x1F\x80\x00\x00\x00\x00\x8B\x06\x80\xB8\x00\x00\x00\x00\x00\x75\x00\x53\x50\xE8\x00\x00\x00\x00\x8B\x4D\x00\x83\xC4\x00\x83\xC6\x00\x3B\x71\x00\x75\x00\x8B\x75\x00\xC7\x45\x00\x00\x00\x00\x00\x85\xF6\x74\x00\x83\xC8\x00\xF0\x0F\xC1\x46\x00\x75\x00\x8B\x06\x8B\xCE\xFF\x10\x83\xC8\x00\xF0\x0F\xC1\x46\x00\x75\x00\x8B\x06\x8B\xCE\xFF\x50\x00\x8B\x07\x8B\xCF\xFF\x50\x00\x48\x83\xF8\x00\x77\x00\x0F\xB6\x80\x00\x00\x00\x00\xFF\x24\x85\x00\x00\x00\x00\x8B\x87", "xxxx?x????xx????xxx?xxxx????xxxxx?xx????xx?xx?xx??x?xx?????x?xx?xxxx????xx?xx?????xxxx?x?xxx????xxxx?????x?xxx????xx?xx?xx?xx?x?xx?xx?????xxx?xx?xxxx?x?xxxxxxxx?xxxx?x?xxxxxx?xxxxxx?xxx?x?xxx????xxx????xx");
    if (oSetElementDimension != nullptr)
    {
        console->success("'oSetEleementDimension' address founded.\n");
    }
    else
    {
        console->error("'oSetEleementDimension' address not founded.\n");
    }     
    
    oSetElementInterior = (SetElementInterior_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\x7D\x00\x8B\x5D\x00\x83\x7F\x00\x00\x0F\x84\x00\x00\x00\x00\x80\xBF\x00\x00\x00\x00\x00\x74\x00\x8D\x45\x00\x8B\xCF\x50\xE8\x00\x00\x00\x00\x8B\x4D\x00\xC7\x45\x00\x00\x00\x00\x00\x8B\x31\x3B\x71\x00\x74\x00\x0F\x1F\x00\x8B\x06\x80\xB8\x00\x00\x00\x00\x00\x75\x00\xFF\x75\x00\xFF\x75\x00\x53\x50\xE8\x00\x00\x00\x00\x8B\x4D\x00\x83\xC4\x00\x83\xC6\x00\x3B\x71\x00\x75\x00\x8B\x75\x00\xC7\x45\x00\x00\x00\x00\x00\x85\xF6\x74\x00\x83\xC8\x00\xF0\x0F\xC1\x46\x00\x75\x00\x8B\x06\x8B\xCE\xFF\x10\x83\xC8\x00\xF0\x0F\xC1\x46\x00\x75\x00\x8B\x06\x8B\xCE\xFF\x50\x00\x8B\x07\x8B\xCF\x53", "xxxx?x????xx????xxx?xxxx????xxxxx?xx????xx?xx?xx??xx????xx?????x?xx?xxxx????xx?xx?????xxxx?x?xxxxxxx?????x?xx?xx?xxx????xx?xx?xx?xx?x?xx?xx?????xxx?xx?xxxx?x?xxxxxxxx?xxxx?x?xxxxxx?xxxxx");
    if (oSetElementInterior != nullptr)
    {
        console->success("'oSetElementInterior' address founded.\n");
    }
    else
    {
        console->error("'oSetElementInterior' address not founded.\n");
    }    
    

    oSetVehicleHandling = (SetVehicleHandling_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x56\x8B\x75\x00\x85\xF6\x75\x00\x89\x35\x00\x00\x00\x00\x8B\xCE\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xF3\x0F\x10\x45", "xxxxxx?xxx?xx????xxx????xxx?xxxx");
    if (oSetVehicleHandling != nullptr)
    {
        console->success("'oSetVehicleHandling' address founded.\n");
    }
    else
    {
        console->error("'oSetVehicleHandling' address not founded.\n");
    }    
    


    oIsElementFrozen = (IsElementFrozen_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x56\x8B\x75\x00\x8B\xCE\x8B\x06\xFF\x50\x00\x48\x83\xF8\x00\x77\x00\x0F\xB6\x80\x00\x00\x00\x00\xFF\x24\x85\x00\x00\x00\x00\x8B\x45\x00\x8A\x8E\x00\x00\x00\x00\x5E\x88\x08\xB0\x00\x5D\xC3\x8B\x45\x00\x8A\x8E\x00\x00\x00\x00\x5E\x88\x08\xB0\x00\x5D\xC3\x8B\x45\x00\x8A\x8E\x00\x00\x00\x00\x5E\x88\x08\xB0\x00\x5D\xC3\x32\xC0", "xxxxxx?xxxxxx?xxx?x?xxx????xxx????xx?xx????xxxx?xxxx?xx????xxxx?xxxx?xx????xxxx?xxxx");
    if (oIsElementFrozen != nullptr)
    {
        console->success("'oIsElementFrozen' address founded.\n");
    }
    else
    {
        console->error("'oIsElementFrozen' address not founded.\n");
    }
    
    
    oGetRotationDegrees = (GetRotationDegrees_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x8B\x01\x56\x8B\x75", "xxxxxxxx");
    if (oGetRotationDegrees != nullptr)
    {
        console->success("'oGetRotationDegress' address founded.\n");
    }
    else
    {
        console->error("'oGetRotationDegress' address not founded.\n");
    }    
    

    
    oWriteVehiclePuresync = (WriteVehiclePuresync_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xD9\x89\x9D", "xxxx?x????xx????xxx????xxxx????xxxxx?xx????xxxx");
    if (oWriteVehiclePuresync != nullptr)
    {
        MH_RemoveHook(oWriteVehiclePuresync);
        MH_CreateHook(oWriteVehiclePuresync, &hkWriteVehiclePuresync, reinterpret_cast<LPVOID*>(&oWriteVehiclePuresync));
        MH_EnableHook(oWriteVehiclePuresync);
        console->success("'oWriteVehiclePuresync' address founded.\n");
    }
    else
    {
        console->error("'oWriteVehiclePuresync' address not founded.\n");
    }    
    
    

    
    



    oBulletImpact = (BulletImpact_t)utilities::c_pattern::find_pattern(xorstr_("client.dll"), ("\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF9\xF3\x0F\x10\x45"), xorstr_("xxxx?x????xx????xxx?xxxx????xxxxx?xx????xxxxxx"));
    if (oBulletImpact != nullptr)
    {
       MH_RemoveHook(oBulletImpact);
       MH_CreateHook(oBulletImpact, hkBulletImpact, reinterpret_cast<LPVOID*>(&oBulletImpact));
       MH_EnableHook(oBulletImpact);
    console->success("'oBulletImpact' address founded.\n");
    }
    else
    {
        console->error("'oBulletImpact' address not founded.\n");
    }

    oBulletFire = (BulletFire_t)utilities::c_pattern::find_pattern(xorstr_("client.dll"), xorstr_("\x55\x8B\xEC\xA1\x00\x00\x00\x00\xC6\x05\x00\x00\x00\x00\x00"), xorstr_("xxxx????xx?????"));
    if (oBulletFire != nullptr)
    {

    MH_RemoveHook(oBulletFire);
    MH_CreateHook(oBulletFire, hkBulletFire, reinterpret_cast<LPVOID*>(&oBulletFire));
    MH_EnableHook(oBulletFire);

    console->success("'oBulletFire' address founded.\n");

    }
    else
    {
        console->error("'oBulletFire' address not founded.\n");
    }

    oResetCam = (Reset_t)utilities::c_pattern::find_pattern("client.dll", "\x56\x8B\xF1\x8B\x8E\x00\x00\x00\x00\x57\x8D\xBE", "xxxxx????xxx");
    if (oResetCam != nullptr)
    {
        console->success("'oResetCam' address founded.\n");
    }
    else
    {
        console->error("'oResetCam' address not founded.\n");
    }    
    
    oVehHealth = (VehGetHealth_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x51\x8B\x91\x00\x00\x00\x00\x85\xD2\x74\x00\x8B\x02\x8B\xCA\x8B\x80\x00\x00\x00\x00\xFF\xD0\xD9\x5D\x00\xD9\x45\x00\x8B\xE5\x5D\xC3\xF3\x0F\x10\x81\x00\x00\x00\x00\xF3\x0F\x11\x45\x00\xD9\x45\x00\x8B\xE5\x5D\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x8B\x91", "xxxxxx????xxx?xxxxxx????xxxx?xx?xxxxxxxx????xxxx?xx?xxxxxxxxxxxxxx");
    if (oVehHealth != nullptr)
    {
        console->success("'oVehHealth' address founded.\n");
    }
    else
    {
        console->error("'oVehHealth' address not founded.\n");
    }







    o_get_resource_name = (get_resource_name_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x51\xA1\x00\x00\x00\x00\x56", "xxxxx????x");
    if (o_get_resource_name != nullptr)
    {
        console->success("'get_resource_name' address founded.\n");
    }
    else
    {
        console->error("'get_resource_name' address founded.\n");
    }

    o_stop_resource = (stop_resource_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x83\xEC\x00\x53\x56\x66\x8B\x75", "xxxxx?xxxxx");
    if (o_stop_resource != nullptr)
    {
        console->success("'stop_resource' address founded.\n");
    }
    else
    {
        console->error("'stop_resource' address founded.\n");
    }    
    
    o_resource_constructor = (resource_constructor_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF1\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x89\x45\x00\xC7\x45\x00\x00\x00\x00\x00\x85\xC0\x74\x00\xFF\x75\x00\x8B\xC8\xFF\x75\x00\xFF\x75\x00\xFF\x75\x00\xFF\x75\x00\xFF\x75\x00\xFF\x75", "xxxx?x????xx????xxx?xxxx????xxxxx?xx????xxx????x????xx?xx?xx?????xxx?xx?xxxx?xx?xx?xx?xx?xx");
    if (o_resource_constructor != nullptr)
    {
        MH_RemoveHook(o_resource_constructor);
        MH_CreateHook(o_resource_constructor, &h_resource_constructor, reinterpret_cast<LPVOID*>(&o_resource_constructor));
        MH_EnableHook(MH_ALL_HOOKS);

        console->success("'o_resource_constructor' address founded.\n");
    }
    else
    {
        console->error("'o_resource_constructor' address founded.\n");
    }

    o_give_weapon = (give_weapon_t)utilities::c_pattern::find_pattern(patterns->give_weapon.module, patterns->give_weapon.pattern, patterns->give_weapon.mask);
    if (o_give_weapon == nullptr)
    {
        console->error("'give_weapon' address not found\n");
    }




    oLoadScriptFromBuffer = (LoadScriptFromBuffer_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xF9\x8B\x45\x00\x8B\x75", "xxxx?x????xx????xxx????x????xxxx?xxxxx?xx????xxxx?xx");
    if (oLoadScriptFromBuffer)
    {
        console->success("asasdasd");

        if (Script_ID > 0)
        {
            for (size_t i = 0; i < Script_ID + 1; i++)
            {
                Call_ECX[i] = nullptr;
                Call_This[i] = nullptr;
                Script_Name[i] = "";
            }
            Script_ID = 0;
        }
        Reset_Script_And_Resources();
        MH_RemoveHook(oLoadScriptFromBuffer);
        MH_CreateHook(oLoadScriptFromBuffer, &LoadScriptFromBuffer, reinterpret_cast<LPVOID*>(&oLoadScriptFromBuffer));
        MH_EnableHook(MH_ALL_HOOKS);
    }
    else
    {
        console->error("Offset_LuaScriptFromBuffer dontwork");
    }






    o_trigger_server_event = (trigger_server_event_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x51\x53\x56\x57\x8B\x7D\x00\x85\xFF\x75\x00\x89\x3D", "xxxxxxxxx?xxx?xx");
    if (o_trigger_server_event != nullptr)
    {
        MH_RemoveHook(o_trigger_server_event);
        MH_CreateHook(o_trigger_server_event, &h_trigger_server_event, reinterpret_cast<LPVOID*>(&o_trigger_server_event));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'trigger_server_event' address founded.\n");
    }
    else
    {
        console->error("'trigger_server_event' address founded.\n");

    }    
    
    
    o_trigger_event = (trigger_event_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\xFF\x75\x00\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\x8B\x4D", "xxxxx?xx????x????xxx?xx");
    if (o_trigger_event != nullptr)
    {
        MH_RemoveHook(o_trigger_event);
        MH_CreateHook(o_trigger_event, &h_trigger_event, reinterpret_cast<LPVOID*>(&o_trigger_event));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_trigger_event' address founded.\n");
    }
    else
    {
        console->error("'o_trigger_event' address founded.\n");

    }      


    
    
    
    
    o_debug_hook_call = (debug_hook_call_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x53\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x80\x3D", "xxxx?x????xx????xxx????x????xxxx?xxxxxx?xx????xx");
    if (o_debug_hook_call != nullptr)
    {
        MH_RemoveHook(o_debug_hook_call);
        MH_CreateHook(o_debug_hook_call, &h_add_debug_call, reinterpret_cast<LPVOID*>(&o_debug_hook_call));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_debug_hook_call' address founded.\n");
    }
    else
    {
        console->error("'o_debug_hook_call' address founded.\n");

    }       
    
    o_add_event = (add_event_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x89\x4D\x00\x8B\x7D\x00\x8B\x45\x00\x89\x45\x00\x85\xFF", "xxxx?x????xx????xxx?x????xxxx?xxxxx?xx????xx?xx?xx?xx?xx");
    if (o_add_event != nullptr)
    {
        MH_RemoveHook(o_add_event);
        MH_CreateHook(o_add_event, &h_add_event_hook, reinterpret_cast<LPVOID*>(&o_add_event));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_add_event' address founded.\n");
    }
    else
    {
        console->error("'o_add_event' address founded.\n");

    }     


    o_take_player_screenshot = (take_player_screenshot_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x89\x4D\x00\x8B\x45\x00\x89\x45\x00\x8B\x45\x00\x89\x45\x00\x85\xC0", "xxxx?x????xx????xxx?x????xxxx?xxxxx?xx????xx?xx?xx?xx?xx?xx");
    if (o_take_player_screenshot != nullptr)
    {
        MH_RemoveHook(o_take_player_screenshot);
        MH_CreateHook(o_take_player_screenshot, &h_take_player_screenshot, reinterpret_cast<LPVOID*>(&o_take_player_screenshot));
        MH_EnableHook(MH_ALL_HOOKS);
    }
    else
    {
        console->success("take_player_screenshot address not found.\n");
    }    
    
    
    o_WritePlayerPuresync = (WritePlayerPuresync_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\xD9\x89\x5D\x00\x8B\x75\x00\x8B\xCE", "xxxx?x????xx????xxx????xxxx????xxxxx?xx????xxxx?xx?xx");
    if (o_WritePlayerPuresync != nullptr)
    {
        MH_RemoveHook(o_WritePlayerPuresync);
        MH_CreateHook(o_WritePlayerPuresync, &hkWritePlayerPuresync, reinterpret_cast<LPVOID*>(&o_WritePlayerPuresync));
        MH_EnableHook(MH_ALL_HOOKS);
    }
    else
    {
        console->success("o_WritePlayerPuresync address not found.\n");
    }


    
    o_add_event_handler = (add_event_handler_hook_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x56\x8B\x75\x00\x85\xF6\x75\x00\x89\x35\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x56\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xF3\x0F\x10\x45", "xxxxxx?xxx?xx????xx????xx????xxx?xxxx");
    if (o_add_event_handler != nullptr)
    {
        MH_RemoveHook(o_add_event_handler);
        MH_CreateHook(o_add_event_handler, &h_add_event_handler_hook, reinterpret_cast<LPVOID*>(&o_add_event_handler));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'o_add_event_handler' address founded.\n");
    }
    else
    {
        console->error("'o_add_event_handler' address founded.\n");

    }    



    

    o_add_debug_hook = (add_debug_hook_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\x00\x53\x56\x57\x50\x8D\x45\x00\x64\xA3\x00\x00\x00\x00\x8B\x45\x00\x8B\x55", "xxxx?x????xx????xxx????x????xxxx?xxxxxx?xx????xx?xx");
    if (o_add_debug_hook != nullptr)
    {
        MH_RemoveHook(o_add_debug_hook);
        MH_CreateHook(o_add_debug_hook, &h_add_debug_hook, reinterpret_cast<LPVOID*>(&o_add_debug_hook));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'add_debug_hook' address founded.\n");
    }
    else
    {
        console->error("'add_debug_hook' address founded.\n");
    }        
    
    



    o_is_world_special_property = (is_world_special_property_t)utilities::c_pattern::find_pattern("client.dll", "\x55\x8B\xEC\x8B\x45\x00\x83\xF8\x00\x0F\x87\x00\x00\x00\x00\xFF\x24\x85\x00\x00\x00\x00\xA1", "xxxxx?xx?xx????xxx????x");
    if (o_is_world_special_property != nullptr)
    {
        MH_RemoveHook(o_is_world_special_property);
        MH_CreateHook(o_is_world_special_property, &h_is_world_special_property, reinterpret_cast<LPVOID*>(&o_is_world_special_property));
        MH_EnableHook(MH_ALL_HOOKS);
        console->success("'is_world_special_property' address founded.\n");
    }
    else
    {
        console->error("'is_world_special_property' address founded.\n");
    }




    if (!element->content.loaded_client) {
        element->content.loaded_client = true;
        element->resource.resources_list.clear();
        element->executor.resources_list.clear();
        element->event.events_list.clear();
        element->dump.dumps_list.clear();

        console->success("Resources initialized successfully.\n");
    }
    else {
        console->warning("Resources already initialized. Skipping initialization.\n");
    }
}




 


void c_client::shutdown() {
    if (element->content.loaded_client) {
        element->content.loaded_client = false;
        console->success("Client shutdown successfully.\n");
    }
    else {
        console->warning("Client already shut down. Skipping shutdown.\n");
    }
}




 