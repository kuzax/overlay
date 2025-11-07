#pragma once
#include <includes.h>
#include <sdk.h>

class c_weapon {
public:
	bool give_weapon(CClientPlayer* player, eWeaponType weaponType, unsigned int uiAmmo, bool bSetAsCurrent);
	void release();
};

inline c_weapon* weapon = new c_weapon();