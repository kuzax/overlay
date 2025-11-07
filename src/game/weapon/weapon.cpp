#include "weapon.h"
#include <data/elements.h>
#include <game/manager/manager.h>
#include <client/client.h>

bool c_weapon::give_weapon(CClientPlayer* player, eWeaponType weaponType, unsigned int uiAmmo, bool bSetAsCurrent)
{
	if (!manager->is_loaded() || !player)
		return false;

	CClientPed* ped = static_cast<CClientPed*>(player);
	if (!ped)
		return false;

	client->o_give_weapon(ped, weaponType, uiAmmo, bSetAsCurrent);

	return true;
}


void rapid_fire()
{
	if (!element->weapon.rapidfire)
		return;

	manager->set_animation_speed("PYTHON_CROUCHFIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("PYTHON_FIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("PYTHON_FIRE_POOR", element->weapon.rapidfire_speed);
	manager->set_animation_speed("RIFLE_CROUCHFIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("RIFLE_FIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("RIFLE_FIRE_POOR", element->weapon.rapidfire_speed);
	manager->set_animation_speed("SHOTGUN_CROUCHFIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("SHOTGUN_FIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("SHOTGUN_FIRE_POOR", element->weapon.rapidfire_speed);
	manager->set_animation_speed("SILENCED_CROUCH_FIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("SILENCED_FIRE", element->weapon.rapidfire_speed);
	manager->set_animation_speed("TEC_crouchfire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("TEC_fire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("UZI_crouchfire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("UZI_fire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("UZI_fire_POOR", element->weapon.rapidfire_speed);
	manager->set_animation_speed("idle_rocket", element->weapon.rapidfire_speed);
	manager->set_animation_speed("Rocket_Fire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("run_rocket", element->weapon.rapidfire_speed);
	manager->set_animation_speed("walk_rocket", element->weapon.rapidfire_speed);
	manager->set_animation_speed("WALK_start_rocket", element->weapon.rapidfire_speed);
	manager->set_animation_speed("WEAPON_sniper", element->weapon.rapidfire_speed);
	manager->set_animation_speed("buddy_crouchfire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("buddy_fire", element->weapon.rapidfire_speed);
	manager->set_animation_speed("buddy_fire_POOR", element->weapon.rapidfire_speed);
}

#include "Client\sdk\game\CWeapon.h"

void no_reload()
{
		if (!element->weapon.no_reload)
		return;


	CClientPlayer* local = manager->get_local_player();
	if (!local || !local->GetGamePlayer())
		return;


	eWeaponSlot currentSlot = local->GetGamePlayer()->GetCurrentWeaponSlot();
	eWeaponType weaponType = manager->get_weapon_type(local);

	if (weaponType >= 22 && weaponType <= 42)
	{
		CWeapon* pWeapon = local->GetGamePlayer()->GetWeapon(currentSlot);

				if (pWeapon)
		{
			int currentAmmo = pWeapon->GetAmmoInClip();  


			if (currentAmmo > 0)
			{
			
				pWeapon->SetAmmoInClip(30);  
			}


		}
	}
}



void c_weapon::release()
{
	if (!manager->is_loaded())
		return;

	rapid_fire();
	no_reload();
}
