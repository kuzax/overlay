#include "visual.h"
#include <globals.h>
#include <data/elements.h>
#include <game/manager/manager.h>
#include <game/aimbot/aimbot.h>
#include "CClientPed.h"
#include "visual.h"
#include <globals.h>
#include <data/elements.h>
#include <game/manager/manager.h>
#include <algorithm>  // min fonksiyonu için bunu ekleyin
#include <cmath> // M_PI için
#include <data/variables.h>
#include "client/client.h"
#include <CEntitySA.h>
#include "menu/menu.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


bool is_visible(const CVector& from, const CVector& to, CClientPed* pTarget)
{
	if (!pTarget || !global->m_pClientGame || !global->m_pGame)
		return false;

	// World geçerli mi?
	CWorld* pWorld = global->m_pGame->GetWorld();
	if (!pWorld)
		return false;

	SLineOfSightFlags flags;
	flags.bCheckBuildings = true;
	flags.bCheckDummies = true;
	flags.bCheckObjects = true;
	flags.bCheckPeds = false;
	flags.bCheckVehicles = pTarget->IsInVehicle() ? false : true;
	flags.bIgnoreSomeObjectsForCamera = false;
	flags.bSeeThroughStuff = true;
	flags.bShootThroughStuff = true;

	return pWorld->IsLineOfSightClear(&from, &to, flags);
}



bool world_to_screen(CVector& vecWorld, CVector& vecScreen, float fEdgeTolerance, bool bRelative)
{
	if (!element->content.loaded_client && !global->m_pCore)
	{
		return false;
	}

	global->m_pCore->GetGraphics()->CalcScreenCoors(&vecWorld, &vecScreen);

	float fResWidth = static_cast<float>(global->m_pCore->GetGraphics()->GetViewportWidth());
	float fResHeight = static_cast<float>(global->m_pCore->GetGraphics()->GetViewportHeight());

	float fToleranceX = bRelative ? fEdgeTolerance * fResWidth : fEdgeTolerance;
	float fToleranceY = bRelative ? fEdgeTolerance * fResHeight : fEdgeTolerance;

	fToleranceX = Clamp(0.f, fToleranceX, fResWidth * 10);
	fToleranceY = Clamp(0.f, fToleranceY, fResHeight * 10);

	if (vecScreen.fX >= -fToleranceX && vecScreen.fX <= fResWidth + fToleranceX && vecScreen.fY >= -fToleranceY && vecScreen.fY <= fResHeight + fToleranceY && vecScreen.fZ > 0.1f)
	{
		return true;
	}

	return false;
}

bool fake_visibility(CClientPlayer* local, CClientPlayer* target)
{
	if (!local || !target)
		return false;

	CVector vecFrom, vecTo, vecScreen;

	// Baş ve baş arası vektörü
	local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecFrom);
	target->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecTo);

	CVector mid = (vecFrom + vecTo) * 0.5f;

	if (!world_to_screen(mid, vecScreen, 0.0f, false))
		return false;

	return true;
}

void draw_ped_line(CClientPed* ped)
{
	if (!ped || !element->esp.ped_line)
		return;

	std::vector<CClientPlayer*> players = manager->get_players();
	for (auto player : players)
	{
		if (player && player->GetGamePlayer() == ped->GetGamePlayer())
			return;
	}

	CClientPed* entity = static_cast<CClientPed*>(ped);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecPedPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPedPos);

	float distance = (vecLocalPos - vecPedPos).Length();
	if (distance > element->esp.max_distance)
		return;

	// is_visible kontrolü için kafa kemiği pozisyonları alalım
	CVector vecHeadLocal, vecHeadPed;
	if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadPed))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadPed, entity);

	CVector screenLocal, screenPed;
	if (!world_to_screen(vecLocalPos, screenLocal, 0.0f, false) || !world_to_screen(vecPedPos, screenPed, 0.0f, false))
		return;

	ImU32 lineColor = bVisible
		? ImGui::ColorConvertFloat4ToU32(element->esp.line_color)
		: IM_COL32(255, 0, 0, 255); // görünmüyorsa kırmızı

	ImGui::GetBackgroundDrawList()->AddLine(
		ImVec2(screenLocal.fX, screenLocal.fY),
		ImVec2(screenPed.fX, screenPed.fY),
		lineColor,
		1.5f
	);
}

void draw_object_distance(CClientObject* object)
{
	if (!object || !element->esp.object_distance)
		return;

	CClientEntity* local = static_cast<CClientEntity*>(global->m_pPlayerManager->GetLocalPlayer());
	CClientEntity* entity = static_cast<CClientEntity*>(object);
	if (!local || !entity)
		return;

	CVector vecLocalPos, vecObjPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecObjPos);

	float distance = (vecLocalPos - vecObjPos).Length();
	if (distance > element->esp.max_distance)
		return;

	vecObjPos.fZ -= 1.0f;

	CVector vecScreen;
	if (!world_to_screen(vecObjPos, vecScreen, 0.0f, false))
		return;

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY + 18.0f);

	char distText[32];
	snprintf(distText, sizeof(distText), "[%.1fm]", distance);

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
	ImU32 textColor = ImGui::ColorConvertFloat4ToU32(element->esp.object_distance_color);

	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, distText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, distText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, distText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, distText);

	draw->AddText(font::Font, 16.0f, pos, textColor, distText);
}


void draw_ped_name(CClientPed* ped)
{
	if (!ped)
		return;

	// Ped oyuncuya aitse çizme
	std::vector<CClientPlayer*> players = manager->get_players();
	for (auto player : players)
	{
		if (player && player->GetGamePlayer() == ped->GetGamePlayer())
			return;
	}

	CClientPed* entity = static_cast<CClientPed*>(ped);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = sqrt(
		pow(vecPos.fX - vecLocalPos.fX, 2) +
		pow(vecPos.fY - vecLocalPos.fY, 2) +
		pow(vecPos.fZ - vecLocalPos.fZ, 2)
	);

	if (distance > element->esp.max_distance)
		return;

	char szName[64];
	snprintf(szName, sizeof(szName), "Ped");

	CVector vecScreen;
	vecPos.fZ -= 1.1f;

	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	// Görünürlük kontrolü
	bool bVisible = is_visible(vecLocalPos, vecPos, entity);

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
	ImU32 nameColor = bVisible
		? ImGui::ColorConvertFloat4ToU32(element->esp.ped_name_color)
		: IM_COL32(255, 0, 0, 255);

	// Outline (kenar çizimleri)
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, szName);

	// Asıl yazı
	draw->AddText(font::Font, 16.0f, pos, nameColor, szName);
}

void draw_ped_corner_box(CClientPed* ped)
{
	if (!ped)
		return;

	// Ped bir oyuncuya aitse çizme
	std::vector<CClientPlayer*> players = manager->get_players();
	for (auto player : players)
	{
		if (player && player->GetGamePlayer() == ped->GetGamePlayer())
			return;
	}

	CClientPed* entity = static_cast<CClientPed*>(ped);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = sqrt(
		pow(vecPos.fX - vecLocalPos.fX, 2) +
		pow(vecPos.fY - vecLocalPos.fY, 2) +
		pow(vecPos.fZ - vecLocalPos.fZ, 2)
	);

	if (distance > element->esp.max_distance)
		return;

	// is_visible için kafa kemiği pozisyonları al
	CVector vecHeadLocal, vecHeadPed;
	if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadPed))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadPed, entity);

	// Ped yüksekliği ve kutu için min/max hesapla
	CVector vecMin = vecPos;
	vecMin.fZ -= 0.9f;  // Pedin altı
	CVector vecMax = vecPos;
	vecMax.fZ += 1.1f;  // Pedin üstü

	CVector screenTopLeft, screenBottomRight;
	if (!world_to_screen(vecMin, screenTopLeft, 0.0f, false) || !world_to_screen(vecMax, screenBottomRight, 0.0f, false))
		return;

	float height = screenBottomRight.fY - screenTopLeft.fY;
	float width = height / 2.0f;
	float left = screenTopLeft.fX - width / 2;
	float top = screenTopLeft.fY;
	float right = screenTopLeft.fX + width / 2;
	float bottom = screenBottomRight.fY;

	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	ImU32 boxColor = bVisible
		? ImGui::ColorConvertFloat4ToU32(element->esp.ped_box_color)
		: IM_COL32(255, 0, 0, 255); // görünmüyorsa kırmızı

	float minSide = std::min(width, height);
	float lineLen = minSide * 0.16f;
	float thickness = 1.60f;

	// Köşeler
	// Sol üst
	draw->AddLine(ImVec2(left, top), ImVec2(left + lineLen, top), boxColor, thickness);
	draw->AddLine(ImVec2(left, top), ImVec2(left, top + lineLen), boxColor, thickness);

	// Sağ üst
	draw->AddLine(ImVec2(right, top), ImVec2(right - lineLen, top), boxColor, thickness);
	draw->AddLine(ImVec2(right, top), ImVec2(right, top + lineLen), boxColor, thickness);

	// Sol alt
	draw->AddLine(ImVec2(left, bottom), ImVec2(left + lineLen, bottom), boxColor, thickness);
	draw->AddLine(ImVec2(left, bottom), ImVec2(left, bottom - lineLen), boxColor, thickness);

	// Sağ alt
	draw->AddLine(ImVec2(right, bottom), ImVec2(right - lineLen, bottom), boxColor, thickness);
	draw->AddLine(ImVec2(right, bottom), ImVec2(right, bottom - lineLen), boxColor, thickness);
}


void draw_ped_skeleton(CClientPed* ped)
{
	if (!ped)
		return;

	// Oyuncu peddense çizme (aynı kontrol)
	std::vector<CClientPlayer*> players = manager->get_players();
	for (auto player : players)
	{
		if (player && player->GetGamePlayer() == ped->GetGamePlayer())
			return;
	}

	CClientEntity* entity = static_cast<CClientEntity*>(ped);
	CClientEntity* local = static_cast<CClientEntity*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = sqrt(
		pow(vecPos.fX - vecLocalPos.fX, 2) +
		pow(vecPos.fY - vecLocalPos.fY, 2) +
		pow(vecPos.fZ - vecLocalPos.fZ, 2)
	);

	if (distance > element->esp.max_distance)
		return;

	CClientPed* pedEntity = static_cast<CClientPed*>(ped);
	CClientPed* localPed = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());

	if (!pedEntity || !localPed)
		return;

	CVector vecHeadLocal, vecHeadPed;
	if (!localPed->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!pedEntity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadPed))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadPed, pedEntity);

	std::vector<std::pair<eBone, eBone>> bones = {
		{ BONE_NECK, BONE_UPPERTORSO },
		{ BONE_UPPERTORSO, BONE_SPINE1 },
		{ BONE_SPINE1, BONE_PELVIS },
		{ BONE_UPPERTORSO, BONE_RIGHTSHOULDER },
		{ BONE_RIGHTSHOULDER, BONE_RIGHTELBOW },
		{ BONE_RIGHTELBOW, BONE_RIGHTWRIST },
		{ BONE_UPPERTORSO, BONE_LEFTSHOULDER },
		{ BONE_LEFTSHOULDER, BONE_LEFTELBOW },
		{ BONE_LEFTELBOW, BONE_LEFTWRIST },
		{ BONE_PELVIS, BONE_RIGHTKNEE },
		{ BONE_RIGHTKNEE, BONE_RIGHTANKLE },
		{ BONE_PELVIS, BONE_LEFTKNEE },
		{ BONE_LEFTKNEE, BONE_LEFTANKLE }
	};

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	ImU32 visibleColor = ImGui::ColorConvertFloat4ToU32(element->esp.ped_bone_color);
	ImU32 hiddenColor = IM_COL32(255, 0, 0, 255);

	ImU32 color = bVisible ? visibleColor : hiddenColor;

	for (const auto& bone_pair : bones)
	{
		CVector vecStart, vecEnd;
		if (!ped->GetGamePlayer()->GetBonePosition(bone_pair.first, &vecStart))
			continue;
		if (!ped->GetGamePlayer()->GetBonePosition(bone_pair.second, &vecEnd))
			continue;

		CVector screenStart, screenEnd;
		if (!world_to_screen(vecStart, screenStart, 0.0f, false))
			continue;
		if (!world_to_screen(vecEnd, screenEnd, 0.0f, false))
			continue;

		drawList->AddLine(
			ImVec2(screenStart.fX, screenStart.fY),
			ImVec2(screenEnd.fX, screenEnd.fY),
			color,
			1.4f
		);
	}
}


void draw_ped_distance(CClientPed* ped)
{
	if (!ped || !element->esp.ped_distance)
		return;

	// Ped bir oyuncuya aitse çizme
	std::vector<CClientPlayer*> players = manager->get_players();
	for (auto player : players)
	{
		if (player && player->GetGamePlayer() == ped->GetGamePlayer())
			return;
	}

	CClientPed* pedEntity = static_cast<CClientPed*>(ped);
	CClientPed* localPed = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!pedEntity || !localPed)
		return;

	CVector vecLocalPos, vecPos;
	localPed->GetPosition(vecLocalPos);
	pedEntity->GetPosition(vecPos);

	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	vecPos.fZ -= 1.0f;

	CVector vecScreen;
	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImVec2 distancePos = ImVec2(pos.x, pos.y + 18.0f);

	char distanceText[32];
	snprintf(distanceText, sizeof(distanceText), "[%.1fm]", distance);

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
	ImU32 textColor = ImGui::ColorConvertFloat4ToU32(element->esp.ped_distance_color);

	// Outline
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x + 1, distancePos.y), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x - 1, distancePos.y), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x, distancePos.y + 1), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x, distancePos.y - 1), outlineColor, distanceText);

	// Ana metin
	draw->AddText(font::Font, 16.0f, distancePos, textColor, distanceText);
}

void draw_player_name(CClientPlayer* player)
{
	if (!player)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecTargetPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecTargetPos);

	float distance = (vecLocalPos - vecTargetPos).Length();
	if (distance > element->esp.max_distance)
		return;

	char szName[64];
	snprintf(szName, sizeof(szName), "%s", player->GetNametagText());

	// Görünürlük için head pozisyonu
	CVector vecHeadPos;
	if (!entity->GetGamePlayer() || !entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadPos))
		return;

	// NaN crash önlemi
	if (!std::isfinite(vecHeadPos.fX) || !std::isfinite(vecHeadPos.fY) || !std::isfinite(vecHeadPos.fZ))
		return;

	// Görünürlük kontrolü
	bool bVisible = is_visible(vecLocalPos, vecHeadPos, entity);

	// Yazı pozisyonu - kafaya binmemesi için biraz aşağı çekilmiş hedef pozisyonu
	CVector vecNamePos = vecTargetPos;
	vecNamePos.fZ -= 1.0f;

	CVector vecScreen;
	if (!world_to_screen(vecNamePos, vecScreen, 0.0f, false))
		return;

	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);

	// Renk belirleme - silent aim target kontrolü
	ImU32 nameColor;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		nameColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		nameColor = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.name_color)
			: IM_COL32(255, 0, 0, 255); // görünmüyorsa kırmızı
	}

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	// Outline (kenar çizimleri)
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, szName);

	// Ana yazı
	draw->AddText(font::Font, 16.0f, pos, nameColor, szName);
}


void draw_player_weapon(CClientPlayer* player)
{
	if (!player)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local || !entity->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecTargetPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecTargetPos);

	float distance = (vecLocalPos - vecTargetPos).Length();
	if (distance > element->esp.max_distance)
		return;

	eWeaponType weaponId = manager->get_weapon_type(player);
	const char* szWeaponName = weaponId == WEAPONTYPE_UNARMED ? "Fist" : manager->get_weapon_name(weaponId);
	if (!szWeaponName)
		return;

	// Ekrana yazdırma pozisyonu
	CVector vecDrawPos = vecTargetPos;
	vecDrawPos.fZ += 0.5f;

	CVector vecScreen;
	if (!world_to_screen(vecDrawPos, vecScreen, 0.0f, false))
		return;

	// Görünürlük kontrolü
	bool bVisible = false;
	CVector vecFrom, vecTo;
	if (local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecFrom) &&
		entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecTo))
	{
		bVisible = is_visible(vecFrom, vecTo, entity);
	}

	// Renk seçimi
	ImU32 textColor;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		textColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color); // Silent aim target
	}
	else
	{
		textColor = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.weapon_color) // Normal ESP
			: IM_COL32(255, 0, 0, 255); // Duvar arkasında kırmızı
	}

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);

	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	// Outline
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, szWeaponName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, szWeaponName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, szWeaponName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, szWeaponName);

	// Ana yazı
	draw->AddText(font::Font, 16.0f, pos, textColor, szWeaponName);
}

static RpAtomic* ChamsRenderAtomicCallback(RpAtomic* atomic, void* data)
{
	if (!atomic)
		return atomic;

	// RpAtomicRender fonksiyonu
	typedef RpAtomic* (__cdecl* RpAtomicRender_t)(RpAtomic*);
	RpAtomicRender_t RpAtomicRender = (RpAtomicRender_t)0x7491C0;

	RpAtomicRender(atomic);

	return atomic;
}

enum ChamsType {
	CHAMS_CRYSTAL = 0,
	CHAMS_GLOW = 1,
	CHAMS_SPARKLE = 2
};


void draw_player_chams(CClientPlayer* player)
{
	if (!player)
		return;
	CPed* pPed = (CPed*)player->GetGamePlayer();
	if (!pPed)
		return;
	CGraphicsInterface* pGraphics = global->m_pCore->GetGraphics();
	IDirect3DDevice9* pDevice = pGraphics->GetDevice();

	// Visibility flag
	struct {
		unsigned char bUsesCollision : 1;
		unsigned char bCollisionProcessed : 1;
		unsigned char bIsStatic : 1;
		unsigned char bHasContacted : 1;
		unsigned char bIsStuck : 1;
		unsigned char bIsInSafePosition : 1;
		unsigned char bWasPostponed : 1;
		unsigned char bIsVisible : 1;
	} *pFlags = (decltype(pFlags))((DWORD)pPed + 0x42);

	bool oldVisible = pFlags->bIsVisible;
	pFlags->bIsVisible = true;

	DWORD oldZEnable, oldZWrite, oldZFunc, oldLighting, oldFog, oldAlphaBlend, oldSrcBlend, oldDestBlend;
	DWORD oldFillMode, oldShadeMode, oldCullMode;
	DWORD dwCONSTANT, dwARG2, dwColorOp, dwColorArg1, dwAlphaOp, dwAlphaArg1;

	pDevice->GetRenderState(D3DRS_ZENABLE, &oldZEnable);
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &oldZWrite);
	pDevice->GetRenderState(D3DRS_ZFUNC, &oldZFunc);
	pDevice->GetRenderState(D3DRS_LIGHTING, &oldLighting);
	pDevice->GetRenderState(D3DRS_FOGENABLE, &oldFog);
	pDevice->GetRenderState(D3DRS_CULLMODE, &oldCullMode);
	pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &oldAlphaBlend);
	pDevice->GetRenderState(D3DRS_SRCBLEND, &oldSrcBlend);
	pDevice->GetRenderState(D3DRS_DESTBLEND, &oldDestBlend);
	pDevice->GetRenderState(D3DRS_FILLMODE, &oldFillMode);
	pDevice->GetRenderState(D3DRS_SHADEMODE, &oldShadeMode);
	pDevice->GetTextureStageState(0, D3DTSS_CONSTANT, &dwCONSTANT);
	pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg1);
	pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwARG2);
	pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
	pDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOp);
	pDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);

	for (int i = 0; i < 8; i++)
		pDevice->SetTexture(i, NULL);

	int chamsType = element->esp.chamsType;

	// Tek renk
	ImVec4 chamsColor = element->esp.chamsVisibleColor;
	DWORD chamsD3D = D3DCOLOR_ARGB(
		(int)(chamsColor.w * 255),
		(int)(chamsColor.x * 255),
		(int)(chamsColor.y * 255),
		(int)(chamsColor.z * 255)
	);

	D3DXMATRIX oldWorld;
	pDevice->GetTransform(D3DTS_WORLD, &oldWorld);

	// Animasyon timer
	static float sparkleTime = 0.0f;
	sparkleTime += 0.05f;
	if (sparkleTime > 360.0f) sparkleTime = 0.0f;

	// ========== PASS 1: Duvar Arkası ==========

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	if (chamsType == 0) { // CRYSTAL - Smooth, cam gibi
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, chamsD3D);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);

		pPed->Render();
	}
	else if (chamsType == 1) { // GLOW - İnce parlama
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, chamsD3D);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);

		// Ana model
		pPed->Render();

		// Glow katmanları - Additive
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		for (int i = 0; i < 8; ++i)
		{
			float scaleAmount = 1.0f + (i + 1) * 0.005f; // Çok ince
			BYTE alpha = 180 - (i * 18);

			DWORD glowColor = D3DCOLOR_ARGB(
				alpha,
				(int)(chamsColor.x * 255),
				(int)(chamsColor.y * 255),
				(int)(chamsColor.z * 255)
			);

			D3DXMATRIX scaleM, newWorld;
			D3DXMatrixScaling(&scaleM, scaleAmount, scaleAmount, scaleAmount);
			newWorld = scaleM * oldWorld;
			pDevice->SetTransform(D3DTS_WORLD, &newWorld);

			pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, glowColor);
			pPed->Render();
		}

		pDevice->SetTransform(D3DTS_WORLD, &oldWorld);
	}
	else if (chamsType == 2) { // SPARKLE - Dönen parıltılar
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, chamsD3D);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);

		// Ana model
		pPed->Render();

		// Parıltılar - Additive, döner, yanıp söner
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		for (int i = 0; i < 6; ++i)
		{
			float angle = sparkleTime * 5.0f + (i * 60.0f); // Hızlı döner
			float pulse = (sin(sparkleTime * 4.0f + i) + 1.0f) * 0.5f; // Yanıp söner
			float sparkleScale = 1.04f + pulse * 0.04f; // 1.04-1.08 arası
			BYTE alpha = (BYTE)(100 + pulse * 140); // 100-240 arası

			D3DXMATRIX rotM, scaleM, newWorld;
			D3DXMatrixRotationY(&rotM, D3DXToRadian(angle));
			D3DXMatrixScaling(&scaleM, sparkleScale, sparkleScale, sparkleScale);
			newWorld = scaleM * rotM * oldWorld;
			pDevice->SetTransform(D3DTS_WORLD, &newWorld);

			// Çok parlak renk
			DWORD sparkleColor = D3DCOLOR_ARGB(
				alpha,
				std::min(255, (int)(chamsColor.x * 255 * 1.8f)),
				std::min(255, (int)(chamsColor.y * 255 * 1.8f)),
				std::min(255, (int)(chamsColor.z * 255 * 1.8f))
			);

			pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, sparkleColor);
			pPed->Render();
		}

		pDevice->SetTransform(D3DTS_WORLD, &oldWorld);
	}

	// ========== PASS 2: Görünür Kısım ==========
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (chamsType == 0) { // CRYSTAL
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, chamsD3D);
		pPed->Render();
	}
	else if (chamsType == 1) { // GLOW
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, chamsD3D);
		pPed->Render();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		for (int i = 0; i < 8; ++i)
		{
			float scaleAmount = 1.0f + (i + 1) * 0.005f;
			BYTE alpha = 180 - (i * 18);

			DWORD glowColor = D3DCOLOR_ARGB(
				alpha,
				(int)(chamsColor.x * 255),
				(int)(chamsColor.y * 255),
				(int)(chamsColor.z * 255)
			);

			D3DXMATRIX scaleM, newWorld;
			D3DXMatrixScaling(&scaleM, scaleAmount, scaleAmount, scaleAmount);
			newWorld = scaleM * oldWorld;
			pDevice->SetTransform(D3DTS_WORLD, &newWorld);

			pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, glowColor);
			pPed->Render();
		}

		pDevice->SetTransform(D3DTS_WORLD, &oldWorld);
	}
	else if (chamsType == 2) { // SPARKLE
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, chamsD3D);
		pPed->Render();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		for (int i = 0; i < 6; ++i)
		{
			float angle = sparkleTime * 5.0f + (i * 60.0f);
			float pulse = (sin(sparkleTime * 4.0f + i) + 1.0f) * 0.5f;
			float sparkleScale = 1.04f + pulse * 0.04f;
			BYTE alpha = (BYTE)(100 + pulse * 140);

			D3DXMATRIX rotM, scaleM, newWorld;
			D3DXMatrixRotationY(&rotM, D3DXToRadian(angle));
			D3DXMatrixScaling(&scaleM, sparkleScale, sparkleScale, sparkleScale);
			newWorld = scaleM * rotM * oldWorld;
			pDevice->SetTransform(D3DTS_WORLD, &newWorld);

			DWORD sparkleColor = D3DCOLOR_ARGB(
				alpha,
				std::min(255, (int)(chamsColor.x * 255 * 1.8f)),
				std::min(255, (int)(chamsColor.y * 255 * 1.8f)),
				std::min(255, (int)(chamsColor.z * 255 * 1.8f))
			);

			pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, sparkleColor);
			pPed->Render();
		}

		pDevice->SetTransform(D3DTS_WORLD, &oldWorld);
	}

	pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	// Restore
	pFlags->bIsVisible = oldVisible;
	pDevice->SetRenderState(D3DRS_ZENABLE, oldZEnable);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, oldZWrite);
	pDevice->SetRenderState(D3DRS_ZFUNC, oldZFunc);
	pDevice->SetRenderState(D3DRS_LIGHTING, oldLighting);
	pDevice->SetRenderState(D3DRS_FOGENABLE, oldFog);
	pDevice->SetRenderState(D3DRS_CULLMODE, oldCullMode);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, oldAlphaBlend);
	pDevice->SetRenderState(D3DRS_SRCBLEND, oldSrcBlend);
	pDevice->SetRenderState(D3DRS_DESTBLEND, oldDestBlend);
	pDevice->SetRenderState(D3DRS_FILLMODE, oldFillMode);
	pDevice->SetRenderState(D3DRS_SHADEMODE, oldShadeMode);
	pDevice->SetTextureStageState(0, D3DTSS_CONSTANT, dwCONSTANT);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwARG2);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOp);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg1);

	pGraphics->LeavingMTARenderZone();
}

void draw_player_box(CClientPlayer* player)
{
	if (!player)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local || !entity->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	CVector vecMin, vecMax;
	entity->GetPosition(vecMin);
	vecMin.fZ += 1.0f;
	entity->GetPosition(vecMax);
	vecMax.fZ -= 1.0f;

	CVector screenTopLeft, screenBottomRight;
	if (!world_to_screen(vecMin, screenTopLeft, 0.0f, false) || !world_to_screen(vecMax, screenBottomRight, 0.0f, false))
		return;

	// Görünürlük kontrolü
	bool bVisible = false;
	CVector vecFrom, vecTo;
	if (local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecFrom) &&
		entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecTo))
	{
		bVisible = is_visible(vecFrom, vecTo, entity);
	}

	// Renk belirleme
	ImU32 boxColor = IM_COL32(255, 255, 255, 255); // default beyaz

	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		boxColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		boxColor = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.box_color)
			: IM_COL32(255, 0, 0, 255); // görünmüyorsa kırmızı
	}

	float height = screenBottomRight.fY - screenTopLeft.fY;
	float width = height / 2.0f;
	float left = screenTopLeft.fX - width / 2;
	float right = screenTopLeft.fX + width / 2;

	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	if (element->esp.filled_box)
	{
		draw->AddRectFilled(ImVec2(left, screenTopLeft.fY), ImVec2(right, screenBottomRight.fY), boxColor, 1.0f);
	}

	draw->AddRect(ImVec2(left, screenTopLeft.fY), ImVec2(right, screenBottomRight.fY), boxColor, 1.0f, 0, 0.8f);
}

void draw_player_corner_box(CClientPlayer* player)
{
	if (!player)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local || !entity->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	CVector vecMin, vecMax;
	entity->GetPosition(vecMin);
	vecMin.fZ += 1.0f;
	entity->GetPosition(vecMax);
	vecMax.fZ -= 1.0f;

	CVector screenTopLeft, screenBottomRight;
	if (!world_to_screen(vecMin, screenTopLeft, 0.0f, false) || !world_to_screen(vecMax, screenBottomRight, 0.0f, false))
		return;

	// Görünürlük kontrolü
	bool bVisible = false;
	CVector vecFrom, vecTo;
	if (local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecFrom) &&
		entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecTo))
	{
		bVisible = is_visible(vecFrom, vecTo, entity);
	}

	// Renk belirleme - silent aim target kontrolü
	ImU32 boxColor;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		boxColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		boxColor = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.box_color)
			: IM_COL32(255, 0, 0, 255); // görünmüyorsa kırmızı
	}

	float height = screenBottomRight.fY - screenTopLeft.fY;
	float width = height / 2.0f;
	float left = screenTopLeft.fX - width / 2;
	float top = screenTopLeft.fY;
	float right = screenTopLeft.fX + width / 2;
	float bottom = screenBottomRight.fY;

	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	float minSide = std::min(width, height);
	float lineLen = minSide * 0.16f;
	float thickness = 1.60f;

	// sol üst
	draw->AddLine(ImVec2(left, top), ImVec2(left + lineLen, top), boxColor, thickness);
	draw->AddLine(ImVec2(left, top), ImVec2(left, top + lineLen), boxColor, thickness);

	// sağ üst
	draw->AddLine(ImVec2(right, top), ImVec2(right - lineLen, top), boxColor, thickness);
	draw->AddLine(ImVec2(right, top), ImVec2(right, top + lineLen), boxColor, thickness);

	// sol alt
	draw->AddLine(ImVec2(left, bottom), ImVec2(left + lineLen, bottom), boxColor, thickness);
	draw->AddLine(ImVec2(left, bottom), ImVec2(left, bottom - lineLen), boxColor, thickness);

	// sağ alt
	draw->AddLine(ImVec2(right, bottom), ImVec2(right - lineLen, bottom), boxColor, thickness);
	draw->AddLine(ImVec2(right, bottom), ImVec2(right, bottom - lineLen), boxColor, thickness);
}


void draw_player_bone(CClientPlayer* player)
{
	if (!player)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local || !player->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	// Kafa kemiği pozisyonları
	CVector vecHeadLocal, vecHeadTarget;
	if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadTarget))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadTarget, entity);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	// Renk belirleme - silent aim target kontrolü
	ImU32 color;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		color = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		color = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.bone_color)
			: IM_COL32(255, 0, 0, 255); // görünmüyorsa kırmızı
	}

	std::vector<std::pair<eBone, eBone>> bones = {
		{ BONE_NECK, BONE_UPPERTORSO },
		{ BONE_UPPERTORSO, BONE_SPINE1 },
		{ BONE_SPINE1, BONE_PELVIS },
		{ BONE_UPPERTORSO, BONE_RIGHTSHOULDER },
		{ BONE_RIGHTSHOULDER, BONE_RIGHTELBOW },
		{ BONE_RIGHTELBOW, BONE_RIGHTWRIST },
		{ BONE_UPPERTORSO, BONE_LEFTSHOULDER },
		{ BONE_LEFTSHOULDER, BONE_LEFTELBOW },
		{ BONE_LEFTELBOW, BONE_LEFTWRIST },
		{ BONE_PELVIS, BONE_RIGHTKNEE },
		{ BONE_RIGHTKNEE, BONE_RIGHTANKLE },
		{ BONE_PELVIS, BONE_LEFTKNEE },
		{ BONE_LEFTKNEE, BONE_LEFTANKLE }
	};

	for (const auto& bone_pair : bones)
	{
		CVector vecStart, vecEnd;
		if (!entity->GetGamePlayer()->GetBonePosition(bone_pair.first, &vecStart))
			continue;
		if (!entity->GetGamePlayer()->GetBonePosition(bone_pair.second, &vecEnd))
			continue;

		CVector screenStart, screenEnd;
		if (!world_to_screen(vecStart, screenStart, 0.0f, false))
			continue;
		if (!world_to_screen(vecEnd, screenEnd, 0.0f, false))
			continue;

		drawList->AddLine(
			ImVec2(screenStart.fX, screenStart.fY),
			ImVec2(screenEnd.fX, screenEnd.fY),
			color,
			1.40f
		);
	}
}
float GetPlayerHealth(CClientPed* player) {
	if (player) {
		float* playerHealthPtr = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(player) + 0x318);
		return *playerHealthPtr;
	}
	return 0.0f;
}


float GetArmorFromPed(CClientPed* player) {
	if (player) {
		float* playerArmorPtr = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(player) + 0x31C);
		return *playerArmorPtr;
	}
	return 0.0f;
}
void draw_player_health_armor(CClientPlayer* player)
{
	if (!player)
		return;

	CClientEntity* entity = static_cast<CClientEntity*>(player);
	CClientEntity* local = static_cast<CClientEntity*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);
	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	// Distance'a göre boyut skalası
	float distanceScale = 1.0f - (distance / element->esp.max_distance);
	distanceScale = std::max(distanceScale, 0.3f); // Minimum %30 boyut

	float health = std::min(GetPlayerHealth(player), 100.0f);
	float armor = std::min(GetArmorFromPed(player), 100.0f);
	float hpPercentage = health / 100.0f;
	float armorPercentage = armor / 100.0f;

	CVector vecMin, vecMax;
	entity->GetPosition(vecMin);
	vecMin.fZ += 1.0f;
	entity->GetPosition(vecMax);
	vecMax.fZ -= 1.0f;

	CVector screenTopLeft, screenBottomRight;
	if (!world_to_screen(vecMin, screenTopLeft, 0.0f, false) ||
		!world_to_screen(vecMax, screenBottomRight, 0.0f, false))
		return;

	float height = screenBottomRight.fY - screenTopLeft.fY;
	float boxWidth = height / 2.0f;

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	// Distance'a göre ölçeklenen tasarım parametreleri
	float barWidth = 8.0f * distanceScale;           // Distance'a göre kalınlık
	float barHeight = (height * 0.85f) * distanceScale; // Distance'a göre uzunluk
	float barOffset = 10.0f;         // Mesafe sabit
	float barSpacing = 6.0f;         // Barlar arası mesafe

	// Merkezi pozisyon hesaplama
	float centerX = screenTopLeft.fX;
	float centerY = screenTopLeft.fY + (height - barHeight) / 2.0f;

	// Health Bar (Sol taraf) - Kompakt Modern Tasarım
	if (element->esp.health)
	{
		float healthBarX = centerX - boxWidth / 2.0f - barOffset - barWidth;

		// Smooth health renk geçişi
		ImColor healthColor;
		if (hpPercentage > 0.75f) {
			healthColor = ImColor(39, 174, 96, 255);  // Yeşil
		}
		else if (hpPercentage > 0.5f) {
			healthColor = ImColor(230, 126, 34, 255); // Turuncu
		}
		else if (hpPercentage > 0.25f) {
			healthColor = ImColor(231, 76, 60, 255);  // Kırmızı
		}
		else {
			healthColor = ImColor(192, 57, 43, 255);  // Koyu kırmızı
		}

		float healthFilled = barHeight * hpPercentage;

		// Arka plan (daha belirgin)
		drawList->AddRectFilled(
			ImVec2(healthBarX, centerY),
			ImVec2(healthBarX + barWidth, centerY + barHeight),
			ImColor(25, 25, 25, 220),
			2.5f
		);

		// Health dolgusu
		if (healthFilled > 2.0f) // Minimum görünür miktar
		{
			drawList->AddRectFilled(
				ImVec2(healthBarX + 1.0f, centerY + barHeight - healthFilled),
				ImVec2(healthBarX + barWidth - 1.0f, centerY + barHeight - 1.0f),
				healthColor,
				2.0f
			);
		}

		// Çerçeve (daha belirgin)
		drawList->AddRect(
			ImVec2(healthBarX, centerY),
			ImVec2(healthBarX + barWidth, centerY + barHeight),
			ImColor(255, 255, 255, 130),
			2.5f,
			0,
			1.0f
		);
	}

	// Armor Bar (Sağ taraf) - Kompakt Modern Tasarım
	if (element->esp.armor && armorPercentage > 0.0f)
	{
		float armorBarX = centerX + boxWidth / 2.0f + barOffset;

		// Modern armor mavisi
		ImColor armorColor = ImColor(74, 144, 226, 255);
		float armorFilled = barHeight * armorPercentage;

		// Arka plan
		drawList->AddRectFilled(
			ImVec2(armorBarX, centerY),
			ImVec2(armorBarX + barWidth, centerY + barHeight),
			ImColor(25, 25, 25, 220),
			2.5f
		);

		if (armorFilled > 2.0f) // Minimum görünür miktar
		{
			drawList->AddRectFilled(
				ImVec2(armorBarX + 1.0f, centerY + barHeight - armorFilled),
				ImVec2(armorBarX + barWidth - 1.0f, centerY + barHeight - 1.0f),
				armorColor,
				2.0f
			);
		}

		// Çerçeve (daha belirgin)
		drawList->AddRect(
			ImVec2(armorBarX, centerY),
			ImVec2(armorBarX + barWidth, centerY + barHeight),
			ImColor(255, 255, 255, 130),
			2.5f,
			0,
			1.0f
		);
	}
}

void draw_player_line(CClientPlayer* player)
{
	if (!player)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local || !player->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecTargetPos;
	local->GetPosition(vecLocalPos);
	vecLocalPos.fZ -= 1.8f;  // Pozisyonu biraz aşağı alıyoruz

	entity->GetPosition(vecTargetPos);
	vecTargetPos.fZ -= 1.0f;

	// Mesafe kontrolü
	float distance = (vecLocalPos - vecTargetPos).Length();
	if (distance > element->esp.max_distance)
		return;

	// Ekran koordinatları
	CVector vecLocalScreen, vecTargetScreen;
	if (!world_to_screen(vecLocalPos, vecLocalScreen, 0.0f, false) ||
		!world_to_screen(vecTargetPos, vecTargetScreen, 0.0f, false))
		return;

	// Kafa kemiği pozisyonları ile görünürlük kontrolü
	CVector vecHeadLocal, vecHeadTarget;
	if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadTarget))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadTarget, entity);

	// Renk belirleme - silent aim target kontrolü
	ImU32 lineColor;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		lineColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		lineColor = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.line_color)
			: IM_COL32(255, 0, 0, 255);  // Görünmüyorsa kırmızı
	}

	ImGui::GetBackgroundDrawList()->AddLine(
		ImVec2(vecLocalScreen.fX, vecLocalScreen.fY),
		ImVec2(vecTargetScreen.fX, vecTargetScreen.fY),
		lineColor,
		1.9f
	);
}



void draw_random_player_line(CClientPlayer* player)
{
	static CClientPlayer* targetPlayer = nullptr;

	if (!targetPlayer || !targetPlayer->GetGamePlayer() || targetPlayer->GetOccupiedVehicle())
	{
		std::vector<CClientPlayer*> validPlayers;

		for (CClientPlayer* p : manager->get_players())
		{
			if (!p || p == manager->get_local_player())
				continue;

			if (!p->GetGamePlayer() || p->GetOccupiedVehicle())
				continue;

			CVector worldPos = *p->GetGamePlayer()->GetPosition();
			CVector screenPos;

			if (!world_to_screen(worldPos, screenPos, 0.0f, false))
				continue;

			float screenCenterX = ImGui::GetIO().DisplaySize.x / 2.0f;
			float screenCenterY = ImGui::GetIO().DisplaySize.y / 2.0f;

			float fovFraction = 0.5f;
			float maxXDiff = ImGui::GetIO().DisplaySize.x * fovFraction;
			float maxYDiff = ImGui::GetIO().DisplaySize.y * fovFraction;

			if (fabs(screenPos.fX - screenCenterX) > maxXDiff || fabs(screenPos.fY - screenCenterY) > maxYDiff)
				continue;

			validPlayers.push_back(p);
		}

		if (!validPlayers.empty())
		{
			int index = rand() % validPlayers.size();
			targetPlayer = validPlayers[index];
		}
	}

	if (!targetPlayer || !targetPlayer->GetGamePlayer() || targetPlayer->GetOccupiedVehicle())
		return;

	g_pLineTarget = targetPlayer;

	CClientEntity* entity = static_cast<CClientEntity*>(targetPlayer);
	CClientEntity* local = static_cast<CClientEntity*>(manager->get_local_player());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecTargetPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecTargetPos);

	CVector vecLocalScreen, vecTargetScreen;
	if (!world_to_screen(vecLocalPos, vecLocalScreen, 0.0f, false) ||
		!world_to_screen(vecTargetPos, vecTargetScreen, 0.0f, false))
	{
		g_pLineTarget = nullptr;
		targetPlayer = nullptr;
		return;
	}

	// Silent aim target kontrolü ve renk seçimi
	ImU32 lineColor;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == targetPlayer)
	{
		lineColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		lineColor = IM_COL32(
			static_cast<int>(element->esp.line_color.Value.x * 255.0f),
			static_cast<int>(element->esp.line_color.Value.y * 255.0f),
			static_cast<int>(element->esp.line_color.Value.z * 255.0f),
			255
		);
	}

	ImGui::GetBackgroundDrawList()->AddLine(
		ImVec2(vecLocalScreen.fX, vecLocalScreen.fY),
		ImVec2(vecTargetScreen.fX, vecTargetScreen.fY),
		lineColor,
		1.0f
	);
}


void draw_player_distance(CClientPlayer* player)
{
	if (!player || !element->esp.distance)
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local || !player->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	vecPos.fZ -= 1.0f; // draw_player_name ile aynı seviyede

	CVector vecScreen;
	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	ImVec2 namePos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImVec2 distancePos = ImVec2(namePos.x, namePos.y + 18.0f);

	char distanceText[32];
	snprintf(distanceText, sizeof(distanceText), "[%.1fm]", distance);

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);

	// Kafa kemiği pozisyonları ile görünürlük kontrolü
	CVector vecHeadLocal, vecHeadTarget;
	if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadTarget))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadTarget, entity);

	// Silent aim target kontrolü ve renk seçimi
	ImU32 textColor;
	if (element->esp.silent_color && aimbot->s_manual_silent_config.i_target_player == player)
	{
		textColor = ImGui::ColorConvertFloat4ToU32(element->esp_color.target_color);
	}
	else
	{
		textColor = bVisible
			? ImGui::ColorConvertFloat4ToU32(element->esp.distance_color)
			: IM_COL32(255, 0, 0, 255);  // Görünmüyorsa kırmızı
		
	}


	// Outline
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x + 1, distancePos.y), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x - 1, distancePos.y), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x, distancePos.y + 1), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(distancePos.x, distancePos.y - 1), outlineColor, distanceText);

	// Ana metin
	draw->AddText(font::Font, 16.0f, distancePos, textColor, distanceText);
}


void draw_player_overlay()
{
	if (GetAsyncKeyState(VK_F5) & 1)
	{
		visual->overlayEnabled = !visual->overlayEnabled;
	}
	if (!visual->overlayEnabled)
		return;

	CClientPed* local = manager->get_local_player();
	if (!local)
		return;

	CVector localPos;
	local->GetPosition(localPos);
	auto players = manager->get_players();
	if (players.empty()) return;

	// Modern stil ayarları
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 originalColors[ImGuiCol_COUNT];

	// Orijinal renkleri kaydet
	for (int i = 0; i < ImGuiCol_COUNT; i++)
		originalColors[i] = style.Colors[i];

	// Karanlık tema - siyah/gri/mavi tonları
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.96f);          // Saf karanlık arkaplan
	style.Colors[ImGuiCol_Border] = ImVec4(0.3f, 0.3f, 0.4f, 0.9f);               // Gri-mavi border
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);            // Koyu gri başlık
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);      // Aktif koyu gri
	style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.18f, 0.8f);             // Koyu gri buton
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.0f);      // Açık gri hover
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);       // Daha koyu aktif
	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);               // Açık gri metin
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.6f, 0.6f, 0.8f, 1.0f);             // Açık mavi checkmark

	// Border kalınlığı
	style.WindowBorderSize = 1.5f;
	style.WindowRounding = 8.0f;    // Köşe yuvarlatma

	// Her oyuncu için ayrı pencere
	for (CClientPlayer* player : players)
	{
		if (!player || player == local) continue;

		CClientPed* entity = static_cast<CClientPed*>(player);
		if (!entity) continue;

		CVector targetPos;
		entity->GetPosition(targetPos);
		float distance = (localPos - targetPos).Length();

		if (distance > element->esp.max_distance) continue;

		targetPos.fZ += 1.5f;
		CVector screenPos;
		if (!world_to_screen(targetPos, screenPos, 0.0f, false)) continue;
		if (screenPos.fZ < 1.0f) continue;

		const char* name = player->GetNametagText();
		if (!name) continue;

		char windowName[64];
		snprintf(windowName, sizeof(windowName), "player_%p", (void*)player);

		ImGui::SetNextWindowPos(ImVec2(screenPos.fX - 90, screenPos.fY - 55));
		ImGui::SetNextWindowSize(ImVec2(181, 111));

		// Stil ayarları
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 6));

		// Ana pencere
		if (ImGui::Begin(name, nullptr,  // Oyuncu ismini başlık olarak kullan
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse))
		{
			// Health bilgisi
			float health = GetPlayerHealth(static_cast<CClientPed*>(player));
			ImVec4 healthColor;
			if (health > 75.0f)
				healthColor = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);      // Açık yeşil (sağlıklı)
			else if (health > 50.0f)
				healthColor = ImVec4(0.8f, 0.8f, 0.4f, 1.0f);      // Sarı (orta)
			else if (health > 25.0f)
				healthColor = ImVec4(0.8f, 0.5f, 0.2f, 1.0f);      // Turuncu (düşük)
			else
				healthColor = ImVec4(0.8f, 0.3f, 0.3f, 1.0f);      // Açık kırmızı (kritik)

			ImGui::TextColored(healthColor, "Health: %.0f%%", health);

			// Mesafe bilgisi (renkli)
			ImVec4 distanceColor;
			if (distance < 50.0f)
				distanceColor = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);      // Açık yeşil (yakın)
			else if (distance < 100.0f)
				distanceColor = ImVec4(0.8f, 0.8f, 0.4f, 1.0f);      // Sarı (orta)
			else
				distanceColor = ImVec4(0.8f, 0.3f, 0.3f, 1.0f);      // Açık kırmızı (uzak)

			ImGui::TextColored(distanceColor, "Distance: %.1fm", distance);

			ImGui::Spacing();

			// Checkboxlar
			static bool vehicleTroll = false;
			static bool spectate = false;
			static bool firePlayer = false;

			ImGui::Checkbox2("Vehicle Troll", &vehicleTroll);
			ImGui::Checkbox2("Spectate", &spectate);
			ImGui::Checkbox2("Fire Player", &firePlayer);

			ImGui::Spacing();

			// Teleport butonu (tam genişlik)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.7f, 0.8f));          // Mavi
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));   // Açık mavi
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.3f, 0.6f, 1.0f));    // Koyu mavi

			if (ImGui::Button("Teleport", ImVec2(-1, 25))) // -1 = tam genişlik
			{
				manager->teleport_to_player(static_cast<CClientEntity*>(player));
			}

			ImGui::PopStyleColor(3);
		}

		ImGui::End();
		ImGui::PopStyleVar(2); // 3 tane StyleVar var artık
	}

	// Orijinal stilleri geri yükle
	for (int i = 0; i < ImGuiCol_COUNT; i++)
		style.Colors[i] = originalColors[i];

	// Orijinal değerleri geri yükle
	style.WindowBorderSize = 1.0f;
	style.WindowRounding = 0.0f;
}

void draw_player_3d_box(CClientPlayer* player)
{
	if (!player || !element->esp.box_3d)
		return;

	CClientEntity* entity = static_cast<CClientEntity*>(player);
	CClientEntity* local = static_cast<CClientEntity*>(global->m_pPlayerManager->GetLocalPlayer());
	if (!entity || !local)
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);  // vecTargetPos yerine vecPos kullanıyoruz

	float distance = sqrt(
		pow(vecPos.fX - vecLocalPos.fX, 2) +
		pow(vecPos.fY - vecLocalPos.fY, 2) +
		pow(vecPos.fZ - vecLocalPos.fZ, 2)
	);

	if (distance > element->esp.max_distance)
		return;

	// 3D box çizimi
	float width = 0.5f;
	float height = 1.8f;

	CVector points[8];
	points[0] = CVector(vecPos.fX - width, vecPos.fY - width, vecPos.fZ - height / 2);
	points[1] = CVector(vecPos.fX + width, vecPos.fY - width, vecPos.fZ - height / 2);
	points[2] = CVector(vecPos.fX + width, vecPos.fY + width, vecPos.fZ - height / 2);
	points[3] = CVector(vecPos.fX - width, vecPos.fY + width, vecPos.fZ - height / 2);
	points[4] = CVector(vecPos.fX - width, vecPos.fY - width, vecPos.fZ + height / 2);
	points[5] = CVector(vecPos.fX + width, vecPos.fY - width, vecPos.fZ + height / 2);
	points[6] = CVector(vecPos.fX + width, vecPos.fY + width, vecPos.fZ + height / 2);
	points[7] = CVector(vecPos.fX - width, vecPos.fY + width, vecPos.fZ + height / 2);

	CVector screen[8];
	for (int i = 0; i < 8; i++) {
		if (!world_to_screen(points[i], screen[i], 0.0f, false))
			return;
	}

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	// Filled box için tüm yüzeyleri doldur
	if (element->esp.filled_box)
	{
		// Alt yüzey
		drawList->AddQuadFilled(
			ImVec2(screen[0].fX, screen[0].fY),
			ImVec2(screen[1].fX, screen[1].fY),
			ImVec2(screen[2].fX, screen[2].fY),
			ImVec2(screen[3].fX, screen[3].fY),
			ImColor(element->esp.box_color.Value.x, element->esp.box_color.Value.y,
				element->esp.box_color.Value.z, element->esp.box_opacity * 0.3f)
		);

		// Üst yüzey
		drawList->AddQuadFilled(
			ImVec2(screen[4].fX, screen[4].fY),
			ImVec2(screen[5].fX, screen[5].fY),
			ImVec2(screen[6].fX, screen[6].fY),
			ImVec2(screen[7].fX, screen[7].fY),
			ImColor(element->esp.box_color.Value.x, element->esp.box_color.Value.y,
				element->esp.box_color.Value.z, element->esp.box_opacity * 0.3f)
		);

		// Ön yüzey
		drawList->AddQuadFilled(
			ImVec2(screen[0].fX, screen[0].fY),
			ImVec2(screen[1].fX, screen[1].fY),
			ImVec2(screen[5].fX, screen[5].fY),
			ImVec2(screen[4].fX, screen[4].fY),
			ImColor(element->esp.box_color.Value.x, element->esp.box_color.Value.y,
				element->esp.box_color.Value.z, element->esp.box_opacity * 0.3f)
		);

		// Arka yüzey
		drawList->AddQuadFilled(
			ImVec2(screen[2].fX, screen[2].fY),
			ImVec2(screen[3].fX, screen[3].fY),
			ImVec2(screen[7].fX, screen[7].fY),
			ImVec2(screen[6].fX, screen[6].fY),
			ImColor(element->esp.box_color.Value.x, element->esp.box_color.Value.y,
				element->esp.box_color.Value.z, element->esp.box_opacity * 0.3f)
		);

		// Sol yüzey
		drawList->AddQuadFilled(
			ImVec2(screen[0].fX, screen[0].fY),
			ImVec2(screen[3].fX, screen[3].fY),
			ImVec2(screen[7].fX, screen[7].fY),
			ImVec2(screen[4].fX, screen[4].fY),
			ImColor(element->esp.box_color.Value.x, element->esp.box_color.Value.y,
				element->esp.box_color.Value.z, element->esp.box_opacity * 0.3f)
		);

		// Sağ yüzey
		drawList->AddQuadFilled(
			ImVec2(screen[1].fX, screen[1].fY),
			ImVec2(screen[2].fX, screen[2].fY),
			ImVec2(screen[6].fX, screen[6].fY),
			ImVec2(screen[5].fX, screen[5].fY),
			ImColor(element->esp.box_color.Value.x, element->esp.box_color.Value.y,
				element->esp.box_color.Value.z, element->esp.box_opacity * 0.3f)
		);
	}

	// Kenarları çiz
	for (int i = 0; i < 4; i++) {
		drawList->AddLine(
			ImVec2(screen[i].fX, screen[i].fY),
			ImVec2(screen[(i + 1) % 4].fX, screen[(i + 1) % 4].fY),
			element->esp.box_color
		);
		drawList->AddLine(
			ImVec2(screen[i + 4].fX, screen[i + 4].fY),
			ImVec2(screen[((i + 1) % 4) + 4].fX, screen[((i + 1) % 4) + 4].fY),
			element->esp.box_color
		);
		drawList->AddLine(
			ImVec2(screen[i].fX, screen[i].fY),
			ImVec2(screen[i + 4].fX, screen[i + 4].fY),
			element->esp.box_color
		);
	}
}

namespace {
	bool is_in_distance(CClientPlayer* player) {
		if (!player)
			return false;

		CClientEntity* entity = static_cast<CClientEntity*>(player);
		CClientEntity* local = static_cast<CClientEntity*>(global->m_pPlayerManager->GetLocalPlayer());
		if (!entity || !local)
			return false;

		CVector vecLocalPos, vecTargetPos;
		local->GetPosition(vecLocalPos);
		entity->GetPosition(vecTargetPos);

		float distance = sqrt(
			pow(vecTargetPos.fX - vecLocalPos.fX, 2) +
			pow(vecTargetPos.fY - vecLocalPos.fY, 2) +
			pow(vecTargetPos.fZ - vecLocalPos.fZ, 2)
		);

		return distance <= element->esp.max_distance;
	}
}

void draw_player_lines(CClientPlayer* player)
{
	if (!player)
		return;
	if (!is_in_distance(player))
		return;

	CClientPed* entity = static_cast<CClientPed*>(player);
	CClientPed* local = static_cast<CClientPed*>(global->m_pPlayerManager->GetLocalPlayer());

	if (!entity || !local || !player->GetGamePlayer() || !local->GetGamePlayer())
		return;

	CVector vecLocalPos, vecPos;
	local->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	// Görünürlük kontrolü
	CVector vecHeadLocal, vecHeadTarget;
	if (!local->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadLocal) ||
		!entity->GetGamePlayer()->GetBonePosition(BONE_HEAD, &vecHeadTarget))
		return;

	bool bVisible = is_visible(vecHeadLocal, vecHeadTarget, entity);
	ImU32 lineColor = bVisible
		? ImGui::ColorConvertFloat4ToU32(element->esp.line_color.Value)
		: IM_COL32(255, 0, 0, 255);

	if (element->esp.line_bottom) {
		CVector vecTarget = vecPos;
		vecTarget.fZ -= 1.0f;
		CVector vecTargetScreen;

		// Çok yüksek tolerance ile çağır (ekran dışını da kabul eder)
		if (world_to_screen(vecTarget, vecTargetScreen, 10000.0f, false)) {
			float fResWidth = static_cast<float>(global->m_pCore->GetGraphics()->GetViewportWidth());
			float fResHeight = static_cast<float>(global->m_pCore->GetGraphics()->GetViewportHeight());

			ImVec2 screenBottom(fResWidth / 2.0f, fResHeight);

			ImGui::GetBackgroundDrawList()->AddLine(
				screenBottom,
				ImVec2(vecTargetScreen.fX, vecTargetScreen.fY),
				lineColor
			);
		}
	}

	// Üstten line
	if (element->esp.line_top) {
		CVector vecTarget = vecPos;
		vecTarget.fZ += 1.0f;
		CVector vecTargetScreen;

		// CalcScreenCoors'u direkt kullan
		global->m_pCore->GetGraphics()->CalcScreenCoors(&vecTarget, &vecTargetScreen);

		// Sadece kameranın arkasında değilse çiz
		if (vecTargetScreen.fZ > 0.1f) {
			float fResWidth = static_cast<float>(global->m_pCore->GetGraphics()->GetViewportWidth());
			float fResHeight = static_cast<float>(global->m_pCore->GetGraphics()->GetViewportHeight());

			// Ekranın üst ortasından başlat
			ImVec2 screenTop(fResWidth / 2.0f, 0.0f);

			ImGui::GetBackgroundDrawList()->AddLine(
				screenTop,
				ImVec2(vecTargetScreen.fX, vecTargetScreen.fY),
				lineColor
			);
		}
	}
}

void draw_vehicle_name(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_name)
		return;

	CClientEntity* entity = static_cast<CClientEntity*>(vehicle);
	CClientPlayer* local = manager->get_local_player();
	CClientEntity* local_entity = static_cast<CClientEntity*>(local);


	if (!entity || !local_entity)
		return;


	if (entity->GetDimension() != local_entity->GetDimension())
		return;

	char szName[64];
	snprintf(szName, sizeof(szName), "%s", vehicle->GetNamePointer());

	CVector vecPos;
	entity->GetPosition(vecPos);
	vecPos.fZ += 0.5f;  

	CVector vecScreen;
	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	ImU32 nameColor = ImGui::ColorConvertFloat4ToU32(element->esp.veh_name_color);
	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);

	// Outline (4 yön)
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, szName);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, szName);

	// Ana yazı
	draw->AddText(font::Font, 16.0f, pos, nameColor, szName);
}

void draw_vehicle_distance(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_distance)
		return;

	CClientEntity* entity = static_cast<CClientEntity*>(vehicle);
	CClientPlayer* local = manager->get_local_player();
	if (!entity || !local)
		return;

	CClientEntity* local_entity = static_cast<CClientEntity*>(local);

	// Dimension kontrolü
	if (entity->GetDimension() != local_entity->GetDimension())
		return;

	CVector vecLocalPos, vecPos;
	local_entity->GetPosition(vecLocalPos);
	entity->GetPosition(vecPos);

	float distance = (vecLocalPos - vecPos).Length();
	if (distance > element->esp.max_distance)
		return;

	vecPos.fZ += 0.2f;  

	CVector vecScreen;
	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	char distanceText[32];
	snprintf(distanceText, sizeof(distanceText), "[%.1fm]", distance);

	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
	ImU32 textColor = ImGui::ColorConvertFloat4ToU32(element->esp.veh_distance_color);

	// Outline (4 yönde)
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, distanceText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, distanceText);

	// Ana metin
	draw->AddText(font::Font, 16.0f, pos, textColor, distanceText);
}

void draw_vehicle_box(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_box)
		return;

	CClientEntity* entity = static_cast<CClientEntity*>(vehicle);
	CClientPlayer* local = manager->get_local_player();
	CClientEntity* local_entity = static_cast<CClientEntity*>(local);

	if (!entity || !local_entity)
		return;

	// Dimension kontrolü
	if (entity->GetDimension() != local_entity->GetDimension())
		return;

	CVector vecPos;
	entity->GetPosition(vecPos);

	// Aracın boyutlarını dinamik olarak hesapla (örnek değerler)
	float width = 2.0f;   // Genişlik
	float length = 4.0f;  // Uzunluk
	float height = 1.5f;  // Yükseklik

	// Box köşe noktaları (aracın merkezine göre hesapla)
	CVector vecMin = CVector(vecPos.fX - width / 2, vecPos.fY - length / 2, vecPos.fZ - height / 2);
	CVector vecMax = CVector(vecPos.fX + width / 2, vecPos.fY + length / 2, vecPos.fZ + height / 2);

	CVector screenMin, screenMax;
	if (!world_to_screen(vecMin, screenMin, 0.0f, false) || !world_to_screen(vecMax, screenMax, 0.0f, false))
		return;

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	// Box'ın içini doldur
	if (element->esp.veh_box_opacity > 0.0f)
	{
		ImU32 fillColor = ImColor(
			element->esp.veh_box_color.Value.x,
			element->esp.veh_box_color.Value.y,
			element->esp.veh_box_color.Value.z,
			element->esp.veh_box_opacity
		);

		drawList->AddRectFilled(
			ImVec2(screenMin.fX, screenMin.fY),
			ImVec2(screenMax.fX, screenMax.fY),
			fillColor
		);
	}

	// Box çerçevesi
	drawList->AddRect(
		ImVec2(screenMin.fX, screenMin.fY),
		ImVec2(screenMax.fX, screenMax.fY),
		element->esp.veh_box_color,
		0.0f,  // rounding
		15,    // flags
		1.0f   // thickness
	);
}

void draw_vehicle_line(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_line)
		return;

	CClientPlayer* local = manager->get_local_player();
	if (!local)
		return;

	CClientEntity* vehicle_entity = static_cast<CClientEntity*>(vehicle);
	CClientEntity* local_entity = static_cast<CClientEntity*>(local);
	if (!vehicle_entity || !local_entity)
		return;

	// Dimension farkı varsa çizme
	if (vehicle_entity->GetDimension() != local_entity->GetDimension())
		return;

	CVector vecLocalPos, vecTargetPos;
	local_entity->GetPosition(vecLocalPos);
	vehicle_entity->GetPosition(vecTargetPos);

	CVector vecLocalScreen, vecTargetScreen;
	if (!world_to_screen(vecLocalPos, vecLocalScreen, 0.0f, false) ||
		!world_to_screen(vecTargetPos, vecTargetScreen, 0.0f, false))
		return;

	ImGui::GetBackgroundDrawList()->AddLine(
		ImVec2(vecLocalScreen.fX, vecLocalScreen.fY),
		ImVec2(vecTargetScreen.fX, vecTargetScreen.fY),
		element->esp.veh_line_color,
		1.5f
	);
}

float GetVehicleHealth(CClientVehicle* vehicle) {
	if (!vehicle) return 0.0f;

	float* healthPtr = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(vehicle) + 0x540);  // Offset değiştirildi
	if (!healthPtr) return 0.0f;
	return *healthPtr / 10.0f;  // Health değerini 0-100 arasına normalize et
}


void draw_vehicle_engine_status(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_engine_status)
		return;

	CClientPlayer* local = manager->get_local_player();
	if (!local)
		return;

	CClientEntity* vehicle_entity = static_cast<CClientEntity*>(vehicle);
	CClientEntity* local_entity = static_cast<CClientEntity*>(local);
	if (!vehicle_entity || !local_entity)
		return;

	// Dimension kontrolü
	if (vehicle_entity->GetDimension() != local_entity->GetDimension())
		return;

	if (!vehicle->GetGameVehicle())
		return;

	bool engineOn = vehicle->GetGameVehicle()->IsEngineOn();

	CVector vecPos;
	vehicle_entity->GetPosition(vecPos);
	vecPos.fZ -= 0.02f; // 0.02 kadar aşağı kaydırır

	CVector vecScreen;
	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	const char* statusText = engineOn ? "[Engine: ON]" : "[Engine: OFF]";

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	ImU32 textColor = engineOn
		? ImGui::ColorConvertFloat4ToU32(element->esp.veh_engine_on_color)  // Motor açık rengi
		: ImGui::ColorConvertFloat4ToU32(element->esp.veh_engine_off_color); // Motor kapalı rengi

	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);

	// Outline
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, statusText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, statusText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, statusText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, statusText);

	// Asıl metin
	draw->AddText(font::Font, 16.0f, pos, textColor, statusText);
}


void draw_vehicle_health(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_health)
		return;

	CClientEntity* entity = static_cast<CClientEntity*>(vehicle);
	CClientPlayer* local = manager->get_local_player();
	if (!entity || !local)
		return;

	CClientEntity* vehicle_entity = static_cast<CClientEntity*>(vehicle);
	CClientEntity* local_entity = static_cast<CClientEntity*>(local);
	if (!vehicle_entity || !local_entity)
		return;

	// Dimension kontrolü
	if (vehicle_entity->GetDimension() != local_entity->GetDimension())
		return;

	float health = GetVehHealth(vehicle);

	CVector vecPos;
	entity->GetPosition(vecPos);

	vecPos.fZ -= 0.3f; // 0.02 kadar aşağı kaydırır

	CVector vecScreen;
	if (!world_to_screen(vecPos, vecScreen, 0.0f, false))
		return;

	char healthText[32];
	snprintf(healthText, sizeof(healthText), "[Health: %.0f]", health);

	ImVec2 pos = ImVec2(vecScreen.fX, vecScreen.fY);
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	ImU32 textColor = ImGui::ColorConvertFloat4ToU32(element->esp.veh_health_color);
	ImU32 outlineColor = IM_COL32(0, 0, 0, 255);

	// Outline (4 yönde)
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x + 1, pos.y), outlineColor, healthText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x - 1, pos.y), outlineColor, healthText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y + 1), outlineColor, healthText);
	draw->AddText(font::Font, 16.0f, ImVec2(pos.x, pos.y - 1), outlineColor, healthText);

	// Ana metin
	draw->AddText(font::Font, 16.0f, pos, textColor, healthText);
}


void draw_vehicle_3d_box(CClientVehicle* vehicle)
{
	if (!vehicle || !element->esp.veh_box_3d)
		return;

	CVector vecPos;
	vehicle->GetPosition(vecPos); // Aracın pozisyonunu al

	// Aracın boyutları (örnek değerler)
	float width = 2.0f;   // Genişlik
	float length = 4.0f;  // Uzunluk
	float height = 1.5f;  // Yükseklik

	// Rotasyon bilgisini al (CClientVehicle üzerinden)
	CVector vecRotation;

	// Dereceyi radyana çevir
	float radYaw = vecRotation.fZ * (M_PI / 180.0f);   // Yaw (Z ekseni etrafında dönme)
	float radPitch = vecRotation.fX * (M_PI / 180.0f); // Pitch (X ekseni etrafında dönme)
	float radRoll = vecRotation.fY * (M_PI / 180.0f);  // Roll (Y ekseni etrafında dönme)

	// Box'un köşe noktalarını hesapla
	CVector points[8];
	points[0] = CVector(-width / 2, -length / 2, -height / 2);
	points[1] = CVector(width / 2, -length / 2, -height / 2);
	points[2] = CVector(width / 2, length / 2, -height / 2);
	points[3] = CVector(-width / 2, length / 2, -height / 2);
	points[4] = CVector(-width / 2, -length / 2, height / 2);
	points[5] = CVector(width / 2, -length / 2, height / 2);
	points[6] = CVector(width / 2, length / 2, height / 2);
	points[7] = CVector(-width / 2, length / 2, height / 2);

	// Rotasyon uygula
	for (int i = 0; i < 8; i++) {
		CVector& point = points[i];

		// Yaw (Z ekseni etrafında dönme)
		float tempX = point.fX * cos(radYaw) - point.fY * sin(radYaw);
		float tempY = point.fX * sin(radYaw) + point.fY * cos(radYaw);
		point.fX = tempX;
		point.fY = tempY;

		// Pitch (X ekseni etrafında dönme)
		tempY = point.fY * cos(radPitch) - point.fZ * sin(radPitch);
		float tempZ = point.fY * sin(radPitch) + point.fZ * cos(radPitch);
		point.fY = tempY;
		point.fZ = tempZ;

		// Roll (Y ekseni etrafında dönme)
		tempX = point.fX * cos(radRoll) - point.fZ * sin(radRoll);
		tempZ = point.fX * sin(radRoll) + point.fZ * cos(radRoll);
		point.fX = tempX;
		point.fZ = tempZ;

		// Aracın pozisyonuna göre ayarla
		point.fX += vecPos.fX;
		point.fY += vecPos.fY;
		point.fZ += vecPos.fZ;
	}

	// Box'un ekran koordinatlarını hesapla
	CVector screen[8];
	bool allVisible = true;
	for (int i = 0; i < 8; i++) {
		if (!world_to_screen(points[i], screen[i], 0.0f, false)) {
			allVisible = false;
			break;
		}
	}

	if (allVisible) {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImU32 boxColor = element->esp.veh_box_color;  // Box rengi
		float thickness = 1.0f;

		// Alt çizgiler
		for (int i = 0; i < 4; i++) {
			drawList->AddLine(
				ImVec2(screen[i].fX, screen[i].fY),
				ImVec2(screen[(i + 1) % 4].fX, screen[(i + 1) % 4].fY),
				boxColor,
				thickness
			);
		}

		// Üst çizgiler
		for (int i = 0; i < 4; i++) {
			drawList->AddLine(
				ImVec2(screen[i + 4].fX, screen[i + 4].fY),
				ImVec2(screen[((i + 1) % 4) + 4].fX, screen[((i + 1) % 4) + 4].fY),
				boxColor,
				thickness
			);
		}

		// Dikey çizgiler
		for (int i = 0; i < 4; i++) {
			drawList->AddLine(
				ImVec2(screen[i].fX, screen[i].fY),
				ImVec2(screen[i + 4].fX, screen[i + 4].fY),
				boxColor,
				thickness
			);
		}
	}
}
void c_visual::draw_player_esp()
{
	if (!manager->is_loaded())
		return;

	std::vector<CClientPlayer*> players = manager->get_players();
	for (auto player : players)
	{
		if (!player)
			continue;

		// Local player kontrolü
		if (player->IsLocalPlayer() && !element->esp.localplayer)
			continue;

		if (element->esp.name)
			draw_player_name(player);

		if (element->esp.weapon)
			draw_player_weapon(player);

		if (element->esp.box)
			draw_player_box(player);


		if (element->esp.yok)
			draw_player_chams(player);

		if (element->esp.corner)
			draw_player_corner_box(player);
		
		if (element->esp.chams)
			draw_random_player_line(player);

		if (element->esp.box_3d)
			draw_player_3d_box(player);

		if (element->esp.bone)
			draw_player_bone(player);

		if (element->esp.line_bottom || element->esp.line_top)
			draw_player_lines(player);

		if (element->esp.health || element->esp.armor)
			draw_player_health_armor(player);

		if (element->esp.distance)
			draw_player_distance(player);
	}

	if (element->esp.troll)
		draw_player_overlay();

}

void c_visual::draw_vehicle_esp()
{
	if (!manager->is_loaded())
		return;

	if (!element->esp.veh_state)
		return;

	std::vector<CClientVehicle*> vehicles = manager->get_vehicles();
	for (auto vehicle : vehicles)
	{
		if (!vehicle)
			continue;

		if (element->esp.veh_name)
			draw_vehicle_name(vehicle);

		if (element->esp.veh_box)
			draw_vehicle_box(vehicle);

		if (element->esp.veh_box_3d)
			draw_vehicle_3d_box(vehicle);

		if (element->esp.veh_line)
			draw_vehicle_line(vehicle);

		if (element->esp.veh_health)
			draw_vehicle_health(vehicle);	
	
		if (element->esp.veh_engine_status)
			draw_vehicle_engine_status(vehicle);
		
		if (element->esp.veh_distance)
			draw_vehicle_distance(vehicle);


	}


}


void c_visual::draw_ped_esp()
{
	if (!manager->is_loaded())
		return;

	if (!element->esp.ped_state)
		return;

	std::vector<CClientPed*> ped = manager->get_peds();
	for (auto ped : ped)
	{
		if (!ped)
			continue;

		if (element->esp.ped_name)
			draw_ped_name(ped);		
		
		if (element->esp.ped_corner)
			draw_ped_corner_box(ped);		
		
		if (element->esp.ped_skeleton)
			draw_ped_skeleton(ped);		
		
		if (element->esp.ped_skeleton)
			draw_ped_distance(ped);		
		
		if (element->esp.ped_line)
			draw_ped_line(ped);


	}
}

void c_visual::draw_object_esp()
{
	if (!manager->is_loaded())
		return;

	if (!element->esp.object_state)
		return;

	std::vector<CClientObject*> object = manager->get_objects();
	for (auto object : object)
	{
		if (!object)
			continue;

		if (element->esp.object_distance)
			draw_object_distance(object);



	}
}


