#pragma once
#include <includes.h>
#include <sdk.h>
#include <d3dx9.h>

// Plugin SDK RenderWare structures
struct RwRGBA {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};




class Chams {
public:
    Chams();
    ~Chams();

    void ProcessChams(IDirect3DDevice9* pDevice);

private:
    void RenderPlayerChams(CClientPlayer* player, IDirect3DDevice9* pDevice);

    static RpMaterial* MaterialCallback(RpMaterial* material, void* data);
    static RpAtomic* AtomicCallback(RpAtomic* atomic, void* data);
    static RpAtomic* RenderAtomicCallback(RpAtomic* atomic, void* data);

    // Renk ayarlama
    void SetMaterialColors(RpClump* clump, RwRGBA color);
    void RestoreMaterialColors(RpClump* clump);

    // Backup
    struct MaterialBackup {
        RpMaterial* material;
        RwRGBA originalColor;

        MaterialBackup(RpMaterial* mat, RwRGBA col) : material(mat), originalColor(col) {}
    };
    std::vector<MaterialBackup> backupColors;
};

inline Chams* pChams = nullptr;