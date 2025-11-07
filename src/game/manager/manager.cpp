#include "manager.h"
#include <globals.h>
#include <data/tables.h>
#include <client/client.h>
#include <data/elements.h>



bool c_manager::is_loaded()
{
    if (!element->content.loaded_client || !global->m_pCore->IsConnected())
        return false;

    return true;
}

CClientPlayer* c_manager::get_local_player()
{
    if (!is_loaded())
        return NULL;

    if (!global->m_pPlayerManager)
        return NULL;

    return global->m_pPlayerManager->GetLocalPlayer();
}


std::vector<CClientPlayer*> c_manager::get_overlays_players()
{
    std::vector<CClientPlayer*> overlayPlayers;

    if (!is_loaded() || !global->m_pPlayerManager)
        return overlayPlayers;

    for (auto iter = global->m_pPlayerManager->IterBegin(); iter != global->m_pPlayerManager->IterEnd(); ++iter)
    {
        if (CClientPlayer* player = *iter)
            overlayPlayers.push_back(player);
    }

    return overlayPlayers; }

std::vector<CClientPlayer*> c_manager::get_players()
{
    table->entity.players.clear();
    table->entity.peds.clear();  

    if (!is_loaded())
        return table->entity.players;

    if (global->m_pPlayerManager)
    {
        std::vector<CClientPlayer*>::const_iterator iter = global->m_pPlayerManager->IterBegin();
        for (; iter != global->m_pPlayerManager->IterEnd(); ++iter)
        {
            CClientPlayer* player = *iter;
            if (player)
            {
                table->entity.players.push_back(player);
            }
        }
    }


    if (global->m_pPedManager)
    {
        std::vector<CClientPed*>::const_iterator iter = global->m_pPedManager->IterBegin();
        for (; iter != global->m_pPedManager->IterEnd(); ++iter)
        {
            CClientPed* ped = *iter;
            if (ped)
            {
                table->entity.peds.push_back(ped);
            }
        }
    }
    return table->entity.players;
}

std::vector<CClientVehicle*> c_manager::get_vehicles()
{
    CClientPlayer* localPlayer = manager->get_local_player();


    table->entity.vehicles.clear();

    if (!is_loaded())
        return table->entity.vehicles;

    if (!global->m_pVehicleManager)
        return table->entity.vehicles;

        uint8_t localDimension = localPlayer ? localPlayer->GetDimension() : 0;

    std::vector<CClientVehicle*>::const_iterator iter = global->m_pVehicleManager->IterBegin();
    for (; iter != global->m_pVehicleManager->IterEnd(); ++iter)
    {
        CClientVehicle* vehicle = *iter;
        if (!vehicle)
            continue;


        if (vehicle->GetDimension() != localDimension)
            continue;

        table->entity.vehicles.push_back(vehicle);
    }

    return table->entity.vehicles;
}

std::vector<CClientPed*> c_manager::get_peds()
{
    table->entity.peds.clear();

    if (!is_loaded())
        return table->entity.peds;

    if (!global->m_pPedManager)
        return table->entity.peds;

    std::vector<CClientPed*>::const_iterator iter = global->m_pPedManager->IterBegin();
    for (; iter != global->m_pPedManager->IterEnd(); ++iter)
    {
        CClientPed* ped = *iter;
        if (ped)
        {
            table->entity.peds.push_back(ped);
        }
    }

    return table->entity.peds;
}


std::vector<CClientObject*> c_manager::get_objects()
{
    table->entity.objects.clear();

    if (!is_loaded())
        return table->entity.objects;

    if (!global->m_pObjectManager)
        return table->entity.objects;

    const auto& objectList = global->m_pObjectManager->GetObjects();
    int count = static_cast<int>(objectList.size());

    for (int i = 0; i < count; ++i)
    {
        CClientObject* object = objectList[i]; 
        if (object)
        {
            table->entity.objects.push_back(object);
        }
    }

    return table->entity.objects;
}

std::vector<CResource*> c_manager::get_resources()
{
    table->entity.resources.clear();

    if (!is_loaded())
        return table->entity.resources;

    if (!global->m_pResourceManager)
        return table->entity.resources;

    CMappedList<CResource*> resources = global->m_pResourceManager->GetResources();
    for (auto it = resources.begin(); it != resources.end(); ++it)
    {
        CResource* resource = *it;
        if (resource)
        {
            table->entity.resources.push_back(resource);
        }
    }

    return table->entity.resources;
}

eWeaponType c_manager::get_weapon_type(CClientPlayer* player)
{
    if (!is_loaded())
        return eWeaponType::WEAPONTYPE_INVALID;

    if (!global->m_pPlayerManager)
        return eWeaponType::WEAPONTYPE_INVALID;

    if (!player || !player->GetGamePlayer())
        return eWeaponType::WEAPONTYPE_INVALID;

    eWeaponSlot currentSlot = player->GetGamePlayer()->GetCurrentWeaponSlot();
    if (currentSlot == eWeaponSlot::WEAPONSLOT_TYPE_UNARMED)
        return eWeaponType::WEAPONTYPE_INVALID;

    CClientPed* ped = static_cast<CClientPlayer*>(player);
    if (!ped)
        return eWeaponType::WEAPONTYPE_INVALID;




    return client->o_get_weapon_type(ped, currentSlot);
}


eWeaponType c_manager::myget_weapon_type(CClientPlayer* player, eWeaponSlot slot)
{
    if (!is_loaded())
        return eWeaponType::WEAPONTYPE_INVALID;

    if (!global->m_pPlayerManager)
        return eWeaponType::WEAPONTYPE_INVALID;

    if (!player || !player->GetGamePlayer())
        return eWeaponType::WEAPONTYPE_INVALID;

    if (slot == eWeaponSlot::WEAPONSLOT_TYPE_UNARMED)
        return eWeaponType::WEAPONTYPE_INVALID;

    CClientPed* ped = static_cast<CClientPlayer*>(player);
    if (!ped)
        return eWeaponType::WEAPONTYPE_INVALID;

    return client->o_get_weapon_type(ped, slot);
}




bool c_manager::set_animation_speed(const SString& anime, float speed)
{
    if (!is_loaded())
        return false;

    if (!global->m_pPlayerManager)
        return false;

    CClientPlayer* local = get_local_player();
    if (!local || !local->GetGamePlayer())
        return false;

    CClientPed* ped = static_cast<CClientPlayer*>(local);
    if (!ped)
        return false;




    if (!global->m_pGame && !global->m_pGame->GetAnimManager())
        return false;

    if (anime.empty())
        return false;

    auto anim_association = global->m_pGame->GetAnimManager()->RpAnimBlendClumpGetAssociation(ped->GetClump(), anime);
    if (!anim_association)
        return false;



    anim_association->SetCurrentSpeed(speed);

    return true;
}

bool c_manager::set_player_skin(CClientPlayer* player, int skinId)
{
    if (!is_loaded() || !player)
        return false;

    CClientEntity* entity = static_cast<CClientEntity*>(player);
    if (!entity)
        return false;

    if (skinId < 0 || skinId > 312)
        return false;

    if (!client->o_set_element_model)
        return false;

    client->o_set_element_model(entity, skinId);

    return true;
}

bool c_manager::set_dimension(CClientPlayer* player, int uid)
{
    if (!is_loaded() || !player)
        return false;

    CClientEntity* entity = static_cast<CClientEntity*>(player);
    if (!entity)
        return false;

    if (uid < 0 || uid > 312)
        return false;

    if (!client->o_output_chat_box)
        return false;

    client->o_output_chat_box(entity, uid);

    return true;
}





bool c_manager::teleport_to_player(CClientEntity* target_entity)
{
    if (!is_loaded() || !target_entity)
        return false;

    CClientPlayer* local = get_local_player();
    if (!local)
        return false;

    CVector vec_position;
    target_entity->GetPosition(vec_position);

    CClientVehicle* vehicle = local->GetOccupiedVehicle();
    if (vehicle)
    {
        vehicle->SetPosition(vec_position);
    }
    else
    {
                CClientEntity* local_entity = static_cast<CClientEntity*>(local);
        if (local_entity)
        {
            local_entity->SetPosition(vec_position);
        }
    }

    return true;
}


bool c_manager::teleport_to_vehicle(CClientVehicle* target_vehicle)
{
    if (!is_loaded() || !target_vehicle)
        return false;

    CClientPlayer* local = get_local_player();
    if (!local)
        return false;

    CVector vec_position;
    target_vehicle->GetPosition(vec_position);

    CClientEntity* local_entity = static_cast<CClientEntity*>(local);
    if (local_entity)
    {
        local_entity->SetPosition(vec_position);
    }

    return true;
}

bool c_manager::create_explosion(CVector vecPosition, eExplosionType type)
{
    if (!is_loaded())
        return false;

    if (!client->o_create_explosion)
        return false;

    client->o_create_explosion(&vecPosition, type, false, 1.0f, true);

    return true;
}

struct SWeaponName { char szName[32]; };

SWeaponName WeaponNames[] = { {"Fist"}, {"Brassknuckle"}, {"Golfclub"}, {"Nightstick"}, {"Knife"}, {"Bat"}, {"Shovel"}, {"Poolstick"}, {"Katana"}, {"Chainsaw"}, {"Dildo"}, {"Dildo"}, {"Vibrator"}, {"Vibrator"}, {"Flower"}, {"Cane"}, {"Grenade"}, {"Teargas"}, {"Molotov"}, {"Rocket"}, {"Rocket"}, {"Freefall Bomb"}, {"Colt 45"}, {"Silenced"}, {"Deagle"}, {"Shotgun"}, {"Sawed-off"}, {"Combat Shotgun"}, {"Uzi"}, {"MP5"}, {"AK-47"}, {"M4"}, {"Tec-9"}, {"Rifle"}, {"Sniper"}, {"Rocket Launcher"}, {"Rocket Launcher HS"}, {"Flamethrower"}, {"Minigun"}, {"Satchel"}, {"Bomb"}, {"Spraycan"}, {"Fire Extinguisher"}, {"Camera"}, {"Nightvision"}, {"Infrared"}, {"Parachute"}, {"Last Weapon"}, {"Armour"}, {"Rammed"}, {"Ranover"}, {"Explosion"}, {"Driveby"}, {"Drowned"}, {"Fall"}, {"Unknown"}, {"Melee"}, {"Weapon"}, {"Flare"}, {"Tank Grenade"} };

const char* c_manager::get_weapon_name(unsigned char weaponId)
{
    if (weaponId < NUMELMS(WeaponNames))
    {
        return WeaponNames[weaponId].szName;
    }

    return "";
}









