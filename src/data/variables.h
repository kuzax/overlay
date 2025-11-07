#pragma once
#include <includes.h>
#include <imgui/imgui_texteditor.h>
#include <D3D11.h>

class c_variables {
public:
	struct
	{
		HMODULE hModule;
		MH_STATUS mh_status;
		WNDPROC wnd_proc;

		// DX11 için
		ID3D11Device* device_dx11{ nullptr };
		ID3D11DeviceContext* context{ nullptr };
		ID3D11RenderTargetView* render_target_view{ nullptr };
	} winapi;

	struct
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;
		ImVec2 size{ 924, 720 };
	} window;

	struct
	{
		bool dpi_changed{ false };
		bool initialized{ false };
		bool is_open{ false };
		bool is_dump_open{ false };
	} gui;

	struct
	{
		TextEditor executor;		
		TextEditor OverWrite;		
		
		TextEditor dumper;
	} editor;

};

inline c_variables* var = new c_variables();