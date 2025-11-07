#include "chams.h"
#include <globals.h>
#include <data/elements.h>
#include <console/console.h>
#include <data/variables.h>
#include <game/manager/manager.h>


#include <cmath>


/// Plugin SDK offsets (GTA SA 1.0)
#define FUNC_RpClumpForAllAtomics           0x749B70
#define FUNC_RpGeometryForAllMaterials      0x74C790
#define FUNC_RpAtomicRender                 0x7491C0

// CPed offsets
#define OFFSET_CPed_pRwObject               0x18

// RpMaterial structure
struct RpMaterialStruct {
    void* texture;
    RwRGBA color;
    void* pipeline;
    void* surfaceProps;
    short refCount;
    short pad;
};

Chams::Chams() {
    backupColors.clear();
}

Chams::~Chams() {
    backupColors.clear();
}

RpMaterial* Chams::MaterialCallback(RpMaterial* material, void* data) {
    if (!material || !data || !pChams) return material;

    RwRGBA* newColor = (RwRGBA*)data;
    RpMaterialStruct* matStruct = (RpMaterialStruct*)material;

    // Backup orijinal renk
    RwRGBA originalColor;
    originalColor.red = matStruct->color.red;
    originalColor.green = matStruct->color.green;
    originalColor.blue = matStruct->color.blue;
    originalColor.alpha = matStruct->color.alpha;

    MaterialBackup backup(material, originalColor);
    pChams->backupColors.push_back(backup);

    // Yeni rengi ayarla
    matStruct->color.red = newColor->red;
    matStruct->color.green = newColor->green;
    matStruct->color.blue = newColor->blue;
    matStruct->color.alpha = newColor->alpha;

    return material;
}

RpAtomic* Chams::AtomicCallback(RpAtomic* atomic, void* data) {
    if (!atomic) return atomic;

    RpGeometry* geometry = *(RpGeometry**)((DWORD)atomic + 0x14);
    if (!geometry) return atomic;

    typedef RpGeometry* (__cdecl* RpGeometryForAllMaterials_t)(RpGeometry*, RpMaterial* (*callback)(RpMaterial*, void*), void*);
    RpGeometryForAllMaterials_t RpGeometryForAllMaterials = (RpGeometryForAllMaterials_t)FUNC_RpGeometryForAllMaterials;

    RpGeometryForAllMaterials(geometry, MaterialCallback, data);

    return atomic;
}

RpAtomic* Chams::RenderAtomicCallback(RpAtomic* atomic, void* data) {
    if (!atomic) return atomic;

    typedef RpAtomic* (__cdecl* RpAtomicRender_t)(RpAtomic*);
    RpAtomicRender_t RpAtomicRender = (RpAtomicRender_t)FUNC_RpAtomicRender;

    RpAtomicRender(atomic);

    return atomic;
}

void Chams::SetMaterialColors(RpClump* clump, RwRGBA color) {
    if (!clump) return;

    typedef RpClump* (__cdecl* RpClumpForAllAtomics_t)(RpClump*, RpAtomic* (*callback)(RpAtomic*, void*), void*);
    RpClumpForAllAtomics_t RpClumpForAllAtomics = (RpClumpForAllAtomics_t)FUNC_RpClumpForAllAtomics;

    RpClumpForAllAtomics(clump, AtomicCallback, &color);
}

void Chams::RestoreMaterialColors(RpClump* clump) {
    for (size_t i = 0; i < backupColors.size(); i++) {
        MaterialBackup& backup = backupColors[i];
        if (backup.material) {
            RpMaterialStruct* matStruct = (RpMaterialStruct*)backup.material;
            matStruct->color.red = backup.originalColor.red;
            matStruct->color.green = backup.originalColor.green;
            matStruct->color.blue = backup.originalColor.blue;
            matStruct->color.alpha = backup.originalColor.alpha;
        }
    }
    backupColors.clear();
}

void Chams::RenderPlayerChams(CClientPlayer* player, IDirect3DDevice9* pDevice) {
    if (!player || !pDevice) return;

    CPed* pPed = player->GetGamePlayer();
    if (!pPed) return;

    RpClump* pClump = *(RpClump**)((DWORD)pPed + OFFSET_CPed_pRwObject);
    if (!pClump) return;

    DWORD dwZEnable, dwZWrite, dwFogEnable, dwLighting, dwCullMode;
    pDevice->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
    pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &dwZWrite);
    pDevice->GetRenderState(D3DRS_FOGENABLE, &dwFogEnable);
    pDevice->GetRenderState(D3DRS_LIGHTING, &dwLighting);
    pDevice->GetRenderState(D3DRS_CULLMODE, &dwCullMode);

    typedef RpClump* (__cdecl* RpClumpForAllAtomics_t)(RpClump*, RpAtomic* (*callback)(RpAtomic*, void*), void*);
    RpClumpForAllAtomics_t RpClumpForAllAtomics = (RpClumpForAllAtomics_t)FUNC_RpClumpForAllAtomics;

    // PASS 1: Hidden
    pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    RwRGBA hiddenColor;
    hiddenColor.red = 255;
    hiddenColor.green = 0;
    hiddenColor.blue = 0;
    hiddenColor.alpha = 255;

    SetMaterialColors(pClump, hiddenColor);
    RpClumpForAllAtomics(pClump, RenderAtomicCallback, NULL);
    RestoreMaterialColors(pClump);

    // PASS 2: Visible
    pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    RwRGBA visibleColor;
    visibleColor.red = 0;
    visibleColor.green = 255;
    visibleColor.blue = 0;
    visibleColor.alpha = 255;

    SetMaterialColors(pClump, visibleColor);
    RpClumpForAllAtomics(pClump, RenderAtomicCallback, NULL);
    RestoreMaterialColors(pClump);

    // Restore states
    pDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable);
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, dwZWrite);
    pDevice->SetRenderState(D3DRS_FOGENABLE, dwFogEnable);
    pDevice->SetRenderState(D3DRS_LIGHTING, dwLighting);
    pDevice->SetRenderState(D3DRS_CULLMODE, dwCullMode);
}

void Chams::ProcessChams(IDirect3DDevice9* pDevice) {
    if (!pDevice) {
        OutputDebugStringA("[Chams] ERROR: pDevice is NULL\n");
        return;
    }

    OutputDebugStringA("[Chams] ProcessChams called successfully\n");

}