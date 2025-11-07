#pragma once
#include "Windows.h"
struct coredll {
    DWORD m_pCore = 0x10521BA8;

    DWORD GetSerial = 0x44;
    DWORD GetNetwork = 0x20; // m_pCore + GetNetwork = CNet
    DWORD GetGame = 0xC; // m_pCore + GetGame = CNet
    DWORD GetMultiplayer = 0x1C; //  m_pCore + GetMultiplayer = g_pMultiplayer

} core_dll;


DWORD m_pTeamManager = 0x521BCC;
struct clientddl {
    //GameOffsets:
    DWORD m_pGame = 0x521BAC;
    struct g_Game
    {
        DWORD GetPlayerInfo = 0x04;
        DWORD GetProjectileInfo = 0x08;
        DWORD GetRadar = 0x0C;
        DWORD GetClock = 0x10;
        DWORD GetCheckpoints = 0x14;
        DWORD GetCoronas = 0x18;
        DWORD GetEventList = 0x1C;
        DWORD GetFireManager = 0x20;
        DWORD GetExplosionManager = 0x24;
        DWORD GetGarages = 0x28;
        DWORD GetHud = 0x2C;
        DWORD GetWeather = 0x30;
        DWORD GetWorld = 0x34;
        DWORD GetCamera = 0x38;
        DWORD GetPickups = 0x3C;
        DWORD Get3DMarkers = 0x40;
        DWORD GetPad = 0x44;
        DWORD GetAERadioTrackManager = 0x48;
        DWORD GetAudioEngine = 0x4C;
        DWORD GetAEAudioHardware = 0x50;
        DWORD GetAESoundManager = 0x54;
        DWORD GetAudioContainer = 0x58;
        DWORD GetStats = 0x5C;
        DWORD GetTasks = 0x60;
        DWORD GetSettings = 0x64;
        DWORD GetCarEnterExit = 0x68;
        DWORD GetControllerConfigManager = 0x6C;
        DWORD GetRenderWare = 0x70;
        DWORD GetHandlingManager = 0x74;
        DWORD GetAnimManager = 0x78;
        DWORD GetStreaming = 0x7C;
        DWORD GetVisibilityPlugins = 0x80;
        DWORD GetKeyGen = 0x84;
        DWORD GetRopes = 0x88;
        DWORD GetFx = 0x8C;
        DWORD GetFxManager = 0x90;
        DWORD GetWaterManager = 0x94;
        DWORD GetWeaponStatManager = 0x98;
        DWORD GetPointLights = 0x9C;
        DWORD GetCollisionStore = 0xA0;
        DWORD GetBuildingRemoval = 0xA4;
        DWORD GetRenderer = 0xA8;
        DWORD GetWeaponInfo = 0xAC;
        DWORD GetModelInfo = 0xB0;
        DWORD GetSystemTime = 0xB4;
        DWORD IsAtMenu = 0xB8;
        DWORD StartGame = 0xBC;
        DWORD SetSystemState = 0xC0;
        DWORD GetSystemState = 0xC4;
        DWORD Pause = 0xC8;
        DWORD SetTimeScale = 0xCC;
        DWORD GetFPS = 0xD0;
        DWORD GetTimeStep = 0xD4;
        DWORD GetOldTimeStep = 0xD8;
        DWORD GetTimeScale = 0xDC;
        DWORD Initialize = 0xE0;
        DWORD Reset = 0xE4;
        DWORD Terminate = 0xE8;
        DWORD InitLocalPlayer = 0xEC;
        DWORD GetGravity = 0xF0;
        DWORD SetGravity = 0xF4;
        DWORD GetGameSpeed = 0xF8;
        DWORD SetGameSpeed = 0xFC;
        DWORD GetMinuteDuration = 0x100;
        DWORD SetMinuteDuration = 0x104;
        DWORD GetBlurLevel = 0x108;
        DWORD SetBlurLevel = 0x10C;
        DWORD SetJetpackWeaponEnabled = 0x110;
        DWORD GetJetpackWeaponEnabled = 0x114;
        DWORD GetGameVersion = 0x118;
        DWORD IsCheatEnabled = 0x11C;
        DWORD SetCheatEnabled = 0x120;
        DWORD ResetCheats = 0x124;
        DWORD IsRandomFoliageEnabled = 0x128;
        DWORD SetRandomFoliageEnabled = 0x12C;
        DWORD IsMoonEasterEggEnabled = 0x130;
        DWORD SetMoonEasterEggEnabled = 0x134;
        DWORD IsExtraAirResistanceEnabled = 0x138;
        DWORD SetExtraAirResistanceEnabled = 0x13C;
        DWORD IsUnderWorldWarpEnabled = 0x140;
        DWORD SetUnderWorldWarpEnabled = 0x144;
        DWORD SetVehicleSunGlareEnabled = 0x148;
        DWORD IsVehicleSunGlareEnabled = 0x14C;
        DWORD SetCoronaZTestEnabled = 0x150;
        DWORD IsCoronaZTestEnabled = 0x154;
        DWORD IsWaterCreaturesEnabled = 0x158;
        DWORD SetWaterCreaturesEnabled = 0x15C;
        DWORD IsBurnFlippedCarsEnabled = 0x160;
        DWORD SetBurnFlippedCarsEnabled = 0x164;
        DWORD IsFireballDestructEnabled = 0x168;
        DWORD SetFireballDestructEnabled = 0x16C;
        DWORD IsExtendedWaterCannonsEnabled = 0x170;
        DWORD SetExtendedWaterCannonsEnabled = 0x174;
        DWORD  IsRoadSignsTextEnabled = 0x178;
        DWORD SetRoadSignsTextEnabled = 0x17C;
        DWORD  IsTunnelWeatherBlendEnabled = 0x180;
        DWORD SetTunnelWeatherBlendEnabled = 0x184;
        DWORD CreateWeapon = 0x188;
        DWORD CreateWeaponStat = 0x18C;
        DWORD VerifySADataFileNames = 0x190;
        DWORD PerformChecks = 0x194;
        DWORD  GetCheckStatus = 0x198;
        DWORD SetAsyncLoadingFromScript = 0x19C;
        DWORD SuspendASyncLoading = 0x1A0;
        DWORD IsASyncLoadingEnabled = 0x1A4;
        DWORD HasCreditScreenFadedOut = 0x1A8;
        DWORD FlushPendingRestreamIPL = 0x1AC;
        DWORD ResetModelLodDistances = 0x1B0;
        DWORD ResetModelFlags = 0x1B4;
        DWORD ResetAlphaTransparencies = 0x1B8;
        DWORD DisableVSync = 0x1BC;
        DWORD ResetModelTimes = 0x1C0;
        DWORD OnPedContextChange = 0x1C4;
        DWORD  GetPedContext = 0x1C8;
        DWORD GetShaderReplacementStats = 0x1CC;
        DWORD SetPreWeaponFireHandler = 0x1D0;
        DWORD SetPostWeaponFireHandler = 0x1D4;
        DWORD SetTaskSimpleBeHitHandler = 0x1D8;
        DWORD GetObjectGroupPhysicalProperties = 0x1DC;
        DWORD GetBaseIDforDFF = 0x1E0;
        DWORD  GetBaseIDforTXD = 0x1E4;
        DWORD GetBaseIDforCOL = 0x1E8;
        DWORD GetBaseIDforIPL = 0x1EC;
        DWORD GetBaseIDforDAT = 0x1F0;
        DWORD GetBaseIDforIFP = 0x1F4;
        DWORD GetBaseIDforRRR = 0x1F8;
        DWORD GetBaseIDforSCM = 0x1FC;
        DWORD GetCountOfAllFileIDs = 0x200;
        DWORD RemoveAllBuildings = 0x204;
        DWORD RestoreGameBuildings = 0x208;
        DWORD  SetBuildingPoolSize = 0x20C;

    };











    //CClientEntity:
    DWORD m_pEvent = 0x521BB8;
    DWORD m_usDimension = 0x64;

    //PlayerOffsets:
    DWORD PLAYER_NAME_OFF = 0x618;
    DWORD PLAYER_HEALTH_OFF = 0x318;
    DWORD PLAYER_ARMOR_OFF = 0x31C;

    DWORD m_shotSyncData = 0x1FC;
    DWORD m_fArmDirectionX = 0x18; // usar m_shotSyncData + m_fArmDirectionX para obter a pos certa.
    DWORD m_fArmDirectionY = 0x1C; // CVector *__thiscall CClientPed::GetAim(CClientPed *this, CVector *result)
    DWORD m_bBulletImpactData = 0x501; //É UM BOOL!!
    DWORD m_vecBulletImpactHit = 0x508; //CVECTOR
    DWORD m_fMoveSpeed = 0x328; // FLOAT
    DWORD m_bDead = 0x320; // bool
    DWORD m_bHasJetPack = 0x3CA; // bool
    DWORD  m_bIsOnFire = 0x4E8; // bool 
    DWORD  m_vecMoveSpeed = 0x360; // cvector
    DWORD m_bUsesCollision = 0x313; //bool

    struct CClientPed
    {

        DWORD m_bDontChangeRadio = 0x194;
        DWORD m_pRequester = 0x198;
        DWORD m_pPlayerPed = 0x19C;
        DWORD m_pTaskManager = 0x1A0;
        DWORD m_pPad = 0x1A4;
        DWORD m_bIsLocalPlayer = 0x1A8;
        DWORD m_pRespawnState = 0x1AC;
        DWORD m_ulModel = 0x1B0;
        DWORD m_matFrozen = 0x1B4;
        DWORD m_bRadioOn = 0x1E4;
        DWORD m_ucRadioChannel = 0x1E5;
        DWORD m_bHealthLocked = 0x1E6;
        DWORD m_bArmorLocked = 0x1E7;
        DWORD m_ulLastOnScreenTime = 0x1E8;
        DWORD m_pOccupiedVehicle = 0x1EC;
        DWORD m_pOccupyingVehicle = 0x1F0;
        DWORD m_uiOccupiedVehicleSeat = 0x1F4;
        DWORD m_bForceGettingIn = 0x1F8;
        DWORD m_bForceGettingOut = 0x1F9;
        DWORD m_shotSyncData = 0x1FC;
        DWORD m_stats = 0x200;
        DWORD m_currentControllerState = 0x204;
        DWORD m_lastControllerState = 0x208;
        DWORD m_rawControllerState = 0x20C;
        DWORD m_remoteDataStorage = 0x23C;
        DWORD m_ulLastTimeFired = 0x240;
        DWORD m_ulLastTimeBeganAiming = 0x244;
        DWORD m_ulLastTimeEndedAiming = 0x248;
        DWORD m_ulLastTimeBeganCrouch = 0x24C;
        DWORD m_ulLastTimeBeganStand = 0x250;
        DWORD m_ulLastTimeMovedWhileCrouched = 0x254;
        DWORD m_ulLastTimePressedLeftOrRight = 0x258;
        DWORD m_ulLastTimeUseGunCrouched = 0x25C;
        DWORD m_ulLastTimeSprintPressed = 0x260;
        DWORD m_ulBlockSprintReleaseTime = 0x264;
        DWORD m_bWasSprintButtonDown = 0x268;
        DWORD m_pLoadedModelInfo = 0x26C;
        DWORD m_pOutOfVehicleWeaponSlot = 0x270;
        DWORD m_fBeginAimX = 0x274;
        DWORD m_fBeginAimY = 0x278;
        DWORD m_fTargetAimX = 0x27C;
        DWORD m_fTargetAimY = 0x280;
        DWORD m_ulBeginAimTime = 0x284;
        DWORD m_ulTargetAimTime = 0x288;
        DWORD m_bTargetAkimboUp = 0x28C;
        DWORD m_ulBeginRotationTime = 0x290;
        DWORD m_ulEndRotationTime = 0x294;
        DWORD m_fBeginRotation = 0x298;
        DWORD m_fTargetRotationA = 0x29C;
        DWORD m_fBeginCameraRotation = 0x2A0;
        DWORD m_fTargetCameraRotation = 0x2A4;
        DWORD m_ulBeginTarget = 0x2A8;
        DWORD m_ulEndTarget = 0x2AC;
        DWORD m_vecBeginSource = 0x2B0; //CVector ?
        DWORD m_vecBeginTarget = 0x2BC;// CVector ?
        DWORD m_vecBeginTargetAngle = 0x2C8;// CVector ?
        DWORD m_vecTargetSource = 0x2D4; // CVector ?
        DWORD m_vecTargetTarget = 0x2E0; // CVector ?
        DWORD m_vecTargetTargetAngle = 0x2EC;  //CVector ?
        DWORD m_vecTargetInterpolateAngle = 0x2F8; // CVector ?
        DWORD m_pTargetedEntity = 0x304; // CClientEntityBasePtr<CClientEntity> ?
        DWORD m_SyncBuffer = 0x308;//   std::list<SDelayedSyncData*> ?
        DWORD m_bDucked = 0x310;
        DWORD m_bWasDucked = 0x311;
        DWORD m_bIsChoking = 0x312;
        DWORD m_bWearingGoggles = 0x313;
        DWORD m_bVisible = 0x314;
        DWORD m_bUsesCollision = 0x315;
        DWORD m_fHealth = 0x318;
        DWORD m_fArmor = 0x31C;
        DWORD m_bDead = 0x320;
        DWORD m_bWorldIgnored = 0x321;
        DWORD m_fCurrentRotation = 0x324;
        DWORD m_fMoveSpeed = 0x328;
        DWORD m_bCanBeKnockedOffBike = 0x32C;
        DWORD m_bBleeding = 0x32D;
        DWORD m_Matrix = 0x330;   //CMatrix ?
        DWORD m_vecMoveSpeed = 0x360; //CVector ?
        DWORD m_vecTurnSpeed = 0x36C;  //CVector ?
        DWORD m_CurrentWeaponSlot = 0x378;  // ; enum eWeaponSlot
        DWORD m_WeaponTypes = 0x37C;//SharedUtil::SFixedArray<enum eWeaponType, 13> ?
        DWORD m_usWeaponAmmo = 0x3B0;   //SharedUtil::SFixedArray<unsigned short, 13> ?
        DWORD m_bHasJetPack = 0x3CA;
        DWORD m_pClothes = 0x3CC;
        DWORD m_FightingStyle = 0x3D0;  //enum eFightingStyle
        DWORD m_MoveAnim = 0x3D4; //enum eMoveAnim
        DWORD m_Projectiles = 0x3D8; //   std::list<CClientProjectile*> ?
        DWORD m_ucAlpha = 0x3E0;
        DWORD m_fTargetRotation = 0x3E4;
        DWORD m_iVehicleInOutState = 0x3E8;
        DWORD m_bRecreatingModel = 0x3EC;
        DWORD m_pCurrentContactEntity = 0x3F0;
        DWORD m_bSunbathing = 0x3F4;
        DWORD m_Pad = 0x3F8;
        DWORD m_bDestroyingSatchels = 0x4A8;
        DWORD m_bDoingGangDriveby = 0x4A9;
        DWORD m_pAnimationBlock = 0x4AC;
        DWORD m_bRequestedAnimation = 0x4B0;
        DWORD m_AnimationCache = 0x4B4;
        DWORD m_bHeadless = 0x4D8;
        DWORD m_bFrozen = 0x4D9;
        DWORD m_bFrozenWaitingForGroundToLoad = 0x4DA;
        DWORD m_fGroundCheckTolerance = 0x4DC;
        DWORD m_fObjectsAroundTolerance = 0x4E0;
        DWORD m_iLoadAllModelsCounter = 0x4E4;
        DWORD m_bIsOnFire = 0x4E8;
        DWORD m_bIsInWater = 0x4E9;
        DWORD m_LastSyncedData = 0x4EC;
        DWORD m_bSpeechEnabled = 0x4F0;
        DWORD m_bStealthAiming = 0x4F1;
        DWORD m_fLighting = 0x4F4;
        DWORD m_ucEnteringDoor = 0x4F8;
        DWORD m_ucLeavingDoor = 0x4F9;
        DWORD m_bPendingRebuildPlayer = 0x4FA;
        DWORD m_uiFrameLastRebuildPlayer = 0x4FC;
        DWORD m_bIsSyncing = 0x500;
        DWORD m_bBulletImpactData = 0x501;
        DWORD m_pBulletImpactEntity = 0x504;
        DWORD m_vecBulletImpactHit = 0x508;
        DWORD m_interp = 0x514;
        DWORD m_ulStoredModel = 0x54C;
        DWORD m_RestoreWeaponList = 0x550;
        DWORD m_vecPrevTargetPosition = 0x558;
        DWORD m_uiForceLocalCounter = 0x564;
        DWORD m_bisNextAnimationCustom = 0x568;
        DWORD m_bisCurrentAnimationCustom = 0x569;
        DWORD m_strCustomIFPBlockName = 0x56C;
        DWORD m_strCustomIFPAnimationName = 0x584;
        DWORD m_u32CustomBlockNameHash = 0x59C;
        DWORD m_u32CustomAnimationNameHash = 0x5A0;
        DWORD m_pIFPAnimations = 0x5A4;
        DWORD m_pCustomAnimationIFP = 0x5A8;
        DWORD m_mapOfReplacedAnimations = 0x5B0;
        DWORD m_bTaskToBeRestoredOnAnimEnd = 0x5B8;
        DWORD m_eTaskTypeToBeRestoredOnAnimEnd = 0x5BC;
        DWORD m_bWarpInToVehicleRequired = 0x5C0;
        DWORD m_ulLastVehicleInOutTime = 0x5C4;
        DWORD m_bIsGettingOutOfVehicle = 0x5C8;
        DWORD m_bIsGettingIntoVehicle = 0x5C9;
        DWORD m_bIsJackingVehicle = 0x5CA;
        DWORD m_bIsGettingJacked = 0x5CB;
        DWORD m_VehicleInOutID = 0x5CC;
        DWORD m_ucVehicleInOutSeat = 0x5D0;
        DWORD m_bNoNewVehicleTask = 0x5D1;
        DWORD m_NoNewVehicleTaskReasonID = 0x5D4;
        DWORD m_pGettingJackedBy = 0x5D8;
        DWORD m_clientModel = 0x5DC;
    }cclientped_offs;









    ////////CClientVehicle:
    DWORD m_pVehicleManager = 0x10521BC0;

    DWORD m_bEngineOn = 0x252;
    DWORD m_vecGravity = 0x464; // ou 0x220 para setar
    DWORD m_fHealth = 0x240;
    struct CClientVehicle
    {
        DWORD m_pObjectManager = 0x184;
        DWORD m_pVehicleManager = 0x188;
        DWORD m_pModelRequester = 0x18C;
        DWORD m_usModel = 0x190;    //    dw ?
        DWORD m_bHasLandingGear = 0x192;
        DWORD m_eVehicleType = 0x194; //enum eClientVehicleType
        DWORD m_ucMaxPassengers = 0x198;
        DWORD m_bIsVirtualized = 0x199;
        DWORD m_pVehicle = 0x19C;
        DWORD m_pDriver = 0x1A0;//       CClientEntityBasePtr<CClientPed> ?
        DWORD m_pPassengers = 0x1A4;//   SharedUtil::SFixedArray<CClientPed*, 8> ?
        DWORD m_pOccupyingDriver = 0x1C4;  //CClientEntityBasePtr<CClientPed> ?
        DWORD m_pOccupyingPassengers = 0x1C8; // SharedUtil::SFixedArray<CClientPed*, 8> ?
        DWORD m_pClump = 0x1E8;
        DWORD m_usRemoveTimer = 0x1EC;//  dw ?
        DWORD m_pPreviousLink = 0x1F0;//  CClientEntityBasePtr<CClientVehicle> ?
        DWORD m_pNextLink = 0x1F4;//      CClientEntityBasePtr<CClientVehicle> ?
        DWORD m_Matrix = 0x1F8;//         CMatrix ?
        DWORD m_vecMoveSpeed = 0x228;//   CVector ?
        DWORD m_vecTurnSpeed = 0x234;//   CVector ?
        DWORD m_fHealth = 0x240;
        DWORD m_fTurretHorizontal = 0x244;
        DWORD m_fTurretVertical = 0x248;
        DWORD m_fGasPedal = 0x24C;
        DWORD m_bVisible = 0x250;
        DWORD m_bIsCollisionEnabled = 0x251;
        DWORD m_bEngineOn = 0x252;
        DWORD m_bEngineBroken = 0x253;
        DWORD m_bSireneOrAlarmActive = 0x254;
        DWORD m_bLandingGearDown = 0x255;
        DWORD m_bHasAdjustableProperty = 0x256;
        DWORD m_usAdjustablePropertyValue = 0x258;
        DWORD m_AutoReallowDoorRatioMap = 0x25C;
        DWORD m_bAllowDoorRatioSetting = 0x264;
        DWORD m_fDoorOpenRatio = 0x26C;
        DWORD m_doorInterp = 0x284;
        DWORD m_bSwingingDoorsAllowed = 0x2E4;
        DWORD m_bDoorsLocked = 0x2E5;
        DWORD m_bDoorsUndamageable = 0x2E6;
        DWORD m_bCanShootPetrolTank = 0x2E7;
        DWORD m_bCanBeTargettedByHeatSeekingMissiles = 0x2E8;
        DWORD m_bCanBeDamaged = 0x2E9;
        DWORD m_bScriptCanBeDamaged = 0x2EA;
        DWORD m_bSyncUnoccupiedDamage = 0x2EB;
        DWORD m_bTyresCanBurst = 0x2EC;
        DWORD m_ucDoorStates = 0x2ED;
        DWORD m_ucWheelStates = 0x2F3;
        DWORD m_ucPanelStates = 0x2F7;
        DWORD m_ucLightStates = 0x2FE;
        DWORD m_bJustBlewUp = 0x302;
        DWORD m_NormalStatus = 0x304; //enum eEntityStatus
        DWORD m_bColorSaved = 0x308;
        DWORD m_Color = 0x30C;
        DWORD m_bIsFrozen = 0x324;
        DWORD m_bScriptFrozen = 0x325;
        DWORD m_bFrozenWaitingForGroundToLoad = 0x326;
        DWORD m_fGroundCheckTolerance = 0x328;
        DWORD m_fObjectsAroundTolerance = 0x32C;
        DWORD m_vecWaitingForGroundSavedMoveSpeed = 0x330; // CVector ?
        DWORD m_vecWaitingForGroundSavedTurnSpeed = 0x33C;// CVector ?
        DWORD m_matFrozen = 0x348;   //CMatrix ?
        DWORD m_pUpgrades = 0x378;
        DWORD m_ucOverrideLights = 0x37C;
        DWORD m_pTowedVehicle = 0x380;
        DWORD m_pTowedByVehicle = 0x384;
        DWORD m_eWinchType = 0x388; //enum eWinchType
        DWORD m_pPickedUpWinchEntity = 0x38C;
        DWORD m_strRegPlate = 0x390;  //  SString ?
        DWORD m_ucPaintjob = 0x3A8;
        DWORD m_fDirtLevel = 0x3AC;
        DWORD m_bSmokeTrail = 0x3B0;
        DWORD m_ucAlpha = 0x3B1;
        DWORD m_bAlphaChanged = 0x3B2;
        DWORD m_dLastRotationTime = 0x3B8;
        DWORD m_blowAfterStreamIn = 0x3C0;
        DWORD m_bIsOnGround = 0x3C1;
        DWORD m_bHeliSearchLightVisible = 0x3C2;
        DWORD m_fHeliRotorSpeed = 0x3C4;
        DWORD m_fPlaneRotorSpeed = 0x3C8;
        DWORD m_pOriginalHandlingEntry = 0x3CC;
        DWORD m_pHandlingEntry = 0x3D0;
        DWORD m_pOriginalFlyingHandlingEntry = 0x3D4;
        DWORD m_pFlyingHandlingEntry = 0x3D8;
        DWORD m_pOriginalBoatHandlingEntry = 0x3DC;
        DWORD m_pBoatHandlingEntry = 0x3E0;
        DWORD m_pOriginalBikeHandlingEntry = 0x3E4;
        DWORD m_pBikeHandlingEntry = 0x3E8;
        DWORD m_fNitroLevel = 0x3EC;
        DWORD m_cNitroCount = 0x3F0;
        DWORD m_fWheelScale = 0x3F4;
        DWORD m_bChainEngine = 0x3F8;
        DWORD m_bIsDerailed = 0x3F9;
        DWORD m_bIsDerailable = 0x3FA;
        DWORD m_bTrainDirection = 0x3FB;
        DWORD m_fTrainSpeed = 0x3FC;
        DWORD m_fTrainPosition = 0x400;
        DWORD m_ucTrackID = 0x404;
        DWORD m_bJustStreamedIn = 0x405;
        DWORD m_bWheelScaleChanged = 0x406;
        DWORD m_interp = 0x408;
        DWORD m_ulIllegalTowBreakTime = 0x450;
        DWORD m_bHasDamageModel = 0x454;
        DWORD m_blowState = 0x455;// enum VehicleBlowState
        DWORD m_bTaxiLightOn = 0x456;
        DWORD m_Projectiles = 0x458;
        DWORD m_bIsInWater = 0x460;
        DWORD m_bNitroActivated = 0x461;
        DWORD m_vecGravity = 0x464;
        DWORD m_HeadLightColor = 0x470;
        DWORD m_bHasCustomHandling = 0x474;
        DWORD m_ucVariation = 0x475;
        DWORD m_ucVariation2 = 0x476;
        DWORD m_LastPushedTime = 0x478;
        DWORD m_uiForceLocalZCounter = 0x480;
        DWORD m_bEnableHeliBladeCollisions = 0x484;
        DWORD m_matCreate = 0x488;     // CMatrix ?
        DWORD m_ucFellThroughMapCount = 0x4B8;
        DWORD m_bWindowOpen = 0x4B9;
        DWORD m_clientModel = 0x4C0;
        DWORD m_LastSyncedData = 0x4C8;
        DWORD m_tSirenBeaconInfo = 0x4CC;
        DWORD m_ComponentData = 0x57C;
        DWORD m_bAsyncLoadingDisabled = 0x584;
        DWORD m_dummyPositions = 0x588;
        DWORD m_copyDummyPositions = 0x630;

    }cclient_vehicles;

}client_dll;
DWORD m_pPlayerManager = 0x525BBC;
DWORD g_mVehicleManager = 0x525BC0;