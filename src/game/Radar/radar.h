#pragma once
#include <Windows.h>
#include <string.h>  // memcpy için
#include <cstring>  // memcpy için

// Radar blip yapýlarý
enum eBlipType {
    BLIP_NONE = 0,
    BLIP_CAR = 1,
    BLIP_CHAR = 2,
    BLIP_OBJECT = 3,
    BLIP_COORD = 4,
    BLIP_CONTACT_POINT = 5
};

enum eBlipDisplay {
    BLIP_DISPLAY_NEITHER = 0,
    BLIP_DISPLAY_MARKER_ONLY = 1,
    BLIP_DISPLAY_BLIP_ONLY = 2,
    BLIP_DISPLAY_BOTH = 3
};

struct tRadarTrace {
    unsigned int    m_nColor;
    unsigned int    m_nEntityHandle;
    float           m_vPosition[3];
    unsigned short  m_nCounter;
    float           m_fSphereRadius;
    unsigned short  m_nBlipSize;
    char            m_nBlipSprite;
    unsigned char   m_bBright : 1;
    unsigned char   m_bTrackingBlip : 1;
    unsigned char   m_bShortRange : 1;
    unsigned char   m_bFriendly : 1;
    unsigned char   m_bBlipRemain : 1;
    unsigned char   m_bBlipFade : 1;
    unsigned char   m_nCoordBlipAppearance : 2;
    unsigned char   m_nBlipDisplayFlag : 2;
    unsigned char   m_nBlipType : 4;
};

class Radar {
public:
    // GTA:SA CRadar offsetleri
    static constexpr unsigned int ARRAY_RadarTrace = 0xBA86F0;  // tRadarTrace array[175]

    // Radar fonksiyon offsetleri
    static constexpr unsigned int FUNC_DrawRadarMap = 0x58A330;
    static constexpr unsigned int FUNC_DrawBlips = 0x58A990;
    static constexpr unsigned int FUNC_DrawRadarSprite = 0x585700;
    static constexpr unsigned int FUNC_SetCoordBlip = 0x583820;
    static constexpr unsigned int FUNC_ClearBlip = 0x587C10;
    static constexpr unsigned int FUNC_DrawRadarMask = 0x58A7C0;

    // Radar deðiþken offsetleri
    static constexpr unsigned int VAR_RadarRange = 0xA444A4;  // float
    static constexpr unsigned int VAR_MapLegendX = 0xBAA248;  // float
    static constexpr unsigned int VAR_MapLegendY = 0xBAA24C;  // float

    // Fonksiyon pointer tipleri
    typedef void(__cdecl* DrawRadarMap_t)();
    typedef void(__cdecl* DrawBlips_t)();
    typedef void(__cdecl* DrawRadarSprite_t)(unsigned short sprite, float x, float y, unsigned char alpha);
    typedef int(__cdecl* SetCoordBlip_t)(eBlipType type, float x, float y, float z, int color, eBlipDisplay display);
    typedef void(__cdecl* ClearBlip_t)(int blipIndex);

    static constexpr unsigned int FUNC_DrawLegend = 0x58A1B0; // Büyük harita fonksiyonu
    typedef void(__cdecl* DrawLegend_t)(float, float, int);
    static constexpr unsigned int VAR_RadarShowMode = 0xBA676C; // radar display mode

    static void DrawRadarSection(int x, int y);// Radar section çizer
    static void DrawRadarSectionMap(int x, int y); // Map section

    static constexpr unsigned int FUNC_DrawRadarSection = 0x586650;
    static constexpr unsigned int FUNC_DrawRadarSectionMap = 0x5869B0;

    // Ana fonksiyonlar
    static void DrawFullScreenMap();
    static void DrawBigMap();
    static void DrawRadarMap();


    static void DrawRadarBlips();

    // Blip yönetimi
    static int CreateBlip(float x, float y, float z, int sprite, int color);
    static void RemoveBlip(int blipIndex);
    static tRadarTrace* GetRadarTrace(int index);

    // Radar ayarlarý
    static void SetRadarRange(float range);
    static float GetRadarRange();

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

private:

};