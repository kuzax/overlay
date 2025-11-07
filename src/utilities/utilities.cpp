#include "utilities.h"
#include <console/console.h>
#include <random>
#include <data/variables.h>
MODULEINFO utilities::c_pattern::get_module_info(const char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(szModule);
	if (hModule == 0) return modinfo;
	K32GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

DWORD utilities::c_pattern::find_pattern(const char* hModule, const char* pattern, const char* mask)
{
	if (!GetModuleHandleA(hModule))  
		return 0;

	HMODULE ModuleHandleA = GetModuleHandleA(hModule);
	_MODULEINFO modinfo{};
	if (ModuleHandleA)
	{
		K32GetModuleInformation(GetCurrentProcess(), ModuleHandleA, &modinfo, sizeof(modinfo));
	}

	unsigned int patternLength = (unsigned int)strlen(mask);
	unsigned int size = modinfo.SizeOfImage;
	unsigned int lastScan = size - patternLength;
	if (size <= patternLength)
		return 0;

	char* base = (char*)modinfo.lpBaseOfDll;

	for (unsigned int i = 0; i < lastScan; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
			return (unsigned int)(base + i);
	}

	return 0; 
}

DWORD utilities::c_device::find_vtable(DWORD type)
{
	DWORD dwBaseObj = 0;

	char info[MAX_PATH];
	GetSystemDirectoryA(info, MAX_PATH);
	strcat_s(info, MAX_PATH, "\\d3d9.dll");

	dwBaseObj = (DWORD)LoadLibraryA(info);

	while (dwBaseObj++ < dwBaseObj + type)
	{
		if ((*(WORD*)(dwBaseObj + 0x00)) == 0x06C7 && (*(WORD*)(dwBaseObj + 0x06)) == 0x8689 && (*(WORD*)(dwBaseObj + 0x0C)) == 0x8689)
		{
			dwBaseObj += 2;
			break;
		}
	}

	return(dwBaseObj);
}

DWORD utilities::c_device::get_address(int vtable)
{
	PDWORD table;
	*(DWORD*)&table = *(DWORD*)find_vtable(0x128000);
	return table[vtable];
}

DWORD utilities::c_device::find_vtable_dx11()
{
    // Geçici bir D3D11 device ve swapchain oluþtur
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 2;
    sd.BufferDesc.Height = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pDevice,
        &featureLevel,
        &pContext
    );

    if (FAILED(hr))
    {
        // Hardware baþarýsýz olursa WARP dene
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            D3D_DRIVER_TYPE_WARP,
            NULL,
            0,
            NULL,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &pSwapChain,
            &pDevice,
            &featureLevel,
            &pContext
        );
    }

    if (SUCCEEDED(hr) && pSwapChain)
    {
        // SwapChain'in vtable adresini al
        DWORD* pSwapChainVtable = *(DWORD**)pSwapChain;
        DWORD vtableAddress = (DWORD)pSwapChainVtable;

        // Temizlik
        pSwapChain->Release();
        pContext->Release();
        pDevice->Release();
        DestroyWindow(hWnd);
        UnregisterClass("DX", wc.hInstance);

        return vtableAddress;
    }

    // Baþarýsýz
    DestroyWindow(hWnd);
    UnregisterClass("DX", wc.hInstance);
    return 0;
}

DWORD utilities::c_device::get_address(int vtable)
{
    PDWORD table;
    *(DWORD*)&table = find_vtable_dx11();

    if (table == 0)
    {
        // Hata: vtable bulunamadý
        return 0;
    }

    return table[vtable];
}

bool is_utf8_bom(const void* pData, unsigned int uiLength)
{
	const unsigned char* pCharData = (const unsigned char*)pData;
	return (uiLength > 2 && pCharData[0] == 0xEF && pCharData[1] == 0xBB && pCharData[2] == 0xBF);
}

bool utilities::c_scripts::is_file_compiled(const void* pData, unsigned int uiLength)
{
	const unsigned char* pCharData = (const unsigned char*)pData;
	if (is_utf8_bom(pCharData, uiLength))
	{
		pCharData += 3;
		uiLength -= 3;
	}
	return (uiLength > 0 && pCharData[0] == 0x1B);
}
