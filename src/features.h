#pragma once
#include "includes.h"
#include "globals.h"

class Features
{
public:
	struct {
		bool SetGameSpeed{ false };
		float speed = 1.0f;
		bool SetGravity{ false };
		float gravity = 0.008f;

	} world;
	struct {

		

	} gta_cheats;
	bool GTACHETS(const WorldSpecialProperty property);
	void release();
};
inline Features* features = new Features();