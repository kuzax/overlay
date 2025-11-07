#pragma once
#include <includes.h>
static float damageBoost = 1.0f; // Baþlangýç deðeri

struct s_resource_dump
{
    std::string script_name;
    std::vector<char> buffer;
    size_t size;
};

 

struct s_executor
{
    std::string resource_name;
    void* lua_vm;
    std::string script_content;
    bool operator==(const s_executor& other) const {
        return lua_vm == other.lua_vm;
    }
};

struct s_resource
{
    std::string resource_name;
    void* resource_ptr;
    unsigned short net_id;

    bool operator==(const s_resource& other) const
    {
        return resource_ptr == other.resource_ptr;
    }
};



struct s_events
{
    std::string event_name;       
    bool is_blocked = false;      
    std::string event_details;   
    std::string arguments; // yeni eklenen alan
    int trigger_count = 0;        // Kaç kere tetiklenmiþ

};

struct s_trigger_Events
{
    std::string event_name;
    bool is_blocked = false;
    std::string event_details;
    std::string arguments; // yeni eklenen alan
    int trigger_count = 0;        

};


struct s_add_event_handler
{
    std::string event_name;
    bool is_blocked = false;
    std::string event_details;
    std::string arguments; // yeni eklenen alan
    int trigger_count = 0;        

};


struct s_element_data
{
    std::string event_name;
    bool is_blocked = false;
    std::string event_details;
    std::string arguments; // yeni eklenen alan
    int trigger_count = 0;        

};

struct s_debug_call
{
    std::string event_name;
    bool is_blocked = false;
    std::string event_details;
    std::string arguments; // yeni eklenen alan
    int trigger_count = 0;        

};

struct s_add_event
{
    std::string event_name;
    bool is_blocked = false;
    std::string event_details;
    std::string event_arguments; // yeni eklenen alan
    int trigger_count = 0;        

};

struct s_debug_event
{
    std::string event_name;
    bool is_blocked = false;
    std::string event_details;
    std::string arguments; // yeni eklenen alan
    int trigger_count = 0;        

};

struct s_dumps
{
    std::string script_name;
    std::vector<char> buffer;
    size_t size;
    bool is_compiled;    
    bool is_blocked = false;
};


class c_elements {
public:
    struct
    {
        bool loaded_client{ false };        
        bool loaded_core{ false };
        bool debug_mode{ true };

        bool enabled_console{ false };
        bool injected_code{ false };
    } content;

    struct
    {
        int menu_bind{ 45 };
        bool is_waiting_for_key{ false };       
        bool bypass{ false };
    } binds;

    struct
    {
        std::string pre_injection;
        std::string post_injection;
    } script;


    struct
    {

        float LocalPlayerColor[4] = { 0.0f, 0.59f, 1.0f, 1.0f }; // Mavi
        float PlayerColor[4] = { 1.0f, 0.2f, 0.2f, 1.0f }; // Kýrmýzý
        float VehicleColor[4] = { 0.2f, 1.0f, 0.2f, 1.0f }; // Yeþil
        float NameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Beyaz

        bool EnableRadar{ false }; 
        bool RadarActive{ false }; 
        int RadarToggleKey{ 0 }; 
        int RadarHoldKey{ 0 }; 

        bool VehicleBlip{ false };
        bool PlayerBlip{ false };
        bool LocalPlayerBlip{ false };

        bool RenderRadar{ false };
        int MapPosition = 4; 
        float MapSize = 800.0f;
    } radar;


    struct {
        bool godmode{ false };        
        int godmodekey{ false };
        float customFOV{ 30.0 };
        float SlapDistance = 1.0f;
        bool invisible{ false };        
        bool heal{ false }; ;
        bool spectate{ false };
        bool antiinvisible{ false };           
        bool test{ false };          
        bool sex{ false };        
        bool slapper{ false };
        bool semimode{ false };        
        bool maprun{ false };
        bool airbreak{ false };
        bool jetpackEnabled{ false };
        bool sprint{ false };
        bool enableHealthKeybind = false;
        float airbreak_speed{ 1.0 };        
        float maprun_speed{ 1.0 };
        float sprint_speed{ 1.0 }; 
        bool fast_respawn{ false };
        bool spinbot{ false };        
        bool HeliBlanding{ false };        
        bool killer{ false };          
        bool rotationchanger{ false };

        int ArmorKey;        
        int HealthKey;        
        int FakeLagKey;
        bool HealthSet;        
        bool ArmorSet;        
        bool FakeLagSet;



        bool fakedead;
        bool fakejetpack;
        bool fakeduck;

        
        bool start{ false };        
        bool CreateMap{ false };
        int skin_id{ 1 };
    } player;

    struct  {
        ImColor box_color = ImColor(255, 255, 255, 255); // normal box rengi
        ImColor name_color = ImColor(255, 255, 255, 255); // isim rengi
        ImColor weapon_color = ImColor(255, 255, 255, 255); // silah rengi
        ImColor bone_color = ImColor(255, 255, 255, 255); // kemik rengi
        ImColor line_color = ImColor(255, 255, 255, 255); // çizgi rengi
        ImColor health_color = ImColor(0, 255, 0, 255);     // saðlýk rengi
        ImColor armor_color = ImColor(0, 0, 255, 255);     // zýrh rengi
        ImColor distance_color = ImColor(255, 255, 255, 255); // mesafe rengi
        ImColor target_color = ImColor(255, 255, 0, 255);   // silent aim target rengi
    }  esp_color;

    struct
    {
        bool state{ false };          // Ana ESP açma/kapama
        bool name{ false };
        bool test{ false };

        bool ped{ false };
        bool weapon{ false };
        bool box{ false };          
        bool troll{ false };         
        bool bos{ false };         
        bool corner{ false };        
        bool yok{ false };        
        bool am{ false };        
        bool chams{ false };
        bool box_3d{ false };        // 3D box seçeneði
        bool filled_box{ false };
        bool box_function_enabled { false }; // 2D veya 3D box açýk mý kontrolü
        bool bone{ false };
        bool line{ false };
        bool line_top{ false };      // Üstten line
        bool line_bottom{ false };   // Alttan line
        float box_opacity{ 0.15f };
        bool health{ false };
        bool armor{ false };
        bool silent_color{ false };
        bool distance{ false };      // Mesafe gösterimi
        float max_distance{ 150.0f }; // Maximum mesafe sýnýrý
        int line_type{ 0 };          // Line tipi (0: TOP, 1: HEAD, 2: BODY, 3: FOOT, 4: BOTTOM)
        int box_type{ 0 };           // Box tipi (0: 2D, 1: 3D)
        ImColor name_color{ 76, 128, 252, 255 };
        ImColor weapon_color{ 76, 128, 252, 255 };
        ImColor box_color{ 76, 128, 252, 255 };

        ImColor bone_color{ 76, 128, 252, 255 };
        ImColor line_color{ 76, 128, 252, 255 };
        ImColor health_color{ 0, 180, 80, 255 };      // Saðlýk için yeþil ton
        ImColor armor_color{ 0, 80, 200, 255 };       // Zýrh için koyu mavi
        ImColor distance_color{ 76, 128, 252, 255 };
        bool behind_wall{ false };    // Line ESP için arka görünürlük
        bool localplayer{ false };    // Local player ESP

        //ped için
        bool ped_state{ false };        
        bool ped_name{ false };        
        bool ped_corner{ false };        
        bool ped_skeleton{ false };        
        bool ped_distance{ false };        
        bool ped_line{ false };






        ImColor ped_name_color{ 255, 255, 255, 255 };
        ImColor ped_box_color{ 255, 255, 255, 255 };
        ImColor ped_bone_color{ 255, 255, 255, 255 };
        ImColor ped_distance_color{ 255, 255, 255, 255 }; 
        ImColor ped_line_color{ 255, 255, 255, 255 };


        //object için
        bool object_state{ false };        
        bool object_distance{ false };




        ImColor object_distance_color{ 255, 255, 255, 255 };

        // Vehicle ESP özellikleri
        bool veh_state{ false };
        bool veh_distance{ false };        
        bool veh_engine_status{ false };

        bool veh_name{ false };
        bool veh_box{ false };
        bool veh_box_3d{ false };
        bool veh_line{ false };
        bool veh_health{ false };
        ImColor veh_distance_color{ 255, 255, 255, 255 }; // Mesafe yazý rengi

        ImColor veh_name_color{ 255, 255, 255, 255 };
        ImColor veh_box_color{ 255, 255, 255, 255 };        
        ImColor veh_engine_on_color{ 0.0f, 1.0f, 0.0f, 1.0f };
        ImColor veh_engine_off_color{ 1.0f, 0.0f, 0.0f, 1.0f };
        ImColor veh_line_color{ 255, 255, 255, 255 };
        ImColor veh_health_color{ 0, 255, 0, 255 };
        float veh_box_opacity{ 0.15f };

        int chamsType = 0; 
        ImVec4 chamsVisibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Yeþil - görünür
        ImVec4 chamsHiddenColor = ImVec4(1.0f, 0.0f, 0.0f, 0.59f);  // Kýrmýzý - duvar arkasý

    } esp;

    struct
    {
        bool state{ false };
        bool b_fov{ false };
        int i_fov{ 100 };
        float smooth{ 1.0f };
        int bone{ 0 };
        bool visible_check{ false };
        bool deadcheck{ false };
        bool trigger_bot{ false };
        float max_distance{ 500.0f };

        // YENÝ ÖZELLÝKLER
        int target_priority{ 0 };      // 0: FOV Center, 1: Closest Distance, 2: Lowest HP
        bool team_check{ false };      // Takým arkadaþlarýný atla
        bool distance_check{ false };  // Mesafe kontrolü
        float min_hp{ 0.0f };          // Minimum hedef caný (0 = disabled)
        bool recoil_control{ false };  // RCS
        float recoil_strength{ 1.0f }; // RCS gücü
        bool aim_lock{ false };        // Agresif aim
        bool predict_movement{ false }; // Hareket tahmini
        float prediction_scale{ 1.0f }; // Tahmin çarpaný
        float bone_offset_x{ 0.0f };   // Manuel X offset
        float bone_offset_y{ 0.0f };   // Manuel Y offset
    } aimbot;
    
    struct
    {
        bool state{ false };
        bool b_fov{ false };
        bool playerfrozed{ false };
        int i_fov{ 100 };
        float smooth{ 1.0f };
        int bone{ 0 };
        bool visible_check{ false };
        bool deadcheck{ false };
        float max_distance{ 500.0f };


        int target_priority{ 0 };      // 0: FOV Center, 1: Closest Distance, 2: Lowest HP
        bool team_check{ false };      // Takým kontrolü
        bool distance_check{ false };  // Mesafe kontrolü
        float min_hp{ 0.0f };          // Minimum hedef HP
        bool auto_shoot{ false };      // Otomatik ateþ (triggerbot benzeri)
        int auto_shoot_delay{ 50 };    // Ateþ gecikmesi (ms)
        bool smooth_movement{ false }; // Yumuþak crosshair hareketi
        float smooth_speed{ 5.0f };    // Smooth hýzý
    } crosshair;

    struct {
        bool enabled = false;
        int type = 0; // 0=Cross, 1=Dot, 2=Circle, 3=T, 4=Square, 5=X, 6=Triangle
        float size = 10.0f;
        float thickness = 2.0f;
        float gap = 3.0f;
        float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // RGBA

        bool useImage = false;
        float imageSize = 32.0f;
    } customcrosshair;

    struct
    {
        bool Slapper { false };

    } car;

    struct {
        bool enabled = false;
        bool team_check = false;
        bool distance_check = false;
        float max_distance = 100.0f;
        float visible_color[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // Yeþil
        float hidden_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };  // Kýrmýzý
        int type = 0; // 0=Flat, 1=Wireframe, 2=Glow
    } chams;

    struct
    {
        bool bBoostActive = false;
        bool HellBlanding = false;
        float bMaxSpeed = 1.0f;        
        
    } buzina_boost;

    struct
    {
        int item_current = 0;
    } network;

    struct
    {
        std::vector<s_executor> resources_list;
        int item_current = 0;
    } executor;

    struct
    {
        std::vector<s_resource> resources_list;
        int item_current = 0;

        char search_input[1028] = "";
    } resource;

    struct
    {
        bool dump_enabled{ false };        

        bool runtime_dump_enabled{ false };
        bool deobfuscate{ false };
        std::vector<s_dumps> dumps_list;
        int item_current = 0;
        bool is_blocked{ false };
        

        bool use_output; // deobfuscate edilmiþ buffer’dan dump
        bool use_input;  // ham buffer’dan dump
    } dump;

    struct
    {
        bool tit{ false };
        std::vector<s_events> events_list;     
        std::vector<std::string> blacklist_words; 
        int item_current = 0;
        int blacklist_item_current = -1;        // Bu satýrý ekleyin
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };        
    } event;

    struct
    {
        bool tit{ false };
        std::vector<s_trigger_Events> events_list;
        std::vector<std::string> blacklist_words;
        int item_current = 0;
        int blacklist_item_current = -1;        
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };
    } triggerEvent;


    struct
    {
        bool tit{ false };
        std::vector<s_add_event_handler> events_list;
        std::vector<std::string> blacklist_words;
        int item_current = 0;
        int blacklist_item_current = -1;
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };
    } addeventhandler;


    struct
    {
        bool tit{ false };
        std::vector<s_element_data> events_list;
        std::vector<std::string> blacklist_words;
        int item_current = 0;
        int blacklist_item_current = -1;
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };
    } setelementdata;


    struct
    {
        bool tit{ false };        
        bool blacklist_selected{ false };
        std::vector<s_debug_call> events_list;
        std::vector<std::string> blacklist_words;
        int item_current = 0;
        int blacklist_item_current = -1;
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };
    } debugcall;       
    
    
    struct
    {
        bool tit{ false };
        std::vector<s_add_event> events_list;
        std::vector<std::string> blacklist_words;
        int item_current = 0;
        int blacklist_item_current = -1;
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };
    } addevent;   
    
    
    struct
    {
        bool tit{ false };
        std::vector<s_debug_event> events_list;
        std::vector<std::string> blacklist_words;
        int item_current = 0;
        int blacklist_item_current = -1;
        int selected_event_index = -1;

        char input_buffer[128] = { 0 };
    } debugevent;

    struct
    {
        bool bypass_lua{ false };
        bool bypass_world{ false };
        bool bypass_screenshot{ false };
        bool stopAcLixo{ false };
        bool whitetheme{ false };
        bool Admin_alert{ true };
        bool inject_scripts = false;
        bool log_scripts = false;
    } info;

    struct
    {
        bool airbreak{ false };
        bool SuperBreak{ false };
        bool SuperBreakCheck{ false };
        bool SuperBreakCheck0{ false };
        bool HornBoostCheck{ false };


        bool Cool{ false };
        bool vehicle_rain{ false };
        float airbreak_speed{ 1.0 };        
        float boost_value{ 1.0 };

    } vehicle;

    struct
    {
        bool no_reload{ false };
        bool test{ false };
        bool rapidfire{ false };
        float rapidfire_speed{ 1.0 };
        bool tek_shot{ false };
        bool one_shot{ false };  
        bool damage_boost{ false };
        bool x2_damage{ false };   
        bool silent_aim_enabled{ false };
        bool lagger{ false };      
        const char* weapon_list[47] = { "Fist", "Brass Knuckles", "Golf Club", "Nightstick", "Knife", "Baseball Bat", "Shovel", "Pool Cue", "Katana", "Chainsaw", "Purple Dildo", "Dildo", "Vibrator", "Silver Vibrator", "Flowers", "Cane", "Grenade", "Tear Gas", "Molotov Cocktail", "Rocket", "Heat Seeking RPG", "Freefall Bomb", "Colt 45", "Silenced 9mm", "Desert Eagle", "Shotgun", "Sawnoff Shotgun", "Combat Shotgun", "Micro SMG/Uzi", "MP5", "AK-47", "M4", "Tec-9", "Country Rifle", "Sniper Rifle", "RPG", "HS Rocket", "Flamethrower", "Minigun", "Satchel Charge", "Detonator", "Spraycan", "Fire Extinguisher", "Camera", "Night Vision Goggles", "Thermal Goggles", "Parachute" };
        int selected_weapon{ 0 };
        int ammo_count = 200; 

    } weapon;


    struct
    {
        bool state{ false };        
        bool deadcheck{ false };        
        bool melee_punch_enable{ false };
        bool b_fov{ false };
        int i_fov{ 100 };
        float smooth{ 1.0f };
        int bone{ 0 };
        bool visible_check{ false };
        bool wallshot_enable{ false };
        float max_distance{ 500.0f };
    } silent;    
    
    struct {
        bool team_check = false;
        bool visible_check = false;
        bool deadcheck = false;
        bool distance_check = false;
        bool b_fov = true;           // FOV çemberi göster
        int bone = 0;
        int i_fov = 100;             // FOV yarýçapý
        float spin_speed = 1440.0f;
        float max_distance = 100.0f;
        float pitch_angle = -45.0f;
        float fov_color[4] = { 1.0f, 0.0f, 1.0f, 1.0f }; 
        bool rgb_fov = false;           // RGB animasyon
        float rgb_speed = 2.0f;         // RGB deðiþim hýzý


    } spinbot;


    struct {
        bool team_check = false;
        bool visible_check = false;
        bool deadcheck = false;
        bool distance_check = false;
        bool b_fov = false;
        bool rgb_fov = false;
        bool rotation_sync = false;
        int bone = 0;
        int i_fov = 100;
        float max_distance = 100.0f;
        float fov_color[4] = { 0.0f, 1.0f, 1.0f, 1.0f }; // Cyan
        float rgb_speed = 2.0f;
    } bullettrack;


    struct
    {
        bool state{ false };
        bool melee_punch_enable{ false };
        bool b_fov{ false };
        int i_fov{ 100 };
        float smooth{ 1.0f };
        int  hitchance_percent;
        int bone{ 0 };
        bool visible_check{ false };
        bool wallshot_enable{ false };
        bool hitchance_enable{ false };
        float max_distance{ 500.0f };
        ImVec4 fov_color;      // FOV rengini ayarlamak için
    } manual;

    struct
    {
        bool crasher{ false };

    } crasher;


    struct {

            bool bSystemSpoofer{ false };

            char szVideoCardName[64]{};
            int iVideoCardRAM{ 0 };               
            int iFreeForMTA{ 0 };            
            int iUsedByRenderTargets{ 0 };
            bool bWindowedMode{ false };       // Pencere modu
            bool bAllowScreenUpload{ true };   // Ekran yükleme izni
            int iTotalPhysicalMemoryGB{ 16 };  


    } dxSpoof;
    struct {
        

        char szCustomSerial[33] = "";        
        char szCurrentSerial[33] = "";        
        char szLastSpoofedSerial[64] = { 0 };
        bool isSpoofed = false;        
        bool isSerialChanged = false;
    } spoofer;
    
    struct {

        bool state{ false };
        bool b_fov{ false };
        int bone{ 0 };
        int i_fov{ 100 };
    } instant;

    struct
    {
        bool state{ false };
        bool deobfuscate{ false };
        std::vector<s_resource_dump> dumps_list;
        int item_current = 0;
    } resources_dump;
};

inline c_elements* element = new c_elements();