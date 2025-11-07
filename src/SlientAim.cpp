#include <includes.h>
#include <sdk.h>
#include <SlientAim.h>
#include <game/manager/manager.h>
#include <game/CallHook/ccallhook.h>
#include <console/console.h>
#include <globals.h>
#include <data/elements.h>
#include <game/aimbot/aimbot.h>
#include <client/client.h>


float fSpinbotPitch = 0.0f;
float spinYaw = 0.0f;
float spinPitch = 0.0f;
float visualYaw = 0.0f;

float GetAngleSpinbot(float fX, float fY, const float& X, const float& Y) {
    float kx = X - fX;
    float ky = Y - fY;
    if (kx == 0) kx = 0.00001f;
    float t = kx / ky;
    if (t < 0) t = -t;
    float a = (float)(180 * atan((float)t) / M_PI);
    if ((kx <= 0) && (ky <= 0))
        a = 180 - a;
    else if ((kx >= 0) && (ky >= 0))
        a = 359.99999f - a;
    else if ((kx >= 0) && (ky <= 0))
        a = -(180 - a);
    return (a * M_PI) / 180.0f;
}

void HandleCameraBasedMovement(CPed* ped, CClientPlayer* local) {
    bool bW = GetAsyncKeyState('W') & 0x8000;
    bool bA = GetAsyncKeyState('A') & 0x8000;
    bool bS = GetAsyncKeyState('S') & 0x8000;
    bool bD = GetAsyncKeyState('D') & 0x8000;

    if (!bW && !bA && !bS && !bD) return;

    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera) return;

    float cameraAngle = camera->GetCameraRotation();
    cameraAngle = -cameraAngle + static_cast<float>(M_PI);

    float moveX = 0.0f, moveY = 0.0f;
    if (bW) moveY -= 1.0f;
    if (bS) moveY += 1.0f;
    if (bD) moveX -= 1.0f;
    if (bA) moveX += 1.0f;

    float length = sqrt(moveX * moveX + moveY * moveY);
    if (length > 0.0f) {
        moveX /= length;
        moveY /= length;
    }

    float finalX = moveX * cos(cameraAngle) - moveY * sin(cameraAngle);
    float finalY = moveX * sin(cameraAngle) + moveY * cos(cameraAngle);

    float speed = 0.15f;
    CVector currentPos;
    CClientEntity* local_entity = static_cast<CClientEntity*>(local);
    local_entity->GetPosition(currentPos);

    currentPos.fX += finalX * speed;
    currentPos.fY += finalY * speed;

    ped->SetPosition(&currentPos);
}

Spinbot::Spinbot() {
    bEnabled = false;
    bBulletTrackEnabled = false;
    bHookActive = false;
    pPedSelf = nullptr;
    targetAngle = 0.0f;
    originalAngle = 0.0f;
    wsState = false;

    hookRot1 = new CCallHook((void*)0x00522C26, 8, 0, cp_after, sc_all, e_jmp);
    hookRot1->enable(this, &Spinbot::HookRotation);

    hookTarget1 = new CCallHook((void*)0x007406C7, 5, 0, cp_after, sc_all, e_jmp);
    hookTarget2 = new CCallHook((void*)0x00740B24, 8, 0, cp_after, sc_all, e_jmp);
    hookTarget3 = new CCallHook((void*)0x0060B819, 5, 0, cp_after, sc_all, e_jmp);

    hookTarget1->enable(this, &Spinbot::HookTarget1);
    hookTarget2->enable(this, &Spinbot::HookTarget2);
    hookTarget3->enable(this, &Spinbot::HookTarget3);
}

Spinbot::~Spinbot() {
    if (hookRot1) {
        hookRot1->disable();
        delete hookRot1;
        hookRot1 = nullptr;
    }
    if (hookTarget1) {
        hookTarget1->disable();
        delete hookTarget1;
        hookTarget1 = nullptr;
    }
    if (hookTarget2) {
        hookTarget2->disable();
        delete hookTarget2;
        hookTarget2 = nullptr;
    }
    if (hookTarget3) {
        hookTarget3->disable();
        delete hookTarget3;
        hookTarget3 = nullptr;
    }
}

void Spinbot::HookRotation() {
    if (bEnabled) {
        CCamera* camera = global->m_pGame->GetCamera();
        if (!camera) return;

        eCamMode cmera_viewmode = static_cast<eCamMode>(camera->GetCam(camera->GetActiveCam())->GetMode());
        if (cmera_viewmode == 53 || cmera_viewmode == 7) {
            bHookActive = false;
        }
        else {
            bHookActive = true;
        }

        auto pAngle = (float*)(hookRot1->reg86(r86::ESP) + 0x68);
        if (!pAngle) return;

        CClientPlayer* local = manager->get_local_player();
        if (!local) return;

        CPed* ped = local->GetGamePlayer();
        if (!ped) return;

        pPedSelf = ped;

        static auto start = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - start;

        float spinSpeed = element->spinbot.spin_speed;
        visualYaw = fmodf(elapsed.count() * spinSpeed, 360.0f);

        float pitchInRadians = element->spinbot.pitch_angle * (M_PI / 180.0f);

        *pAngle = visualYaw;
        *(pAngle + 1) = pitchInRadians;

        originalAngle = visualYaw * (M_PI / 180.0f);

        CVector position;
        CClientEntity* local_entity = static_cast<CClientEntity*>(local);
        local_entity->GetPosition(position);

        if (aimbot->s_spinbot_silent_config.is_active && aimbot->s_spinbot_silent_config.i_target_player) {
            CClientPlayer* target = aimbot->s_spinbot_silent_config.i_target_player;
            if (target && target->GetGamePlayer()) {
                CVector LocalPos;
                CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                local_entity->GetPosition(LocalPos);

                CVector OutTargetPos;
                CClientEntity* target_entity = static_cast<CClientEntity*>(target);
                target_entity->GetPosition(OutTargetPos);

                targetAngle = GetAngleSpinbot(LocalPos.fX, LocalPos.fY, OutTargetPos.fX, OutTargetPos.fY);
            }
            else {
                targetAngle = originalAngle;
            }
        }
        else {
            targetAngle = originalAngle;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        HandleCameraBasedMovement(ped, local);
    }
    else if (bBulletTrackEnabled) {
        CClientPlayer* local = manager->get_local_player();
        if (!local) return;

        CPed* ped = local->GetGamePlayer();
        if (!ped) return;

        pPedSelf = ped;

        if (element->bullettrack.rotation_sync && aimbot->s_bullettrack_silent_config.is_active
            && aimbot->s_bullettrack_silent_config.i_target_player) {

            auto pAngle = (float*)(hookRot1->reg86(r86::ESP) + 0x68);
            if (!pAngle) return;

            CClientPlayer* target = aimbot->s_bullettrack_silent_config.i_target_player;
            if (target && target->GetGamePlayer()) {
                auto pAngle = (float*)(hookRot1->reg86(r86::ESP) + 0x68);

                originalAngle = *pAngle;
                CVector LocalPos;
                CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                local_entity->GetPosition(LocalPos);

                CVector OutTargetPos;
                CClientEntity* target_entity = static_cast<CClientEntity*>(target);
                target_entity->GetPosition(OutTargetPos);

                *pAngle = targetAngle;                


            }
        }
    }
}

void Spinbot::HookTarget(CVector& gun, CVector& vec) {
    if (!pPedSelf) return;

    // Spinbot aktifse spinbot silent kullan
    if (bEnabled) {
        if (!aimbot->s_spinbot_silent_config.is_active) return;

        CVector LocalBonePos;
        if (!pPedSelf->GetBonePosition(BONE_RIGHTWRIST, &LocalBonePos))
            return;

        float distX = LocalBonePos.fX - gun.fX;
        float distY = LocalBonePos.fY - gun.fY;
        float distZ = LocalBonePos.fZ - gun.fZ;
        float distance = sqrt(distX * distX + distY * distY + distZ * distZ);

        if (distance > 1.0f) return;

        bool rewrite = false;

        if (aimbot->s_spinbot_silent_config.i_target_player != nullptr) {
            CClientPlayer* target = aimbot->s_spinbot_silent_config.i_target_player;

            if (target && target->GetGamePlayer() && !target->IsBeingDeleted()) {
                CVector Bone_Pos;
                eBone targetBone = (eBone)aimbot->s_spinbot_silent_config.i_target_bone;

                if (target->GetGamePlayer()->GetBonePosition(targetBone, &Bone_Pos)) {
                    rewrite = true;
                    vec = Bone_Pos;
                }
            }
        }

        if (!wsState && rewrite) {
            memsafe::write((void*)0x00740701, false);
            memsafe::write((void*)0x00740703, false);
            memsafe::write((void*)0x00740709, false);
            memsafe::write((void*)0x0074B749, false);
            memsafe::write((void*)0x0074B74B, false);
            memsafe::write((void*)0x0074B751, false);
            wsState = true;
        }
        else if (wsState && !rewrite) {
            memsafe::write((void*)0x00740701, true);
            memsafe::write((void*)0x00740703, true);
            memsafe::write((void*)0x00740707, true);
            memsafe::write((void*)0x00740709, true);
            memsafe::write((void*)0x0074B749, true);
            memsafe::write((void*)0x0074B74B, true);
            memsafe::write((void*)0x0074B74F, true);
            memsafe::write((void*)0x0074B751, true);
            wsState = false;
        }
    }
    // Bullet Track aktifse bullettrack silent kullan
    else if (bBulletTrackEnabled) {
        if (!aimbot->s_bullettrack_silent_config.is_active) return;

        CVector LocalBonePos;
        if (!pPedSelf->GetBonePosition(BONE_RIGHTWRIST, &LocalBonePos))
            return;

        float distX = LocalBonePos.fX - gun.fX;
        float distY = LocalBonePos.fY - gun.fY;
        float distZ = LocalBonePos.fZ - gun.fZ;
        float distance = sqrt(distX * distX + distY * distY + distZ * distZ);

        if (distance > 1.0f) return;

        bool rewrite = false;

        if (aimbot->s_bullettrack_silent_config.i_target_player != nullptr) {
            CClientPlayer* target = aimbot->s_bullettrack_silent_config.i_target_player;

            if (target && target->GetGamePlayer() && !target->IsBeingDeleted()) {
                CVector Bone_Pos;
                eBone targetBone = (eBone)aimbot->s_bullettrack_silent_config.i_target_bone;

                if (target->GetGamePlayer()->GetBonePosition(targetBone, &Bone_Pos)) {
                    rewrite = true;
                    vec = Bone_Pos;
                }
            }
        }

        if (!wsState && rewrite) {
            memsafe::write((void*)0x00740701, false);
            memsafe::write((void*)0x00740703, false);
            memsafe::write((void*)0x00740709, false);
            memsafe::write((void*)0x0074B749, false);
            memsafe::write((void*)0x0074B74B, false);
            memsafe::write((void*)0x0074B751, false);
            wsState = true;
        }
        else if (wsState && !rewrite) {
            memsafe::write((void*)0x00740701, true);
            memsafe::write((void*)0x00740703, true);
            memsafe::write((void*)0x00740707, true);
            memsafe::write((void*)0x00740709, true);
            memsafe::write((void*)0x0074B749, true);
            memsafe::write((void*)0x0074B74B, true);
            memsafe::write((void*)0x0074B74F, true);
            memsafe::write((void*)0x0074B751, true);
            wsState = false;
        }
    }
}

void Spinbot::HookTarget1() {
    auto esp = hookTarget1->reg86(r86::ESP);
    auto& vec = *(CVector*)(esp + 0x10);
    auto& gun = *(CVector*)(esp + 0x2C);
    HookTarget(gun, vec);
}

void Spinbot::HookTarget2() {
    auto esp = hookTarget2->reg86(r86::ESP);
    auto& vec = *(CVector*)(esp + 0x10);
    auto& gun = *(CVector*)(esp + 0x2C);
    HookTarget(gun, vec);
}

void Spinbot::HookTarget3() {
    if (!pPedSelf) return;

    CMatrix camMatrix;
    if (!pPedSelf->GetMatrix(&camMatrix)) return;

    auto esp = hookTarget3->reg86(r86::ESP);
    auto& vec = *(CVector*)(esp + 0x38);
    auto& cam = *(CVector*)(esp + 0x44);

    CVector LocalBonePos;
    if (!pPedSelf->GetBonePosition(BONE_SPINE1, &LocalBonePos))
        return;

    float distX = cam.fX - LocalBonePos.fX;
    float distY = cam.fY - LocalBonePos.fY;
    float distZ = cam.fZ - LocalBonePos.fZ;
    float distance = sqrt(distX * distX + distY * distY + distZ * distZ);

    if (distance > 1.0f) return;

    CClientPlayer* local = manager->get_local_player();
    if (!local) return;

    // Spinbot için
    if (bEnabled && aimbot->s_spinbot_silent_config.is_active) {
        if (aimbot->s_spinbot_silent_config.i_target_player != nullptr) {
            CClientPlayer* target = aimbot->s_spinbot_silent_config.i_target_player;
            if (target && target->GetGamePlayer()) {
                CVector LocalPos;
                CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                local_entity->GetPosition(LocalPos);

                CVector OutTargetPos;
                CClientEntity* target_entity = static_cast<CClientEntity*>(target);
                target_entity->GetPosition(OutTargetPos);

                targetAngle = GetAngleSpinbot(LocalPos.fX, LocalPos.fY, OutTargetPos.fX, OutTargetPos.fY);
            }
        }
        else {
            targetAngle = originalAngle;
        }
    }
    else if (bBulletTrackEnabled && aimbot->s_bullettrack_silent_config.is_active) {
        if (aimbot->s_bullettrack_silent_config.i_target_player != nullptr) {
            CClientPlayer* target = aimbot->s_bullettrack_silent_config.i_target_player;
            if (target && target->GetGamePlayer()) {
                CVector LocalPos;
                CClientEntity* local_entity = static_cast<CClientEntity*>(local);
                local_entity->GetPosition(LocalPos);

                CVector OutTargetPos;
                CClientEntity* target_entity = static_cast<CClientEntity*>(target);
                target_entity->GetPosition(OutTargetPos);

                targetAngle = GetAngleSpinbot(LocalPos.fX, LocalPos.fY, OutTargetPos.fX, OutTargetPos.fY);
            }
        }
        else {
            targetAngle = originalAngle;
        }
    }
    else {
        targetAngle = originalAngle;
    }
}