#pragma once
#include <includes.h>
#include <sdk.h>

class c_player {
public:
	void release();

};

inline c_player* player = new c_player();



class Freecam {
private:
    // Camera offsets (GTA SA 1.0)
    static constexpr uintptr_t CAMERA_BASE = 0xB6F028;
    static constexpr uintptr_t OFFSET_ACTIVE_CAM = 0x59;
    static constexpr uintptr_t OFFSET_ACAMS = 0x170;
    static constexpr uintptr_t CCAM_SIZE = 0x238;

    // CCam offsets
    static constexpr uintptr_t CCAM_FRONT = 0x184;
    static constexpr uintptr_t CCAM_SOURCE = 0x190;
    static constexpr uintptr_t CCAM_UP = 0x1A8;

    // Freecam state
    bool m_bEnabled;
    float m_fPosX, m_fPosY, m_fPosZ;
    float m_fRotX, m_fRotY;
    float m_fSpeed;
    float m_fMouseSensitivity;
    float m_fFOV;

    // Dummy character
    int m_nDummyPed;
    void* m_pDummyPed;

    // Original states
    unsigned char m_bHudState;
    unsigned char m_bRadarState;

    // Mouse tracking
    bool m_bMouseCaptured;
    POINT m_LastMousePos;

    template <typename T>
    static bool SafeWriteMemory(uintptr_t address, const T& value) {
        DWORD oldProtect;
        if (VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect)) {
            std::memcpy(reinterpret_cast<void*>(address), &value, sizeof(T));
            VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(T), oldProtect, &oldProtect);
            return true;
        }
        return false;
    }

    template <typename T>
    static T ReadMemory(uintptr_t address) {
        return *reinterpret_cast<T*>(address);
    }

    void UpdateCameraVectors();
    void ProcessInput();
    uintptr_t GetActiveCCamAddress();

public:
    Freecam();
    ~Freecam();

    void Enable();
    void Disable();
    void Update();
    void Toggle();

    bool IsEnabled() const { return m_bEnabled; }
    void SetSpeed(float speed) { m_fSpeed = speed; }
    void SetSensitivity(float sensitivity) { m_fMouseSensitivity = sensitivity; }
    float GetSpeed() const { return m_fSpeed; }
    float GetSensitivity() const { return m_fMouseSensitivity; }
};

extern Freecam g_Freecam;