#include "Radar.h"






void Radar::DrawRadarMap() {
    DrawRadarMap_t drawRadarMap = (DrawRadarMap_t)0x586880;
    drawRadarMap();
}



void Radar::DrawFullScreenMap() {
    // Minimap'i gizle
    SafeWriteMemory<unsigned char>(0xBA676C, 1);

    // Radar pozisyonunu merkeze al
    SafeWriteMemory<float>(0xBAA248, 320.0f); // Center X
    SafeWriteMemory<float>(0xBAA24C, 240.0f); // Center Y

    // Radar'ý büyüt
    SafeWriteMemory<float>(0xBA8314, 5000.0f); // m_radarRange - çok geniþ

    // DrawRadarMap ile çiz
    typedef void(__cdecl* DrawRadarMap_t)();
    DrawRadarMap_t drawRadarMap = (DrawRadarMap_t)0x586880;
    drawRadarMap();
}


void Radar::DrawRadarBlips() {
    // Tüm blipleri çiz
    DrawBlips_t drawBlips = (DrawBlips_t)FUNC_DrawBlips;
    drawBlips();
}

int Radar::CreateBlip(float x, float y, float z, int sprite, int color) {
    // Koordinat blip oluþtur
    SetCoordBlip_t setCoordBlip = (SetCoordBlip_t)FUNC_SetCoordBlip;

    // eBlipType BLIP_COORD = 4, eBlipDisplay BLIP_DISPLAY_BOTH = 3
    return setCoordBlip((eBlipType)4, x, y, z, color, (eBlipDisplay)3);
}

void Radar::RemoveBlip(int blipIndex) {
    if (blipIndex < 0 || blipIndex >= 175) return;

    ClearBlip_t clearBlip = (ClearBlip_t)FUNC_ClearBlip;
    clearBlip(blipIndex);
}

tRadarTrace* Radar::GetRadarTrace(int index) {
    if (index < 0 || index >= 175) return nullptr;

    // RadarTrace array'inden belirli bir blip'i al
    return (tRadarTrace*)(ARRAY_RadarTrace + (index * sizeof(tRadarTrace)));
}

void Radar::SetRadarRange(float range) {
    // Radar zoom/range ayarla
    SafeWriteMemory<float>(VAR_RadarRange, range);
}

float Radar::GetRadarRange() {
    // Mevcut radar range'i al
    float* radarRange = (float*)VAR_RadarRange;
    return *radarRange;
}