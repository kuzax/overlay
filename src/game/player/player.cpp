#include "player.h"
#include <globals.h>
#include <data/elements.h>
#include <game/manager/manager.h>
#include "sdk.h"
#include <game/CColPoint.h>
void draw_airbreak()
{
    static float lastZ = 0.f; 
    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    if (!global->m_pGame || !global->m_pGame->GetCamera())
        return;

    CClientVehicle* vehicle = local->GetOccupiedVehicle();
    CClientEntity* entity = vehicle ? static_cast<CClientEntity*>(vehicle) : static_cast<CClientEntity*>(local);
    if (!entity)
        return;

    float f_rotation = global->m_pGame->GetCamera()->GetCameraRotation();
    float f_speed = 0.0f;

    CVector vec_position;
    entity->GetPosition(vec_position);

        CVector vec_move_speed(0.0f, 0.0f, 0.0f);       
        if (element->player.airbreak && !vehicle)
    {
        f_speed = element->player.airbreak_speed;

        local->GetGamePlayer()->SetCurrentRotation(-f_rotation);
        local->GetGamePlayer()->SetMoveSpeed(vec_move_speed);
        local->GetGamePlayer()->SetIsStanding(true);
    }
        else if (element->vehicle.airbreak && vehicle)
    {
        f_speed = element->vehicle.airbreak_speed;

        CVector Crash(INFINITE, INFINITE, INFINITE);       
        vehicle->GetRotationDegrees(Crash);

                CVector rotation(0.0f, 0.0f, -f_rotation);



        vehicle->GetGameVehicle()->SetMoveSpeed(vec_move_speed);

        local->GetGamePlayer()->SetMoveSpeed(CVector(0.0f, 0.0f, 0.0f));
        local->GetGamePlayer()->SetIsStanding(true);
    }
        else if (element->player.maprun && !vehicle)
    {
        f_speed = element->player.maprun_speed;

        CVector playerPos;
        entity->GetPosition(playerPos);

                CVector groundCheckPos = vec_position;
        float newZ = global->m_pGame->GetWorld()->FindGroundZFor3DPosition(&groundCheckPos) + 1.0f;

        CColPoint* pColPoint = nullptr;
        CEntity* pCollisionEntity = nullptr;

        SLineOfSightFlags flags;
        flags.bCheckBuildings = true;
        flags.bCheckVehicles = true;
        flags.bCheckObjects = true;
        flags.bCheckDummies = false;
        flags.bSeeThroughStuff = false;
        flags.bIgnoreSomeObjectsForCamera = false;
        flags.bShootThroughStuff = false;

        if (global->m_pGame->GetWorld()->ProcessLineOfSight(&vec_position, &playerPos, &pColPoint, &pCollisionEntity, flags))
        {
            if (pColPoint)
            {
                CVector hitPos = pColPoint->GetPosition();
                newZ = hitPos.fZ + 1.0f;
            }
        }

        float checkZ = global->m_pGame->GetWorld()->FindGroundZFor3DPosition(&playerPos);
        if (newZ < checkZ)
            newZ = checkZ + 1.0f;

                float waterLevel = 0.0f;
        if (global->m_pGame->GetWaterManager()->GetWaterLevel(playerPos, &waterLevel, true, nullptr))
        {
            if (newZ < waterLevel)
                newZ = waterLevel + 1.0f;
        }

                if (newZ < lastZ)
            newZ = lastZ;

        lastZ = newZ;

        vec_position.fZ = newZ;
    }
    else
    {
        return;
    }

        if (GetAsyncKeyState('W'))
    {
        vec_position.fX += sinf(f_rotation) * f_speed;
        vec_position.fY += cosf(f_rotation) * f_speed;
    }
    if (GetAsyncKeyState('S'))
    {
        vec_position.fX -= sinf(f_rotation) * f_speed;
        vec_position.fY -= cosf(f_rotation) * f_speed;
    }
    if (GetAsyncKeyState('D'))
    {
        vec_position.fX += cosf(f_rotation) * f_speed;
        vec_position.fY -= sinf(f_rotation) * f_speed;
    }
    if (GetAsyncKeyState('A'))
    {
        vec_position.fX -= cosf(f_rotation) * f_speed;
        vec_position.fY += sinf(f_rotation) * f_speed;
    }

        if ((element->player.airbreak && !vehicle) || (element->vehicle.airbreak && vehicle))
    {
        if (GetAsyncKeyState(VK_SPACE))
        {
            vec_position.fZ += f_speed;
        }
        if (GetAsyncKeyState(VK_CONTROL))
        {
            vec_position.fZ -= f_speed;
        }
    }

    entity->SetPosition(vec_position);
}

 



void draw_seprint()
{
    if (!element->player.sprint)
        return;

    manager->set_animation_speed("WOMAN_RUN", element->player.sprint_speed);
    manager->set_animation_speed("WOMAN_RUNBUSY", element->player.sprint_speed);
    manager->set_animation_speed("WOMAN_RUNPANIC", element->player.sprint_speed);
    manager->set_animation_speed("WOMAN_RUNSEXY", element->player.sprint_speed);
    manager->set_animation_speed("WOMAN_RUNSEXY", element->player.sprint_speed);
    manager->set_animation_speed("SPRINT_CIVI", element->player.sprint_speed);
    manager->set_animation_speed("SPRINT_PANIC", element->player.sprint_speed);
    manager->set_animation_speed("SWAT_RUN", element->player.sprint_speed);
    manager->set_animation_speed("FATSPRINT", element->player.sprint_speed);
}

void c_player::release()
{
	if (!manager->is_loaded())
		return;
	draw_airbreak();
    draw_seprint();
}


