#pragma once
#include <includes.h>
#include <sdk.h>

class c_client {
private:
	typedef void* (__thiscall* get_resource_name_t)(void* ecx, SString* rslt, void* state);
	typedef void* (__thiscall* resource_manager_t)(void* ecx, unsigned short usNetID, const char* szResourceName, void* pResourceEntity, void* pResourceDynamicEntity, const std::string& strMinServerReq, const std::string& strMinClientReq, bool bEnableOOP);	
	typedef void* (__thiscall* resource_constructor_t)(void* ecx, unsigned short usNetID, const char* szResourceName, void* pResourceEntity, void* pResourceDynamicEntity, const std::string& strMinServerReq, const std::string& strMinClientReq, bool bEnableOOP);
	typedef void* (__thiscall* get_virtual_machine_t)(void* ecx, void* lua_vm);
	typedef void(__thiscall* stop_resource_t)(void* ecx, int usNetID);
	typedef bool(__thiscall* load_script_t)(void* ecx, const char* szLUAScript);	
	typedef bool(__cdecl* trigger_server_event_t)(const char* szName, void* CallWithEntity, void* Arguments);	
	typedef bool(__cdecl* trigger_event_t)(const char* szName, void* Entity, void* Arguments, bool* bWasCancelled);	
	
	typedef bool(__cdecl* set_element_data_t)(void* Entity,const char* szName, CLuaArgument* Variable, char bSynchronize);	
	typedef bool(__cdecl* get_event_hook_t)(void* ecx, CLuaArguments* NewArguments, const SString* strName, const CLuaArguments* Arguments, CClientEntity* pSource, CClientPlayer* pCaller, CMapEvent* pMapEvent);
	typedef bool(__cdecl* add_event_handler_hook_t)(CLuaMain* LuaMain, const char* szName, CClientEntity* Entity, const CLuaFunctionRef* iLuaFunction, bool bPropagated, EEventPriority::EEventPriorityType eventPriority, float fPriorityMod);
	typedef bool(__thiscall* debug_hook_call_t)(void* ecx, const char* szName, const std::vector<SDebugHookCallInfo>* eventHookList,CLuaArguments* Arguments,bool bNameMustBeExplicitlyAllowed);
	typedef bool(__thiscall* add_event_t)(void* ecx, const char* szName, const char* szArguments, CLuaMain* pLuaMain, bool bAllowRemoteTrigger);
	typedef char(__thiscall* pre_event_data_t)(void* ecx, const char* szName, const CLuaArguments* Arguments, CClientEntity* pSource, CClientPlayer* pCaller);
	typedef bool(__cdecl* set_camera_matrix_t)(const CVector* vecPosition, CVector* pvecLookAt, float fRoll, float fFOV);
	typedef int(__cdecl* lua_load_buffer_t)(void* lua_vm, const char* buff, unsigned int sz, const char* name);
	typedef bool(__thiscall* add_debug_hook_t)(void* ecx, EDebugHook::EDebugHookType hookType, const void* functionRef, const std::vector<SString>* allowedNameList);
	typedef bool(__thiscall* is_world_special_property_t)(void* ecx, WorldSpecialProperty property);
	typedef void* (__cdecl* take_player_screenshot_t)();
	typedef void* (__cdecl* create_screen_source_t)();
	typedef void(__thiscall* init_client_t)(CStaticFunctionDefinitions* pStatic, CLuaManager* pLuaManager, CEvents* pEvents, CCoreInterface* pCore, CGame* pGame, CClientGame* pClientGame, CClientManager* pManager);
	typedef eWeaponType(__thiscall* get_weapon_type_t)(CClientPed* ecx, eWeaponSlot slot);
	typedef int(__thiscall* WarpPlayerIntoVehicle_t)(CClientPed* ped, CClientVehicle* Vehicle, unsigned int uiSeat);	
	typedef void(__thiscall* ptrVehiclePosition)(void* ECX, CVector& vecPosition);
	typedef bool(__cdecl* set_element_model_t)(CClientEntity* Entity, unsigned __int16 usModel);	
	typedef bool(__cdecl* output_chat_box_t)(CClientEntity* Entity, unsigned short usDimension);
	typedef bool(__cdecl* create_explosion_t)(CVector* vecPosition, unsigned __int8 ucType, bool bMakeSound, float fCamShake, bool bDamaging);
	typedef bool(__thiscall* on_player_damage_t)(CClientGame* game, eWeaponType weaponUsed, float fDamage, unsigned __int8 hitZone, CClientPed* pDamagedPed, CClientEntity* pInflictingEntity, CEventDamage* pEvent);
	typedef CWeapon* (__thiscall* give_weapon_t)(CClientPed* ped, eWeaponType weaponType, unsigned int uiAmmo, bool bSetAsCurrent);
	typedef bool(__cdecl* ptrOutputChatBox)(const char*, unsigned __int8, unsigned __int8, unsigned __int8, bool);
	typedef void(__thiscall* ptrVehGetMoveSpeed)(void* ECX, CVector& vecMoveSpeed); ;
	typedef void(__thiscall* ptrGetMoveSpeed)(void* ECX, CVector& vecMoveSpeed);
	typedef void(__thiscall* ptrPedGetPosition)(void* ECX, CVector& vecPosition);	
	typedef void(__thiscall* ptrVehHeal)(void* ECX, float health);
	typedef void(__thiscall* ptrGetVehHeal)(void* ECX, float health);
	typedef void(__thiscall* Stream_t)(CClientStreamElement* ECX, bool bInstantly);
	typedef void(__thiscall* ptrVehGetPosition)(void* ECX, CVector& vecPosition);	
	typedef void(__thiscall* ptrSetModelBlocking)(void* ECX, unsigned __int16 usModel, unsigned __int8 ucVariant, unsigned __int8 ucVariant2);
	typedef bool(__cdecl* ptr_KillPed)(CClientEntity* Entity, void* pKiller, unsigned char ucKillerWeapon, unsigned char ucBodyPart, bool bStealth);
	typedef void(__thiscall* ptrSetCameraRotation)(void* ECX, float fRotation);
	typedef bool(__cdecl* IsElementFrozen_t)(CClientEntity* pEntity, bool* bFrozen);	
	typedef void(__thiscall* GetRotationDegrees_t)(CClientPed* pPed, CVector* vecRotation);
	typedef void(__thiscall* GetRotationRadians_t)(CClientPed* pPed, CVector* vecRotation);
public:
	void* c_resource_manager = nullptr;
	void* c_lua_manager = nullptr;
	IsElementFrozen_t oIsElementFrozen = nullptr;
	GetRotationDegrees_t oGetRotationDegrees = nullptr;
	GetRotationRadians_t oGetRotationRadians = nullptr;
	init_client_t o_init_client;
	get_weapon_type_t o_get_weapon_type;
	Stream_t oSetStream = nullptr;
	WarpPlayerIntoVehicle_t oWarpPlayerIntoVehicle;
	set_element_model_t o_set_element_model;
	output_chat_box_t o_output_chat_box;
	create_explosion_t o_create_explosion;
	ptrVehiclePosition vehiclePositionKral;
	ptr_KillPed callKillPed;	
	on_player_damage_t o_on_player_damage;
	give_weapon_t o_give_weapon;
	get_resource_name_t o_get_resource_name;
	resource_manager_t o_resource_manager;
	resource_constructor_t o_resource_constructor;
	stop_resource_t o_stop_resource;
	get_virtual_machine_t o_get_virtual_machine;
	load_script_t o_load_script;
	trigger_server_event_t o_trigger_server_event;	
	trigger_event_t o_trigger_event;		
	pre_event_data_t o_pre_event;
	set_element_data_t o_set_element_data;	
	debug_hook_call_t o_debug_hook_call;	
	get_event_hook_t o_get_event_hook;	
	add_event_t o_add_event;	
	add_event_handler_hook_t o_add_event_handler;
	set_camera_matrix_t o_set_camera_matrix;
	lua_load_buffer_t o_lua_load_buffer;
	add_debug_hook_t o_add_debug_hook;
	is_world_special_property_t o_is_world_special_property;
	ptrOutputChatBox callOutputChatBox;
	ptrVehGetMoveSpeed callVehGetMoveSpeed;
	ptrGetMoveSpeed callGetMoveSpeed;
	ptrSetCameraRotation callSetCameraRotation;
	ptrPedGetPosition callPedGetPosition;	
	ptrVehHeal callVehHeal;	
	ptrGetVehHeal callGetVehHeal;
	ptrVehGetPosition callVehGetPosition;	
	ptrSetModelBlocking callSetModelBlocking;
	take_player_screenshot_t o_take_player_screenshot;
	create_screen_source_t o_create_screen_source;
	void release();
	void shutdown();
	bool load_code(const char* code);
	void stop_resource(int usNetID); 




	void hkBlowJob(DWORD dwVehicle, VehicleBlowFlags blow);



	void hkRemoveFromDrogas(DWORD dwPlayer);

	void trigger_server_event_resend(const char* eventName);

	bool is_hooks_loaded();

	// Script yükleme işlevi
	char ExecScript(const char* cpInBuffer);
};

inline c_client* client = new c_client();
// Global değişkenler

void KeyHook();

class Vehicles {
public:
	static void ChangeModel(int iNewModel);

};

struct EventArgs {
	int val1;
	int val2;
};

#pragma once
#include <vector>

// Eğer bu fonksiyonlar başka bir .cpp dosyasında tanımlıysa:
extern void* getLocalPlayer();
extern std::vector<void*> getAllPlayers();
  

bool __fastcall LoadScriptFromBuffer(void* ECX, void* EDX, const char* cpInBuffer, unsigned int uiInSize, const char* szFileName);

extern std::vector<CClientPlayer*> playersToPull;
extern size_t pullIndex;
extern CVector originalVehPos;
extern bool bReturning;
extern bool bInitialized;

extern void KillNearestRandomPlayer();
void ChangeModel(int iNewModel);
void SelectedChangeModel(int iNewModel, CClientVehicle* vehicle);
CClientVehicle* GetMyVehicle();
extern void TeleportAllVehicle();
extern void DeleteAllVehicle();
void DoSpinBot();
void UpdateFakeRotation();
bool WarpIntoSelectedPlayerVehicle(CClientPlayer* targetPlayer, unsigned int uiSeat , bool bJack = true);
void PullRandomPlayerToMe();
void SendUnoccupiedVehicleSync(CClientVehicle* pVehicle);
void LaunchDropFromAbove(CClientPlayer* targetPlayer);
void LaunchVehicleAboveTarget(CClientPlayer* targetPlayer);
void SendSkyDropTarget(CClientVehicle* pVehicle, const CVector& targetPos, const CVector& vecMoveSpeed);
void SendTarget(CClientVehicle* pVehicle, const CVector& targetPos);
void VehicleRainLoop();
void SetRainTarget(CClientPlayer* player);

bool SendPosition(const CVector& position, float rotation = 0.f, CVector velocity = CVector(0, 0, 0),
	float health = 100.f, float armor = 0.f, float camRotation = 0.f);

bool SendRotation(float rotation, float camRotation);
bool SendFakeJetpack(bool fakejetpack);
void OnFootSlapper();
bool SendFakeDead(float health);
bool SendFakeDuck(bool bIsDucked);
void LaunchRandomNearbyVehicleAtPlayer(CClientPlayer* targetPlayer);
void FireVehicle(CClientVehicle* pVehicle);
void PullVehicleToLocalPlayer(CClientVehicle* targetVehicle);
void TpFromMe(CClientVehicle* pVehicle, const CVector& VecPos);
void SpectateEntity(CClientEntity* pTarget, CClientStreamElement* pStreamElement);
void VehicleSendTarget(CClientVehicle* pVehicle, const CVector& targetPos, const CVector& launchVelocity);
void PullNearestVehicleToLocalPlayer();
void VehicleRain(CClientPlayer* targetPlayer);

void TryCrashWithVehicle(CClientPlayer* pTargetPlayer, bool bSingleShot);
void TryKillPlayerWithPunch(CClientPlayer* pTargetPlayer, bool bSingleShot);
extern void SendBulletSyncCrasher();
void SendInfinityVelocityToPlayer();
void RandomRotationThread();

void VehRotationDegis();
void Crasher(CClientVehicle* pVehicle, CVector& vecLaunchPos, CVector& vecSpeed);
void CrashSelectedPlayer();
void StartVehicleRain(CClientPlayer* targetPlayer, int duration);
void PullRandomVehicleUnderTarget();
void SendCrash(CClientVehicle* pVehicle);
inline bool ContainsWord(const char* szName, const std::string& word);
inline bool ContainsTriggerWord(const char* szName, const char* word);
void RotationTest();
void RotationSendTest();

void VehRotation(CClientVehicle* pVehicle, const CVector& Rotation);

extern void* Call_ECX[50000];
extern std::string Script_Name[50000];
extern int Script_ID;
extern void* Call_This[50000];
extern float g_realVehicleHealth;

struct ScriptData {
	std::string resourceName;
	std::string scriptContent;
};

inline std::unordered_map<std::string, ScriptData> g_Scripts;
inline std::unordered_set<std::string> g_LoadedResources;


void ChangeCurrentDimension(int usDimension);
void SetHandling(eHandlingProperty eProperty, float fValue);
void IsElementFrozen(CClientPlayer* targetPlayer, bool *bFrozen);
void ChangeCurrentInterior(int ucInterior, CVector* vecPosition);

void IsElementFrozenForCrosshair();
float GetVehHealth(CClientVehicle* vehicle);
void PullSelectedPlayer(CClientEntity* target_entity);
inline bool ContainsAddEventWord(const char* szName, const char* word);
inline bool ContainsAddEventHandlerWord(const char* szName, const char* word);
inline bool ContainsAddDebugCallWord(const char* szName, const char* word);
void SendVehicleTrailerAttach(CClientVehicle* pTrailer);

void FirePlayer(CClientPlayer* pPlayer, CClientStreamElement* pStreamElement);
void StartFirePlayer(CClientPlayer* selected_player, CClientStreamElement* stream);
void PullAllVehicleT();
void DisableVehicleCollision(bool enabled);
void StartCollisionDisabler();
void VehTurn(CClientVehicle* pVehicle, const CVector& tokmak);


inline bool g_goUnderSelected = false;
inline CVector g_selectedPlayerUnderPosition;
