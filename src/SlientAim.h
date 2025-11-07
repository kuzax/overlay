#pragma once
#include <windows.h>

// Forward declarations
class CCallHook;
class CPed;
class Spinbot {
public:
    bool bEnabled;
    bool bBulletTrackEnabled; // Yeni eklendi
    bool bHookActive;
    CPed* pPedSelf;

    CCallHook* hookRot1;
    CCallHook* hookTarget1;
    CCallHook* hookTarget2;
    CCallHook* hookTarget3;

    float targetAngle;
    float originalAngle;
    float messi;
    bool wsState;

    Spinbot();
    ~Spinbot();

    void HookRotation();
    void HookTarget(CVector& gun, CVector& vec);
    void HookTarget1();
    void HookTarget2();
    void HookTarget3();
};

inline Spinbot* pSpinbot = nullptr;

