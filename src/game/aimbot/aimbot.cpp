#include "aimbot.h"
#include <globals.h>
#include <game/manager/manager.h>
#include <data/elements.h>
#include <client/client.h>
#include <SlientAim.h>

bool is_visiblee(const CVector& from, const CVector& to, CClientPed* pTarget)
{
    if (!pTarget || !global->m_pClientGame || !global->m_pGame)
        return false;

        CWorld* pWorld = global->m_pGame->GetWorld();
    if (!pWorld)
        return false;

    SLineOfSightFlags flags;
    flags.bCheckBuildings = true;
    flags.bCheckDummies = true;
    flags.bCheckObjects = true;
    flags.bCheckPeds = false;
    flags.bCheckVehicles = pTarget->IsInVehicle() ? false : true;
    flags.bIgnoreSomeObjectsForCamera = false;
    flags.bSeeThroughStuff = true;
    flags.bShootThroughStuff = true;

    return pWorld->IsLineOfSightClear(&from, &to, flags);
}

void c_aimbot::draw_aimbot_fov()
{
    if (!element->aimbot.state)
        return;

    if (!element->aimbot.b_fov)
        return;

    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera)
        return;

    eCamMode cmera_viewmode = static_cast<eCamMode>(camera->GetCam(camera->GetActiveCam())->GetMode());
    if (cmera_viewmode == 53 || cmera_viewmode == 7)
    {
        s_aimbot_config.is_player_aiming = true;
    }
    else
    {
        s_aimbot_config.is_player_aiming = false;
    }

    eWeaponType weapon = manager->get_weapon_type(manager->get_local_player());
    if (weapon != WEAPONTYPE_SNIPERRIFLE)
    {
        s_aimbot_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        s_aimbot_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y * 0.4f;
    }
    else
    {
        s_aimbot_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
        s_aimbot_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;
    }

    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(s_aimbot_config.vec_crosshair.fX, s_aimbot_config.vec_crosshair.fY), element->aimbot.i_fov, IM_COL32(255, 255, 255, 255));
}

void c_aimbot::draw_crosshair_fov()
{
    if (!element->crosshair.state)
        return;

    if (!element->crosshair.b_fov)
        return;

    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera)
        return;

    eCamMode cmera_viewmode = static_cast<eCamMode>(camera->GetCam(camera->GetActiveCam())->GetMode());
    if (cmera_viewmode == 53 || cmera_viewmode == 7)
    {
        s_crosshair_config.is_player_aiming = true;
    }
    else
    {
        s_crosshair_config.is_player_aiming = false;
    }

    eWeaponType weapon = manager->get_weapon_type(manager->get_local_player());
    if (weapon != WEAPONTYPE_SNIPERRIFLE)
    {
        s_crosshair_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        s_crosshair_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y * 0.4f;
    }
    else
    {
        s_crosshair_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
        s_crosshair_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;
    }



    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(s_crosshair_config.vec_crosshair.fX, s_crosshair_config.vec_crosshair.fY), element->crosshair.i_fov, IM_COL32(255, 255, 255, 255));
}

void c_aimbot::draw_crosshair_target()
{
    s_crosshair_config.i_target_player = nullptr;
    float nearestDistance = 9999.f;
    float nearestFOVDistance = 9999.f;
    float closest3DDistance = 9999.f;
    float lowestHP = 9999.f;

    if (!element->crosshair.state || !s_crosshair_config.is_player_aiming)
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CVector vecLocalPos;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_PELVIS, &vecLocalPos))
        return;

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer() || target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;


        if (element->crosshair.team_check)
        {
            if (is_friend(target->GetNametagText()))
                continue;
        }



        if (element->crosshair.deadcheck)
        {
            CClientEntity* pPlayer = static_cast<CClientEntity*>(target);
            bool frozen = false;
            if (client->oIsElementFrozen)
            {
                client->oIsElementFrozen(pPlayer, &frozen);
                if (frozen)
                    continue;
            }
        }

        if (element->crosshair.min_hp > 0.0f)
        {
            float fTargetHP = target->GetGamePlayer()->GetHealth();
            if (fTargetHP < element->crosshair.min_hp)
                continue;
        }

        eBone targetBone;
        switch (element->crosshair.bone)
        {
        case 0: targetBone = BONE_HEAD;         break;
        case 1: targetBone = BONE_NECK;         break;
        case 2: targetBone = BONE_UPPERTORSO;   break;
        case 3: targetBone = BONE_SPINE1;       break;
        case 4: targetBone = BONE_PELVIS;       break;
        case 5: targetBone = BONE_BELLY;        break;
        case 6: targetBone = BONE_LEFTHAND;     break;
        case 7: targetBone = BONE_RIGHTHAND;    break;
        case 8: targetBone = BONE_LEFTFOOT;     break;
        case 9: targetBone = BONE_RIGHTFOOT;    break;
        default: targetBone = BONE_HEAD;        break;
        }

        CVector bonePos, boneScreen;

        CVector vecHeadLocal, vecHeadTarget;
        if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
            !target->GetGamePlayer()->GetBonePosition(targetBone, &vecHeadTarget))
            continue;

        if (element->crosshair.visible_check && !is_visiblee(vecHeadLocal, vecHeadTarget, target))
            continue;

        if (!target->GetGamePlayer()->GetBonePosition(targetBone, &bonePos))
            continue;

        float f3DDistance = sqrt(
            pow(bonePos.fX - vecLocalPos.fX, 2) +
            pow(bonePos.fY - vecLocalPos.fY, 2) +
            pow(bonePos.fZ - vecLocalPos.fZ, 2)
        );

        if (element->crosshair.distance_check)
        {
            if (f3DDistance > element->crosshair.max_distance)
                continue;
        }

        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        global->m_pCore->GetGraphics()->CalcScreenCoors(&bonePos, &boneScreen);

        if (boneScreen.fZ < 1.0f)
            continue;

        float distanceToCrosshair = sqrtf(
            powf(boneScreen.fX - s_crosshair_config.vec_crosshair.fX, 2) +
            powf(boneScreen.fY - s_crosshair_config.vec_crosshair.fY, 2)
        );

        if (distanceToCrosshair > element->crosshair.i_fov)
            continue;

        bool bShouldTarget = false;

        switch (element->crosshair.target_priority)
        {
        case 0: 
        {
            if (distanceToCrosshair < nearestFOVDistance)
            {
                nearestFOVDistance = distanceToCrosshair;
                bShouldTarget = true;
            }
            break;
        }

        case 1: 
        {
            if (f3DDistance < closest3DDistance)
            {
                closest3DDistance = f3DDistance;
                bShouldTarget = true;
            }
            break;
        }

        case 2: 
        {
            float fCurrentHP = target->GetGamePlayer()->GetHealth();

            if (fCurrentHP < lowestHP)
            {
                lowestHP = fCurrentHP;
                bShouldTarget = true;
            }
            break;
        }
        }

        if (bShouldTarget)
        {
            s_crosshair_config.i_target_player = target;
            s_crosshair_config.i_target_bone = targetBone;
            s_crosshair_config.vec_target_bone = boneScreen;
        }
    }
}

void c_aimbot::update_crosshair_position()
{
    if (!element->crosshair.state)
        return;

    if (!s_crosshair_config.is_player_aiming)
        return;

    if (!s_crosshair_config.i_target_player)
        return;

    if (!s_crosshair_config.i_target_player->GetGamePlayer())
        return;

    if (s_crosshair_config.i_target_player->IsBeingDeleted())
        return;

    CVector bonePos, boneScreen;
    if (!s_crosshair_config.i_target_player->GetGamePlayer()->GetBonePosition((eBone)s_crosshair_config.i_target_bone, &bonePos))
        return;

    if (!global->m_pCore || !global->m_pCore->GetGraphics())
        return;

    global->m_pCore->GetGraphics()->CalcScreenCoors(&bonePos, &boneScreen);

    if (boneScreen.fZ < 1.0f)
        return;

    float screenWidth = (float)GetSystemMetrics(SM_CXSCREEN);
    float screenHeight = (float)GetSystemMetrics(SM_CYSCREEN);

    if (boneScreen.fX < 0.0f || boneScreen.fX > screenWidth ||
        boneScreen.fY < 0.0f || boneScreen.fY > screenHeight)
        return;

    float normalizedX = boneScreen.fX / screenWidth;
    float normalizedY = boneScreen.fY / screenHeight;

    if (normalizedX < 0.0f || normalizedX > 1.0f ||
        normalizedY < 0.0f || normalizedY > 1.0f)
        return;

    float* crosshairOffsetX = (float*)0xB6EC14;
    float* crosshairOffsetY = (float*)0xB6EC10;

    if (element->crosshair.smooth_movement)
    {
        float currentX = crosshairOffsetX[0];
        float currentY = crosshairOffsetY[0];

        float deltaX = normalizedX - currentX;
        float deltaY = normalizedY - currentY;

        float smoothFactor = element->crosshair.smooth_speed * 0.01f; 

        crosshairOffsetX[0] = currentX + (deltaX * smoothFactor);
        crosshairOffsetY[0] = currentY + (deltaY * smoothFactor);
    }
    else
    {
        crosshairOffsetX[0] = normalizedX;
        crosshairOffsetY[0] = normalizedY;
    }

    s_aimbot_config.vec_crosshair.fX = boneScreen.fX;
    s_aimbot_config.vec_crosshair.fY = boneScreen.fY;

}

void c_aimbot::draw_target_player()
{
    s_aimbot_config.i_target_player = nullptr;
    float fNearestDistance = 9999.f;
    float fNearestFOVDistance = 9999.f;
    float fClosest3DDistance = 9999.f;
    float fLowestHP = 9999.f;

    if (!element->aimbot.state || !s_aimbot_config.is_player_aiming)
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CVector vec_camera, vec_origin, vec_target;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_RIGHTWRIST, &vec_origin))
        return;

    if (!global->m_pGame || !global->m_pGame->GetCamera() || !global->m_pGame->GetWorld())
        return;

    global->m_pGame->GetCamera()->Find3rdPersonCamTargetVector(100.f, &vec_origin, &vec_camera, &vec_target);

    CVector vecLocalPos;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_PELVIS, &vecLocalPos))
        return;

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer() || target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;


        if (element->aimbot.team_check)
        {
            if (is_friend(target->GetNametagText()))
                continue;
        }


        if (element->aimbot.deadcheck)
        {
            CClientEntity* pPlayer = static_cast<CClientEntity*>(target);
            bool frozen = false;
            if (client->oIsElementFrozen)
            {
                client->oIsElementFrozen(pPlayer, &frozen);
                if (frozen)
                    continue;
            }
        }

        if (element->aimbot.min_hp > 0.0f)
        {
            float fTargetHP = target->GetGamePlayer()->GetHealth();
            if (fTargetHP < element->aimbot.min_hp)
                continue;
        }

        eBone targetBone = (element->aimbot.bone == 1) ? BONE_PELVIS : BONE_HEAD;

        CVector vecHeadLocal, vecHeadTarget;
        if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
            !target->GetGamePlayer()->GetBonePosition(targetBone, &vecHeadTarget))
            continue;

        if (element->aimbot.visible_check && !is_visiblee(vecHeadLocal, vecHeadTarget, target))
            continue;

        CVector vec_bone, vec_bone_screen;
        if (!target->GetGamePlayer()->GetBonePosition(targetBone, &vec_bone))
            continue;

        float f3DDistance = sqrt(
            pow(vec_bone.fX - vecLocalPos.fX, 2) +
            pow(vec_bone.fY - vecLocalPos.fY, 2) +
            pow(vec_bone.fZ - vecLocalPos.fZ, 2)
        );

        if (element->aimbot.distance_check)
        {
            if (f3DDistance > element->aimbot.max_distance)
                continue;
        }

        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        global->m_pCore->GetGraphics()->CalcScreenCoors(&vec_bone, &vec_bone_screen);

        if (vec_bone_screen.fZ < 1.0f)
            continue;

        float fCentreDistance = sqrt(
            pow(vec_bone_screen.fX - s_aimbot_config.vec_crosshair.fX, 2) +
            pow(vec_bone_screen.fY - s_aimbot_config.vec_crosshair.fY, 2)
        );

        if (fCentreDistance >= element->aimbot.i_fov)
            continue;

        bool bShouldTarget = false;

        switch (element->aimbot.target_priority)
        {
        case 0: 
        {
            if (fCentreDistance < fNearestFOVDistance)
            {
                fNearestFOVDistance = fCentreDistance;
                bShouldTarget = true;
            }
            break;
        }

        case 1: 
        {
            if (f3DDistance < fClosest3DDistance)
            {
                fClosest3DDistance = f3DDistance;
                bShouldTarget = true;
            }
            break;
        }

        case 2:
        {
            float fCurrentHP = target->GetGamePlayer()->GetHealth();

            if (fCurrentHP < fLowestHP)
            {
                fLowestHP = fCurrentHP;
                bShouldTarget = true;
            }
            break;
        }
        }

        if (bShouldTarget)
        {
            s_aimbot_config.i_target_player = target;
            s_aimbot_config.i_target_bone = targetBone;
            s_aimbot_config.vec_target_bone = vec_bone_screen;
        }
    }
}

void c_aimbot::draw_aimbot()
{
    if (!element->aimbot.state)
        return;

    if (!s_aimbot_config.is_player_aiming)
        return;

    if (!s_aimbot_config.i_target_player)
        return;

    if (!s_aimbot_config.i_target_player->GetGamePlayer())
        return;

    if (s_aimbot_config.i_target_player->IsBeingDeleted())
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CCamera* m_pCamera = global->m_pGame->GetCamera();
    if (!m_pCamera)
        return;

    CCam* m_pCam = m_pCamera->GetCam(m_pCamera->GetActiveCam());
    if (!m_pCam)
        return;

    CVector* source = m_pCam->GetSource();
    if (!source)
        return;

    CVector vec_origin = *source;

    CVector vec_target;
    if (!s_aimbot_config.i_target_player->GetGamePlayer()->GetBonePosition((eBone)s_aimbot_config.i_target_bone, &vec_target))
        return;

    if (element->aimbot.predict_movement)
    {
        CVector vecVelocity;
        s_aimbot_config.i_target_player->GetGamePlayer()->GetMoveSpeed(&vecVelocity);

        float fDistance = sqrt(
            pow(vec_target.fX - vec_origin.fX, 2) +
            pow(vec_target.fY - vec_origin.fY, 2) +
            pow(vec_target.fZ - vec_origin.fZ, 2)
        );

        float fPredictionTime = (fDistance / 300.0f) * element->aimbot.prediction_scale;

        vec_target.fX += vecVelocity.fX * fPredictionTime;
        vec_target.fY += vecVelocity.fY * fPredictionTime;
        vec_target.fZ += vecVelocity.fZ * fPredictionTime;
    }

    vec_target.fX += element->aimbot.bone_offset_x;
    vec_target.fY += element->aimbot.bone_offset_y;

    CVector vec_vector = vec_origin - vec_target;

    float fVecX = 0.f;
    float fFix = 0.f;
    float fZ = 0.f;

    eCamMode cmera_viewmode = static_cast<eCamMode>(m_pCamera->GetCam(m_pCamera->GetActiveCam())->GetMode());
    if (cmera_viewmode == 53 || cmera_viewmode == 55)
    {
        float fMult = tan(m_pCam->GetFOV() / 2.0f * 0.017453292f);
        float& fAspectRatio = *(float*)0xC3EFA4;
        float& f3rdPersonCHairMultY = *reinterpret_cast<float*>(0xB6EC10);

        fZ = M_PI - atan2(1.0f, fMult * ((1.0f - f3rdPersonCHairMultY * 2.0f) * (1.0f / fAspectRatio)));
    }
    else
    {
        fZ = M_PI / 2;
    }

    eWeaponType byteWeapon = manager->get_weapon_type(local);

    bool bIsAutoWeapon = (byteWeapon >= 22 && byteWeapon <= 29 || byteWeapon == 32);

    if (byteWeapon >= 22 && byteWeapon <= 29 || byteWeapon == 32)
    {
        fFix = 0.04253f;

        if (element->aimbot.recoil_control && bIsAutoWeapon)
        {
            static int iShotCounter = 0;
            static auto lastShotTime = std::chrono::steady_clock::now();
            auto currentTime = std::chrono::steady_clock::now();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastShotTime).count() > 500)
            {
                iShotCounter = 0;
            }

            fFix += (iShotCounter * 0.001f) * element->aimbot.recoil_strength;
            iShotCounter++;

            if (iShotCounter > 30)
                iShotCounter = 30; 

            lastShotTime = currentTime;
        }
    }
    else if (byteWeapon == 30 || byteWeapon == 31)
    {
        fFix = 0.028f;
    }
    else if (byteWeapon == 33)
    {
        fFix = 0.01897f;
    }

    float fDistX = sqrt(vec_vector.fX * vec_vector.fX + vec_vector.fY * vec_vector.fY);
    if (fDistX < 0.0001f)
        return;

    float fDistZ = sqrt(vec_vector.fX * vec_vector.fX + vec_vector.fY * vec_vector.fY);
    if (fDistX < 0.0001f)
        return;

    float horizontal, vertical;
    m_pCam->GetDirection(horizontal, vertical);

    if (fDistX > 0.0f)
    {
        if (vec_vector.fX <= 0.0f && vec_vector.fY >= 0.0f || vec_vector.fX >= 0.0f && vec_vector.fY >= 0.0f)
        {
            fVecX = (acosf(vec_vector.fX / fDistX) + fFix) - horizontal;
        }
        else if (vec_vector.fX >= 0.0f && vec_vector.fY <= 0.0f || vec_vector.fX <= 0.0f && vec_vector.fY <= 0.0f)
        {
            fVecX = (-acosf(vec_vector.fX / fDistX) + fFix) - horizontal;
        }

        float fSmoothValue = element->aimbot.smooth;


        if (element->aimbot.aim_lock)
        {
            fSmoothValue = fSmoothValue * 0.5f; 
        }

        if (fSmoothValue > 0.0f)
        {
            float fSmoothX = fVecX / fSmoothValue;
            float fSmoothZ = (atan2f(fDistZ, vec_vector.fZ) - fZ - vertical) / fSmoothValue;

            if (fSmoothX > -1.0f && fSmoothX < 0.5f && fVecX > -2.0f && fVecX < 2.0f)
            {
                m_pCam->SetDirection(horizontal + fSmoothX, vertical + fSmoothZ);
            }
        }
    }
}

 

void c_aimbot::draw_trigger_bot()
{
    if (!element->aimbot.trigger_bot)         return;

    if (!s_aimbot_config.i_target_player)         return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer() || local->IsDeadOnNetwork())     {
                static bool isShooting = false;
        if (isShooting)
        {
            isShooting = false;
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        return;
    }

        eWeaponType currentWeapon = manager->get_weapon_type(local);
    static eWeaponType lastWeapon = WEAPONTYPE_UNARMED;

    if (currentWeapon == WEAPONTYPE_UNARMED)     {
        static bool isShooting = false;
        if (isShooting)
        {
            isShooting = false;
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        return;
    }

    if (currentWeapon != lastWeapon)     {
        lastWeapon = currentWeapon;         INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;         SendInput(1, &input, sizeof(INPUT));
        return;
    }

    CVector vec_target;
    if (!s_aimbot_config.i_target_player->GetGamePlayer()->GetBonePosition((eBone)s_aimbot_config.i_target_bone, &vec_target))
        return;

        static bool isShooting = false;

    if (element->aimbot.trigger_bot)     {
        if (!isShooting)         {
            isShooting = true;
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }
    }
    else
    {
        if (isShooting)         {
            isShooting = false;
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
    }
}



void c_aimbot::draw_silent()
{
    if (!element->silent.state)
        return;

    if (!element->silent.b_fov)
        return;

    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera)
        return;

    eCamMode cmera_viewmode = static_cast<eCamMode>(camera->GetCam(camera->GetActiveCam())->GetMode());
    if (cmera_viewmode == 53 || cmera_viewmode == 7)
    {
        s_silent_config.is_player_aiming = true;
    }
    else
    {
        s_silent_config.is_player_aiming = false;
    }

    eWeaponType weapon = manager->get_weapon_type(manager->get_local_player());
    if (weapon != WEAPONTYPE_SNIPERRIFLE)
    {
        s_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        s_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y * 0.4f;
    }
    else
    {
        s_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
        s_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;
    }

    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(s_silent_config.vec_crosshair.fX, s_silent_config.vec_crosshair.fY), element->silent.i_fov, IM_COL32(255, 255, 100, 255));
}

void c_aimbot::select_target_player()
{
    s_target_config.i_target_player = nullptr;
    float fNearestDistance = 9999.f;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer())
            continue;

        if (target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;

        if (is_friend(target->GetNametagText()))
            continue;



        CVector& targetPos = *target->GetGamePlayer()->GetPosition();
        CVector vec_player;
        vec_player.fX = targetPos.fX;
        vec_player.fY = targetPos.fY;
        vec_player.fZ = targetPos.fZ;

        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        CVector vec_screen;
        global->m_pCore->GetGraphics()->CalcScreenCoors(&vec_player, &vec_screen);

        if (vec_screen.fZ < 1.0f) 
            continue;

        float fScreenCenterX = ImGui::GetIO().DisplaySize.x / 2.f;
        float fScreenCenterY = ImGui::GetIO().DisplaySize.y / 2.f;

        float fDistance = sqrt(pow(vec_screen.fX - fScreenCenterX, 2) +
            pow(vec_screen.fY - fScreenCenterY, 2));

        if (fDistance < fNearestDistance)
        {
            fNearestDistance = fDistance;
            s_target_config.i_target_player = target;
            s_target_config.vec_target_pos = vec_player;
        }
    }
}

void c_aimbot::draw_silent_player()
{
    s_silent_config.i_target_player = nullptr;
    float fNearestDistance = 9999.f;

    if (!element->silent.state || !s_silent_config.is_player_aiming)
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CVector vec_camera, vec_origin, vec_target;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_RIGHTWRIST, &vec_origin))
        return;

    if (!global->m_pGame || !global->m_pGame->GetCamera() || !global->m_pGame->GetWorld())
        return;

    global->m_pGame->GetCamera()->Find3rdPersonCamTargetVector(100.f, &vec_origin, &vec_camera, &vec_target);

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer() || target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;

        if (is_friend(target->GetNametagText()))
            continue;

        eBone targetBone = (element->silent.bone == 0) ? BONE_HEAD : BONE_HEAD;

        CVector vec_bone, vec_bone_screen;
        if (!target->GetGamePlayer()->GetBonePosition(targetBone, &vec_bone))
            continue;

        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        global->m_pCore->GetGraphics()->CalcScreenCoors(&vec_bone, &vec_bone_screen);

        if (vec_bone_screen.fZ < 1.0f)
            continue;



        float fCentreDistance = sqrt(pow(vec_bone_screen.fX - s_silent_config.vec_crosshair.fX, 2) + pow(vec_bone_screen.fY - s_silent_config.vec_crosshair.fY, 2));
        if (fCentreDistance >= element->silent.i_fov)
            continue;

        if (fCentreDistance < fNearestDistance)
        {
            fNearestDistance = fCentreDistance;
            s_silent_config.i_target_player = target;
            s_silent_config.i_target_bone = targetBone;
            s_silent_config.vec_target_bone = vec_bone_screen;
        }
    }
}

void c_aimbot::draw_manual_silent()
{
    if (!element->manual.state)
        return;

    if (!element->manual.b_fov)
        return;

    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera)
        return;

    eCamMode cmera_viewmode = static_cast<eCamMode>(camera->GetCam(camera->GetActiveCam())->GetMode());
    if (cmera_viewmode == 53 || cmera_viewmode == 7)
    {
        s_manual_silent_config.is_player_aiming = true;
    }
    else
    {
        s_manual_silent_config.is_player_aiming = false;
    }

    eWeaponType weapon = manager->get_weapon_type(manager->get_local_player());
    if (weapon != WEAPONTYPE_SNIPERRIFLE)
    {
        s_manual_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        s_manual_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y * 0.4f;
    }
    else
    {
        s_manual_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
        s_manual_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;
    }

    ImGui::GetBackgroundDrawList()->AddCircle(
        ImVec2(s_manual_silent_config.vec_crosshair.fX, s_manual_silent_config.vec_crosshair.fY),
        element->manual.i_fov,
        IM_COL32(255, 255, 255, 255)
    );
}


void c_aimbot::draw_manual_silent_player()
{
    s_manual_silent_config.i_target_player = nullptr;
    float fNearestDistance = 9999.f;

    if (!element->manual.state || !s_manual_silent_config.is_player_aiming)
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CVector vec_camera, vec_origin, vec_target;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vec_origin))
        return;

    if (!global->m_pGame || !global->m_pGame->GetCamera() || !global->m_pGame->GetWorld())
        return;

    global->m_pGame->GetCamera()->Find3rdPersonCamTargetVector(100.f, &vec_origin, &vec_camera, &vec_target);

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer() || target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;

        if (is_friend(target->GetNametagText()))
            continue;

        if (element->silent.deadcheck)
        {
            CClientEntity* pPlayer = static_cast<CClientEntity*>(target);
            bool frozen = false;
            if (client->oIsElementFrozen)
            {
                client->oIsElementFrozen(pPlayer, &frozen);
                if (frozen)
                    continue;
            }
        }

        eBone targetBone = (element->manual.bone == 0) ? BONE_HEAD : BONE_HEAD;

        CVector vec_bone, vec_bone_screen;
        if (!target->GetGamePlayer()->GetBonePosition(targetBone, &vec_bone))
            continue;

        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        global->m_pCore->GetGraphics()->CalcScreenCoors(&vec_bone, &vec_bone_screen);

        if (vec_bone_screen.fZ < 1.0f)
            continue;

                CVector vecHeadLocal, vecHeadTarget;
        if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
            !target->GetGamePlayer()->GetBonePosition(targetBone, &vecHeadTarget))
            continue;

        if (element->manual.visible_check && !is_visiblee(vecHeadLocal, vecHeadTarget, target))
            continue;
        
        float fCentreDistance = sqrt(pow(vec_bone_screen.fX - s_manual_silent_config.vec_crosshair.fX, 2) +
            pow(vec_bone_screen.fY - s_manual_silent_config.vec_crosshair.fY, 2));
        if (fCentreDistance >= element->manual.i_fov)
            continue;

        if (fCentreDistance < fNearestDistance)
        {
            fNearestDistance = fCentreDistance;
            s_manual_silent_config.i_target_player = target;
            s_manual_silent_config.i_target_bone = targetBone;
            s_manual_silent_config.vec_target_bone = vec_bone_screen;

        }
    }
}


void c_aimbot::draw_spinbot_silent_player()
{
    s_spinbot_silent_config.i_target_player = nullptr;
    s_spinbot_silent_config.is_active = false;
    float fNearestDistance = 9999.f;

    if (!pSpinbot || !pSpinbot->bEnabled)
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CVector vecLocalPos;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_PELVIS, &vecLocalPos))
        return;

    // FOV merkez noktasý
    s_spinbot_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
    s_spinbot_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer() || target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;

        if (element->spinbot.team_check && is_friend(target->GetNametagText()))
            continue;

        if (element->spinbot.deadcheck)
        {
            CClientEntity* pPlayer = static_cast<CClientEntity*>(target);
            bool frozen = false;
            if (client->oIsElementFrozen)
            {
                client->oIsElementFrozen(pPlayer, &frozen);
                if (frozen) continue;
            }
        }

        eBone targetBone = (element->spinbot.bone == 0) ? BONE_HEAD : BONE_PELVIS;

        CVector vecHeadLocal, vecHeadTarget;
        if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
            !target->GetGamePlayer()->GetBonePosition(targetBone, &vecHeadTarget))
            continue;

        if (element->spinbot.visible_check && !is_visiblee(vecHeadLocal, vecHeadTarget, target))
            continue;

        CVector vec_bone;
        if (!target->GetGamePlayer()->GetBonePosition(targetBone, &vec_bone))
            continue;

        float f3DDistance = sqrt(
            pow(vec_bone.fX - vecLocalPos.fX, 2) +
            pow(vec_bone.fY - vecLocalPos.fY, 2) +
            pow(vec_bone.fZ - vecLocalPos.fZ, 2)
        );

        if (element->spinbot.distance_check && f3DDistance > element->spinbot.max_distance)
            continue;

        // Ekran pozisyonu hesapla
        CVector vec_bone_screen;
        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        global->m_pCore->GetGraphics()->CalcScreenCoors(&vec_bone, &vec_bone_screen);

        if (vec_bone_screen.fZ < 1.0f)
            continue;

        // FOV kontrolü
        if (element->spinbot.b_fov)
        {
            float fCentreDistance = sqrt(
                pow(vec_bone_screen.fX - s_spinbot_silent_config.vec_crosshair.fX, 2) +
                pow(vec_bone_screen.fY - s_spinbot_silent_config.vec_crosshair.fY, 2)
            );

            if (fCentreDistance >= element->spinbot.i_fov)
                continue;
        }

        if (f3DDistance < fNearestDistance)
        {
            fNearestDistance = f3DDistance;
            s_spinbot_silent_config.i_target_player = target;
            s_spinbot_silent_config.i_target_bone = targetBone;
            s_spinbot_silent_config.vec_target_bone = vec_bone;
            s_spinbot_silent_config.is_active = true;
        }
    }
}

void c_aimbot::draw_spinbot_fov()
{
    if (!pSpinbot || !pSpinbot->bEnabled)
        return;

    if (!element->spinbot.b_fov)
        return;


    eWeaponType weapon = manager->get_weapon_type(manager->get_local_player());
    if (weapon != WEAPONTYPE_SNIPERRIFLE)
    {
        s_spinbot_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        s_spinbot_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y * 0.4f;
    }
    else
    {
        s_spinbot_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
        s_spinbot_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;
    }

    ImU32 color;


    if (element->spinbot.rgb_fov) {
        // RGB animasyon
        static auto start = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - start;

        float time = elapsed.count() * element->spinbot.rgb_speed;

        // HSV'den RGB'ye dönüþüm (renk geçiþi için)
        float hue = fmodf(time * 60.0f, 360.0f); // 0-360 derece
        float saturation = 1.0f;
        float value = 1.0f;

        // HSV to RGB
        float c = value * saturation;
        float x = c * (1.0f - fabs(fmodf(hue / 60.0f, 2.0f) - 1.0f));
        float m = value - c;

        float r, g, b;
        if (hue < 60) {
            r = c; g = x; b = 0;
        }
        else if (hue < 120) {
            r = x; g = c; b = 0;
        }
        else if (hue < 180) {
            r = 0; g = c; b = x;
        }
        else if (hue < 240) {
            r = 0; g = x; b = c;
        }
        else if (hue < 300) {
            r = x; g = 0; b = c;
        }
        else {
            r = c; g = 0; b = x;
        }

        color = IM_COL32(
            (int)((r + m) * 255),
            (int)((g + m) * 255),
            (int)((b + m) * 255),
            (int)(element->spinbot.fov_color[3] * 255) // Alpha deðeri sabit
        );
    }
    else {
        // Normal renk
        color = IM_COL32(
            (int)(element->spinbot.fov_color[0] * 255),
            (int)(element->spinbot.fov_color[1] * 255),
            (int)(element->spinbot.fov_color[2] * 255),
            (int)(element->spinbot.fov_color[3] * 255)
        );
    }

    ImGui::GetBackgroundDrawList()->AddCircle(
        ImVec2(s_spinbot_silent_config.vec_crosshair.fX, s_spinbot_silent_config.vec_crosshair.fY),
        element->spinbot.i_fov,
        color,
        64
    );

}

void c_aimbot::draw_bullettrack_fov()
{
    if (!pSpinbot || !pSpinbot->bBulletTrackEnabled)
        return;

    if (!element->bullettrack.b_fov)
        return;


    eWeaponType weapon = manager->get_weapon_type(manager->get_local_player());
    if (weapon != WEAPONTYPE_SNIPERRIFLE)
    {
        s_bullettrack_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        s_bullettrack_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y * 0.4f;
    }
    else
    {
        s_bullettrack_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
        s_bullettrack_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;
    }

    ImU32 color;

    if (element->bullettrack.rgb_fov) {
        static auto start = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - start;

        float time = elapsed.count() * element->bullettrack.rgb_speed;
        float hue = fmodf(time * 60.0f, 360.0f);
        float saturation = 1.0f;
        float value = 1.0f;

        float c = value * saturation;
        float x = c * (1.0f - fabs(fmodf(hue / 60.0f, 2.0f) - 1.0f));
        float m = value - c;

        float r, g, b;
        if (hue < 60) {
            r = c; g = x; b = 0;
        }
        else if (hue < 120) {
            r = x; g = c; b = 0;
        }
        else if (hue < 180) {
            r = 0; g = c; b = x;
        }
        else if (hue < 240) {
            r = 0; g = x; b = c;
        }
        else if (hue < 300) {
            r = x; g = 0; b = c;
        }
        else {
            r = c; g = 0; b = x;
        }

        color = IM_COL32(
            (int)((r + m) * 255),
            (int)((g + m) * 255),
            (int)((b + m) * 255),
            (int)(element->bullettrack.fov_color[3] * 255)
        );
    }
    else {
        color = IM_COL32(
            (int)(element->bullettrack.fov_color[0] * 255),
            (int)(element->bullettrack.fov_color[1] * 255),
            (int)(element->bullettrack.fov_color[2] * 255),
            (int)(element->bullettrack.fov_color[3] * 255)
        );
    }

    ImGui::GetBackgroundDrawList()->AddCircle(
        ImVec2(s_bullettrack_silent_config.vec_crosshair.fX, s_bullettrack_silent_config.vec_crosshair.fY),
        element->bullettrack.i_fov,
        color,
        64
    );
}

void c_aimbot::draw_bullettrack_silent_player()
{
    s_bullettrack_silent_config.i_target_player = nullptr;
    s_bullettrack_silent_config.is_active = false;
    float fNearestDistance = 9999.f;

    if (!pSpinbot || !pSpinbot->bBulletTrackEnabled)
        return;

    CClientPlayer* local = manager->get_local_player();
    if (!local || !local->GetGamePlayer())
        return;

    CVector vecLocalPos;
    if (!local->GetGamePlayer()->GetBonePosition(BONE_PELVIS, &vecLocalPos))
        return;

    s_bullettrack_silent_config.vec_crosshair.fX = ImGui::GetIO().DisplaySize.x / 2.f;
    s_bullettrack_silent_config.vec_crosshair.fY = ImGui::GetIO().DisplaySize.y / 2.f;

    std::vector<CClientPlayer*> players = manager->get_players();
    for (CClientPlayer* target : players)
    {
        if (!target || !target->GetGamePlayer() || target->IsLocalPlayer() || target->IsDeadOnNetwork())
            continue;

        if (element->bullettrack.team_check && is_friend(target->GetNametagText()))
            continue;

        if (element->bullettrack.deadcheck)
        {
            CClientEntity* pPlayer = static_cast<CClientEntity*>(target);
            bool frozen = false;
            if (client->oIsElementFrozen)
            {
                client->oIsElementFrozen(pPlayer, &frozen);
                if (frozen) continue;
            }
        }

        eBone targetBone = (element->bullettrack.bone == 0) ? BONE_HEAD : BONE_PELVIS;

        CVector vecHeadLocal, vecHeadTarget;
        if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
            !target->GetGamePlayer()->GetBonePosition(targetBone, &vecHeadTarget))
            continue;

        if (element->bullettrack.visible_check && !is_visiblee(vecHeadLocal, vecHeadTarget, target))
            continue;

        CVector vec_bone;
        if (!target->GetGamePlayer()->GetBonePosition(targetBone, &vec_bone))
            continue;

        float f3DDistance = sqrt(
            pow(vec_bone.fX - vecLocalPos.fX, 2) +
            pow(vec_bone.fY - vecLocalPos.fY, 2) +
            pow(vec_bone.fZ - vecLocalPos.fZ, 2)
        );

        if (element->bullettrack.distance_check && f3DDistance > element->bullettrack.max_distance)
            continue;

        CVector vec_bone_screen;
        if (!global->m_pCore || !global->m_pCore->GetGraphics())
            continue;

        global->m_pCore->GetGraphics()->CalcScreenCoors(&vec_bone, &vec_bone_screen);

        if (vec_bone_screen.fZ < 1.0f)
            continue;

        // FOV kontrolü
        if (element->bullettrack.b_fov)
        {
            float fCentreDistance = sqrt(
                pow(vec_bone_screen.fX - s_bullettrack_silent_config.vec_crosshair.fX, 2) +
                pow(vec_bone_screen.fY - s_bullettrack_silent_config.vec_crosshair.fY, 2)
            );

            if (fCentreDistance >= element->bullettrack.i_fov)
                continue;
        }

        if (f3DDistance < fNearestDistance)
        {
            fNearestDistance = f3DDistance;
            s_bullettrack_silent_config.i_target_player = target;
            s_bullettrack_silent_config.i_target_bone = targetBone;
            s_bullettrack_silent_config.vec_target_bone = vec_bone;
            s_bullettrack_silent_config.is_active = true;
        }
    }
}

void c_aimbot::draw_player_aimbot()
{
    if (!manager->is_loaded())
        return;

    draw_aimbot_fov();
    draw_aimbot();
    draw_manual_silent();
    select_target_player();
    draw_manual_silent_player();
    draw_target_player();
    draw_crosshair_fov();
    draw_crosshair_target();
    update_crosshair_position();

    // Spinbot
    draw_spinbot_fov();
    draw_spinbot_silent_player();


    // Bullet Track
    draw_bullettrack_fov();
    draw_bullettrack_silent_player();
}


bool c_aimbot::is_friend(const std::string& player_name) {
    return std::find(friends_list.begin(), friends_list.end(), player_name) != friends_list.end();
}

void c_aimbot::add_friend(const std::string& player_name) {
    if (!is_friend(player_name)) {
        friends_list.push_back(player_name);
    }
}

void c_aimbot::remove_friend(const std::string& player_name) {
    friends_list.erase(
        std::remove(friends_list.begin(), friends_list.end(), player_name),
        friends_list.end()
    );
}

