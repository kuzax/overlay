#include "crosshair.h"
#include <globals.h>
#include <data/elements.h>
#include <console/console.h>
#include <commdlg.h>
#include <d3dx9.h>
#include <data/variables.h>

#pragma comment(lib, "d3dx9.lib")

CustomCrosshair::CustomCrosshair() {
    bOriginalCrosshairHidden = false;
    crosshairOffsetX = (float*)0xB6EC14;
    crosshairOffsetY = (float*)0xB6EC10;

    pCrosshairTexture = nullptr;
    pPendingRelease = nullptr;
    imageWidth = 0;
    imageHeight = 0;
    bHasCustomImage = false;

    memcpy(originalCrosshairBytes, (void*)0x58E020, 5);
    HideOriginalCrosshair();
}

CustomCrosshair::~CustomCrosshair() {
    ShowOriginalCrosshair();
    ReleaseCrosshairImage();

    // Pending texture'ı da temizle
    if (pPendingRelease) {
        pPendingRelease->Release();
        pPendingRelease = nullptr;
    }
}

void CustomCrosshair::ReleaseCrosshairImage() {
    // Önce flag'i kapat ki rendering durur
    bHasCustomImage = false;

    // Mevcut texture'ı pending'e taşı (bir sonraki frame'de silinecek)
    if (pCrosshairTexture) {
        pPendingRelease = pCrosshairTexture;
        pCrosshairTexture = nullptr;
    }

    imageWidth = 0;
    imageHeight = 0;
}

bool CustomCrosshair::LoadCrosshairImage(const char* filepath) {
    // Eski texture'ı temizle
    ReleaseCrosshairImage();

    IDirect3DDevice9* pDevice = var->winapi.device_dx9;

    if (!pDevice) {
        console->error("D3D Device not found!");
        return false;
    }

    D3DXIMAGE_INFO imageInfo;
    HRESULT hr = D3DXGetImageInfoFromFileA(filepath, &imageInfo);

    if (FAILED(hr)) {
        console->error("Failed to load image info: %s", filepath);
        return false;
    }

    imageWidth = imageInfo.Width;
    imageHeight = imageInfo.Height;

    hr = D3DXCreateTextureFromFileExA(
        pDevice,
        filepath,
        imageInfo.Width,
        imageInfo.Height,
        D3DX_DEFAULT,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        &imageInfo,
        nullptr,
        &pCrosshairTexture
    );

    if (FAILED(hr)) {
        console->error("Failed to create texture: %s", filepath);
        return false;
    }

    bHasCustomImage = true;
    console->success("Crosshair image loaded: %dx%d", imageWidth, imageHeight);
    return true;
}

void CustomCrosshair::OpenFileDialog() {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "PNG Files\0*.PNG\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        if (LoadCrosshairImage(szFile)) {
            element->customcrosshair.useImage = true;
        }
    }
}

void CustomCrosshair::HideOriginalCrosshair() {
    if (bOriginalCrosshairHidden) return;

    DWORD crosshairFunc = 0x58E020;
    DWORD oldProtect;

    VirtualProtect((LPVOID)crosshairFunc, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    BYTE patch[] = { 0xC3, 0x90, 0x90, 0x90, 0x90 };
    memcpy((void*)crosshairFunc, patch, 5);
    VirtualProtect((LPVOID)crosshairFunc, 5, oldProtect, &oldProtect);

    bOriginalCrosshairHidden = true;
}

void CustomCrosshair::ShowOriginalCrosshair() {
    if (!bOriginalCrosshairHidden) return;

    DWORD crosshairFunc = 0x58E020;
    DWORD oldProtect;

    VirtualProtect((LPVOID)crosshairFunc, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((void*)crosshairFunc, originalCrosshairBytes, 5);
    VirtualProtect((LPVOID)crosshairFunc, 5, oldProtect, &oldProtect);

    bOriginalCrosshairHidden = false;
}

void CustomCrosshair::DrawCrosshair() {
    // Her frame başında pending texture'ı temizle (güvenli release)
    if (pPendingRelease) {
        ULONG refCount = pPendingRelease->Release();
        pPendingRelease = nullptr;
    }

    if (!element->customcrosshair.enabled) {
        if (bOriginalCrosshairHidden)
            ShowOriginalCrosshair();
        return;
    }

    // Kamera kontrolü - Sadece aim mode'da göster
    CCamera* camera = global->m_pGame->GetCamera();
    if (!camera) {
        if (bOriginalCrosshairHidden)
            ShowOriginalCrosshair();
        return;
    }

    eCamMode camera_viewmode = static_cast<eCamMode>(camera->GetCam(camera->GetActiveCam())->GetMode());

    // Sadece aim mode'larda custom crosshair göster (mode 53 veya 7)
    if (camera_viewmode != 53 && camera_viewmode != 7) {
        if (bOriginalCrosshairHidden)
            ShowOriginalCrosshair();
        return;
    }

    // Aim mode'dayız, orijinal crosshair'ı gizle
    if (!bOriginalCrosshairHidden)
        HideOriginalCrosshair();

    float offsetX = *crosshairOffsetX;
    float offsetY = *crosshairOffsetY;

    float baseX = offsetX * 640.0f;
    float baseY = offsetY * 448.0f;

    float screenWidth = ImGui::GetIO().DisplaySize.x;
    float screenHeight = ImGui::GetIO().DisplaySize.y;

    float scaleX = screenWidth / 640.0f;
    float scaleY = screenHeight / 448.0f;

    float posX = baseX * scaleX;
    float posY = baseY * scaleY;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Image mode - Yuvarlak
    if (element->customcrosshair.useImage && bHasCustomImage && pCrosshairTexture) {
        float imgSize = element->customcrosshair.imageSize;
        float radius = imgSize / 2.0f;

        ImVec2 center(posX, posY);

        ImU32 tintColor = IM_COL32(
            (int)(element->customcrosshair.color[0] * 255),
            (int)(element->customcrosshair.color[1] * 255),
            (int)(element->customcrosshair.color[2] * 255),
            (int)(element->customcrosshair.color[3] * 255)
        );

        const ImTextureID tex_id = (ImTextureID)pCrosshairTexture;
        const int segments = 64;

        drawList->PushTextureID(tex_id);
        drawList->PrimReserve((segments - 2) * 3, segments);

        const ImDrawIdx vtx_base = (ImDrawIdx)drawList->_VtxCurrentIdx;

        for (int i = 0; i < segments; i++) {
            float angle = ((float)i / (float)segments) * 2.0f * IM_PI;
            float x = center.x + cosf(angle) * radius;
            float y = center.y + sinf(angle) * radius;
            float u = 0.5f + cosf(angle) * 0.5f;
            float v = 0.5f + sinf(angle) * 0.5f;

            drawList->PrimWriteVtx(ImVec2(x, y), ImVec2(u, v), tintColor);
        }

        for (int i = 2; i < segments; i++) {
            drawList->PrimWriteIdx(vtx_base);
            drawList->PrimWriteIdx((ImDrawIdx)(vtx_base + i - 1));
            drawList->PrimWriteIdx((ImDrawIdx)(vtx_base + i));
        }

        drawList->PrimWriteIdx(vtx_base);
        drawList->PrimWriteIdx((ImDrawIdx)(vtx_base + segments - 1));
        drawList->PrimWriteIdx((ImDrawIdx)(vtx_base + 0));

        drawList->PopTextureID();
        return;
    }

    // Shape mode
    ImU32 color = IM_COL32(
        (int)(element->customcrosshair.color[0] * 255),
        (int)(element->customcrosshair.color[1] * 255),
        (int)(element->customcrosshair.color[2] * 255),
        (int)(element->customcrosshair.color[3] * 255)
    );

    float size = element->customcrosshair.size;
    float thickness = element->customcrosshair.thickness;
    float gap = element->customcrosshair.gap;

    switch (element->customcrosshair.type) {
    case 0: // Cross
        drawList->AddLine(ImVec2(posX - size - gap, posY), ImVec2(posX - gap, posY), color, thickness);
        drawList->AddLine(ImVec2(posX + gap, posY), ImVec2(posX + size + gap, posY), color, thickness);
        drawList->AddLine(ImVec2(posX, posY - size - gap), ImVec2(posX, posY - gap), color, thickness);
        drawList->AddLine(ImVec2(posX, posY + gap), ImVec2(posX, posY + size + gap), color, thickness);
        break;

    case 1: // Dot
        drawList->AddCircleFilled(ImVec2(posX, posY), size, color);
        break;

    case 2: // Circle
        drawList->AddCircle(ImVec2(posX, posY), size, color, 32, thickness);
        break;

    case 3: // T Shape
        drawList->AddLine(ImVec2(posX - size, posY - gap), ImVec2(posX + size, posY - gap), color, thickness);
        drawList->AddLine(ImVec2(posX, posY - gap), ImVec2(posX, posY + size), color, thickness);
        break;

    case 4: // Square
        drawList->AddRect(ImVec2(posX - size, posY - size), ImVec2(posX + size, posY + size), color, 0.0f, 0, thickness);
        break;

    case 5: // X Shape
        drawList->AddLine(ImVec2(posX - size - gap, posY - size - gap), ImVec2(posX - gap, posY - gap), color, thickness);
        drawList->AddLine(ImVec2(posX + gap, posY + gap), ImVec2(posX + size + gap, posY + size + gap), color, thickness);
        drawList->AddLine(ImVec2(posX + size + gap, posY - size - gap), ImVec2(posX + gap, posY - gap), color, thickness);
        drawList->AddLine(ImVec2(posX - gap, posY + gap), ImVec2(posX - size - gap, posY + size + gap), color, thickness);
        break;

    case 6: // Triangle
        float halfSize = size;
        drawList->AddTriangle(
            ImVec2(posX, posY - halfSize),
            ImVec2(posX - halfSize, posY + halfSize),
            ImVec2(posX + halfSize, posY + halfSize),
            color, thickness
        );
        break;
    }
}