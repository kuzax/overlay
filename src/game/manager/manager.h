#pragma once
#include <includes.h>
#include <sdk.h>

class c_manager {
public:
	bool is_loaded();
	CClientPlayer* get_local_player();
	std::vector<CClientPlayer*> get_players();		
	std::vector<CClientPlayer*> get_overlays_players();	
	std::vector<CClientObject*> get_objects();
	std::vector<CClientVehicle*> get_vehicles();
	std::vector<CClientPed*> get_peds();
	std::vector<CResource*> get_resources();
	eWeaponType get_weapon_type(CClientPlayer* player);	
	eWeaponType myget_weapon_type(CClientPlayer* player, eWeaponSlot slot);
	bool set_animation_speed(const SString& anime, float speed);
	bool set_player_skin(CClientPlayer* player, int skinId);
	bool set_dimension(CClientPlayer* player, int uid);
	bool teleport_to_player(CClientEntity* target_entity);	
	bool teleport_to_vehicle(CClientVehicle* target_entity);
	bool create_explosion(CVector vecPosition, eExplosionType type);
	const char* get_weapon_name(unsigned char weaponId);

};

inline c_manager* manager = new c_manager();

