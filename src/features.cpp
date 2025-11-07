#include "features.h"



void Features::release()
{
	if (world.SetGameSpeed)
	{
		global->m_pGame->SetGameSpeed(world.speed);
	}
	else
	{
		global->m_pGame->SetGameSpeed(1.0f);

	}
	if (world.SetGravity)
	{
		global->m_pGame->SetGravity(world.gravity);
	}
	else
	{	
		global->m_pGame->SetGravity(0.008f);
	}
}

