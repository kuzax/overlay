#pragma once
#include <includes.h>
#include <sdk.h>

class c_visual {
public:
	void release();
	void draw_player_esp();	
	void draw_ped_esp();
	void draw_object_esp();
	void draw_vehicle_esp();
	bool overlayEnabled = false;

	float GetVehicleHealth(CClientVehicle* vehicle);
};

inline c_visual* visual = new c_visual();

extern float GetPlayerHealth(CClientPed* player);
extern float GetArmorFromPed(CClientPed* player);

struct font
{
	static inline ImFont* Font;
	static inline ImFont* FontBold;
	inline static float TextSize = 18;
};