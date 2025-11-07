#pragma once
#include <includes.h>
#include <d3d9.h>

class CustomCrosshair {
public:
    CustomCrosshair();
    ~CustomCrosshair();

    void DrawCrosshair();
    void HideOriginalCrosshair();
    void ShowOriginalCrosshair();

    bool LoadCrosshairImage(const char* filepath);
    void ReleaseCrosshairImage();
    void OpenFileDialog();

private:
    bool bOriginalCrosshairHidden;
    BYTE originalCrosshairBytes[5];

    float* crosshairOffsetX;
    float* crosshairOffsetY;

    IDirect3DTexture9* pCrosshairTexture;
    IDirect3DTexture9* pPendingRelease;  
    int imageWidth;
    int imageHeight;
    bool bHasCustomImage;
};

inline CustomCrosshair* pCustomCrosshair = nullptr;